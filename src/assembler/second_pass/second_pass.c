#include "second_pass.h"

// Define a string array for all the data processing instructions
static const char *data_processing_instructions[11] = {"add", "sub", "rsb", "and", "eor", "orr", "mov", "tst", "teq", "cmp", ARRAY_TERMINATION_STR};

// Define a string array for all the multiply instructions
static const char *multiply_instructions[3] = {"mul", "mla", ARRAY_TERMINATION_STR};

// Define a string array for all the branch instructions
static const char *branch_instructions[8] = {"beq", "bne", "bge", "blt", "bgt", "ble", "b", ARRAY_TERMINATION_STR};

// Define a string array for all the special instructions
static const char *special_instructions[9] = {"lsl", "andeq", ARRAY_TERMINATION_STR};

static const char *sdt_instructions[3] = {"ldr", "str", ARRAY_TERMINATION_STR};

// Function to check if a string is in an array of strings
static bool is_in_array(char *str, const char **array) {
    for (int i = 0; strcmp(array[i], ARRAY_TERMINATION_STR); i++) {
        if (!strcmp(str, array[i])) {
            return 1;
        }
    }
    return 0;
}

// Define the get_mnemonic function here.
char* get_mnemonic(char *line) {
    char *mnemonic = malloc(sizeof(char) * MAX_MNEMONIC_LENGTH);
    int i = 0;
    while (line[i] != '\0' && line[i] != ' ') {
        mnemonic[i] = line[i];
        i++;
    }
    mnemonic[i] = '\0';
    return mnemonic;
}

int second_pass(char **lines, int num_lines, symbol_table_t *labels_to_addresses, binary_instruction *binary_instructions) {
    word sdt_constants[MAX_NUM_INSTRUCTIONS];
    int num_saved_sdt_constants = 0;
    for (int i = 0; i < num_lines; i++) {
        char *line = lines[i];
        char *mnemonic = get_mnemonic(line);
        tokenized_source_code tokenized_line = tokenize_line(line);
        
        if (is_in_array(mnemonic, data_processing_instructions) || is_in_array(mnemonic, special_instructions)) {
            binary_instructions[i] = data_processing(&tokenized_line);
        }
        else if (is_in_array(mnemonic, multiply_instructions)) {
            binary_instructions[i] = multiply(&tokenized_line);
        }
        else if (is_in_array(mnemonic, branch_instructions)) {
            binary_instructions[i] = branch(&tokenized_line, i * sizeof(binary_instruction), labels_to_addresses);
        }
        /*
        else if(is_in_array(mnemonic, special_instructions)) {
            binary_instructions[i] = special(&tokenized_line, &labels_to_addresses, binary_instructions);
        }
        */
        else if(is_in_array(mnemonic, sdt_instructions)) {
            word sdt_constant = 0;
            word last_instr_address = (num_lines + num_saved_sdt_constants) * sizeof(binary_instructions);
            binary_instructions[i] = single_data_transfer(&tokenized_line, i * sizeof(binary_instructions), last_instr_address, &sdt_constant);
            if(sdt_constant != 0) { // word != 0 iff sdt_constant modified (constants < 0xFF saved in instruction)
                sdt_constants[num_saved_sdt_constants] = sdt_constant;
                num_saved_sdt_constants++;
            }
        }
    }

    for(int i = num_lines; i < num_lines + num_saved_sdt_constants; i++) {
        binary_instructions[i] = sdt_constants[i];
    }

    return num_lines + num_saved_sdt_constants;
}
