#ifndef MULTIPLY
#define MULTIPLY

#include "../utils.h"
#include "second_pass_utils.h"

#define MUL_RD_MASK 16
#define MUL_RS_MASK 8
#define MUL_S_MASK 20 
#define MUL_A_MASK 21
#define MUL_BITS_4_TO_7_MASK 4
#define MUL_RN_MASK 12
#define MUL_RM_MASK 0

word multiply(tokenized_source_code *tokens);

#endif

