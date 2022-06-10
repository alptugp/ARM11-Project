#ifndef DATA_PROCESSING
#define DATA_PROCESSING

#include "../emulate_utils.h"

#define MNEMONIC_LSB 21
#define MNEMONIC_MSB 24
#define DEST_REG_LSB 12
#define FIRST_OPERAND_LSB 16
#define IMMEDIATE_OPERAND_BIT 25
#define DONT_WRITE_OPCODE 2 // 10
#define SET_COND_CODE_BIT 20
#define SHIFTED_VALUE_LSB 0
#define SIGN_BIT 31
#define OFFSET_LSB 0

typedef enum {
    AND = 0,
    EOR = 1,
    SUB = 2,
    RSB = 3,
    ADD = 4,
    // adc = 5,
    // sbc = 6,
    // rsc = 7,
    TST = 8,
    TEQ = 9,
    CMP = 10,
    // cmn = 11,
    ORR = 12,
    MOV = 13,
    // bic = 14,
    // mvn = 15
} opcode_t;

short data_processing(word *instruction, struct RegisterFile *registers, memory_t memory);

#endif