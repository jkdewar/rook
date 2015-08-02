#pragma once
#include "parse.h"
#include "bytestream.h"
#include "types.h"
#include "hash_table.h"
#include "allocator/allocator.h"

typedef struct function_parameter_t {
    const char *name;
    type_t *type;
    struct function_parameter_t *next;
} function_parameter_t;

typedef struct {
    const char *name;
    uint32_t address;
    type_t *return_type;
    function_parameter_t *first_parameter;
    size_t parameter_count;
    size_t parameters_size;
} function_table_entry_t;

typedef struct {
    allocator_t *allocator;
    parse_output_t *parse_out;
} compile_input_t;

typedef struct {
    bytestream_t bytestream;
    int error;
    hash_table_t *function_table;
} compile_output_t;

void compile(compile_input_t *in, compile_output_t *out);
