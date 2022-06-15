#ifndef FIRST_PASS
#define FIRST_PASS

#include "../utils.h"

// Two purposes: 
// 1. Construct symbol_table from lines (array of strings, i.e. 2D array of chars)
// 2. Build up modified_lines by ignoring labels in lines, and adding all other lines
int first_pass(char **raw_lines, symbol_table_t *labels_to_addresses, int num_raw_lines, char **delabelled_lines);

#endif
