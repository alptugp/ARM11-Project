#include "single_data_transfer.h"

static long get_value(char *expression) {
    return strtol(expression, NULL, 0);
}

static void sdt_shift_helper(tokenized_source_code *tokens, word *rs, word *rm, word *u, word *i, word *shift_type, word *shift_amount, int index) {
    *i = 1;

    if (tokens->string_array[5][0] != '-') {
        *rm = get_register_address(tokens->string_array[index]);
    } else {
        *rm = get_register_address(&tokens->string_array[index][1]);
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
                *rs = get_register_address(tokens_for_shift->string_array[1]); 
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


word single_data_transfer(tokenized_source_code *tokens, word current_instr_address, word last_instr_address, word *sdt_constant) {
    word cond = AL;
    word u = 1;
    word p = 0;
    word i = 0;
    word rd = get_register_address(tokens->string_array[1]);
    word rn = -1;
    word rm = -1;
    word rs = -1;
    word offset, shift_type, shift_amount, expression;

    word l = tokens->string_array[0][0] == 'l' ? 1 : 0;

    if (tokens->string_array[2][0] != '=') {
        if (tokens->size != 5) {
            if (tokens->string_array[4][0] != ']') {
                // <address> has the pre-indexed form
                rn = get_register_address(tokens->string_array[3]);
                p = 1;
                
                if (tokens->string_array[4][0] != '#') {
                    // <address> has the form [Rn, {+/-}Rm{,<shift>}]
                    sdt_shift_helper(tokens, &rs, &rm, &u, &i, &shift_type, &shift_amount, 4);
                } else {
                    // <address> has the form [Rn,<#expression>]
                    long value = get_value(&tokens->string_array[4][1]);
                    u = value >= 0;
                    offset = abs(value);
                }
            } else {
                // <address> has the post-indexed form
                if (tokens->string_array[5][0] != '#') {
                    // <address> has the form [Rn, {+/-}Rm{,<shift>}]
                    rn = get_register_address(tokens->string_array[3]);
                    sdt_shift_helper(tokens, &rs, &rm, &u, &i, &shift_type, &shift_amount, 5);
                } else {
                    // <address> has the form [Rn,<#expression>]
                    offset = get_value(&tokens->string_array[5][1]);   
                    rn = get_register_address(tokens->string_array[3]);
                }
            }
        } else {
            // <address> has the form [Rn]
            p = 1;
            rn = get_register_address(tokens->string_array[3]);
        }
    } else {
        // <address> has the form <=expression>
        expression = get_value(&tokens->string_array[2][1]);
        if (0xFF < expression) {
            tokenized_source_code *tokens_for_mov = malloc(sizeof(tokenized_source_code));
            if (tokens_for_mov) {
                tokens_for_mov->string_array = malloc(3 * sizeof(char *));
                if (tokens_for_mov->string_array) {
                    tokens_for_mov->size = 3;
                    tokens_for_mov->string_array[0] = "mov";
                    tokens_for_mov->string_array[1] = tokens->string_array[1];
                    tokens_for_mov->string_array[2] = tokens->string_array[2];
                    tokens_for_mov->string_array[2][0] = '#';
                    free(tokens_for_mov->string_array);
                    free(tokens_for_mov);
                    return data_processing(tokens_for_mov);
                }
                perror("Memory for the tokens_for_mov array could not have been allocated.\n");
                exit(EXIT_FAILURE);
            }
            perror("Memory for tokens for mov could not have been allocated.\n");
            exit(EXIT_FAILURE);
        } else {
            last_instr_address += 1;
            offset = ((last_instr_address - 1 - current_instr_address) * sizeof(binary_instruction)) 
                                                            - PIPELINE_EFFECT;
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