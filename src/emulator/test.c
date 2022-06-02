#include "pipeliner.h"
#include "pipeliner.c"
#include "emulate.h"
// #include "emulate.c"
#include <assert.h>

// a unit test that asserts that extractBits works
void test_extractBits() {
    // 0x12345678
    assert(extract_bits(305419896, 0, 4) == 18);
    assert(extract_bits(305419896, 4, 8) == 52);
    assert(extract_bits(305419896, 8, 12) == 86);
    assert(extract_bits(305419896, 12, 16) == 120);
    printf("All tests passed for extract_bits\n");
}

// a unit test that asserts that decode works
// MAKE THIS ACTUALLY WORK!
void test_decode() {
    assert(decode(0x12345678) == &single_data_transfer);
    assert(decode(0x12345678) == &branch);
    assert(decode(0x12345678) == &data_processing);
    printf("All tests passed for decode\n");
}

// a unit test that asserts that cond_check works


