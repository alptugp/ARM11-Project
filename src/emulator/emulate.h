#ifndef EMULATE
#define EMULATE

#include <stdint.h>
#include <stdio.h>

typedef uint32_t word;
typedef int32_t signed_word;
typedef uint8_t byte;
typedef char *memory_t;

#define MEMSIZE 65536
#define NUM_GENERAL_PURPOSE_REGISTERS 13
#define REGISTER_ADDRESS_LENGTH 4
#define BYTES_PER_INSTR sizeof(word)

#define HEX_OUTPUT_LENGTH 8

#define COND_MSB 31
#define COND_LSB 28
#define N_FLAG_CPSR 31
#define Z_FLAG_CPSR 30
#define C_FLAG_CPSR 29
#define V_FLAG_CPSR 28

#define SHIFTED_IMM_OFFSET 0
#define SHIFTED_IMM_SIZE 8
#define SHIFTED_ROTATE_OFFSET 8
#define SHIFTED_ROTATE_SIZE 4
#define SHIFTED_RM_OFFSET 0
#define SHIFTED_RM_SIZE REGISTER_ADDRESS_LENGTH
#define SHIFTED_SHIFTER_OFFSET 4
#define SHIFTED_SHIFTER_SIZE 1
#define SHIFTED_SHIFT_TYPE_OFFSET 5
#define SHIFTED_SHIFT_TYPE_SIZE 2
#define SHIFTED_INTEGER_OFFSET 7
#define SHIFTED_INTEGER_SIZE 5
#define SHIFTED_RS_OFFSET 8
#define SHIFTED_RS_SIZE REGISTER_ADDRESS_LENGTH

struct RegisterFile {
    word general_purpose[NUM_GENERAL_PURPOSE_REGISTERS];
    word lr;
    word sp;
    word program_counter;
    word cpsr;
};

// Pointer to a generic instruction function.
// Defines interface of instruction functions, and simplifies code.
// Return value is a short that takes on 3 values:
// 0 for do nothing, 1 for terminate program, 2 for clear pipeline
typedef short (*instruction_ptr)(word*, struct RegisterFile*, memory_t);

// Condition codes for ARM instructions multiply, data processing, and single data transfer, and branch.
// added some extra codes beyond the spec, can implement/remove later depending on time. 
enum cond_code {
    EQ = 0, // equal
    NE = 1, // not equal
    // CS = 2, // carry set
    // CC = 3, // carry clear
    // MI = 4, // minus
    // PL = 5, // plus
    // VS = 6, // overflow
    // VC = 7, // no overflow
    // HI = 8, // higher
    // LS = 9, // lower or same
    GE = 10, // greater or equal
    LT = 11, // less than
    GT = 12, // greater than
    LE = 13, // less than or equal
    AL = 14 // always
};

enum shift_types { LSL, LSR, ASR, ROR };

unsigned long extract_bits_64bit(const unsigned long value, unsigned int begin, unsigned int end);

word extract_bits(const word value, unsigned int begin, unsigned int end);

void write_bits(word *value, unsigned int begin, unsigned int end, word replacement_bits);

void clear_registers(struct RegisterFile *registers);

word load_register(word instruction, short *cout, int shifted_lsb, struct RegisterFile *registers);

word rotate_right(const word unrotated, const word rotation);

word extract_shifted_bits(const word shifted, unsigned int interval_lsb, unsigned int interval_size, unsigned int shifted_lsb);

#endif
