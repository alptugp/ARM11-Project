#ifndef BINARY_WRITER
#define BINARY_WRITER

#include "../utils.h"

// Take filename of binary file (e.g. “add01.bin”),
// and write array of binary instructions into it
void write_binary_instructions(char filename[], binary_instruction *buffer, int num_instructions);

#endif
