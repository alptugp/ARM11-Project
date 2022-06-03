#include <stdio.h>
#include <stdint.h>
#include "pipeliner.h"
#include "terminate.h"
#include "data_processing.h"
#include "single_data_transfer.h"
#include "multiply.h"



// implements the 3 stage pipeline 

/* TODO:
- execute
- maybe put all the instruction functions in a struct?
- maybe put all the instruction function definitions in emulate.h?
*/

void pipeline(memory_t main_memory, struct RegisterFile registers)   {
    const int bytes_per_instr = sizeof(word) / sizeof(*main_memory);
    instruction_ptr instr_to_execute = NULL;
    word fetched = 0;
    int num_cycles = 0;
    do {
        if(num_cycles >= 2) {
            // execute and break if necessary
        }
        if(num_cycles >= 1) {
            instr_to_execute = decode(fetched);
        }
        fetched = main_memory[registers.program_counter];
        registers.program_counter += bytes_per_instr;
        num_cycles++;
    } while(1);
}

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

/*
void execute(word instruction) {
    word cond_code =
    switch(instructionCode) {
        case STOP:
            //stop
            //output the state of the register
            break;
        case MULTIPLY:
            multiply(instruction);
            break;
        case DATA_PROCESSING:
            process_data(instruction);
            break;
        case SINGLE_DATA_TRANSFER:
            single_data_transfer(instruction);
            break;
        case BRANCH:
            branch(instruction);
            break;
        default:
            fprintf(stderr, "Invalid instruction\n");
            break;
    }
*/