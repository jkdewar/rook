#pragma once
#include <stddef.h>

#define MAX_STRUCT_FIELD_LEN 127

typedef enum {
    T_INT8,
    T_INT16,
    T_INT32,
    T_INT64,
    T_UINT8,
    T_UINT16,
    T_UINT32,
    T_UINT64,
    T_FLOAT,
    T_DOUBLE
} basic_type_t;

typedef enum {
    TTAG_BASIC_TYPE,
    TTAG_STRUCT,
    TTAG_POINTER
} type_tag_t;

typedef struct struct_type_t {
    struct struct_field_t *first;
} struct_type_t;

typedef union {
    basic_type_t basic_type;
    struct_type_t struct_type;
    struct type_t *pointer_type;
} type_union_t;

typedef struct type_t {
    type_tag_t tag;
    type_union_t u;
    size_t size;
} type_t;

typedef struct struct_field_t {
    char name[MAX_STRUCT_FIELD_LEN + 1];
    type_t type;
    size_t offset;
    struct struct_field_t *next;
} struct_field_t;

int types_match(type_t *a, type_t *b);
