#pragma once
#include <stddef.h>

typedef struct allocator_t allocator_t;
struct allocator_t {
    void *start;
    size_t capacity;
    size_t used;
    void *(*alloc_fn)(allocator_t *, size_t);
    void (*free_fn)(allocator_t *, void *);
};

#define ALLOCATOR_ALLOC(allocator, size) (allocator)->alloc_fn(allocator, size)
#define ALLOCATOR_FREE(allocator, p) (allocator)->free_fn(allocator, p)
#define BZERO(X) memset(X, 0, sizeof(*(X)))
