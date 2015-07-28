#include "allocator/linear_allocator.h"
#include <stdlib.h>
#include <assert.h>

/*----------------------------------------------------------------------*/
void *linear_alloc(void *user_data, size_t size) {
    linear_allocator_data_t *linear_allocator_data;
    void *allocated;

    linear_allocator_data = (linear_allocator_data_t *)user_data;
    assert(linear_allocator_data->capacity - linear_allocator_data->used >= size);
    allocated = (char*) linear_allocator_data->start + linear_allocator_data->used;
    linear_allocator_data->used += size;
    return allocated;
}
