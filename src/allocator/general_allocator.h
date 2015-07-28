#include "allocator/allocator.h"

typedef struct {
    void *start;
} general_allocator_data_t;

void general_allocator_init(general_allocator_data_t *allocator, void *start, size_t capacity);
void *general_allocator_alloc(void *, size_t);
void general_allocator_free(void *, void *);
void general_allocator_debug_dump(general_allocator_data_t *allocator);
