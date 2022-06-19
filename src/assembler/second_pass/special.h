#ifndef SPECIAL
#define SPECIAL

#include "../utils.h"

#define EQ 0
#define ANDEQ_RESULT 0x00000000

typedef enum {
    ANDEQ, 
    LSL_SPECIAL
} SPECIAL_OPCODE;

word special(tokenized_source_code *tokens);

#endif 
