#include "hash_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define MAX_KEY_LEN 128
#define NUM_BIN_BITS 12
#define NUM_BINS (1 << NUM_BIN_BITS)
#define BIN_MASK ((1 << NUM_BIN_BITS) - 1)

#define HASH(S) djb2(S)

struct hash_table_entry_t {
    char key[MAX_KEY_LEN + 1];
    void *value;
    struct hash_table_entry_t *next;
};

struct hash_table_t {
    allocator_t *allocator;
    hash_table_entry_t *bins[NUM_BINS];
};

/*----------------------------------------------------------------------*/
static uint32_t djb2(const char *s) {
    const char *c;
    uint32_t h = 0;
    for (c = s; *c != '\0'; ++c) {
        h = h * 33 ^ (uint32_t)*c;
    }
    return h;
}

/*----------------------------------------------------------------------*/
hash_table_t *hash_table_create(allocator_t *allocator) {
    hash_table_t *hash_table;
    hash_table = ALLOCATOR_ALLOC(allocator, sizeof(hash_table_t));
    memset(hash_table, 0, sizeof(hash_table_t));
    hash_table->allocator = allocator;
    return hash_table;
}

#include <stdio.h> /* XXX:jkd */
/*----------------------------------------------------------------------*/
void hash_table_clear(hash_table_t *hash_table) {
    /* TODO:jkd*/
    printf("WARNING: hash_table_clear not implemented\n");
}

/*----------------------------------------------------------------------*/
void hash_table_insert(hash_table_t *hash_table, const char *key, void *value) {
    hash_table_entry_t *new_entry;
    uint32_t bin_index = HASH(key) & BIN_MASK;
    new_entry = ALLOCATOR_ALLOC(hash_table->allocator, sizeof(hash_table_entry_t));
    assert(strlen(key) < MAX_KEY_LEN);
    strcpy(new_entry->key, key);
    new_entry->value = value;
    new_entry->next = hash_table->bins[bin_index];
    hash_table->bins[bin_index] = new_entry;
}

/*----------------------------------------------------------------------*/
void *hash_table_find(hash_table_t *hash_table, const char *key) {
    hash_table_entry_t *entry;
    uint32_t bin_index = HASH(key) & BIN_MASK;
    for (entry = hash_table->bins[bin_index]; entry != NULL; entry = entry->next) {
        if (strcmp(key, entry->key) == 0)
            return entry->value;
    }
    return NULL;
}
