#include "binary_loader.h"
#include "pipeliner/pipeliner.h"
#include "emulate.h"
#include "main.h"
#include <assert.h>
#include <string.h>

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
        char to_print[HEX_LENGTH] = "";
        short is_nonzero = 0;
        int str_length = 0;

        for(int offset = 0; offset < ADDRESSES_PER_OUTPUT_LINE; offset++) {
            char val = memory[address + offset];
            is_nonzero = is_nonzero || val;
            // Cast val to unsigned char so 2-digit hex value is printed
            str_length += sprintf(to_print + str_length, "%02x", (unsigned char) val);
        }

        if(is_nonzero) {
            printf("0x%0*lx: 0x", HEX_LENGTH, address);
            printf("%s", to_print);
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
    pipeline(memory, &registers, num_instructions);
    output_state(memory, &registers);
    free(memory);
    return 0;
}
