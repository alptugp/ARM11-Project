#include <assert.h>
#include "pipeliner.h"
#include "pipeliner.c"

// a unit test that asserts that decode works
// MAKE THIS ACTUALLY WORK!
void test_decode() {
    assert(decode(0x12345678) == &single_data_transfer);
    assert(decode(0x12345678) == &branch);
    assert(decode(0x12345678) == &data_processing);
    printf("All tests passed for decode\n");
}