#include "allocator/linear_allocator.h"
#include <stdlib.h>
#include <assert.h>

/*----------------------------------------------------------------------*/
allocator_t make_linear_allocator(void *memory, size_t capacity) {
    allocator_t allocator;
    allocator.start = memory;
    allocator.capacity = capacity;
    allocator.used = 0;
    allocator.alloc_fn = linear_alloc;
    allocator.free_fn = linear_free;
    return allocator;
}

/*----------------------------------------------------------------------*/
void *linear_alloc(allocator_t *allocator, size_t size) {
    void *allocated;

    assert(allocator->capacity - allocator->used >= size);
    allocated = (char *)allocator->start + allocator->used;
    allocator->used += size;
    return allocated;
}

/*----------------------------------------------------------------------*/
void linear_free(allocator_t *allocator, void *p) {
    /* do nothing */
}
