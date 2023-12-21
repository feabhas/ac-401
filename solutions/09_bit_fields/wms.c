// wms.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdio.h>
#include "timer.h"
#include "wms.h"
#include "gpio.h"

#define WASH_STEP_SIZE 15

typedef void(*WashStep)(int);

// Wash Step functions

static void wms_fill(int step)
{
    printf("Step %d: action %s\n", step, "Fill");
    sleep(500);
}

static void wms_empty(int step)
{
    printf("Step %d: action %s\n", step, "Empty");
    sleep(500);
}

static void wms_heat(int step)
{
    printf("Step %d: action %s\n", step, "Heat");
    sleep(1000);
}

static void wms_wash(int step)
{
    printf("Step %d: action %s\n", step, "Wash");
    sleep(2000);
}

static void wms_rinse(int step)
{
    printf("Step %d: action %s\n", step, "Rinse");
    sleep(1000);
}

static void wms_spin(int step)
{
    printf("Step %d: action %s\n", step, "Spin");
    sleep(1000);
}

// Optional wash steps

static void wms_wash_motor(int step)
{
    printf("Step %d: action %s\n", step, "Wash");
    bool direction = true;
    gpio_motor_on();
    for (int i=0; i<4; ++i) {
        gpio_motor_direction(direction);
        sleep(1000);
        direction = !direction;
    }
    gpio_motor_off();
}

static void wms_rinse_motor(int step)
{
    printf("Step %d: action %s\n", step, "Rinse");
    gpio_motor_direction(true);
    gpio_motor_on();
    sleep(1000);
    gpio_motor_direction(false);
    sleep(1000);
    gpio_motor_off();
}

static void wms_spin_motor(int step)
{
    printf("Step %d: action %s\n", step, "Spin");
    gpio_motor_direction(true);
    gpio_motor_on();
    sleep(2000);
    gpio_motor_off();
}

// Wash programmes

static const WashStep wms_end = NULL;

static WashStep wash_types[WASH_TYPE_SIZE][WASH_STEP_SIZE] = {
    [WASH_WHITE] = { wms_fill, wms_heat, wms_wash, wms_empty,
                     wms_fill, wms_rinse, wms_empty, wms_spin, wms_end},
    [WASH_COLOR] = { wms_fill, wms_wash_motor, wms_empty,
                     wms_fill, wms_heat, wms_wash_motor, wms_empty,
                     wms_fill, wms_rinse_motor, wms_empty, wms_spin_motor, wms_end},
};

// Wash algorithm

static WashStep* current_wash;
static WashStep* next;

void wms_init(void) {
    wms_select_wash_type(WASH_WHITE);
}

void wms_select_wash_type(WmsWashType wash) {
	// check if no wash in progress
    if (next == current_wash) {
        current_wash = wash_types[wash];
        next = current_wash;
	}
}

bool wms_next_wash_step(void)
{
    int step = next-current_wash;
    gpio_set_seven_segment(step);
    if (*next != wms_end) {
        (*next)(step);
        ++next;
        return true;
    }
    next = current_wash;
    return false;
}

