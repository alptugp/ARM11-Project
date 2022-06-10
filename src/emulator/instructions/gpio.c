#include "gpio.h"
// Returns true if the address maps to a GPIO pin.
bool is_gpio_pin_address(word address) {
    return ((address >= GPIO_PIN_0_9_ADDRESS && address <= GPIO_PIN_20_29_ADDRESS) || address == GPIO_SET_ADDRESS || address == GPIO_CLEAR_ADDRESS);
}


int gpio_output(word address, memory_t memory) {
    switch (address)
    {
        case GPIO_PIN_0_9_ADDRESS:
            printf("One GPIO pin from 0 to 9 has been accessed\n");
            return 0;
        case GPIO_PIN_10_19_ADDRESS:
            printf("One GPIO pin from 10 to 19 has been accessed\n");
            return 0;
        case GPIO_PIN_20_29_ADDRESS:
            printf("One GPIO pin from 20 to 29 has been accessed\n");
            return 0;
        case GPIO_CLEAR_ADDRESS:
            printf("PIN OFF\n");
            return 1;
        case GPIO_SET_ADDRESS:
            printf("PIN ON\n");
            return 1;
        default:
            return 0;
    }

}
