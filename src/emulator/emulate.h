#include <stdint.h>
<<<<<<< HEAD
#define GENERAL_PURPOSE_REGISTERS 13
#define COND_MSB 31
#define COND_LSB 28

typedef uint32_t word;
typedef uint4_t byte;

typedef int8_t memory_t[];

struct RegisterFile {
    word general_purpose[GENERAL_PURPOSE_REGISTERS];
=======
#include <stdio.h>


#define NUM_GENERAL_PURPOSE_REGISTERS 13

#define COND_MSB 31
#define COND_LSB 28
#define N_FLAG_CPSR 31
#define Z_FLAG_CPSR 30
#define C_FLAG_CPSR 29
#define V_FLAG_CPSR 28


typedef uint32_t word;
typedef uint8_t byte;
typedef int8_t memory_t[];

struct RegisterFile {
    word general_purpose[NUM_GENERAL_PURPOSE_REGISTERS];
>>>>>>> adiDanish
    word lr;
    word sp;
    word program_counter;
    word cpsr;
};

typedef short (*instruction_ptr)(word*, struct RegisterFile*, memory_t);

// Condition codes for ARM instructions multiply, data processing, and single data transfer, and branch.
// added some extra codes beyond the spec, can implement/remove later depending on time. 
<<<<<<< HEAD
typedef enum condition_code{
    eq = 0, // equal
    ne = 1, // not equal
    // cs = 2, // carry set
    // cc = 3, // carry clear
    // mi = 4, // minus
    // pl = 5, // plus
    // vs = 6, // overflow
    // vc = 7, // no overflow
    // hi = 8, // higher
    // ls = 9, // lower or same
    ge = 10, // greater or equal
    lt = 11, // less than
    gt = 12, // greater than
    le = 13, // less than or equal
    al = 14 // always
};

typedef struct instruction {
    // The condition code 
    byte cond; 

    //
}
=======
enum cond_code{
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

word extract_bits(const word value, int begin, int end);

short cond_check(const word instruction, struct RegisterFile *const registers);

void clear_registers(struct RegisterFile *registers);
>>>>>>> adiDanish
