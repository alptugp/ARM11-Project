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

static word rotate_right(const word unrotated, const word rotation) {
    return (unrotated >> rotation) | (unrotated << (sizeof(word) * 8 - rotation));
}

static word load_immediate_value(word *instruction, short *cout) {
    word unrotated = extract_bits(*instruction, OPERAND2_IMM_LSB, OPERAND2_IMM_MSB);
    word rotation = 2 * extract_bits(*instruction, ROTATE_LSB, ROTATE_MSB);
    return rotate_right(unrotated, rotation);
}

static word load_register_value(word *instruction, struct RegisterFile *registers, short *cout) {
    word value = registers->general_purpose[extract_bits(*instruction, RM_LSB, RM_MSB)];
    word shift_amount;
    if(extract_bits(*instruction, SHIFTER_BIT, SHIFTER_BIT)) {
        shift_amount = registers->general_purpose[extract_bits(*instruction, SHIFT_REGISTER_LSB, SHIFT_REGISTER_MSB)];
    }
    else {
        shift_amount = extract_bits(*instruction, SHIFT_INTEGER_LSB, SHIFT_INTEGER_MSB);
    }
    word shift_type = extract_bits(*instruction, SHIFT_TYPE_LSB, SHIFT_TYPE_MSB);
    word old_value = value;
    unsigned int carry_bit_location;
    switch(shift_type) {
        case LSL:
            carry_bit_location = sizeof(word) * 8 - shift_amount;
            value <<= shift_amount;
            break;
        case LSR:
            carry_bit_location = shift_amount - 1;
            value >>= shift_amount;
            break;
        case ASR:
            carry_bit_location = shift_amount - 1;
            signed_word signed_value = (signed_word) value;
            value = (word) (signed_value >>= shift_amount);
            break;
        case ROR:
            carry_bit_location = shift_amount - 1;
            value = rotate_right(value, shift_amount);
            break;
        default:
            printf("Invalid shift type: %x", shift_type);
            assert(0);
    }
    *cout = (shift_amount != 0) ? extract_bits(old_value, carry_bit_location, carry_bit_location) 
        : 0;
    return value;
}

short data_processing(word *instruction, struct RegisterFile *registers, memory_t memory) {
    word *dest_reg = &(registers->general_purpose[extract_bits_64bit(*instruction, DEST_REG_LSB, REGISTER_ADDRESS_LENGTH - 1 + DEST_REG_LSB)]);
    word operand1 = registers->general_purpose[extract_bits(*instruction, FIRST_OPERAND_LSB, REGISTER_ADDRESS_LENGTH - 1 + FIRST_OPERAND_LSB)];
    word load_immediate = extract_bits(*instruction, IMMEDIATE_OPERAND_BIT, IMMEDIATE_OPERAND_BIT);
    short cout;
    long res_64bit;
    // Next line will set cout to the shifter carry out
    word operand2 = (load_immediate == 1) ? load_immediate_value(instruction, &cout) : load_register_value(instruction, registers, &cout);

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
            // set cond flags here 
            break;

        case MOV: 
            res_64bit = operand2;
            // set cond flags here
            break;

        case ORR:
            res_64bit = operand2 | operand1;
            // set cond flags here 
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
