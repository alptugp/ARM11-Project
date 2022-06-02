#include <stdio.h>
#include <stdint.h>
#include "pipeliner.h"
#include "terminate.h"
#include "data_processing.h"
#include "single_data_transfer.h"
#include "multiply.h"



// implements the 3 stage pipeline 

/* TODO:
- make sure all functions in .h
- decode
- execute
- fetch
- maybe put all the instruction functions in a struct?
- maybe put all the instruction function definitions in emulate.h?
*/

void pipeline(memory_t main_memory, struct RegisterFile registers)   {
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
        registers.program_counter += 4;
        num_cycles++;
    } while(1);
}

// need to change output of decode
// need to define functions later

// define magic numbers in pipeline.h later 
static instruction_ptr decode(const word instruction) {
    word bits27_26 = extract_bits(instruction, 26, 27);
    if(bits27_26 == 1) { // compare to 01
        // RETURN SINGLE DATA TRANSFER
        return &single_data_transfer;
    }
    if(bits27_26 == 2) { // compare to 10
        return &branch;
    }
    if(extract_bits(instruction, 25, 25) == 1) { // compare to 1
        // RETURN DATA PROCESSING
        return &data_processing;

    }
    if(extract_bits(instruction, 4, 7) == 3) { // compare to 
        return &multiply;
    }
    if(instruction == 0) {
        return &terminate;
    }
    else return &data_processing;
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