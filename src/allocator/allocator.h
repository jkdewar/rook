#pragma once
#include <stddef.h>

typedef void *(*alloc_fn_t)(void *, size_t);
typedef void (*free_fn_t)(void *, void *);

typedef struct {
    void *user_data;
    alloc_fn_t alloc_fn;
    free_fn_t free_fn;
} allocator_t;
