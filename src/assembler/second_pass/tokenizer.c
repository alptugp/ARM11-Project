#include "tokenizer.h"
#include "./first_pass/first_pass.h"

// A tokenizer for breaking a line into its label, opcode and operand field(s) 

tokenized_source_code tokenize_line(char* line) {
    line = trim_whitespace(line);
    line = remove_comments(line);
    if (strlen(line) == 0) {
        // Return a null tokenized_source_code
        tokenized_source_code tokens;
        tokens.string_array = NULL;
        tokens.size = 0;
        return tokens;
    }
    if (is_label(line)) {
        tokenized_source_code result;
        result.string_array = get_label(line);
        result.size = NELEMS(get_label(line));
        return result;
    }

    tokenized_source_code tokens;
    int tokens_index = 0;
    char* token = strtok(line, " ");
    while (token != NULL) {
        token = strtok(NULL, " ");
        remove_token_commas(token);
        trim_whitespace(token);
        tokens.string_array[tokens_index] = token;
        tokens_index++;
    }
    tokens.string_array[tokens_index] = NULL;
    return tokens;
}

tokenized_source_code* tokenize_file(char** input) {
    tokenized_source_code* output = malloc(sizeof(tokenized_source_code) * NELEMS(input));
    int i = 0;
    while (input[i] != NULL) {
        output->string_array[i] = tokenize_line(input[i]).string_array;
        i++;
    }
    output->string_array[i] = NULL;
    output->size = i;
    return output;
}

// Function to check if a line is a label 
bool is_label(char *line) {
    trim_whitespace(line);
    return line[strlen(line)] == ':';
}

// PRE: line only contains a label. So use is_label to check if line is a label first.
// gets the label from the line, e.g. "label: ", returns "label"
char* get_label(char* line) {
    char* label = line;
    while (*label != NULL) {
        if (*label == ':') {
            *label = NULL;
            break;
        }
        label++;
    }
    return label;
    // trim_whitespace(line);
    // line(strlen(line) - 1) = NULL;
    // return line;
}


// Removes commas from a token
// E.g. "R0, " -> "R0"
char* remove_token_commas(char* str) {
    char* ptr = str;
    while (*ptr != NULL) {
        if (*ptr == ',') {
            *ptr = NULL;
            break;
        }
        ptr++;
    }
    return str;
}

// Trims leading and trailing whitespace from a string. 
char* trim_whitespace(char* str) {

    // Trim leading space
    while(isspace(*str)) {
        str++;
    }

    // Trim trailing space
    char *end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) {
        end--;
    }

    // Write new null terminator
    *(end+1) = NULL;

    return str;
}

// Removes comments from a line in assembly code
char* remove_comments(char* str) {
    char* ptr = str;
    while (*ptr != NULL) {
        if (*ptr == ';') {
            *ptr = NULL;
            break;
        }
        ptr++;
    }
    return str;
}
