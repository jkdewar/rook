#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

/*----------------------------------------------------------------------*/
void symbol_table_init(symbol_table_t *symbol_table) {
    symbol_table->first = NULL;
}

/*----------------------------------------------------------------------*/
void symbol_table_clear(symbol_table_t *symbol_table, allocator_t *allocator) {
    symbol_table_entry_t *entry;
    symbol_table_entry_t *next;
    for (entry = symbol_table->first; entry != NULL; entry = next) {
        next = entry->next;
        ALLOCATOR_FREE(allocator, entry);
    }
    symbol_table->first = NULL;
}

/*----------------------------------------------------------------------*/
void symbol_table_insert(symbol_table_t *symbol_table, symbol_table_entry_t *entry) {
    entry->next = symbol_table->first;
    symbol_table->first = entry;
}

/*----------------------------------------------------------------------*/
symbol_table_entry_t *symbol_table_find(symbol_table_t *symbol_table, const char *name) {
    symbol_table_entry_t *entry;
    for (entry = symbol_table->first; entry != NULL; entry = entry->next) {
        if (strcmp(name, entry->name) == 0)
            return entry;
    }
    return 0;
}
