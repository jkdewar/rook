#include "compile.h"
#include "ast.h"
#include "bytecode.h"
#include <stdio.h>
#include <setjmp.h>
#include <assert.h>

typedef struct {
    compile_input_t *in;
    compile_output_t *out;
    jmp_buf jmpbuf; /* used for error handling */
} compile_state_t;

static void error(compile_state_t *c, const char *msg);
static void compile_statement_list(compile_state_t *c, ast_statement_t *first_statement);
static void compile_statement(compile_state_t *c, ast_statement_t *statement);

/*----------------------------------------------------------------------*/
void compile(compile_input_t *in, compile_output_t *out) {
    compile_state_t compile_state;
    compile_state_t *c = &compile_state;

    c->in = in;
    c->out = out;
    bytestream_init(&c->out->bytestream, 1024 * 16);

    if (setjmp(c->jmpbuf)) {
        return;
    }

}

/*----------------------------------------------------------------------*/
static void error(compile_state_t *p, const char *msg) {
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
    longjmp(p->jmpbuf, 1);
}

/*----------------------------------------------------------------------*/
static void compile_statement_list(compile_state_t *c, ast_statement_t *first_statement) {
    ast_statement_t *statement;

    for (statement = first_statement; statement != NULL; statement = statement->next) {
        
    }
}

/*----------------------------------------------------------------------*/
static void compile_statement(compile_state_t *c, ast_statement_t *statement) {
}
