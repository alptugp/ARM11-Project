#include "emulate.h"

#define OPCODE_LSB 21
#define OPCODE_MSB 24
//#define OPERAND2_START 0
//#define OPERAND2_SIZE 12
#define DEST_REG_LSB 12
#define FIRST_OPERAND_LSB 16
#define IMMEDIATE_OPERAND_BIT 25
#define DONT_WRITE_OPCODE 2 // 10
#define SET_COND_CODE_BIT 20
#define OPERAND2_IMM_LSB 0
#define OPERAND2_IMM_MSB 7
#define ROTATE_LSB 8
#define ROTATE_MSB 11
#define RM_LSB 0
#define RM_MSB 3
#define SHIFTER_BIT 4
#define SHIFT_TYPE_LSB 5
#define SHIFT_TYPE_MSB 6
#define SHIFT_REGISTER_LSB 8
#define SHIFT_REGISTER_MSB 11
#define SHIFT_INTEGER_LSB 7
#define SHIFT_INTEGER_MSB 11
#define SIGN_BIT 31

typedef enum opcode_t {
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
};

enum shift_type { LSL, LSR, ASR, ROR };

short data_processing(word *instruction, struct RegisterFile *registers, memory_t memory);