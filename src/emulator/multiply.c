#include "multiply.h"


short multiply(word *instruction, struct RegisterFile *registers, memory_t memory) {
    word *rd, *rn, *rs, *rm;
    int reg_locs[NUM_REGISTERS] = {RD_LOC, RN_LOC, RS_LOC, RM_LOC};
    word *reg_ptrs[NUM_REGISTERS] = {rd, rn, rs, rm};
    for(int reg_index = 0; reg_index < NUM_REGISTERS; reg_index++) {
        int reg_loc = reg_locs[reg_index];
        *reg_ptrs[reg_index] = registers->general_purpose[extract_bits(*instruction, reg_loc - REGISTER_ADDRESS_LENGTH + 1, reg_loc)];
    }

    short accumulate = extract_bits(*instruction, ACC_BIT, ACC_BIT);
    *rd = (*rm * *rs) + (accumulate ? *rn : 0);
    registers->general_purpose[extract_bits(*instruction, RD_LOC - REGISTER_ADDRESS_LENGTH + 1, RD_LOC)] = *rd;

    short to_set = extract_bits(*instruction, SET_BIT, SET_BIT);
    if(to_set) {
        write_bits(&registers->cpsr, N_FLAG_CPSR, N_FLAG_CPSR, extract_bits(*rd, RES_SIGN_BIT, RES_SIGN_BIT));
        write_bits(&registers->cpsr, Z_FLAG_CPSR, Z_FLAG_CPSR, *rd == 0);
    }
}
