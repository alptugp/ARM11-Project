#include "../symbol_table/symbol_table.h"

// Two purposes: 
// 1. Construct symbol_table from lines (array of strings, i.e. 2D array of chars)
// 2. Build up modified_lines by ignoring labels in lines, and adding all other lines
char **first_pass(char **lines, symbol_table_t *labels_to_addresses);
