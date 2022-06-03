#include "emulate.h"

#define OFFSET_LS_BIT 0
#define OFFSET_MS_BIT 11
#define RD_LS_BIT 12
#define RD_MS_BIT 15
#define RN_LS_BIT 16
#define RN_MS_BIT 19
#define L_BIT 20
#define U_BIT 23
#define P_BIT 24
#define IMM_BIT 25

short single_data_transfer(word *instruction, struct RegisterFile *registers, memory_t memory);
