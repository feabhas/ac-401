// gpio.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdint.h>
#include "gpio.h"

enum {GPIO_A, GPIO_B, GPIO_C, GPIO_D};

#define AHB1ENR    0x40023830u
#define GPIOD_BASE 0x40020C00u

static volatile uint32_t *const ahb1enr = (uint32_t*)AHB1ENR;

static volatile uint32_t *const moder = (uint32_t*) GPIOD_BASE;
// static volatile const uint32_t *const idr = (uint32_t*) (GPIOD_BASE + 0x10u);
static volatile uint32_t *const odr = (uint32_t*) (GPIOD_BASE + 0x14u);

void gpio_init(void)
{
    uint32_t port = *ahb1enr;
    port |= (0x1u << GPIO_D);
    *ahb1enr = port;

    port = *moder;
    port &= 0xFFFF;
    port |= (0x1u << GPIO_SS1*2) | (0x1u << GPIO_SS2*2) |
            (0x1u << GPIO_SS3*2) | (0x1u << GPIO_SS4*2);
    port |= (0x1u << GPIO_MOTOR*2) | (0x1u << GPIO_MOTOR_DIR*2);
    *moder = port;

    gpio_pin_off(GPIO_MOTOR);
}

void gpio_pin_on(GpioPin pin)
{
    uint32_t port = *odr;
    port |= (0x1u << pin);
    *odr = port;
}

void gpio_pin_off(GpioPin pin)
{
    uint32_t port = *odr;
    port &= ~(0x1u << pin);
    *odr = port;
}

void gpio_set_seven_segment(int value)
{
    uint32_t port = *odr;
    port &= ~(0xFu << GPIO_SS1);
    port |= (((uint32_t)value & 0xFu) << GPIO_SS1);
    *odr = port;
}

// Optional

void gpio_motor_on()
{
    gpio_pin_on(GPIO_MOTOR);
}

void gpio_motor_off()
{
    gpio_pin_off(GPIO_MOTOR);
}

void gpio_motor_direction(bool clockwise)
{
    if (clockwise) {
        gpio_pin_off(GPIO_MOTOR_DIR);
    }
    else {
        gpio_pin_on(GPIO_MOTOR_DIR);
    }
}
