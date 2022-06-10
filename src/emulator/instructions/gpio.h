#include "../emulate_utils.h"
#define GPIO_PIN_0_9_ADDRESS 0x20200000
#define GPIO_PIN_10_19_ADDRESS 0x20200004
#define GPIO_PIN_20_29_ADDRESS 0x20200008
#define GPIO_CLEAR_ADDRESS 0x20200028
#define GPIO_SET_ADDRESS 0x2020001C
 
bool is_gpio_pin_address(word address);
int gpio_output(word address, memory_t memory);
