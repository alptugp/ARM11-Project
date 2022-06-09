#include "single_data_transfer.h"

// Performs a single data transfer instruction.
// Parameters:
//     instruction - the instruction to be executed
//     registers - the registers to be used
//     memory - the memory to be used

static short address_in_bounds(memory_t memory, word start_address) {
    if(start_address >= (MEMSIZE - sizeof(word) + 1)) {
        printf("Error: Out of bounds memory access at address 0x%0*x\n", HEX_OUTPUT_LENGTH, start_address);
        return 1;
    }
    return 0;
}

static word read_word(memory_t memory, word start_address) {
    word read = 0;
    for(int offset_from_start = 0; offset_from_start < sizeof(word); offset_from_start++) {
        read |= (((unsigned char) memory[offset_from_start + start_address]) << (offset_from_start * 8));
    }
    return read;
}

static void write_word(memory_t memory, word start_address, word write) {     
    // for each byte in the word, write the byte to the memory
    for (int offset_from_start = 0; offset_from_start < sizeof(word); offset_from_start++) {
        memory[offset_from_start + start_address] = (write >> (offset_from_start * 8));
    }
    
}


short single_data_transfer(word *instruction, struct RegisterFile *registers, memory_t memory) {
    word rd_index = extract_bits(*instruction, RD_LSB, RD_MSB);
    word rn_index = extract_bits(*instruction, RN_LSB, RN_MSB);
    word imm = extract_bits(*instruction, IMM_BIT, IMM_BIT);
    word p = extract_bits(*instruction, P_BIT, P_BIT);
    word u = extract_bits(*instruction, U_BIT, U_BIT);
    word l = extract_bits(*instruction, L_BIT, L_BIT);
    
    short cout_dummy;
    word offset_value = imm ? load_register(*instruction, &cout_dummy, SHIFTED_LSB, registers) : extract_bits(*instruction, SHIFTED_LSB, SHIFTED_MSB);
    word addr = registers->general_purpose[rn_index] + (!p ? 0 : (u ? offset_value : (-offset_value)));

    if(address_in_bounds(memory, addr)) {
        return 0;
    }

    if(l) {
        registers->general_purpose[rd_index] = read_word(memory, addr);
    } else {
        write_word(memory, addr, registers->general_purpose[rd_index]);
    }
    
    if(!p) {
        registers->general_purpose[rn_index] += (u ? offset_value : (-offset_value));
    }
}
