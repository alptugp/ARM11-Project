#include "utils.h"

typedef symbol_table_t; // TODO - purpose is exact same as a HashMap in Java

word get(symbol_table_t *symbol_table, char key[]);

word put(symbol_table_t *symbol_table, char key[], word value);
