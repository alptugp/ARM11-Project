#include "branch.h"

word branch(tokenized_source_code *tokens, word current_instr_address, symbol_table_t *symbol_table) {
    word instruction = 0x0A000000;
    word cond;

    if (tokens->string_array[0][1]) {
        cond = get_cond(&(tokens->string_array[0][1]));
    } else {
        cond = AL;
    }

    word offset = (get_instr_address(tokens->string_array[1], symbol_table) 
                                    - current_instr_address - PIPELINE_EFFECT) >> SHIFT_VALUE_OFFSET;

    instruction |= cond << COND_MASK;
    instruction |= offset & FIRST_8_BITS_MASK;
    return instruction;
}