#include "branch.h"
#include <assert.h>

short branch(word *instruction, struct RegisterFile *registers, memory_t memory) {
    signed_word offset = ((extract_bits(*instruction, OFFSET_LSB, OFFSET_MSB))) << SHIFT_VALUE_OFFSET;
    int sign_extension_length = (sizeof(word) * 8 - 1) - (OFFSET_MSB + SHIFT_VALUE_OFFSET);
    offset <<= sign_extension_length;
    offset >>= sign_extension_length;

    signed_word pc_value = offset + (signed_word) registers->program_counter;
    if(pc_value < 0) {
        printf("Cannot be branched to a negative instruction address.\n");
        assert(0);
    }
    registers->program_counter = pc_value;
    return 2;
}
