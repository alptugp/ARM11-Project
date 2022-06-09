#ifndef BRANCH_H
#define BRANCH_H

#include <stdint.h>
#include "emulate.h"

#define OFFSET_MSB 23
#define OFFSET_LSB 0
#define SHIFT_VALUE_OFFSET 2

short branch(word *instruction, struct RegisterFile *registers, memory_t memory);

#endif
