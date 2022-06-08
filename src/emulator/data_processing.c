#include "data_processing.h"
#include <assert.h>

static void calc_result_and_cout(short write_result, short write_cout, long res_64bit, word *dest_reg, short *cout) {
    if(write_result) {
        *dest_reg = (word) res_64bit;
    }
    if(write_cout) {
        *cout = (word) (extract_bits_64bit((unsigned long) res_64bit, sizeof(word) * 8 + 1, sizeof(word) * 8 + 1));
    }
}

static word load_immediate_value(word *instruction, short *cout) {
    word unrotated = extract_bits(*instruction, OPERAND2_IMM_LSB, OPERAND2_IMM_MSB);
    word rotation = 2 * extract_bits(*instruction, ROTATE_LSB, ROTATE_MSB);
    return (unrotated <<= rotation);
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
            value = (value >> shift_amount) | (value << (sizeof(word) * 8 - shift_amount));
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
    word first_operand_reg = registers->general_purpose[extract_bits(*instruction, FIRST_OPERAND_LSB, REGISTER_ADDRESS_LENGTH - 1 + FIRST_OPERAND_LSB)];
    word immediate_operand = extract_bits(*instruction, IMMEDIATE_OPERAND_BIT, IMMEDIATE_OPERAND_BIT);
    short cout;
    long res_64bit;
    // Next line will set cout to the shifter carry out
    word operand2 = (immediate_operand == 1) ? load_immediate_value(instruction, &cout) : load_register_value(instruction, registers, &cout);

    word opcode = extract_bits(*instruction, MNEMONIC_LSB, MNEMONIC_MSB);
    switch (opcode) {
        // ARITHMETIC OPERATIONS
        case CMP:
        case SUB:
            res_64bit = first_operand_reg - operand2;
            break;

        case ADD:
            res_64bit = operand2 + first_operand_reg;
            break;
        
        case RSB:
            res_64bit = operand2 - first_operand_reg;
            break;

        // LOGICAL OPERATIONS
        case TEQ:
        case EOR:
            res_64bit = operand2 ^ first_operand_reg;
            break;

        case TST: 
        case AND: 
            res_64bit = operand2 & first_operand_reg;
            // set cond flags here 
            break;

        case MOV: 
            res_64bit = operand2;
            // set cond flags here
            break;

        case ORR:
            res_64bit = operand2 | first_operand_reg;
            // set cond flags here 
            break;
        
        default: 
            printf("Invalid opcode to data processing instruction: %x", opcode);
            assert(0);
    }

    short overwite_cout = (opcode == CMP || opcode == SUB || opcode == ADD || opcode == RSB);
    short write_result = !(opcode == TST || opcode == TEQ || opcode == CMP);
    calc_result_and_cout(write_result, overwite_cout, res_64bit, dest_reg, &cout);

    if(extract_bits(*instruction, SET_COND_CODE_BIT, SET_COND_CODE_BIT)) {
        write_bits(&(registers->cpsr), Z_FLAG_CPSR, Z_FLAG_CPSR, *dest_reg == 0);
        write_bits(&(registers->cpsr), N_FLAG_CPSR, N_FLAG_CPSR, extract_bits(*dest_reg, SIGN_BIT, SIGN_BIT));
    }

    return 0;
}
