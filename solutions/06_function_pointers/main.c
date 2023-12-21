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
    wms_init();
    wms_select_wash_type(WASH_WHITE);
    while (wms_next_wash_step()) {
		// pass
	}
}
