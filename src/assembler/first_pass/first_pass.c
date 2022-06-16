#include "first_pass.h"
#include <string.h>

int first_pass(char **raw_lines, symbol_table_t *labels_to_addresses, int num_raw_lines, char **delabelled_lines) {
    int instr_count = 0;
    for(int line_num = 0; line_num < num_raw_lines; line_num++) {
        char *line = raw_lines[line_num];
        if(line[strlen(line) - 1] == ':') {
            char label[strlen(line) - 1];
            strncpy(label, line, strlen(line) - 1); // copies up to and excluding ':'
            label[strlen(line) - 1] = '\0';
            put(labels_to_addresses, label, instr_count * sizeof(binary_instruction));
        }
        else {
            delabelled_lines[instr_count] = malloc(MAX_LINE_CHARS);
            assert(delabelled_lines[instr_count]);
            strcpy(delabelled_lines[instr_count], raw_lines[line_num]);
            instr_count++;
        }
    }
    return instr_count;
}
