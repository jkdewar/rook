#include "hash_table.h"
#include "allocator/std_heap_allocator.h"
#include <stdio.h>
#include <assert.h>

void test() {
    allocator_t allocator = make_std_heap_allocator();
    hash_table_t *ht = hash_table_create(&allocator);
    int num[100];
    void *p;
    int i;
    char buf[256];

    for (i = 0; i < 100; ++i) {
        num[i] = 100 * i;
    }
    for (i = 0; i < 10000; ++i) {
        sprintf(buf, "HAHA%d", i);
        hash_table_insert(ht, buf, &num[i % 100]);
    }
    sprintf(buf, "HAHA9981");
    for (i = 0; i < 1000000; ++i) {
        p = hash_table_find(ht, buf);
        if ((i & 1023) == 0) printf("%d/%d\n", i, 100000);
    }
}
