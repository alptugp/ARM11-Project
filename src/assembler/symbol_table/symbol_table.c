#include "symbol_table.h"
#include <string.h>

static int find(const symbol_table_t *symbol_table, const char key[]) {
    for(int i = 0; i < symbol_table->next_pair_index; i++) {
        if(symbol_table->entries[i].key == key) {
            return i;
        }
    }
    // key not in table
    return NO_KEY_CODE;
}

symbol_table_t initialise_symbol_table(int max_key_length) {
    symbol_table_entry *entries = calloc(INIT_SYMBOL_TABLE_SIZE, sizeof(symbol_table_entry));
    assert(entries);
    symbol_table_t new_symbol_table = {INIT_SYMBOL_TABLE_SIZE, 0, max_key_length, entries};
    return new_symbol_table;
}

symbol_table_value get(const symbol_table_t *symbol_table, const char key[]) {
    int key_location = find(symbol_table, key);
    if(key_location == NO_KEY_CODE) {
        printf("Key %s is not in symbol_table.", key);
        assert(0);
    }
    return symbol_table->entries[key_location].value;
}

bool contains(const symbol_table_t *symbol_table, const char key[]) {
    return !(find(symbol_table, key) == NO_KEY_CODE);
}

static symbol_table_entry create_new_entry(char *key, const symbol_table_value value, int max_key_length) {
    // Key may point to a string on the stack which could then be destroyed; 
    // so we should copy it into a heap-allocated key string
    char *heap_key = malloc(max_key_length);
    strcpy(heap_key, key);
    symbol_table_entry new_entry = {heap_key, value};
    return new_entry;
}

void put(symbol_table_t *symbol_table, char *key, const symbol_table_value value) {
    int key_location = find(symbol_table, key);
    if(key_location != NO_KEY_CODE) {
        symbol_table->entries[key_location] = create_new_entry(key, value, symbol_table->max_key_length);
        return;
    }

    if(symbol_table->next_pair_index >= symbol_table->length) {
        symbol_table->length += 1;
        symbol_table->entries = realloc(symbol_table->entries, symbol_table->length * sizeof(symbol_table_entry));
        if(!symbol_table) {
            printf("No space left.");
            assert(0);
        }
    }

    symbol_table->entries[symbol_table->next_pair_index] = create_new_entry(key, value, symbol_table->max_key_length);
    symbol_table->next_pair_index++;
}

void free_symbol_table(symbol_table_t *symbol_table) {
    free(symbol_table->entries);
}

void print_symbol_table(symbol_table_t *symbol_table) {
    for(int i = 0; i < symbol_table->next_pair_index; i++) {
        symbol_table_entry entry = symbol_table->entries[i];
        symbol_table_value got = get(symbol_table, entry.key);

        if(got != entry.value) {
            printf("WARNING: {%s, %d} is in symbol table but get(%s) = %d\n", entry.key, entry.value, entry.key, got);
        }
        else {
            printf("Entry %d: key is %s, value in decimal is %d\n", i, entry.key, entry.value);
        }
    }
}
