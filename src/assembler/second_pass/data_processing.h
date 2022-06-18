#ifndef DATA_PROCESSING 
#define DATA_PROCESSING

#include "../utils.h"
#include <string.h>
#include <stdio.h>

#define AL 14
#define DPI_S_MASK 20
#define DPI_I_MASK 25
#define DPI_OPCODE_MASK 21
#define DPI_RD_MASK 12
#define DPI_RS_MASK 8
#define DPI_RN_MASK 16

typedef struct {
    int size;
    char **string_array;
} tokenized_source_code;
typedef enum {
    AND = 0,
    EOR = 1,
    SUB = 2,
    RSB = 3,
    ADD = 4,
    TST = 8,
    TEQ = 9,
    CMP = 10,
    ORR = 12,
    MOV = 13
} OPCODE;
typedef enum {
    LSL = 0,
    LSR = 1,
    ASR = 2,
    ROR = 3,
} SHIFT_TYPE;

word data_processing(tokenized_source_code *tokens);
void operand_parser(word *operand2, tokenized_source_code *tokens_for_operand, 
                    word *Rm, word *shift_type, word *shift_amount, word *Rs, 
                    word *I, word *imm_val);
SHIFT_TYPE get_shift_type(char *shift_string);
OPCODE get_opcode(char *opcode_string);
word get_register_address(char *register_string);

#endif
