#include "branch.h"
#include <assert.h>

short branch(word *instruction, struct RegisterFile *registers, memory_t memory) {
    if (cond_check(*instruction, registers) == 1) {
        signed_word shifted_offset = ((signed_word) (extract_bits(*instruction, OFFSET_LSB, OFFSET_MSB))) << SHIFT_VALUE_OFFSET;
        signed_word pc_value = shifted_offset + (signed_word) registers->program_counter;
        if(pc_value < 0) {
            printf("Cannot be branched to a negative instruction address.\n");
            assert(0);
        }
        registers->program_counter = pc_value;
    }
    return 0;
}
