#pragma once
#include "allocator/allocator.h"

allocator_t make_std_heap_allocator();
void *std_heap_alloc(allocator_t *, size_t);
void std_heap_free(allocator_t *, void *);
