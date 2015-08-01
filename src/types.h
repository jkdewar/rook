#pragma once

typedef enum {
    T_INT8,
    T_INT16,
    T_INT32,
    T_INT64,
    T_FLOAT,
    T_DOUBLE
} basic_type_t;

typedef enum {
    TTAG_BASIC,
    TTAG_STRUCT,
    TTAG_POINTER
} type_tag_t;

typedef struct {
    int blah;
} struct_type_t;

typedef struct type_t type_t;

typedef union {
    basic_type_t basic_type;
    struct_type_t struct_type;
    type_t *pointer_type;
} type_union_t;

struct type_t {
    type_tag_t tag;
    type_union_t u;
};
