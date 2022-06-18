#include "special.h" 
#include "data_processing.c"

word special_instructions(tokenized_source_code *tokens) {

    word cond, I, S, Rn, Rd, Operand2, result;

    cond = EQ;
    Operand2, Rn, Rd = 0;
    I = 1;

    int instruction_opcode = get_opcode(tokens->string_array[0]);
    Rd = tokens->string_array[1];

    switch (instruction_opcode) {
        case ANDEQ:
            result = ANDEQ_RESULT;
            break;
        case LSL:
            OPCODE new_opcode = MOV; 

            char expression[50] = tokens->string_array[2];

            tokens->string_array[0] = new_opcode;
            tokens->string_array[1] = Rn;
            tokens->string_array[2] = Rn;
            tokens->string_array[3] = instruction_opcode;
            tokens->string_array[4] = expression;

            result = data_processing(tokens);
            break;    

    }
    return result;
}
