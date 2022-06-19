#ifndef SECOND_PASS
#define SECOND_PASS

#include "../utils.h"
#include "tokenizer.h"
#include "branch.h"
#include "multiply.h"
#include "data_processing.h"
#include "single_data_transfer.h"
#include "special.h"
#include "../symbol_table/symbol_table.h"

#define MAX_MNEMONIC_LENGTH 5
#define MAX_OPERAND_LENGTH 0
#define MAX_LINE_LENGTH 511
#define MAX_TOKEN_LENGTH 100
#define ARRAY_TERMINATION_STR "\0"


char* get_mnemonic(char *line);
char* get_operand(char *line);
int second_pass(char **lines, int num_lines, symbol_table_t *labels_to_addresses, binary_instruction *binary_instructions);

#endif
