#include "allocator/std_heap_allocator.h"
#include <stdlib.h>

/*----------------------------------------------------------------------*/
allocator_t make_std_heap_allocator() {
    allocator_t allocator;
    allocator.start = NULL;
    allocator.capacity = ~0;
    allocator.used = 0;
    allocator.alloc_fn = std_heap_alloc;
    allocator.free_fn = std_heap_free;
    return allocator;
}

/*----------------------------------------------------------------------*/
void *std_heap_alloc(allocator_t *allocator, size_t size) {
    return malloc(size);
}

/*----------------------------------------------------------------------*/
void std_heap_free(allocator_t *allocator, void *p) {
    free(p);
}
