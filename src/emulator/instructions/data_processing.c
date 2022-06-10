#include "data_processing.h"
#include <assert.h>

static short no_borrow_sub(long long minuend, long long subtrahend, long long result) {
    // Cast all values to max possible # of bits (at least 64), and check if 
    // subtraction valid in 64-bit - an overflowed 32-bit sub won't be valid in 
    // 64 bits. Sub not valid -> overflow produced -> borrow.
    // NOTE that this function cannot detect 64-bit subtraction overflows,
    // but it does not need to.
    return result == minuend - subtrahend;
}

static word load_immediate(word instruction, short *cout, int shifted_lsb) {
    word unrotated = extract_shifted_bits(instruction, SHIFTED_IMM_OFFSET, SHIFTED_IMM_SIZE, shifted_lsb);
    word rotation = 2 * extract_shifted_bits(instruction, SHIFTED_ROTATE_OFFSET, SHIFTED_ROTATE_SIZE, shifted_lsb);
    return rotate_right(unrotated, rotation);
}

short data_processing(word *instruction, struct RegisterFile *registers, memory_t memory) {
    word *dest_reg = &(registers->general_purpose[extract_bits_64bit(*instruction, DEST_REG_LSB, REGISTER_ADDRESS_LENGTH - 1 + DEST_REG_LSB)]);
    word operand1 = registers->general_purpose[extract_bits(*instruction, FIRST_OPERAND_LSB, REGISTER_ADDRESS_LENGTH - 1 + FIRST_OPERAND_LSB)];
    short cout;
    long res_64bit;
    // Next line will set cout to the shifter carry out after shifting the value
    // word operand2 = (to_load_immediate == 1) ? load_immediate(instruction, &cout, 0) : load_register(instruction, &cout, 0, registers);
    word operand2;
    if(extract_bits(*instruction, IMMEDIATE_OPERAND_BIT, IMMEDIATE_OPERAND_BIT)) {
        operand2 = load_immediate(*instruction, &cout, OFFSET_LSB);
    }
    else {
        operand2 = load_register(*instruction, &cout, OFFSET_LSB, registers);
    }

    word opcode = extract_bits(*instruction, MNEMONIC_LSB, MNEMONIC_MSB);
    switch (opcode) {
        // ARITHMETIC OPERATIONS
        case CMP:
        case SUB:
            res_64bit = operand1 - operand2;
            break;

        case ADD:
            res_64bit = operand2 + operand1;
            break;
        
        case RSB:
            res_64bit = operand2 - operand1;
            break;

        // LOGICAL OPERATIONS
        case TEQ:
        case EOR:
            res_64bit = operand2 ^ operand1;
            break;

        case TST: 
        case AND: 
            res_64bit = operand2 & operand1;
            break;

        case MOV: 
            res_64bit = operand2;
            break;

        case ORR:
            res_64bit = operand2 | operand1;
            break;
        
        default: 
            printf("Invalid opcode to data processing instruction: %x", opcode);
            assert(0);
    }
    
    word res_32bit = (word) res_64bit;
    if(opcode == ADD) {
        cout = (word) (extract_bits_64bit((unsigned long) res_64bit, sizeof(word) * 8, sizeof(word) * 8));
    }
    else if(opcode == CMP || opcode == SUB) {
        cout = no_borrow_sub(operand1, operand2, res_32bit);
    }
    else if(opcode == RSB) {
        cout = no_borrow_sub(operand2, operand1, res_32bit);
    }
    if(!(opcode == TST || opcode == TEQ || opcode == CMP)) {
        *dest_reg = res_32bit;
    }

    if(extract_bits(*instruction, SET_COND_CODE_BIT, SET_COND_CODE_BIT)) {
        write_bits(&(registers->cpsr), Z_FLAG_CPSR, Z_FLAG_CPSR, res_32bit == 0);
        write_bits(&(registers->cpsr), N_FLAG_CPSR, N_FLAG_CPSR, extract_bits(res_32bit, SIGN_BIT, SIGN_BIT));
        write_bits(&(registers->cpsr), C_FLAG_CPSR, C_FLAG_CPSR, extract_bits(cout, 0, 0));
    }

    return 0;
}
