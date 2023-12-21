// main.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdio.h>
#include "timer.h"
#include "gpio.h"
#include "wms.h"

int main(void)
{
    gpio_init();
    for (unsigned i=0; i<5; ++i) {
        gpio_pin_on(GPIO_SS1);
        sleep(250);
        gpio_pin_off(GPIO_SS1);
        sleep(250);
    }

    wms_init();
    while (wms_next_wash_step())
    { /* pass */ }

    gpio_motor_direction(true);
    gpio_motor_on();
    sleep(3000);
    gpio_motor_direction(false);
    sleep(3000);
    gpio_motor_off();
}
