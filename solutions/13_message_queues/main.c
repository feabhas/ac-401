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
#include "feabhOS_queue.h"


static void ui_task(void* param)
{
    feabhOS_QUEUE queue = (feabhOS_QUEUE)param;
    usart_init();
    usart_send_str(
        "Action choices:\r\n"
        "  1) white wash\r\n"
        "  2) colour wash\r\n"
        "  p) pause\r\n"
        "  r) resume\r\n"
        "  s) start\r\n"
        "Choice? "
    );
    for (;;) {
        char ch = usart_get();
        usart_send(ch);
        StmEvent event = STM_NONE;
        switch (ch) {
            case '1': event = STM_WHITE_WASH; break;
            case '2': event = STM_COLOR_WASH; break;
            case 'p': case 'P': event = STM_PAUSE; break;
            case 'r': case 'R': event = STM_RESUME; break;
            case 's': case 'S': event = STM_START; break;
        }
        if (event != STM_NONE) {
            feabhOS_queue_post(&queue, &event, NO_WAIT);
        }
    }
}

static void wms_task(void* param)
{
    feabhOS_QUEUE queue = (feabhOS_QUEUE)param;
    wms_init();
    stm_init(queue);
    for (;;) {
        stm_run();
    }
}

int main(void)
{
    gpio_init();
    buffer_init();
    feabhOS_scheduler_init();

    feabhOS_TASK wms;
    feabhOS_TASK ui;
    feabhOS_QUEUE queue;

    feabhOS_error error;

    error = feabhOS_queue_create(&queue, sizeof(StmEvent), 8);
    assert(error == ERROR_OK);

    error = feabhOS_task_create(&wms, wms_task, queue, STACK_NORMAL, PRIORITY_NORMAL);
    assert(error == ERROR_OK);

    error = feabhOS_task_create(&ui, ui_task, queue, STACK_NORMAL, PRIORITY_NORMAL);
    assert(error == ERROR_OK);

    feabhOS_scheduler_start();
}
