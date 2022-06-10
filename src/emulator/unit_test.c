#include "emulate_utils.h"
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

// a unit test that asserts that cond_check works
