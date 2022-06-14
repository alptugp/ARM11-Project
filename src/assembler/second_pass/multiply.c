#include "../utils.h"
#include "multiply.h"

word multiply(tokenized_source_code *tokens) {
    word Rd = get_register_adress(tokens->string_array[1]);
    word Rm = get_register_adress(tokens->string_array[2]);
    word Rs = get_register_adress(tokens->string_array[3]);
    word cond = AL;
    word S = 0;
    word bits_4_to_7 = 9;
    word instruction;
    word Rn, A;

    if (tokens->size == 4) {
        // is mul instruction
        Rn = 0;
        A = 0;
    } else {
        // is mla instruction
        Rn = get_register_adress(tokens->string_array[4]);
        A = 1;
    }

    // all fields are placed to their corresponding bits in the multiply 
    // instruction by using shifting and OR

    instruction |= Rd << MUL_RD_MASK;
    instruction |= Rs << MUL_RS_MASK;
    instruction |= cond << COND_MASK;
    instruction |= S << MUL_S_MASK;
    instruction |= A << MUL_A_MASK;
    instruction |= bits_4_to_7 << MUL_BITS_4_TO_7_MASK;
    
    if (A) {
        instruction |= Rn << MUL_RN_MASK;
    }

    return instruction;
}
   