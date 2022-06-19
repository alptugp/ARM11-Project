#ifndef BRANCH
#define BRANCH

#include "../utils.h"

#define PIPELINE_EFFECT 8
#define SHIFT_VALUE_OFFSET 2
#define FIRST_8_BITS_MASK 0x00ffffff

typedef enum {
    EQ,
    NE,
    GE = 10,
    LT,
    GT,
    LE
} COND_CODE;


word branch(tokenized_source_code *tokens, word current_instr_address, symbol_table_t *symbol_table);

#endif
