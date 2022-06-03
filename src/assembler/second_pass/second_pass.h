#include "../symbol_table/symbol_table.h"

// Translates, line by line, array of asm instructinos in lines to array of binary instructions
binary_instruction *second_pass(char **lines, symbol_table_t *labels_to_addresses);
