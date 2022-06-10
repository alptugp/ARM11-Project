#ifndef SINGLE_DATA_TRANSFER
#define SINGLE_DATA_TRANSFER

#include "../emulate_utils.h"
#include "gpio.h"

#define SHIFTED_LSB 0
#define SHIFTED_MSB 11
#define RD_LSB 12
#define RD_MSB 15
#define RN_LSB 16
#define RN_MSB 19
#define L_BIT 20
#define U_BIT 23
#define P_BIT 24
#define IMM_BIT 25

short single_data_transfer(word *instruction, struct RegisterFile *registers, memory_t memory);
static short address_out_of_bounds(word start_address);
static word read_word(memory_t memory, word start_address);
static void write_word(memory_t memory, word start_address, word write);



#endif
