#ifndef BRANCH_H
#define BRANCH_H

#include <stdint.h>
#include "emulate.h"

#define OFFSET_MSB 23
#define OFFSET_LSB 0
#define SIGNED_OFFSET_EXTEND 0xfc000000
#define SHIFT_VALUE_OFFSET 2
#define SIGN_BIT_OFFSET 23

word branch(word *instruction, struct RegisterFile *registers, const memory_t memory);

#endif
