#ifndef MULTIPLY
#define MULTIPLY

#include "../utils.h"

#define AL 14; // must be defined in top
#define MUL_RD_MASK 16;
#define MUL_RS_MASK 8;
#define MUL_S_MASK 20; 
#define MUL_A_MASK 21;
#define MUL_BITS_4_TO_7_MASK 4;
#define MUL_RN_MASK 12;

word multiply(tokenized_source_code *tokens);

#endif

