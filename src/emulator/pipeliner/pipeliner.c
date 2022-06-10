#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "pipeliner.h"
#include "../instructions/data_processing.h"
#include "../instructions/single_data_transfer.h"
#include "../instructions/multiply.h"
#include "../instructions/branch.h"

// Use of function pointers requires that .c instruction files also included
// TODO: when instructions are moved to separate subdirectory, find way to...
// ...include ALL instruction programs automatically
#include "../instructions/data_processing.c"
#include "../instructions/single_data_transfer.c"
#include "../instructions/multiply.c"
#include "../instructions/branch.c"

static instruction_ptr decode(const word instruction) {
    word opcode = extract_bits(instruction, OPCODE_LSB, OPCODE_MSB);
    if(instruction == TERMINATE_VALUE) {
        return NULL;
    }
    if(opcode == SINGLE_DATA_TRANSFER_OPCODE) {
        return &single_data_transfer;
    }
    if(opcode == BRANCH_OPCODE) {
        return &branch;
    }
    if(extract_bits(instruction, DATA_PROC_I, DATA_PROC_I) == 1) {
        return &data_processing;
    }
    if(extract_bits(instruction, MULTIPLY_OPCODE_LSB, MULTIPLY_OPCODE_MSB) == MULTIPLY_OPCODE) { // compare to 
        return &multiply;
    }
    return &data_processing;
}

// Returns 1 if the conditiion required by the decoded instruction is met by the CPSR state.
// Returns 0 otherwise.
static short cond_check(const word instruction, struct RegisterFile *const registers) {
    const word cond = extract_bits(instruction, COND_LSB, COND_MSB);
    const word z_flag = extract_bits(registers->cpsr, Z_FLAG_CPSR, Z_FLAG_CPSR);
    const word n_flag = extract_bits(registers->cpsr, N_FLAG_CPSR, N_FLAG_CPSR);
    const word v_flag = extract_bits(registers->cpsr, V_FLAG_CPSR, V_FLAG_CPSR);

    const short z_clear = z_flag == 0; 
    const short n_equals_v = n_flag == v_flag;

    return (cond == EQ && !z_clear)
        || (cond == NE && z_clear)
        || (cond == GE && n_equals_v)
        || (cond == LT && !n_equals_v)
        || (cond == GT && (z_clear && n_equals_v))
        || (cond == LE && (!z_clear || (!n_equals_v)))
        || (cond == AL);
}

void pipeline(memory_t main_memory, struct RegisterFile *registers, int num_instructions)   {
    instruction_ptr instr_func = NULL;
    word instr_to_exec;
    word fetched;
    int cycles_since_cleared = 0;
    do {
        if(cycles_since_cleared >= 2) {
            // Execute
            short should_terminate = (instr_to_exec == TERMINATE_VALUE);
            if(!should_terminate && cond_check(instr_to_exec, registers)) {
                assert(instr_func);
                should_terminate = (*instr_func)(&instr_to_exec, registers, main_memory);
            }
            if(should_terminate == 1) {
                break;
            }
            else if(should_terminate == 2) {
                // clear pipeline
                fetched = 0;
                cycles_since_cleared = 0;
            }
        }

        //Decode
        instr_func = decode(fetched);
        instr_to_exec = fetched;

        // Fetch
        fetched = 0;
        // Load next 4 bytes in REVERSE order into fetched,
        // as ARM binary is little-endian but instructions assume big-endianness
        for(int pc_offset = 0; pc_offset < BYTES_PER_INSTR; pc_offset++) {
            unsigned char next_byte = main_memory[registers->program_counter + pc_offset];
            fetched |= (next_byte << (pc_offset * 8));
        }
        registers->program_counter += BYTES_PER_INSTR;

        cycles_since_cleared++;
    } while(1);
}
