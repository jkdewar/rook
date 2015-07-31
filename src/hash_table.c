#include "hash_table.h"
#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_MAX_KEY_LENGTH 1024

struct hash_table_entry_t {
    char key[HASH_TABLE_MAX_KEY_LENGTH + 1];
    void *value;
    struct hash_table_entry_t *next;
};

/*----------------------------------------------------------------------*/
hash_table_t *hash_table_create(allocator_t *allocator) {
    hash_table_t *hash_table;
    hash_table = ALLOCATOR_ALLOC(allocator, sizeof(hash_table_t));
    hash_table->allocator = allocator;
    hash_table->first = NULL;
    return hash_table;
}

/*----------------------------------------------------------------------*/
void hash_table_clear(hash_table_t *hash_table) {
    hash_table_entry_t *entry;
    hash_table_entry_t *next;
    for (entry = hash_table->first; entry != NULL; entry = next) {
        next = entry->next;
        ALLOCATOR_FREE(hash_table->allocator, entry);
    }
    hash_table->first = NULL;
}

/*----------------------------------------------------------------------*/
void hash_table_insert(hash_table_t *hash_table, const char *key, void *value) {
    hash_table_entry_t *new_entry;
    new_entry = ALLOCATOR_ALLOC(hash_table->allocator, sizeof(hash_table_entry_t));
    strcpy(new_entry->key, key);
    new_entry->value = value;
    new_entry->next = hash_table->first;
    hash_table->first = new_entry;
}

/*----------------------------------------------------------------------*/
void *hash_table_find(hash_table_t *hash_table, const char *key) {
    hash_table_entry_t *entry;
    for (entry = hash_table->first; entry != NULL; entry = entry->next) {
        if (strcmp(key, entry->key) == 0)
            return entry->value;
    }
    return NULL;
}
