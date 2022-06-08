#include "single_data_transfer.h"

// Performs a single data transfer instruction.
// Parameters:
//     instruction - the instruction to be executed
//     registers - the registers to be used
//     memory - the memory to be used

short single_data_transfer(word *instruction, struct RegisterFile *registers, memory_t memory) {
    word offset = extract_bits(*instruction, OFFSET_LS_BIT, OFFSET_MS_BIT);
    word rd = extract_bits(*instruction, RD_LS_BIT, RD_MS_BIT);
    word rn = extract_bits(*instruction, RN_LS_BIT, RN_MS_BIT);
    word imm = extract_bits(*instruction, IMM_BIT, IMM_BIT);
    word p = extract_bits(*instruction, P_BIT, P_BIT);
    word u = extract_bits(*instruction, U_BIT, U_BIT);
    word l = extract_bits(*instruction, L_BIT, L_BIT);
    
    word offset_value = offset ? imm : registers->general_purpose[rn];
    if(p) {
        offset_value = offset_value << 2;
    }
    if(u) {
        offset_value = registers->general_purpose[rn] + offset_value;
    } else {
        offset_value = registers->general_purpose[rn] - offset_value;
    }
    if(l) {
        registers->general_purpose[rd] = memory[offset_value];
    } else {
        memory[offset_value] = registers->general_purpose[rd];
    }
    
}
