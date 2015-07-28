#include "allocator/general_allocator.h"
#include <stdint.h>
#include <assert.h>
#include <stdio.h>

typedef struct allocation_header_t {
    uint8_t is_allocated;
    size_t capacity;
    struct allocation_header_t *prev;
    struct allocation_header_t *next;
} allocation_header_t;

/*----------------------------------------------------------------------*/
void general_allocator_init(general_allocator_data_t *allocator,
                            void *start, size_t capacity) {
    allocation_header_t *header;

    assert(capacity > sizeof(allocation_header_t));
    allocator->start = start;
    header = (allocation_header_t*)allocator->start;
    header->is_allocated = 0;
    header->capacity = capacity - sizeof(allocation_header_t);
    header->prev = NULL;
    header->next = NULL;
}

/*----------------------------------------------------------------------*/
void *general_allocator_alloc(void *user_data, size_t size) {
    general_allocator_data_t *allocator = (general_allocator_data_t*)user_data;
    allocation_header_t *header;
    allocation_header_t *new_header;
    size_t required_size = size + sizeof(allocation_header_t);

    for (header = allocator->start; header != NULL; header = header->next) {
        if (header->is_allocated) continue;
        if (header->capacity < required_size) continue;

        new_header = (allocation_header_t*)((uint8_t*)header + sizeof(allocation_header_t) + size);
        new_header->is_allocated = 0;
        new_header->capacity = header->capacity - required_size;
        new_header->prev = header;
        new_header->next = header->next;

        header->is_allocated = 1;
        header->capacity = size;
        header->next = new_header;

        return (uint8_t*)header + sizeof(allocation_header_t);
    }
    assert(0 && "general_allocator_alloc: out of memory\n");
    return NULL;
}

/*----------------------------------------------------------------------*/
void general_allocator_free(void *user_data, void *p) {
    allocation_header_t *header;

    if (p == NULL) return;
    header = (allocation_header_t *)((uint8_t*)p - sizeof(allocation_header_t));
    assert(header->is_allocated);
    header->is_allocated = 0;

    /* merge back next un-allocated cells with current */
    while (header->next != NULL && !header->next->is_allocated) {
        header->capacity += header->next->capacity + sizeof(allocation_header_t);
        header->next = header->next->next;
        if (header->next != NULL) {
            header->next->prev = header;
        }
    }
    /* merge back current with prev un-allocated cells */
    while (header->prev != NULL && !header->prev->is_allocated) {
        header->prev->capacity += header->capacity + sizeof(allocation_header_t);
        header->prev->next = header->next;
        if (header->next != NULL) {
            header->next->prev = header->prev;
        }
        header = header->prev;
    }
}

/*----------------------------------------------------------------------*/
void general_allocator_debug_dump(general_allocator_data_t *allocator) {
    allocation_header_t *header;

    for (header = allocator->start; header != NULL; header = header->next) {
        printf("is_allocated:%d capacity:%ld\n", header->is_allocated, header->capacity);
    }
}
