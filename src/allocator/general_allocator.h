#if 0
#include "allocator/allocator.h"

allocator_t make_general_allocator(void *memory, size_t capacity);
void *general_allocator_alloc(allocator_t *, size_t);
void general_allocator_free(allocator_t *, void *);
void general_allocator_debug_dump(allocator_t *);
#endif