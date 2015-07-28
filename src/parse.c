#include "parse.h"
#include "ast.h"
#include <stdio.h>
#include <string.h>
#include <setjmp.h>
#include <assert.h>

typedef struct {
    parse_input_t *parse_in;
    parse_output_t *parse_out;
    size_t token_index;
    jmp_buf jmpbuf; /* used for error handling */
} parse_state_t;

#define EXPECT(TOKEN, TYPE, MSG) if (!test_token(TOKEN, TYPE)) { error(p, MSG); }
#define ALLOC(SIZE) p->parse_in->allocator.alloc_fn(p->parse_in->allocator.user_data, SIZE)

static void error(parse_state_t *p, const char *msg);
static token_t *next_token(parse_state_t *p);
static token_t *peek_token(parse_state_t *p);
static void rewind_token(parse_state_t *p);
static int test_token(token_t *token, token_type_t token_type);
static ast_statement_t *parse_statement_list(parse_state_t *p);
static ast_statement_t *parse_statement(parse_state_t *p);
static ast_statement_t *parse_declare_var(parse_state_t *p);
static ast_statement_t *parse_define_function(parse_state_t *p);
static ast_expression_t *parse_expression(parse_state_t *p);
static ast_expression_t *parse_sum_op(parse_state_t *p, ast_expression_t *left);
static ast_expression_t *parse_signed_term(parse_state_t *p);
static ast_expression_t *parse_term(parse_state_t *p);
static ast_expression_t *parse_term_op(parse_state_t *p, ast_expression_t *left);
static ast_expression_t *parse_signed_factor(parse_state_t *p);
static ast_expression_t *parse_argument(parse_state_t *p);
static ast_expression_t *parse_value(parse_state_t *p);

/*----------------------------------------------------------------------*/
void parse(parse_input_t *parse_in, parse_output_t *parse_out) {
    parse_state_t parse_state;
    parse_state_t *p = &parse_state;
    
    p->parse_in = parse_in;
    p->parse_out = parse_out;
    p->token_index = 0;

    if (setjmp(p->jmpbuf)) {
        return;
    }

    p->parse_out->first_statement = parse_statement_list(p);
}

/*----------------------------------------------------------------------*/
static void error(parse_state_t *p, const char *msg) {
    token_pos_t token_pos;
    char buf[1024];
    int i;

    lex_token_pos(p->parse_in->lex_out, p->token_index == 0 ? 0 : p->token_index - 1, &token_pos);
    printf("test.bas:%ld:%ld: error: %s\n", token_pos.line_num, token_pos.line_pos, msg);
    memcpy(buf, token_pos.line_start, token_pos.line_end - token_pos.line_start);
    printf("%s\n", buf);
    for (i = 0; i < token_pos.line_pos; ++i)
        printf(" ");
    printf("^\n");
    longjmp(p->jmpbuf, 1);
}

/*----------------------------------------------------------------------*/
static token_t *next_token(parse_state_t *p) {
    if (p->token_index >= p->parse_in->lex_out->token_count)
        return NULL;
    return &p->parse_in->lex_out->tokens[p->token_index++];
}

/*----------------------------------------------------------------------*/
static token_t *peek_token(parse_state_t *p) {
    if (p->token_index >= p->parse_in->lex_out->token_count)
        return NULL;
    return &p->parse_in->lex_out->tokens[p->token_index];
}

/*----------------------------------------------------------------------*/
static void rewind_token(parse_state_t *p) {
    if (p->token_index != 0)
        p->token_index -= 1;
}

/*----------------------------------------------------------------------*/
static int test_token(token_t *token, token_type_t token_type) {
    return token != NULL && token->type == token_type;
}

/*----------------------------------------------------------------------*/
static ast_statement_t *parse_statement_list(parse_state_t *p) {
    ast_statement_t *first;
    ast_statement_t *s;

    first = parse_statement(p);
    s = first;
    for (s = first; s != NULL; s = s->next) {
        s->next = parse_statement(p);
    }
    return first;
}

/*----------------------------------------------------------------------*/
static ast_statement_t *parse_statement(parse_state_t *p) {
    token_t *token;
    token = peek_token(p);
    if (token == NULL) {
        return NULL;
    } else if (test_token(token, TK_END)) {
        return NULL;
    } else if (test_token(token, TK_VAR)) {
        return parse_declare_var(p);
    } else if (test_token(token, TK_FUNCTION)) {
        return parse_define_function(p);
    }
    next_token(p);
    error(p, "statement expected");
    /* unreachable */
    assert(0);
    return NULL;
}

/*----------------------------------------------------------------------*/
static ast_statement_t *parse_declare_var(parse_state_t *p) {
    token_t *token;
    ast_statement_t *statement;

    statement = (ast_statement_t*) ALLOC(sizeof(ast_statement_t));
    statement->next = NULL;
    statement->type = AST_STATEMENT_DECLARE_VARIABLE;

    /* var */
    token = next_token(p);
    EXPECT(token, TK_VAR, "'var' expected");

    /* identifier */
    token = next_token(p);
    EXPECT(token, TK_IDENTIFIER, "variable name expected");
    statement->u.declare_variable.token = *token;

    return statement;
}

/*----------------------------------------------------------------------*/
static ast_statement_t *parse_define_function(parse_state_t *p) {
    token_t *token;
    ast_statement_t *statement;
    ast_function_parameter_t *parameter = NULL;
    ast_function_parameter_t *prev_parameter = NULL;

    statement = (ast_statement_t*) ALLOC(sizeof(ast_statement_t));
    statement->next = NULL;
    statement->type = AST_STATEMENT_DEFINE_FUNCTION;
    statement->u.define_function.first_parameter = NULL;
    statement->u.define_function.first_statement = NULL;
    statement->u.define_function.return_type_token.type =
            TK_FUNCTION; /* TODO:jkd hacky way to specify no return type */

    /* function keyword */
    token = next_token(p);
    EXPECT(token, TK_FUNCTION, "'function' expected");

    /* function name */
    token = next_token(p);
    EXPECT(token, TK_IDENTIFIER, "function name expected");
    statement->u.define_function.name_token = *token;

    /* function parameters */
    token = peek_token(p);
    if (test_token(token, TK_LBRACKET)) { /* opening bracket? */
        next_token(p);
        for (;;) {
            token = peek_token(p);
            if (token == NULL || token->type != TK_IDENTIFIER)
                break;
            next_token(p);

            prev_parameter = parameter;
            parameter = (ast_function_parameter_t *)
                        ALLOC(sizeof(ast_function_parameter_t));
            if (prev_parameter == NULL) {
                statement->u.define_function.first_parameter = parameter;
            } else {
                prev_parameter->next = parameter;
            }

            /* parameter name */
            parameter->identifier_token = *token;

            /* colon before parameter type */
            token = next_token(p);
            EXPECT(token, TK_COLON, "':' expected");

            /* parameter type */
            token = next_token(p);
            EXPECT(token, TK_IDENTIFIER, "parameter type expected");
            parameter->type_token = *token;

            /* comma? */
            token = peek_token(p);
            if (!test_token(token, TK_COMMA))
                break;
            next_token(p);
        }

        /* closing bracket */
        token = next_token(p);
        EXPECT(token, TK_RBRACKET, "')' expected");
    }

    /* colon before return type? */
    token = peek_token(p);
    if (test_token(token, TK_COLON)) {
        next_token(p);

        /* return type */
        token = next_token(p);
        statement->u.define_function.return_type_token = *token;
        EXPECT(token, TK_IDENTIFIER, "return type expected");
    }

    /* function body statement list */
    statement->u.define_function.first_statement = parse_statement_list(p);

    /* end */
    token = next_token(p);
    EXPECT(token, TK_END, "'end' expected");

    return statement;
}

/*----------------------------------------------------------------------*/
static ast_expression_t *parse_expression(parse_state_t *p) {
    ast_expression_t *left;
    ast_expression_t *sum_op;

    left = parse_signed_term(p);
    sum_op = parse_sum_op(p, left);
    return sum_op != NULL ? sum_op : left;
}

/*----------------------------------------------------------------------*/
static ast_expression_t *parse_sum_op(parse_state_t *p, ast_expression_t *left) {
    token_t* token;

    token = peek_token(p);
    if (token == NULL) {
        return NULL;
    } else if (token->type == TK_PLUS || token->type == TK_MINUS) {
        ast_expression_t *right;
        ast_expression_t *bin_op;
        ast_expression_t *sum_op;

        next_token(p);
        right = parse_term(p);

        bin_op = (ast_expression_t *)ALLOC(sizeof(ast_expression_t));
        bin_op->type = AST_EXPRESSION_BIN_OP;
        bin_op->u.bin_op.left = left;
        bin_op->u.bin_op.right = right;
        bin_op->u.bin_op.operation = token->type;

        sum_op = parse_sum_op(p, bin_op);
        return sum_op != NULL ? sum_op : bin_op;
    } else {
        return NULL;
    }
}

/*----------------------------------------------------------------------*/
static ast_expression_t *parse_signed_term(parse_state_t *p) {
    ast_expression_t *term;

    /* TODO:jkd handle unary op */
    term = parse_term(p);
    return term;
}

/*----------------------------------------------------------------------*/
static ast_expression_t *parse_term(parse_state_t *p) {
    ast_expression_t *left;
    ast_expression_t *term_op;

    left = parse_argument(p);
    term_op = parse_term_op(p, left);
    return term_op != NULL ? term_op : left;
}

/*----------------------------------------------------------------------*/
static ast_expression_t *parse_term_op(parse_state_t *p, ast_expression_t *left) {
    token_t* token;

    token = peek_token(p);
    if (token == NULL) {
        return NULL;
    } else if (token->type == TK_STAR || token->type == TK_SLASH) {
        ast_expression_t *right;
        ast_expression_t *bin_op;
        ast_expression_t *term_op;

        next_token(p);
        right = parse_signed_factor(p);

        bin_op = (ast_expression_t *)ALLOC(sizeof(ast_expression_t));
        bin_op->type = AST_EXPRESSION_BIN_OP;
        bin_op->u.bin_op.left = left;
        bin_op->u.bin_op.right = right;
        bin_op->u.bin_op.operation = token->type;

        term_op = parse_term_op(p, bin_op);
        return term_op != NULL ? term_op : bin_op;
    } else {
        return NULL;
    }
}

/*----------------------------------------------------------------------*/
static ast_expression_t *parse_signed_factor(parse_state_t *p) {
    ast_expression_t *term;

    /* TODO:jkd handle unary op */
    term = parse_argument(p);
    return term;}

/*----------------------------------------------------------------------*/
static ast_expression_t *parse_argument(parse_state_t *p) {
    /* TODO:jkd function call, variable... */
    return parse_value(p);
}

/*----------------------------------------------------------------------*/
static ast_expression_t *parse_value(parse_state_t *p) {
    token_t *token = next_token(p);
    ast_expression_t *expression =
            (ast_expression_t *)ALLOC(sizeof(ast_expression_t));

    if (token == NULL) {
    } else if (token->type == TK_INT_LITERAL ||
               token->type == TK_FLOAT_LITERAL ||
               token->type == TK_DOUBLE_LITERAL ||
               token->type == TK_STRING_LITERAL) {
        expression->type = AST_EXPRESSION_LITERAL;
        expression->u.literal.token = *token; /* TODO:jkd copy string literal? */
    } else {
        error(p, "");
    }
    return expression;
    /* TODO:jkd */
}
