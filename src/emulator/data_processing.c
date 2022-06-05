#include "data_processing.h"

static void calc_result_and_cout(long res_64bit, word *result, short *cout) {
    *result = (word) res_64bit;
    *cout = extract_bits_64bit(res_64bit, sizeof(word) + 1, sizeof(word) + 1);
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
            carry_bit_location = sizeof(word) - shift_amount;
            value <<= shift_amount;
        case LSR:
            carry_bit_location = shift_amount - 1;
            value >>= shift_amount;
        case ASR:
            carry_bit_location = shift_amount - 1;
            signed_word signed_value = (signed_word) value;
            value = (word) (signed_value >>= shift_amount);
        case ROR:
            carry_bit_location = shift_amount - 1;
            value = (value >> shift_amount) | (value << (sizeof(word) - shift_amount));
        default:
            printf("Invalid shift type: %x", shift_type);
            assert(0);
    }
    *cout = (shift_amount != 0) ? extract_bits(old_value, carry_bit_location, carry_bit_location) 
        : 0;
    return value;
}

short data_processing(word *instruction, struct RegisterFile *registers, memory_t memory) {
    if (cond_check(*instruction, registers) != 0) {
        word dest_reg = extract_bits(*instruction, DEST_REG_LSB, REGISTER_ADDRESS_LENGTH + DEST_REG_LSB);
        word first_operand_reg = registers->general_purpose[extract_bits(*instruction, FIRST_OPERAND_LSB, REGISTER_ADDRESS_LENGTH + FIRST_OPERAND_LSB)];
        word immediate_operand = extract_bits(*instruction, IMMEDIATE_OPERAND_BIT, IMMEDIATE_OPERAND_BIT);
        short *cout;
        word *result;
        long res_64bit;
        // Next line will set cout to the shifter carry out
        word operand2 = (immediate_operand == 1) ? load_immediate_value(instruction, cout) : load_register_value(instruction, registers, cout);

        word opcode = extract_bits(*instruction, OPCODE_LSB, OPCODE_MSB);
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
                *result = operand2 ^ first_operand_reg;
                // set cond flags here
                break;

            case TST: 
            case AND: 
                *result = operand2 & first_operand_reg;
                // set cond flags here 
                break;

            case MOV: 
                *result = operand2;
                // set cond flags here
                break;

            case ORR:
                *result = operand2 | first_operand_reg;
                // set cond flags here 
                break;
            
            default: 
                printf("Invalid opcode to data processing instruction: %x", opcode);
                assert(0);
        }

        // If we have an arithmetic op, we must overwrite cout (which is currently the shifter carry out) and get result
        if(opcode == CMP || opcode == SUB || opcode == ADD || opcode == RSB) {
            calc_result_and_cout(res_64bit, result, cout);
        }


        if(extract_bits(*instruction, SET_COND_CODE_BIT, SET_COND_CODE_BIT)) {
            write_bits(&(registers->cpsr), Z_FLAG_CPSR, Z_FLAG_CPSR, *result == 0);
            write_bits(&(registers->cpsr), N_FLAG_CPSR, N_FLAG_CPSR, extract_bits(*result, SIGN_BIT, SIGN_BIT));
        }
    }
    return 0;
}
