#include "emulate.h"

// Extracts bits from a word in the range [begin, end]
word extract_bits(const word value, int begin, int end) {
    word mask = (1 << (end - begin)) - 1;
    return (value >> begin) & mask; 
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
    for (int i = 0; i < GENERAL_PURPOSE_REGISTERS; i++) {
        registers->general_purpose[i] = 0;
    }
    registers->lr = 0;
    registers->sp = 0;
    registers->program_counter = 0;
    registers->cpsr = 0;
}
