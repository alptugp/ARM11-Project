#ifndef MULTIPLY
#define MULTIPLY

#include "../emulate_utils.h"


#define ACC_BIT 21
#define SET_BIT 20
#define NUM_REGISTERS 4
#define RES_SIGN_BIT 31

// MSB of each register's address in the instruction
#define RD_LOC 19
#define RN_LOC 15
#define RS_LOC 11
#define RM_LOC 3

#define ACC_LOC 21
#define SET_LOC 20


short multiply(word *instruction, struct RegisterFile *registers, memory_t memory);

#endif
