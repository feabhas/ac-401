// gpio.h
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#pragma once
#ifndef GPIO_H
#define GPIO_H
#include <stdbool.h>

typedef enum gpio_pin {
    GPIO_SS1 = 8, GPIO_SS2, GPIO_SS3, GPIO_SS4,
    GPIO_MOTOR, GPIO_MOTOR_DIR
} GpioPin;

extern void gpio_init(void);
extern void gpio_set_seven_segment(int value);

extern void gpio_pin_on(GpioPin pin);
extern void gpio_pin_off(GpioPin pin);

// Optional

void gpio_motor_on();
void gpio_motor_off();
void gpio_motor_direction(bool clockwise);

#endif
