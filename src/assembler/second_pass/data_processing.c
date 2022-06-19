#include "data_processing.h"

static DATA_PROC_OPCODE get_data_proc_opcode(char *opcode_string) {
    if (!strcmp(opcode_string, "and")) {
        return AND;
    } else if (!strcmp(opcode_string, "rsb")) {
        return RSB;
    } else if (!strcmp(opcode_string, "teq")) {
        return TEQ;
    } else if (!strcmp(opcode_string, "add")) {
        return ADD;
    } else if (!strcmp(opcode_string, "cmp")) {
        return CMP;
    } else if (!strcmp(opcode_string, "tst")) {
        return TST;
    } else if (!strcmp(opcode_string, "orr")) {
        return ORR;
    } else if (!strcmp(opcode_string, "eor")) {
        return EOR;
    } else if (!strcmp(opcode_string, "mov")) {
        return MOV;
    } else if (!strcmp(opcode_string, "sub")) {
        return SUB;
    } else {
        printf("No corresponding opcode found");
        exit(EXIT_FAILURE);
    }
}

static void operand2_parser(word *operand2, tokenized_source_code *tokens_for_operand2, word *I, word *Rm, word *shift_type, word *shift_amount, word *Rs) {
    if (tokens_for_operand2->string_array[0][0] == 'r') {
        *I = 0;

        // operand2 has the form Rm{,<shift>}
        *Rm = get_register_address(tokens_for_operand2->string_array[0]);
        
        if (tokens_for_operand2->size >= 2) {
            // Involves shift
            *shift_type = get_shift_type(tokens_for_operand2->string_array[1]);
            if (tokens_for_operand2->string_array[2][0] == 'r') {
                // has the form <shiftname> <register>
                *Rs = get_register_address(tokens_for_operand2->string_array[2]);
            } else if (tokens_for_operand2->string_array[2][0] == '#') {
                // has the form <shiftname> <#expression>

                // expression in hex or decimal
                *shift_amount = strstr(&tokens_for_operand2->string_array[2][1], "0x") 
                    ? strtol(&tokens_for_operand2->string_array[2][1], (char **)NULL, 16)
                    : strtol(&tokens_for_operand2->string_array[2][1], (char **)NULL, 10);
            } else {
                perror("Shift is neither number nor register");
                exit(EXIT_FAILURE);
            }
        }

        else {
            *operand2 = *Rm;
        }
    } else if (tokens_for_operand2->string_array[0][0] == '#') {
        // operand2 has the form <#expression>

        // expression in hex or decimal
        *operand2 = strstr(&tokens_for_operand2->string_array[0][1], "0x") 
                    ? strtol(&tokens_for_operand2->string_array[0][1], (char **)NULL, 16)
                    : strtol(&tokens_for_operand2->string_array[0][1], (char **)NULL, 10);
        *I = 1;
        word shift_val = sizeof(word) * 8;

        if (0xFF < *operand2) {
            while (!(0x3 & *operand2)) {
                shift_val--;
                *operand2 >>= 2;
            }
        }

        *shift_amount = shift_val;
    } else {
        perror("Operand is neither number nor register");
        exit(EXIT_FAILURE);
    }
}

word data_processing(tokenized_source_code *tokens) {

    word instruction = 0, Rn = 0, S = 0, Rd = 0, I = 0, shift_amount = 0, operand2 = 0, shift_type = 0, Rm = 0;
    word cond = AL;
    word Rs = -1;
    DATA_PROC_OPCODE opcode = get_data_proc_opcode(tokens->string_array[0]);
    tokenized_source_code *tokens_for_operand2 = malloc(sizeof(tokenized_source_code));
    if(!tokens_for_operand2) {
        perror("Memory for tokens for operand could not have been allocated");
        exit(EXIT_FAILURE); 
    }

    switch (opcode) {
        case AND:
        case EOR:
        case SUB:
        case RSB:
        case ADD:
        case ORR:
            tokens_for_operand2->size = tokens->size - 3;
            tokens_for_operand2->string_array = &tokens->string_array[3];
            Rd = get_register_address(tokens->string_array[1]);
            Rn = get_register_address(tokens->string_array[2]);
            operand2_parser(&operand2, tokens_for_operand2, &I, &Rm, &shift_type, &shift_amount, &Rs);
            break;
        case TST:
        case TEQ:
        case CMP:
            tokens_for_operand2->size = tokens->size - 2;
            tokens_for_operand2->string_array = &tokens->string_array[2];
            S = 1;
            Rn = get_register_address(tokens->string_array[1]);
            operand2_parser(&operand2, tokens_for_operand2, &I, &Rm, &shift_type, &shift_amount, &Rs);
            break;
        case MOV: 
            tokens_for_operand2->size = tokens->size - 2;
            tokens_for_operand2->string_array = &tokens->string_array[2];
            Rd = get_register_address(tokens->string_array[1]);
            operand2_parser(&operand2, tokens_for_operand2, &I, &Rm, &shift_type, &shift_amount, &Rs);
            break;
    }

    free(tokens_for_operand2);

    S <<= DPI_S_MASK;  
    I <<= DPI_I_MASK;
    opcode <<= DPI_OPCODE_MASK;
    Rd <<= DPI_RD_MASK;
    cond <<= COND_MASK;
    Rn <<= DPI_RN_MASK;

    if (!I) {
        instruction |= Rm;
        instruction |= shift_type << 5;
        if (Rs == -1) {
            // constant shift
            instruction |= shift_amount << 7;
        }
        else {
            // register shift
            instruction |= 1L << 4;
            instruction |= Rs << DPI_RS_MASK;
        }
    }
    else {
        instruction |= operand2;
        shift_amount &= 0xF;
        instruction |= shift_amount << 8;
    }

    return instruction | opcode | cond | I | S | Rn | Rd | operand2;
} 
