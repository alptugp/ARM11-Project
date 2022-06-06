#include <stdio.h>
#include <stdint.h>
#include "pipeliner.h"
#include "../terminate.h"
#include "../data_processing.h"
#include "../single_data_transfer.h"
#include "../multiply.h"
#include "../branch.h"

// Use of function pointers requires that .c instruction files also included
// TODO: when instructions are moved to separate subdirectory, find way to...
// ...include ALL instruction programs automatically
#include "../terminate.c"
#include "../data_processing.c"
#include "../single_data_transfer.c"
#include "../multiply.c"
#include "../branch.c"

static instruction_ptr decode(const word instruction) {
    word opcode = extract_bits(instruction, OPCODE_LSB, OPCODE_MSB);
    if(instruction == TERMINATE_VALUE) {
        return &terminate;
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

void pipeline(memory_t main_memory, struct RegisterFile *registers)   {
    const int bytes_per_instr = sizeof(word) / sizeof(*main_memory);
    instruction_ptr instr_func = NULL;
    word instr_to_exec;
    word fetched;
    int num_cycles;
    do {
        if(num_cycles >= 2) {
            // Execute
            short should_terminate = (*instr_func)(&instr_to_exec, registers, main_memory);
            if(should_terminate) {
                break;
            }
        }
        if(num_cycles >= 1) {
            // Decode
            instr_func = decode(fetched);
            instr_to_exec = fetched;
        }
        // Fetch
        fetched = main_memory[registers->program_counter];
        registers->program_counter += bytes_per_instr;
        num_cycles++;
    } while(1);
}
