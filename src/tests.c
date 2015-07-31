#include "hash_table.h"
#include "allocator/std_heap_allocator.h"
#include <stdio.h>

void test() {
    allocator_t allocator = make_std_heap_allocator();
    hash_table_t *ht = hash_table_create(&allocator);
    int i = 42;
    int j = 33;
    void *p;

    hash_table_insert(ht, "hello", &i);
    hash_table_insert(ht, "world", &j);
    p = hash_table_find(ht, "world");
    printf("%p\n", p);
    printf("%d\n", *((int*)p));
    p = hash_table_find(ht, "hello");
    printf("%p\n", p);
    printf("%d\n", *((int*)p));
    p = hash_table_find(ht, "HELL");
    printf("%p\n", p);
}
