#include <assert.h>
#include "emulate_utils.h"

static void validate_begin_end(unsigned int begin, unsigned int end, unsigned int max_end) {
    assert(end <= max_end);
    assert(begin <= end);
}

// Wrapper function around extract_bits specialised for extracting values to be shifted,
// (operand2 and offset in data processing and single data transfer), to simplify code
word extract_shifted_bits(const word shifted, unsigned int interval_lsb, unsigned int interval_size, unsigned int shifted_lsb) {
    return extract_bits(shifted, shifted_lsb + interval_lsb, shifted_lsb + interval_lsb + interval_size - 1);
}

word rotate_right(const word unrotated, const word rotation) {
    const long long unrotated_64bit = unrotated;
    return (unrotated_64bit >> rotation) | (unrotated_64bit << (sizeof(word) * 8 - rotation));
}

word load_register(word instruction, short *cout, int shifted_lsb, struct RegisterFile *registers) {
    word value = registers->general_purpose[extract_shifted_bits(instruction, SHIFTED_RM_OFFSET, SHIFTED_RM_SIZE, shifted_lsb)];
    word shift_amount;
    if(extract_shifted_bits(instruction, SHIFTED_SHIFTER_OFFSET, SHIFTED_SHIFTER_SIZE, shifted_lsb)) {
        shift_amount = registers->general_purpose[extract_shifted_bits(instruction, SHIFTED_RS_OFFSET, SHIFTED_RS_SIZE, shifted_lsb)];
    }
    else {
        shift_amount = extract_shifted_bits(instruction, SHIFTED_INTEGER_OFFSET, SHIFTED_INTEGER_SIZE, shifted_lsb);
    }
    word shift_type = extract_shifted_bits(instruction, SHIFTED_SHIFT_TYPE_OFFSET, SHIFTED_SHIFT_TYPE_SIZE, shifted_lsb);
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

// Extract bits from a 64 bit number in the range [begin, end]
unsigned long extract_bits_64bit(const unsigned long value, unsigned int begin, unsigned int end) {
    validate_begin_end(begin, end, (unsigned int) 64);
    // mask is (end - begin) 1s in the least significant bits
    unsigned long mask = ((1 << (end - begin)) - 1) | (1 << (end - begin));
    return (value >> begin) & mask;
}

// Extracts bits from a word in the range [begin, end]
word extract_bits(const word value, unsigned int begin, unsigned int end) {
    validate_begin_end(begin, end, (unsigned int) 32);
    return (word) (extract_bits_64bit((unsigned long) value, begin, end));
}

void write_bits(word *value, unsigned int begin, unsigned int end, word replacement_bits) {
    validate_begin_end(begin, end, 32);
    // Check replacment only has 1s up to bit at end minus begin
    assert(!(replacement_bits >> (end - begin + 1)));
    
    word ones_after_begin = ((word) 1 << begin) - 1; 
    word ones_before_end = 0;
    for(int bit_index = end + 1; bit_index < sizeof(word) * 8; bit_index++) {
        ones_before_end |= ((word) 1 << bit_index);
    }
    // Mask is 0s in replacement region, 1s everywhere else
    word mask = ones_after_begin | ones_before_end;
    // word mask = ((1 << begin) - 1) | (~(((1 << (end + 1)) - 1)));
    *value &= mask;
    *value |= (replacement_bits << begin);
}

void clear_registers(struct RegisterFile *registers) {
    for (int i = 0; i < NUM_GENERAL_PURPOSE_REGISTERS; i++) {
        registers->general_purpose[i] = 0;
    }
    registers->lr = 0;
    registers->sp = 0;
    registers->program_counter = 0;
    registers->cpsr = 0;
}
