#pragma once
#include "parse.h"
#include "bytestream.h"
#include "allocator/allocator.h"

typedef struct {
    allocator_t *allocator;
    parse_output_t *parse_out;
} compile_input_t;

typedef struct {
    bytestream_t bytestream;
    int error;
} compile_output_t;

void compile(compile_input_t *in, compile_output_t *out);
