// main.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdio.h>
#include <assert.h>
#include "timer.h"
#include "gpio.h"
#include "wms.h"
#include "buffer.h"
#include "stm.h"
#include "usart.h"
#include "feabhOS_scheduler.h"
#include "feabhOS_task.h"

static void ui_task(void* param)
{
    (void)param;        // supress warning
    usart_init();
    usart_send_str("Echo loop: ");
    for (;;) {
        char ch = usart_get();
        usart_send(ch);
    }
}

static void wms_task(void* param)
{
    (void)param;        // supress warning
    wms_init();
    wms_select_wash_type(WASH_WHITE);
    while(wms_next_wash_step()) {
        // pass
    }
}

int main(void)
{
    gpio_init();
    buffer_init();
    feabhOS_scheduler_init();

    feabhOS_TASK wms;
    feabhOS_TASK ui;

    feabhOS_error error;

    error = feabhOS_task_create(&wms, wms_task, NULL, STACK_NORMAL, PRIORITY_NORMAL);
    assert(error == ERROR_OK);

    error = feabhOS_task_create(&ui, ui_task, NULL, STACK_NORMAL, PRIORITY_NORMAL);
    assert(error == ERROR_OK);

    feabhOS_scheduler_start();
}
