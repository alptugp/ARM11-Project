#ifndef PIPELINER
#define PIPELINER

#include <stdint.h>
#include "../emulate.h"


#define OPCODE_MSB 27
#define OPCODE_LSB 26
#define SINGLE_DATA_TRANSFER_OPCODE 1 // 01
#define BRANCH_OPCODE 2 // 10
#define DATA_PROC_I 25
#define MULTIPLY_OPCODE_MSB 7
#define MULTIPLY_OPCODE_LSB 4
#define MULTIPLY_OPCODE 9 // 1001
#define TERMINATE_VALUE 0

void pipeline(memory_t main_memory, struct RegisterFile *registers);

#endif