#include "binary_loader.h"
#include "pipeliner/pipeliner.h"
#include "emulate.h"

int main(int argc, char *argv[]) {
    const char *filename = argv[0];
    struct RegisterFile registers;
    clear_registers(&registers);
    memory_t memory;
    int num_instructions = file_load(filename, MEMSIZE, memory);
    pipeline(memory, &registers, num_instructions);
    return 0;
}
