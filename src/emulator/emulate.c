#include <assert.h>
#include "emulate.h"

static void validate_begin_end(unsigned int begin, unsigned int end, unsigned int max_end) {
    assert(end <= max_end);
    assert(begin <= end);
}

// Extract bits from a 64 bit number in the range [begin, end]
word extract_bits_64bit(const long value, unsigned int begin, unsigned int end) {
    validate_begin_end(begin, end, (unsigned int) 64);
    long mask = (1 << (end - begin)) - 1;
    return (value >> begin) & mask;
}


// Extracts bits from a word in the range [begin, end]
word extract_bits(const word value, unsigned int begin, unsigned int end) {
    validate_begin_end(begin, end, (unsigned int) 32);
    long long_res = extract_bits_64bit((long) value, begin, end);
    return (word) long_res; 
}

void write_bits(word *value, unsigned int begin, unsigned int end, word replacement_bits) {
    validate_begin_end(begin, end, 32);
    // Check replacment only has 1s up to bit at end minus begin
    assert(!(replacement_bits >> (end - begin)));
    
    // Mask is 0s in replacement region, 1s everywhere else
    word mask = ((1 << begin) - 1) | (~(((1 << (end + 1)) - 1)));
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
