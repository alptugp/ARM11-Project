#include <stdint.h>

#define COND_MASK 28;

typedef uint32_t word;
typedef word binary_instruction;
typedef symbol_table_t;
typedef struct {
    char **string_array;
    int size;
} tokenized_source_code; //source code is a string array

