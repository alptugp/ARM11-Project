#include "symbol_table.h"

static int find(const symbol_table_t *symbol_table, const char key[]) {
    for(int i = 0; i < symbol_table->next_pair_index; i++) {
        if(symbol_table->entries[i].key == key) {
            return i;
        }
    }
    // key not in table
    return NO_KEY_CODE;
}

symbol_table_t *initialise_symbol_table() {
    symbol_table_t *symbol_table_ptr = malloc(sizeof(symbol_table_t) + sizeof(symbol_table_entry[INIT_SYMBOL_TABLE_SIZE]));
    assert(symbol_table_ptr);
    symbol_table_ptr->length = INIT_SYMBOL_TABLE_SIZE;
    symbol_table_ptr->next_pair_index = 0;
    return symbol_table_ptr;
}

symbol_table_value get(const symbol_table_t *symbol_table, const char key[]) {
    int key_location = find(symbol_table, key);
    if(key_location == NO_KEY_CODE) {
        printf("Key %s is not in symbol_table.", key);
        assert(0);
    }
    return symbol_table->entries[key_location].value;
}

void put(symbol_table_t *symbol_table, char *key, const symbol_table_value value) {
    int key_location = find(symbol_table, key);
    if(key_location != NO_KEY_CODE) {
        symbol_table_entry new_entry = {key, value};
        symbol_table->entries[key_location] = new_entry;
        return;
    }

    if(symbol_table->next_pair_index >= symbol_table->length) {
        symbol_table = realloc(symbol_table, sizeof(symbol_table_t) + sizeof(symbol_table_entry[symbol_table->length]));
        if(!symbol_table) {
            printf("No space left.");
            assert(0);
        }
    }
    symbol_table_entry new_entry = {key, value};
    symbol_table->entries[symbol_table->next_pair_index] = new_entry;
    symbol_table->next_pair_index++;
}

void free_symbol_table(symbol_table_t *symbol_table) {
    free(symbol_table);
}
