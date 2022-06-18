#include "second_pass.h"

int second_pass(char **lines, int num_lines, symbol_table_t *labels_to_addresses, binary_instruction *binary_instructions) {
    for (int i = 0; i < num_lines; i++) {
        char *line = lines[i];
        char *mnemonic = get_mnemonic(line);
        tokenized_source_code tokenized_line = tokenize_line(line);
        
        if (is_in_array(mnemonic, data_processing_instructions)) {
            binary_instructions[i] = data_processing(&tokenized_line);
        }
        else if (is_in_array(mnemonic, multiply_instructions)) {
            binary_instructions[i] = multiply(&tokenized_line);
        }
        else if (is_in_array(mnemonic, branch_instructions)) {
            binary_instructions[i] = branch(&tokenized_line, &labels_to_addresses, binary_instructions);
        }
        else if(is_in_array(mnemonic, special_instructions)) {
            binary_instructions[i] = special(&tokenized_line, &labels_to_addresses, binary_instructions);
        }
    }
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


// Function to check if a string is in an array of strings
bool is_in_array(char *str, char **array) {
    for (int i = 0; i < NELEMS(array); i++) {
        if (!strcmp(str, array[i])) {
            return 1;
        }
    }
    return 0;
}