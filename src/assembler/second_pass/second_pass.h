#ifndef SECOND_PASS
#define SECOND_PASS

#include "../utils.h"

// Translates, line by line, array of asm instructinos in lines to array of binary instructions
int second_pass(char **lines, symbol_table_t *labels_to_addresses, binary_instruction *instructions);

#endif
