#include "binary_loader.h"
#include "pipeliner/pipeliner.h"
#include "emulate.h"

int main(int argc, char *argv[]) {
    char *filename = argv[0];
    struct RegisterFile registers;
    memory_t memory = file_load(filename, MEMSIZE);
    pipeline(memory, &registers);
    return 0;
}