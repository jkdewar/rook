#pragma once
#include "types.h"
#include "allocator/allocator.h"
#include <stdint.h>

typedef struct symbol_table_entry_t {
    const char *name;
    type_t type;
    int32_t stack_pos;
    struct symbol_table_entry_t *next;
} symbol_table_entry_t;

typedef struct {
    symbol_table_entry_t *first;
} symbol_table_t;

void symbol_table_init(symbol_table_t *symbol_table);
void symbol_table_clear(symbol_table_t *symbol_table, allocator_t *allocator);
void symbol_table_insert(symbol_table_t *symbol_table, symbol_table_entry_t *entry);
symbol_table_entry_t *symbol_table_find(symbol_table_t *symbol_table, const char *name);
