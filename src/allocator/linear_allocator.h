#pragma once
#include "allocator/allocator.h"

allocator_t make_linear_allocator(void *memory, size_t capacity);
void *linear_alloc(allocator_t *allocator, size_t size);
void linear_free(allocator_t *allocator, void *p);
