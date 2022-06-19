#include "branch.h"
#include <string.h>

static word get_cond(char *cond_str) {
    if(!strcmp(cond_str, "eq")) {
        return EQ;
    }
    else if(!strcmp(cond_str, "ne")) {
        return NE;
    }
    else if(!strcmp(cond_str, "ge")) {
        return GE;
    }
    else if(!strcmp(cond_str, "lt")) {
        return LT;
    }
    else if(!strcmp(cond_str, "gt")) {
        return GT;
    }
    else if(!strcmp(cond_str, "le")) {
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

    signed_word label_address = get_instr_address(tokens->string_array[1], symbol_table);
    signed_word offset = (label_address - (signed_word) current_instr_address - PIPELINE_EFFECT) >> SHIFT_VALUE_OFFSET;
    // int sign_extension_length = (sizeof(word) * 8 - 1) - (OFFSET_MSB + SHIFT_VALUE_OFFSET);
    // offset <<= sign_extension_length;
    // offset >>= sign_extension_length;

    cond <<= COND_MASK;
    offset &= FIRST_8_BITS_MASK;
    return instruction | cond | offset;
}

/*short branch(word *instruction, struct RegisterFile *registers, memory_t memory) {
    signed_word offset = ((extract_bits(*instruction, OFFSET_LSB, OFFSET_MSB))) << SHIFT_VALUE_OFFSET;

    signed_word pc_value = offset + (signed_word) registers->program_counter;
    if(pc_value < 0) {
        printf("Cannot be branched to a negative instruction address.\n");
        assert(0);
    }
    registers->program_counter = pc_value;
    return 2;
}*/
