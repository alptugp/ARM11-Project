#include "branch.h"

word branch(word *instruction, RegisterFile *registers, const memory_t memory) { 
    if (cond_check(*instruction, registers) == 1) {
        word shifted_offset = (extract_bits(*instruction, OFFSET_LSB, OFFSET_MSB)) <<= SHIFT_VALUE_OFFSET;
        offset_extend(shifted_offset, OFFSET_SIGN_BIT, OFFSET_SIGN_BIT);
        word pc_value = shifted_offset + registers->program_counter;
        is_negative(pc_value) ? printf("Cannot be branched to a negative instruction address.\n") : registers->program_counter = pc_value;
        return 1;
    }
    return 0;
}

int offset_extend(word shifted_offset, short offset_sign_bit) {
    sign_bit = extract_bits(shifted_offset, offset_sign_bit, offset_sign_bit);
    if (offset_sign_bit == 1) {
        shifted_offset |= SIGNED_OFFSET_EXTEND; //Should we define SINGED_OFFSET_EXTEND as MASK_FOR_SIGNED_OFFSET
    }
}

int is_negative(word instruction_address) {
    return extract_bits(instruction_address, INSTRUCTION_ADDRESS_SIGN_BIT, INSTRUCTION_ADDRESS_SIGN_BIT) == 1;
}