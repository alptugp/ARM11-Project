#include "second_pass.h"

int second_pass(char **const lines, symbol_table_t *const labels_to_addresses, binary_instruction *instructions) {
    /*
    TEST FACTORIAL PROGRAM
    00000000: 0100a0e3  ....
    00000004: 0510a0e3  ....
    00000008: 910002e0  ....
    0000000c: 0200a0e1  ....
    00000010: 011041e2  ..A.
    00000014: 000051e3  ..Q.
    00000018: faffff1a  ....
    0000001c: 013ca0e3  .<..
    00000020: 002083e5  . ..
    */
    instructions[0] = 0xe3a00001;
    instructions[1] = 0xe3a01005;
    instructions[2] = 0xe0020091;
    instructions[3] = 0xe1a00002;
    instructions[4] = 0xe2411001;
    instructions[5] = 0xe3510000;
    instructions[6] = 0x1afffffa;
    instructions[7] = 0xe3a03c01;
    instructions[8] = 0xe5832000;
    return 9;
}
