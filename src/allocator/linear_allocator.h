#pragma once
#include "allocator/allocator.h"

typedef struct {
    void *start;
    size_t capacity;
    size_t used;
} linear_allocator_data_t;

void *linear_alloc(void *user_data, size_t size);
