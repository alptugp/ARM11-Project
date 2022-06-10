#ifndef BRANCH_H
#define BRANCH_H

#include <stdint.h>
#include "../emulate_utils.h"

#define OFFSET_MSB 23
#define OFFSET_LSB 0
#define SHIFT_VALUE_OFFSET 2

// executes a branch instruction
// the parameters *registers and memory are the current cpu state.
short branch(word *instruction, struct RegisterFile *registers, memory_t memory);

#endif
