#ifndef UTILS
#define UTILS

#include <stdint.h>
#include <stdlib.h>
#include "symbol_table/symbol_table.h"

#define COND_MASK 28;

typedef uint32_t word;
typedef word binary_instruction;
typedef struct {
    char **string_array;
    int size;
} tokenized_source_code; //source code is a string array

#endif
