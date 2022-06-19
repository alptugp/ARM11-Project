#ifndef SECOND_PASS_UTILS
#define SECOND_PASS_UTILS

#include "../utils.h"

#define AL 14

typedef enum {
    LSL = 0,
    LSR = 1,
    ASR = 2,
    ROR = 3,
} SHIFT_TYPE;

word get_register_address(char *register_string);

SHIFT_TYPE get_shift_type(char *shift_string);

#endif
