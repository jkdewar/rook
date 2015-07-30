static int blah;
#if 0
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
allocator_t make_general_allocator(void *memory, size_t capacity) {
    allocation_header_t *header;
    allocator_t allocator;

    header = memory;
    header->is_allocated = 0;
    header->capacity = capacity;
    header->prev = NULL;
    header->next = NULL;

    allocator.start = memory;
    allocator.capacity = capacity;
    allocator.used = 0;
    allocator.alloc_fn = general_allocator_alloc;
    allocator.free_fn = general_allocator_free;
    return allocator;
}

/*----------------------------------------------------------------------*/
void *general_allocator_alloc(allocator_t *allocator, size_t size) {
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
void general_allocator_free(allocator_t *allocator, void *p) {
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
void general_allocator_debug_dump(allocator_t *allocator) {
    allocation_header_t *header;

    for (header = allocator->start; header != NULL; header = header->next) {
        printf("is_allocated:%d capacity:%ld\n", header->is_allocated, header->capacity);
    }
}
#endif
