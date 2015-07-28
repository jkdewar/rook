#pragma once
#include "parse.h"

typedef struct {
    parse_output_t *parse_out;
} compile_input_t;

typedef struct {
    int blah;
} compile_output_t;

void compile(compile_input_t *in, compile_output_t *out);
