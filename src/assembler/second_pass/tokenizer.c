#include "tokenizer.h"

// Removes commas from a token
// E.g. "R0, " -> "R0"
static char *remove_token_commas(char *str) {
    char *ptr = str;
    while (*ptr) {
        if (*ptr == ',') {
            *ptr = '\0';
            break;
        }
        ptr++;
    }
    return str;
}

// Trims leading and trailing whitespace from a string. 
static char *trim_whitespace(char *str) {

    // Trim leading space
    while(*str == ' ') {
        str++;
    }

    // Trim trailing space
    char *end = str + strlen(str) - 1;
    while(end > str && *end == ' ') {
        end--;
    }

    *(end+1) = '\0';

    return str;
}

// Removes comments from a line in assembly code
static char *remove_comments(char *str) {
    char* ptr = str;
    while (*ptr) {
        if (*ptr == ';') {
            *ptr = '\0';
            break;
        }
        ptr++;
    }
    return str;
}

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
    /*
    if (is_label(line)) {
        tokenized_source_code result;
        result.string_array = get_label(line);
        result.size = NELEMS(get_label(line));
        return result;
    }
    */

    tokenized_source_code tokens;
    tokens.string_array = malloc(sizeof(char *) * MAX_NUMBER_TOKENS);
    int tokens_index = 0;
    char *token = strtok(line, TOKEN_DELIMITERS);
    while (token != NULL) {
        remove_token_commas(token);
        trim_whitespace(token);
        tokens.string_array[tokens_index] = malloc(sizeof(char) * MAX_TOKEN_LENGTH);
        tokens.string_array[tokens_index] = token;
        tokens_index++;
        token = strtok(NULL, TOKEN_DELIMITERS);
    }
    tokens.string_array[tokens_index] = NULL;
    tokens.size = tokens_index - 1;
    return tokens;
}

/*
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
*/

/*
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
*/
