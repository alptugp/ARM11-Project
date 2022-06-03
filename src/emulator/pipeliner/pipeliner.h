#include <stdint.h>
#include "emulate.h"

// pipeline stages: 
void pipeline(memory_t main_memory, struct RegisterFile registers);
static instruction_ptr decode(word instruction);
static void execute(word instruction, word instructionCode);
