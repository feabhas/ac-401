// main.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdio.h>
#include <assert.h>
#include "timer.h"
#include "gpio.h"
#include "wms.h"
#include "stm.h"

int main(void)
{
    gpio_init();
    wms_init();
    buffer_init();
    stm_init();

    // simulate concurrent event handling
    stm_run();
    stm_run();
    stm_send_event(STM_WHITE_WASH);
    stm_send_event(STM_START);
    stm_run();
    stm_run();
    stm_send_event(STM_START); // ignored
    stm_run();
    stm_run();
    stm_send_event(STM_PAUSE);
    stm_run();
    stm_run();
    stm_send_event(STM_PAUSE); // ignored
    stm_run();
    stm_send_event(STM_START); // ignored
    stm_run();
    stm_run();
    stm_send_event(STM_RESUME);
    stm_run();
    stm_run();
    stm_send_event(STM_RESUME); // ignored
    stm_run();
    stm_run();
    stm_run();
    stm_run();
    stm_run();
    stm_run();
    stm_run();
    stm_run();
    stm_run();
    stm_run();
    stm_run();
    stm_run();
}
