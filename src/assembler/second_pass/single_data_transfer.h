#ifndef SINGLE_DATA_TRANSFER 
#define SINGLE_DATA_TRANSFER

#include "../utils.h"
#include "second_pass_utils.h"
#include "data_processing.h"

#define PIPELINE_EFFECT 8
#define SDT_I_MASK 25
#define SDT_P_MASK 24
#define SDT_U_MASK 23
#define SDT_L_MASK 20
#define SDT_RD_MASK 12
#define SDT_RN_MASK 16
#define SDT_RS_MASK 8
#define PC_REGISTER_INDEX 15
#define OPERAND2_TOKEN_START 2

word single_data_transfer(tokenized_source_code *tokens, word current_instr_address, word last_instr_address, word *sdt_constant);

#endif
