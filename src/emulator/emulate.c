#include <assert.h>
#include "emulate.h"

word extract_bits_64bit(const long value, unsigned int begin, unsigned int end) {
    validate_begin_end(begin, end, 64);
    long mask = (1 << (end - begin)) - 1;
    return (value >> begin) & mask;
}

static void validate_begin_end(unsigned int begin, unsigned int end, unsigned int max_end) {
    assert(end <= max_end);
    assert(begin <= end);
}

// Extracts bits from a word in the range [begin, end]
word extract_bits(const word value, unsigned int begin, unsigned int end) {
    validate_begin_end(begin, end, 32);
    long long_res = extract_bits_64bit((long) value, begin, end);
    return (word) long_res; 
}

void write_bits(word *value, unsigned int begin, unsigned int end, word replacement_bits) {
    validate_begin_end(begin, end, 32);
    // Check replacment only has 1s up to bit at end minus begin
    assert(!(replacement_bits >> (end - begin)));
    
    // Mask is 0s in replacement region, 1s everywhere else
    word mask = ((1 << begin) - 1) | (~((1 << (end + 1) - 1)));
    *value &= mask;
    *value |= (replacement_bits << begin);
}

// Returns 1 if the conditiion required by the decoded instruction is met by the CPSR state.
// Returns 0 otherwise.
short cond_check(const word instruction, struct RegisterFile *const registers) {
    const word cond = extract_bits(instruction, COND_LSB, COND_MSB);
    const word z_flag = extract_bits(registers->cpsr, Z_FLAG_CPSR, Z_FLAG_CPSR);
    const word n_flag = extract_bits(registers->cpsr, N_FLAG_CPSR, N_FLAG_CPSR);
    const word v_flag = extract_bits(registers->cpsr, V_FLAG_CPSR, V_FLAG_CPSR);

    const z_clear = z_flag == 0; 
    const n_equals_v = n_flag == v_flag;

    return (cond == EQ && !z_clear)
        || (cond == NE && z_clear)
        || (cond == GE && n_equals_v)
        || (cond == LT && !n_equals_v)
        || (cond == GT && (z_clear && n_equals_v))
        || (cond == LE && (!z_clear || (!n_equals_v)))
        || (cond == AL);
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
