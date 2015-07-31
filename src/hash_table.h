#pragma once
#include "types.h"
#include "allocator/allocator.h"
#include <stdint.h>

typedef struct hash_table_entry_t hash_table_entry_t;

typedef struct {
    allocator_t *allocator;
    hash_table_entry_t *first;
} hash_table_t;

hash_table_t *hash_table_create(allocator_t *allocator);
void hash_table_destroy(hash_table_t *hash_table);
void hash_table_clear(hash_table_t *hash_table);
void hash_table_insert(hash_table_t *hash_table, const char *key, void *value);
void *hash_table_find(hash_table_t *hash_table, const char *key);
