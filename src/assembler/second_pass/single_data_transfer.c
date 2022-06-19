#include "single_data_transfer.h" 
#include <regex.h>
#include <stdarg.h>

static long get_value(char *expression) {
    return strtol(expression, NULL, 0);
}

static char *remove_square_brackets(char *str) {
    char *new_str = str;
    if(new_str[0] == '[') {
        new_str++;
    }
    if(new_str[strlen(new_str) - 1] == ']') {
        new_str[strlen(new_str) - 1] = '\0';
    }
    return new_str;
}

static void sdt_shift_helper(tokenized_source_code *tokens, word *rs, word *rm, word *u, word *i, word *shift_type, word *shift_amount, int index) {
    *i = 1;

    if (tokens->string_array[4][0] != '-') {
        *rm = get_register_address(remove_square_brackets(tokens->string_array[index]));
    } else {
        *rm = get_register_address(remove_square_brackets(&tokens->string_array[index][1]));
        *u = 0;
    }
    
    if (6 < tokens->size) {
        // <shift> is present 
        tokenized_source_code *tokens_for_shift = malloc(sizeof(tokens));
        if (tokens_for_shift) {
            tokens_for_shift->size = tokens->size - index + 1;
            tokens_for_shift->string_array = &tokens->string_array[index + 1];
            *shift_type = get_shift_type(tokens_for_shift->string_array[0]);
            if (tokens_for_shift->string_array[1][0] == 'r') {
                *rs = get_register_address(remove_square_brackets(tokens_for_shift->string_array[1]));
            } else if (tokens_for_shift->string_array[1][0] == '#') {
                *shift_amount = get_value(&tokens_for_shift->string_array[1][1]);
            } else {
                perror("Shift is neither a register nor number");
                exit(EXIT_FAILURE);
            }
            free(tokens_for_shift);
        }
        perror("Memory for tokens for shift could not have been alloacted");
        exit(EXIT_FAILURE);
    }
}

static bool all_tokens_match(tokenized_source_code tokens, int num_regex_tokens, ...) {
    if(tokens.size != num_regex_tokens + OPERAND2_TOKEN_START) {
        return false;
    }
    va_list argp;
    va_start(argp, num_regex_tokens);
    for(int i = OPERAND2_TOKEN_START; i < OPERAND2_TOKEN_START + num_regex_tokens; i++) {
        regex_t regex = va_arg(argp, regex_t);
        if(0 != regexec(&regex, tokens.string_array[i], 0, NULL, 0)) { // string and regex failed to match
            return false;
        }
    }
    va_end(argp);
    return true;
}


word single_data_transfer(tokenized_source_code *tokens, word current_instr_address, word last_instr_address, word *sdt_constant) {
    word cond = AL;
    word u = 1;
    word p = 0;
    word i = 0;
    word rd = get_register_address(remove_square_brackets(tokens->string_array[1]));
    word rn = -1;
    word rm = -1;
    word rs = -1;
    word offset, shift_type, shift_amount, expression;

    word l = tokens->string_array[0][0] == 'l' ? 1 : 0;

    regex_t case1_tok3, case2_tok3, case2_tok4, case3_tok3, case3_tok4, case3_tok5, case4_tok3, case4_tok4, case5_tok3, case5_tok4, case5_tok5;
    int status = regcomp(&case1_tok3, "[[]r([0-9]|(1[0-9]))[]]", REG_EXTENDED); // [rn]
    status |= regcomp(&case2_tok3, "[[]r([0-9]|(1[0-2]))", REG_EXTENDED); // [rn
    status |= regcomp(&case2_tok4, "#[+-]?(0x)?[0-9]+[]]", REG_EXTENDED); // #{+-}expression]
    case3_tok3 = case2_tok3; // [rn
    status |= regcomp(&case3_tok4, "[+-]?r([0-9]|(1[0-2]))", REG_EXTENDED); // {+-}Rm
    status |= regcomp(&case3_tok5, "[a-z]+(0x)?[0-9]+[]]", REG_EXTENDED); // shift], i.e. shiftname #expression| 
    case4_tok3 = case1_tok3; // [rn]
    status |= regcomp(&case4_tok4, "#[+-]?(0x)?[0-9]+", REG_EXTENDED); // #{+-}expression
    case5_tok3 = case1_tok3; // [rn]
    case5_tok4 = case3_tok4; // {+-} Rm
    status |= regcomp(&case5_tok5, "(0x)?[0-9]+", REG_EXTENDED); // shift
    assert(!status); // check all regexes compiled

    if (tokens->string_array[2][0] != '=') {
        if(all_tokens_match(*tokens, 1, case1_tok3)) {
            // CASE 1
            // <address> has the form [Rn]
            p = 1;
            rn = get_register_address(remove_square_brackets(tokens->string_array[2]));
        }
        else if(all_tokens_match(*tokens, 2, case2_tok3, case2_tok4)) {
            // CASE 2
            // <address> has the form [Rn,<#expression>]
            p = 1;
            rn = get_register_address(remove_square_brackets(tokens->string_array[2])); // WE NEED TO DISCARD "["
            u = get_value(&tokens->string_array[3][1]) >= 0;
            offset = abs(get_value(&tokens->string_array[3][1]));
        }
        else if(all_tokens_match(*tokens, 3, case3_tok3, case3_tok4, case3_tok5)) {
            // CASE 3 optional
            // <address> has the form [Rn, {+/-}Rm{,<shift>}] with shift
            sdt_shift_helper(tokens, &rs, &rm, &u, &i, &shift_type, &shift_amount, 3);
        }
        else if(all_tokens_match(*tokens, 2, case3_tok3, case3_tok4)) {
            // CASE 3 without shift
            // <address> has the form [Rn, {+/-}Rm{,<shift>}] without shift
            sdt_shift_helper(tokens, &rs, &rm, &u, &i, &shift_type, &shift_amount, 3);
        }
        else if(all_tokens_match(*tokens, 2, case4_tok3, case4_tok4)) {
            // CASE 4
            // <address> has the form [Rn,<#expression>]
            offset = get_value(&tokens->string_array[3][1]);   
            rn = get_register_address(remove_square_brackets(tokens->string_array[2])); // WE NEED TO DISCARD "["
        }
        else if(all_tokens_match(*tokens, 3, case5_tok3, case5_tok4, case5_tok5)) {
            // CASE 5 optional
            // <address> has the form [Rn, {+/-}Rm{,<shift>}] with shift
            rn = get_register_address(remove_square_brackets(tokens->string_array[2]));
            sdt_shift_helper(tokens, &rs, &rm, &u, &i, &shift_type, &shift_amount, 4);
        }
        else if(all_tokens_match(*tokens, 2, case5_tok3, case5_tok4)) {
            // CASE 5
            // <address> has the form [Rn, {+/-}Rm{,<shift>}] without shift
            rn = get_register_address(remove_square_brackets(tokens->string_array[2]));
            sdt_shift_helper(tokens, &rs, &rm, &u, &i, &shift_type, &shift_amount, 4);
        }
        else {
            printf("Token matching failed.\n");
            assert(0);
        }
    } else {
        // <address> has the form <=expression>
        expression = get_value(&tokens->string_array[2][1]);
        if (0xFF >= expression) {
            tokenized_source_code *tokens_for_mov = malloc(sizeof(tokenized_source_code));
            if (tokens_for_mov) {
                tokens_for_mov->string_array = malloc(3 * sizeof(char *));
                if (tokens_for_mov->string_array) {
                    tokens_for_mov->size = 3;
                    tokens_for_mov->string_array[0] = "mov";
                    tokens_for_mov->string_array[1] = tokens->string_array[1];
                    tokens_for_mov->string_array[2] = tokens->string_array[2];
                    tokens_for_mov->string_array[2][0] = '#';
                    return data_processing(tokens_for_mov);
                }
                perror("Memory for the tokens_for_mov array could not have been allocated.\n");
                exit(EXIT_FAILURE);
            }
            perror("Memory for tokens for mov could not have been allocated.\n");
            exit(EXIT_FAILURE);
        } else {
            offset = last_instr_address - current_instr_address - PIPELINE_EFFECT;
            rn = PC_REGISTER_INDEX;
            p = 1;
            *sdt_constant = expression;
        }
    }

    word instruction = 0x04000000;
    // all fields are placed to their corresponding bits in the single data transfer
    // instruction by using shifting and OR
    instruction |= cond << COND_MASK;
    instruction |= i << SDT_I_MASK;
    instruction |= p << SDT_P_MASK;
    instruction |= u << SDT_U_MASK;
    instruction |= l << SDT_L_MASK;
    instruction |= rd << SDT_RD_MASK;

    if (!i) {
        instruction |= 0xFF & offset;
    } else {
        if (rs == -1) {
            // constant shift
            instruction |= (shift_amount & 0x1F) << 7;
        }
        else {
            // register shift
            instruction |= rs << SDT_RS_MASK;
            instruction |= 1L << 4;
        }

        instruction |= shift_type << 5; 

        if (rm != -1) {
            instruction |= rm;
        }
    }

    instruction |= rd << SDT_RD_MASK;

    if (rn != -1) {
        instruction |= rn << SDT_RN_MASK;
    }

    return instruction;    
}