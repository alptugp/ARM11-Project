#include "special.h" 
#include "data_processing.h"

static SPECIAL_OPCODE get_special_opcode(char *opcode_string) {
    if (!strcmp(opcode_string, "andeq")) {
        return ANDEQ;
    } else if (!strcmp(opcode_string, "lsl")) {
        return LSL_SPECIAL;
    } else {
        printf("No corresponding opcode found");
        exit(EXIT_FAILURE);
    }
}

word special(tokenized_source_code *tokens) {
    char *Rn;
    word result;
    char expression[MAX_LINE_CHARS];

    SPECIAL_OPCODE instruction_opcode = get_special_opcode(tokens->string_array[0]);
    Rn = tokens->string_array[1];

    switch (instruction_opcode) {
        case ANDEQ:
            result = ANDEQ_RESULT;
            break;
        case LSL_SPECIAL:
            // char *third_argument = tokens->string_array[2];
            strcpy(expression, tokens->string_array[2]);

            tokens->string_array[0] = "mov";
            tokens->string_array[1] = Rn;
            tokens->string_array[2] = Rn;
            tokens->string_array[3] = "lsl";
            tokens->string_array[4] = expression;
            tokens->size = 5;

            result = data_processing(tokens);
            break;    

    }
    return result;
}
