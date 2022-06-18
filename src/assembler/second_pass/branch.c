#include "branch.h"
#include <string.h>

static word get_cond(char *cond_str) {
    if(!strcmp(cond_str, "EQ")) {
        return EQ;
    }
    else if(!strcmp(cond_str, "NE")) {
        return NE;
    }
    else if(!strcmp(cond_str, "GE")) {
        return GE;
    }
    else if(!strcmp(cond_str, "LT")) {
        return LT;
    }
    else if(!strcmp(cond_str, "GT")) {
        return GT;
    }
    else if(!strcmp(cond_str, "LE")) {
        return LE;
    }
    else {
        printf("Invalid cond string received by get_cond.\n");
        assert(0);
    }
}

static word get_instr_address(char *expression, symbol_table_t *symbol_table) {
    if(!contains(symbol_table, expression)) {
        printf("Branching to an expression which is not a label - behaviour not defined in spec.\n");
        assert(0);
    }
    return get(symbol_table, expression);
}

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