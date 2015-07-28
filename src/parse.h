#pragma once
#include "lex.h"
#include "allocator.h"

typedef struct {
    lex_output_t *lex_out;
    allocator_t allocator;
} parse_input_t;

typedef struct {
    struct ast_statement_t *first_statement;
} parse_output_t;

void parse(parse_input_t *in, parse_output_t *out);
