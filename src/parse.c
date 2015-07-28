#include "parse.h"
#include "ast.h"
#include <stdio.h>
#include <string.h>
#include <setjmp.h>

typedef struct {
    parse_input_t *parse_in;
    parse_output_t *parse_out;
    size_t token_index;
    jmp_buf jmpbuf; /* used for error handling */
} parse_state_t;

#define EXPECT(TOKEN, TYPE) if ((TOKEN) == NULL || (TOKEN)->type != (TYPE)) { error(p); }
#define ALLOC(SIZE) p->parse_in->allocator.alloc_fn(p->parse_in->allocator.user_data, SIZE)

static void error(parse_state_t *p);
static token_t *next_token(parse_state_t *p);
static token_t *peek_token(parse_state_t *p);
static void rewind_token(parse_state_t *p);
static int test_token(token_t *token, token_type_t token_type);
static ast_statement_t *parse_statement_list(parse_state_t *p);
static ast_statement_t *parse_statement(parse_state_t *p);
static ast_statement_t *parse_declare_var(parse_state_t *p);
static ast_expression_t *parse_expression(parse_state_t *p);

/*----------------------------------------------------------------------*/
void parse(parse_input_t *parse_in, parse_output_t *parse_out) {
    parse_state_t p;
    
    p.parse_in = parse_in;
    p.parse_out = parse_out;

    if (setjmp(p.jmpbuf)) {
        printf("PARSE ERROR!\n");
        return;
    }

    p.parse_out->first_statement = parse_statement_list(&p);
}

/*----------------------------------------------------------------------*/
static void error(parse_state_t *p) {
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
    if (token == NULL)
        return NULL;
    if (token->type == TK_IDENTIFIER && strcmp(token->data.s, "var") == 0) {
        return parse_declare_var(p);
    }
    return NULL;
}

/*----------------------------------------------------------------------*/
static ast_statement_t *parse_declare_var(parse_state_t *p) {
    token_t *token;
    ast_statement_t *statement;

    statement = (ast_statement_t*) ALLOC(sizeof(ast_statement_t));
    statement->type = AST_STATEMENT_DECLARE_VARIABLE;
    statement->type_specific =
            (ast_statement_declare_variable_t*)
            ALLOC(sizeof(ast_statement_declare_variable_t));
    statement->next = NULL;

#if 0
    /* var */
    token = next_token(p);
    EXPECT(token, TK_VAR);

    /* identifier */
    token = next_token(p);
    EXPECT(token, TK_IDENTIFIER);

    /* equals */
    token = next_token(p);
    EXPECT(token, TK_EQUALS);

    /* expression */
    parse_expression(p);
#endif

    return statement;
}

/*----------------------------------------------------------------------*/
static ast_expression_t *parse_expression(parse_state_t *p) {
    return NULL;
}
