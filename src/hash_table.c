#include "hash_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define MAX_KEY_LEN 127
#define NUM_BIN_BITS 8
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

/*----------------------------------------------------------------------*/
void hash_table_clear(hash_table_t *hash_table, int free_values) {
    size_t i;
    hash_table_entry_t *entry;
    for (i = 0; i < NUM_BINS; ++i) {
        for (entry = hash_table->bins[i]; entry != NULL; entry = entry->next) {
            if (free_values)
                ALLOCATOR_FREE(hash_table->allocator, entry->value);
            ALLOCATOR_FREE(hash_table->allocator, entry);
        }
        hash_table->bins[i] = NULL;
    }
}

/*----------------------------------------------------------------------*/
void hash_table_insert(hash_table_t *hash_table, const char *key, void *value) {
    hash_table_entry_t *new_entry;
    uint32_t bin = HASH(key) & BIN_MASK;
    new_entry = ALLOCATOR_ALLOC(hash_table->allocator, sizeof(hash_table_entry_t));
    assert(strlen(key) < MAX_KEY_LEN);
    strcpy(new_entry->key, key);
    new_entry->value = value;
    new_entry->next = hash_table->bins[bin];
    hash_table->bins[bin] = new_entry;
}

/*----------------------------------------------------------------------*/
void *hash_table_find(hash_table_t *hash_table, const char *key) {
    hash_table_entry_t *entry;
    uint32_t bin = HASH(key) & BIN_MASK;
    for (entry = hash_table->bins[bin]; entry != NULL; entry = entry->next) {
        if (strcmp(key, entry->key) == 0)
            return entry->value;
    }
    return NULL;
}

/*----------------------------------------------------------------------*/
void hash_table_first(hash_table_t *hash_table, hash_table_iter_t *iter) {
    iter->entry = NULL;
    iter->bin = ~0; /* -1, will be incremented to 0 by _next() */
    hash_table_next(hash_table, iter);
}

/*----------------------------------------------------------------------*/
void hash_table_next(hash_table_t *hash_table, hash_table_iter_t *iter) {
    if (iter->entry != NULL && iter->entry->next != NULL) {
        iter->entry = iter->entry->next;
        iter->value = iter->entry->value;
        return;
    }
    iter->bin += 1;
    while (iter->bin < NUM_BINS && hash_table->bins[iter->bin] == NULL) {
        iter->bin += 1;
    }
    if (iter->bin < NUM_BINS) {
        iter->entry = hash_table->bins[iter->bin];
        iter->value = iter->entry->value;
    } else {
        iter->value = NULL;
    }
}
