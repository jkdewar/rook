#pragma once
#include "allocator.h"
#include <stdlib.h>

typedef struct {
    const char *s;
    allocator_t allocator;
} lex_input_t;

typedef enum {
    TK_IDENTIFIER,
    TK_INT_LITERAL,
    TK_FLOAT_LITERAL,
    TK_DOUBLE_LITERAL,
    TK_STRING_LITERAL
} token_type_t;

typedef union {
    int i;
    float f;
    double d;
    char *s;
} token_data_t;

typedef struct {
    token_type_t type;
    token_data_t data;
} token_t;

#define LEX_ERROR_BUF_SIZE 1024

typedef struct {
    token_t *tokens;
    size_t token_count;
    char error[LEX_ERROR_BUF_SIZE];
} lex_output_t;

void lex(lex_input_t *in, lex_output_t *out);
void lex_output_free(lex_output_t *out);
const char *token_type_str(token_type_t token_type);
