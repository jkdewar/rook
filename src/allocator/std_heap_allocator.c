#include "allocator/std_heap_allocator.h"
#include <stdlib.h>

/*----------------------------------------------------------------------*/
static void *std_heap_alloc(void *user_data, size_t size) {
    return malloc(size);
}

/*----------------------------------------------------------------------*/
static void std_heap_free(void *user_data, void *p) {
    free(p);
}

/*----------------------------------------------------------------------*/
allocator_t std_heap_allocator = {
    NULL,
    std_heap_alloc,
    std_heap_free,
};
