#ifndef DATA_PROCESSING 
#define DATA_PROCESSING

#include "../utils.h"
#include "second_pass_utils.h"
#include <string.h>
#include <stdio.h>

#define DPI_S_MASK 20
#define DPI_I_MASK 25
#define DPI_OPCODE_MASK 21
#define DPI_RD_MASK 12
#define DPI_RS_MASK 8
#define DPI_RN_MASK 16

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
} DATA_PROC_OPCODE;

word data_processing(tokenized_source_code *tokens);
void operand_parser(word *operand2, tokenized_source_code *tokens_for_operand, 
                    word *Rm, word *shift_type, word *shift_amount, word *Rs, 
                    word *I, word *imm_val);

#endif
