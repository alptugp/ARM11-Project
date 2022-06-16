#ifndef UTILS
#define UTILS

#include <stdlib.h>
#include "symbol_table/symbol_table.h"
#include "common_typedefs.h"

#define COND_MASK 28
#define MEMSIZE 65536
#define MAX_NUM_INSTRUCTIONS (MEMSIZE / sizeof(binary_instruction))
#define MAX_LINE_CHARS 511

typedef word binary_instruction;
typedef struct {
    char **string_array;
    int size;
} tokenized_source_code; //source code is a string array

#endif
