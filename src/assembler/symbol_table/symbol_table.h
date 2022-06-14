#include <stdbool.h>

#define MAX_KEY_LENGTH 511
#define NO_KEY_CODE -1
#define INIT_SYMBOL_TABLE_SIZE 1

typedef symbol_table_value; // TODO - union

typedef struct symbol_table_entry {
    char key[MAX_KEY_LENGTH];
    symbol_table_value value;
} symbol_table_entry;

typedef struct symbol_table_t {
    int length;
    int next_pair_index;
    symbol_table_entry entries[];
} symbol_table_t;

symbol_table_t *initialise_symbol_table();

symbol_table_value get(const symbol_table_t *symbol_table, const char key[]);

void put(symbol_table_t *symbol_table, const char key[], const symbol_table_value value);

void free_symbol_table(symbol_table_t *symbol_table);
