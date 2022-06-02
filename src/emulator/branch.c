#include "branch.h"

word branch(word *instruction, RegisterFile *registers, const memory_t memory) {
    if (cond_check(*instruction, registers) == 1) {
        word shifted_offset = (extract_bits(*instruction, OFFSET_START, OFFSET_SIZE + OFFSET_START)) <<= SHIFT_VALUE_OFFSET;
        offset_extend(shifted_offset, SIGN_BIT_OFFSET, SIGN_BIT_OFFSET + SIGN_BIT_SIZE);
        word pc_value = shifted_offset + *registers[PC_INDEX]; 
        is_negative(pc_value) ? printf("Cannot be branched to a negative instruction address.\n") : *registers[PC_INDEX] = pc_value;
        return 1;
    }
    return 0;
}
    
int offset_extend(word shifted_offset, sign_bit_start, sign_bit_end) {
    sign_bit = extract_bits(shifted_offset, sign_bit_start, sign_bit_end);
    if (sign_bit == 1) {
        shifted_offset |= SIGNED_OFFSET_EXTEND;
    }
}

int is_negative(word instruction_address) {
    return extract_bits(instruction_address, SIGN_BIT_INSTRUCTION_ADDRESS, SIGN_BIT_INSTRUCTION_ADDRESS + SIGN_BIT_SIZE) == 1;
}


