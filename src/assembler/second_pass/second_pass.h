#ifndef SECOND_PASS
#define SECOND_PASS

#include "../utils.h"
#include "tokenizer.h"
#include "../first_pass/first_pass.h"
#include "branch.h"
#include "multiply.h"
#include "../symbol_table/symbol_table.h"

#define MAX_MNEMONIC_LENGTH 5
#define MAX_OPERAND_LENGTH 0
#define MAX_LINE_LENGTH 511
#define MAX_TOKEN_LENGTH 100


char* get_mnemonic(char *line);
char* get_operand(char *line);
bool is_in_array(char *str, char **array);
int second_pass(char **lines, int num_lines, symbol_table_t *labels_to_addresses, binary_instruction *binary_instructions);

// Define a string array for all the data processing instructions
char **data_processing_instructions[9] = {"add", "sub", "rsb", "and", "eor", "orr", "mov", "tst", "teq", "cmp"};

// Define a string array for all the multiply instructions
char **multiply_instructions[1] = {"mul", "mla"};

// Define a string array for all the branch instructions
char **branch_instructions[6] = {"beq", "bne", "bge", "blt", "bgt", "ble", "b"};

// Define a string array for all the special instructions
char **special_instructions[1] = {"lsl", "andeq"};

#endif
