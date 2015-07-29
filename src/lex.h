#pragma once
#include "allocator.h"
#include <stdlib.h>

typedef enum {
/* information-holding */
    TK_IDENTIFIER,
    TK_INT_LITERAL,
    TK_FLOAT_LITERAL,
    TK_DOUBLE_LITERAL,
    TK_STRING_LITERAL,
/* symbols */
    TK_PLUS,
    TK_MINUS,
    TK_STAR,
    TK_SLASH,
    TK_EQUALS,
    TK_LESS,
    TK_LESS_EQUAL,
    TK_EQUALS_EQUALS,
    TK_GREATER_EQUAL,
    TK_GREATER,
    TK_LBRACKET,
    TK_RBRACKET,
    TK_COMMA,
    TK_COLON,
/* keywords */
    TK_END,
    TK_VAR,
    TK_FUNCTION,
    TK_IF,
    TK_FOR,
    TK_RETURN
} token_type_t;

typedef union {
    int i;
    float f;
    double d;
    char *s;
} token_union_t;

typedef struct {
    token_type_t type;
    token_union_t u;
    size_t source_pos;
} token_t;

typedef struct {
    const char *line_start;
    const char *line_end;
    size_t line_num;
    size_t line_pos;
} token_pos_t;

#define LEX_ERROR_BUF_SIZE 1024

typedef struct {
    const char *s;
    allocator_t *allocator;
} lex_input_t;

typedef struct {
    const char *s;
    token_t *tokens;
    size_t token_count;
    int is_error;
    char error_string[LEX_ERROR_BUF_SIZE];
} lex_output_t;

void lex(lex_input_t *in, lex_output_t *out);
void lex_token_pos(lex_output_t *out, size_t token_index, token_pos_t *token_pos);
