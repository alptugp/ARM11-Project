#include "binary_loader.h"
#include "pipeliner/pipeliner.h"
#include "emulate.h"
#include "main.h"
#include <assert.h>

static void output_register(char *register_name, word register_value) {
    printf("%-*s:%*d (0x%0*x)\n", REG_NAME_LENGTH, register_name, REG_VALUE_LENGTH, register_value, HEX_LENGTH, register_value);
}

static void output_state(memory_t memory, struct RegisterFile *registers) {
    printf("Registers:\n");
    for(int register_num = 0; register_num < NUM_GENERAL_PURPOSE_REGISTERS; register_num++) {
        char register_name[REG_NAME_LENGTH];
        sprintf(register_name,"$%d", register_num);
        assert(register_num < NUM_GENERAL_PURPOSE_REGISTERS);
        output_register(register_name, registers->general_purpose[register_num]);
    }
    output_register("PC", registers->program_counter);
    output_register("CPSR", registers->cpsr);
    printf("Non-zero memory:\n");
    for(uint64_t address = 0; address < MEMSIZE; address+=ADDRESSES_PER_OUTPUT_LINE) {
        assert((address + 3) < MEMSIZE);
        uint32_t value = (((uint32_t) memory[address]) << (3 * sizeof(char)))
                        + (((uint32_t) memory[address+1]) << (2 * sizeof(char)))
                        + (((uint32_t) memory[address+2]) << (1 * sizeof(char)))
                        + ((uint32_t) memory[address+3]);
        if(value != 0) {
            printf("0x%0*lx: ", HEX_LENGTH, address);
            for(int offset = 0; offset < ADDRESSES_PER_OUTPUT_LINE; offset++) {
                printf("%02x", (unsigned char) memory[address + offset]);
            }
            printf("\n");
        }

    }
}

int main(int argc, char *argv[]) {
    const char *filename = argv[1];
    struct RegisterFile registers;
    clear_registers(&registers);
    memory_t memory = (memory_t) (calloc(MEMSIZE, sizeof(char)));
    assert(memory);
    int num_instructions = file_load(filename, MEMSIZE, memory);
    //output_state(memory, &registers);
    pipeline(memory, &registers, num_instructions);
    output_state(memory, &registers);
    free(memory);
    return 0;
}
