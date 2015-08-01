#include "compile.h"
#include "ast.h"
#include "hash_table.h"
#include "bytecode.h"
#include <stdio.h>
#include <setjmp.h>
#include <assert.h>

#define STRINGIZE_DETAIL(x) #x
#define STRINGIZE(x) STRINGIZE_DETAIL(x)
#define INTERNAL_ERROR(c) error(c, "internal compiler error at " __FILE__ ":" STRINGIZE(__LINE__))

typedef enum {
    COMPILE_CONTEXT_GLOBAL,
    COMPILE_CONTEXT_FUNCTION_BODY
} compile_context_t;

typedef struct {
    compile_input_t *in;
    compile_output_t *out;
    jmp_buf jmpbuf; /* used for error handling */
    compile_context_t context;
    hash_table_t *local_symbol_table;
    uint32_t next_local_symbol_offset;
    hash_table_t *function_table;
} compile_state_t;

typedef struct {
    const char *name;
    type_t type;
    uint32_t stack_pos;
} symbol_table_entry_t;

typedef struct function_parameter_t {
    const char *name;
    type_t type;
    struct function_parameter_t *next;
} function_parameter_t;

typedef struct {
    const char *name;
    type_t return_type;
    function_parameter_t *first_parameter;
    size_t parameter_count;
} function_table_entry_t;

static void error(compile_state_t *c, const char *msg);
static void compile_statement_list(compile_state_t *c, ast_statement_t *first_statement);
static void compile_statement(compile_state_t *c, ast_statement_t *statement);
static void compile_declare_variable(compile_state_t *c, ast_statement_t *statement);
static void compile_define_function(compile_state_t *c, ast_statement_t *statement);
static void compile_return(compile_state_t *c, ast_statement_t *statement);
static void compile_if(compile_state_t *c, ast_statement_t *statement);
static void compile_for(compile_state_t *c, ast_statement_t *statement);
static void compile_assignment(compile_state_t *c, ast_statement_t *statement);
static void compile_expression(compile_state_t *c, ast_expression_t *expression);
static void compile_literal(compile_state_t *c, ast_expression_t *expression);
static void compile_variable(compile_state_t *c, ast_expression_t *expression);
static void compile_function_call(compile_state_t *c, ast_expression_t *expression);
static void compile_bin_op(compile_state_t *c, ast_expression_t *expression);

/*----------------------------------------------------------------------*/
void compile(compile_input_t *in, compile_output_t *out) {
    compile_state_t compile_state;
    compile_state_t *c = &compile_state;

    c->in = in;
    c->out = out;
    c->out->error = 0;
    c->out->bytestream.allocator = c->in->allocator;
    bytestream_init(&c->out->bytestream, 1024 * 16);
    c->context = COMPILE_CONTEXT_GLOBAL;
    c->local_symbol_table = hash_table_create(c->in->allocator);
    c->function_table = hash_table_create(c->in->allocator);

    if (setjmp(c->jmpbuf)) {
        return;
    }

    compile_statement_list(c, c->in->parse_out->first_statement);
}

/*----------------------------------------------------------------------*/
static void error(compile_state_t *c, const char *msg) {
#if 0 /* TODO:jkd */
    token_pos_t token_pos;
    char buf[1024]; /* TODO:jkd */
    int i;

    lex_token_pos(p->in->lex_out, p->token_index == 0 ? 0 : p->token_index - 1, &token_pos);
    printf("test.bas:%ld:%ld: error: %s\n", token_pos.line_num, token_pos.line_pos, msg);
    memcpy(buf, token_pos.line_start, token_pos.line_end - token_pos.line_start);
    printf("%s\n", buf);
    for (i = 0; i < token_pos.line_pos; ++i)
        printf(" ");
    printf("^\n");
#else
    printf("%s\n", msg);
#endif
    c->out->error = 1;
    longjmp(c->jmpbuf, 1);
}

/*----------------------------------------------------------------------*/
static void compile_statement_list(compile_state_t *c, ast_statement_t *first_statement) {
    ast_statement_t *statement;

    for (statement = first_statement; statement != NULL; statement = statement->next) {
        compile_statement(c, statement);
    }
}

/*----------------------------------------------------------------------*/
static void compile_statement(compile_state_t *c, ast_statement_t *statement) {
    switch (statement->tag) {
        case AST_STATEMENT_DECLARE_VARIABLE: compile_declare_variable(c, statement); break;
        case AST_STATEMENT_DEFINE_FUNCTION: compile_define_function(c, statement); break;
        case AST_STATEMENT_RETURN: compile_return(c, statement); break;
        case AST_STATEMENT_IF: compile_if(c, statement); break;
        case AST_STATEMENT_FOR: compile_for(c, statement); break;
        case AST_STATEMENT_ASSIGNMENT: compile_assignment(c, statement); break;
        default: INTERNAL_ERROR(c);
    }
}

/*----------------------------------------------------------------------*/
static void compile_declare_variable(compile_state_t *c, ast_statement_t *statement) {
    const char *name;
    symbol_table_entry_t *entry;

    if (c->context != COMPILE_CONTEXT_FUNCTION_BODY) {
        error(c, "variable defined outside of function body");
    }

    name = statement->u.declare_variable.token.u.s;

    /* check for duplicate symbols */
    entry = hash_table_find(c->local_symbol_table, name);
    if (entry != NULL) {
        printf("'%s': ", name);
        error(c, "duplicate symbol");
    }

    /* add variable to local symbol table */
    entry = ALLOCATOR_ALLOC(c->out->bytestream.allocator, sizeof(symbol_table_entry_t));
    entry->name = name; /* TODO:jkd copy? */
    entry->stack_pos = c->next_local_symbol_offset;
    hash_table_insert(c->local_symbol_table, name, entry);
    c->next_local_symbol_offset += sizeof(int32_t); /* TODO:jkd */
}

/*----------------------------------------------------------------------*/
static void compile_define_function(compile_state_t *c, ast_statement_t *statement) {
    uint32_t frame_size_loc;

    if (c->context != COMPILE_CONTEXT_GLOBAL)
        error(c, "local function definitions are not allowed");
    c->context = COMPILE_CONTEXT_FUNCTION_BODY;

    /* add function to table */
    {
        function_table_entry_t *entry;
        function_parameter_t *param, *prev_param;
        ast_function_parameter_t *ast_param;

        entry = ALLOCATOR_ALLOC(c->out->bytestream.allocator, sizeof(function_table_entry_t));
        entry->name = statement->u.define_function.name_token.u.s; /* TODO:jkd copy? */
        entry->return_type.tag = TTAG_BASIC;
        entry->return_type.u.basic_type = T_INT32; /* TODO:jkd */
        entry->parameter_count = 0;
        prev_param = NULL;
        for (ast_param = statement->u.define_function.first_parameter;
                ast_param != NULL;  ast_param = ast_param->next) {
            param = ALLOCATOR_ALLOC(c->out->bytestream.allocator, sizeof(function_parameter_t));
            param->type.tag = TTAG_BASIC;
            param->type.u.basic_type = T_INT32; /* TODO:jkd */
            if (prev_param == NULL) {
                entry->first_parameter = param;
            } else {
                prev_param->next = param;
            }
            prev_param = param;
            entry->parameter_count += 1;
        }
        hash_table_insert(c->function_table, entry->name, entry);
    }

    bcbuild_FRAME(&c->out->bytestream, 0, &frame_size_loc); /* frame size filled in below */

    /* clear local symbol table */
    hash_table_clear(c->local_symbol_table);

    /* Put function parameters into the symbol table.
     * They have negative locations, because they are stored above bp. */
    {
        uint32_t frame_junk_size = sizeof(uint32_t) * 3; /* sp, bp, return address */
        uint32_t parameter_location = -frame_junk_size;
        ast_function_parameter_t *param;
        symbol_table_entry_t *entry;
        for (param = statement->u.define_function.first_parameter;
                param != NULL; param = param->next) {
            parameter_location -= sizeof(int32_t); /* size of parameter TODO:jkd */
            entry = ALLOCATOR_ALLOC(c->out->bytestream.allocator, sizeof(symbol_table_entry_t));
            entry->name = param->identifier_token.u.s; /* TODO:jkd copy? */
            entry->type.tag = TTAG_BASIC;       /* TODO:jkd */
            entry->type.u.basic_type = T_INT32;
            /* TODO:jkd entry->location = parameter_location; */
            hash_table_insert(c->local_symbol_table, entry->name, entry);
        }
    }
    c->next_local_symbol_offset = 0;

    /* function body */
    compile_statement_list(c, statement->u.define_function.block);

    /* fill in the frame size now that we know about the local variables */
    bytestream_set32(&c->out->bytestream, frame_size_loc, c->next_local_symbol_offset);

    c->context = COMPILE_CONTEXT_GLOBAL;
}

/*----------------------------------------------------------------------*/
static void compile_return(compile_state_t *c, ast_statement_t *statement) {
    /* return value? */
    if (statement->u.return_.expr) {
        /* return value expression */
        compile_expression(c, statement->u.return_.expr);

        /* move return value to just above the top of frame and parameters */
        {
            int32_t frameJunkSize = sizeof(int) * 3; /* sp, bp, return address */
            int32_t returnTypeSize = sizeof(int32_t); /* TODO:jkd */
            bcbuild_STORE(&c->out->bytestream, returnTypeSize,
                    - returnTypeSize - frameJunkSize);
        }
    }

    /* return */
    bcbuild_RET(&c->out->bytestream);
}

/*----------------------------------------------------------------------*/
static void compile_if(compile_state_t *c, ast_statement_t *statement) {
    uint32_t else_block_address_loc;
    uint32_t bottom_address_loc;
    uint32_t addr;

    /* if test */
    compile_expression(c, statement->u.if_.condition);
    /* jump past if-block if predicate is false (filled in below) */
    bcbuild_JF(&c->out->bytestream, 0, &else_block_address_loc); /* dest address calculated below */

    /* compile if-block */
    compile_statement_list(c, statement->u.if_.if_block);

    if (statement->u.if_.else_block != NULL) {
        /* jump from end of if-block past else-block (filled in below) */
        bcbuild_J(&c->out->bytestream, 0, &bottom_address_loc);
    }

    /* fill in else-block address */
    addr = bytestream_where(&c->out->bytestream);
    bytestream_set32(&c->out->bytestream, else_block_address_loc, addr);

    if (statement->u.if_.else_block != NULL) {
        /* compile else-block */
        compile_statement_list(c, statement->u.if_.else_block);
        /* fill in bottom address */
        addr = bytestream_where(&c->out->bytestream);
        bytestream_set32(&c->out->bytestream, bottom_address_loc, addr);
    }
}

/*----------------------------------------------------------------------*/
static void compile_for(compile_state_t *c, ast_statement_t *statement) {
    uint32_t bottom_loc;
    uint32_t top_addr;
    uint32_t bottom_addr;

    /* initialize */
    compile_statement_list(c, statement->u.for_.initialize);

/* top: */
    top_addr = bytestream_where(&c->out->bytestream);

    /* if condition is false, jump to bottom */
    compile_expression(c, statement->u.for_.condition);
    bcbuild_JF(&c->out->bytestream, 0, &bottom_loc); /* filled in below */

    /* body */
    compile_statement_list(c, statement->u.for_.block);

    /* increment */
    compile_statement_list(c, statement->u.for_.increment);

    /* jump to top */
    bcbuild_J(&c->out->bytestream, top_addr, NULL);

/* bottom: */
    bottom_addr = bytestream_where(&c->out->bytestream);
    bytestream_set32(&c->out->bytestream, bottom_loc, bottom_addr);
}

/*----------------------------------------------------------------------*/
static void compile_assignment(compile_state_t *c, ast_statement_t *statement) {
    const char *name;
    uint32_t size;
    symbol_table_entry_t *entry;

    /* evaluate rvalue */
    compile_expression(c, statement->u.assignment.expr);

    /* look up symbol */
    name = statement->u.assignment.identifier.u.s;
    entry = hash_table_find(c->local_symbol_table, name);
    if (entry == NULL)
        error(c, "undeclared identifier");
    size = sizeof(int32_t); /* TODO:jkd */

    /* store in lvalue */
    bcbuild_STORE(&c->out->bytestream, size, entry->stack_pos);
}

/*----------------------------------------------------------------------*/
static void compile_expression(compile_state_t *c, ast_expression_t *expression) {
    switch (expression->tag) {
        case AST_EXPRESSION_LITERAL: compile_literal(c, expression); break;
        case AST_EXPRESSION_VARIABLE: compile_variable(c, expression); break;
        case AST_EXPRESSION_FUNCTION_CALL: compile_function_call(c, expression); break;
        case AST_EXPRESSION_BIN_OP: compile_bin_op(c, expression); break;
        default: INTERNAL_ERROR(c);
    }
}

/*----------------------------------------------------------------------*/
static void compile_literal(compile_state_t *c, ast_expression_t *expression) {
    switch (expression->u.literal.token.type) {
        case TK_INT_LITERAL:
            bcbuild_PUSH_SI32(&c->out->bytestream, expression->u.literal.token.u.i);
            break;
        /* TODO:jkd
        case TK_FLOAT_LITERAL:
        case TK_DOUBLE_LITERAL:
        case TK_STRING_LITERAL:
        */
        default: INTERNAL_ERROR(c);
    }
}

/*----------------------------------------------------------------------*/
static void compile_variable(compile_state_t *c, ast_expression_t *expression) {
    const char *name;
    symbol_table_entry_t *entry;
    uint32_t size;

    name = expression->u.variable.token.u.s;
    entry = hash_table_find(c->local_symbol_table, name);
    if (entry == NULL)
        error(c, "undeclared identifier");
    size = sizeof(int32_t); /* TODO:jkd */
    bcbuild_LOAD(&c->out->bytestream, size, entry->stack_pos);
}

/*----------------------------------------------------------------------*/
static void compile_function_call(compile_state_t *c, ast_expression_t *expression) {
    const char *function_name;
    function_table_entry_t *entry;
    function_parameter_t *param_desc;
    ast_expression_list_t *param_expr;

    /* function name */
    if (expression->u.function_call.identifier.type != TK_IDENTIFIER)
        INTERNAL_ERROR(c);
    function_name = expression->u.function_call.identifier.u.s;

    /* look up function in table */
    entry = hash_table_find(c->function_table, function_name);
    if (entry == NULL)
        error(c, "call to undefined function");

    /* verify number of parameters */
    if (expression->u.function_call.parameter_count != entry->parameter_count)
        error(c, "wrong number of parameters");

    /* parameters */
    param_desc = entry->first_parameter;
    param_expr = expression->u.function_call.parameter_expr_list;
    while (param_desc != NULL) {
         /* compile param */
         compile_expression(c, param_expr->expr);
         /* type check param */
         
         /* next param */
         param_desc = param_desc->next;
         param_expr = param_expr->next;
    }

    INTERNAL_ERROR(c); /* TODO:jkd */
}

/*----------------------------------------------------------------------*/
static void compile_bin_op(compile_state_t *c, ast_expression_t *expression) {
    token_type_t operation = expression->u.bin_op.operation;
    opcode_subtype_t subtype;

    subtype = OP_ST_SI32; /* TODO:jkd */

    compile_expression(c, expression->u.bin_op.right);
    compile_expression(c, expression->u.bin_op.left);

    switch (operation) {
        case TK_PLUS:           bcbuild_ADD(&c->out->bytestream, subtype); break;
        case TK_MINUS:          bcbuild_SUB(&c->out->bytestream, subtype); break;
        case TK_STAR:           bcbuild_MUL(&c->out->bytestream, subtype); break;
        case TK_SLASH:          bcbuild_DIV(&c->out->bytestream, subtype); break;
        case TK_LESS:           bcbuild_TL (&c->out->bytestream, subtype); break;
        case TK_LESS_EQUAL:     bcbuild_TLE(&c->out->bytestream, subtype); break;
        case TK_EQUALS_EQUALS:  bcbuild_TE (&c->out->bytestream, subtype); break;
        case TK_GREATER_EQUAL:  bcbuild_TGE(&c->out->bytestream, subtype); break;
        case TK_GREATER:        bcbuild_TG (&c->out->bytestream, subtype); break;
        default: INTERNAL_ERROR(c);
    }
}
