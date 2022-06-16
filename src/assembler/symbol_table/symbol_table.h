#ifndef SYMBOL_TABLE
#define SYMBOL_TABLE

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include "../common_typedefs.h"

#define NO_KEY_CODE -1
#define INIT_SYMBOL_TABLE_SIZE 1000

typedef word symbol_table_value;

typedef struct symbol_table_entry {
    char *key;
    symbol_table_value value;
} symbol_table_entry;

typedef struct symbol_table_t {
    int length;
    int next_pair_index;
    int max_key_length;
    symbol_table_entry *entries;
} symbol_table_t;

symbol_table_t initialise_symbol_table();

symbol_table_value get(const symbol_table_t *symbol_table, const char key[]);

void put(symbol_table_t *symbol_table, char key[], const symbol_table_value value);

void free_symbol_table(symbol_table_t *symbol_table);

void print_symbol_table(symbol_table_t *symbol_table);

#endif
