// main.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdio.h>
#include <assert.h>
#include "timer.h"
#include "gpio.h"
#include "wms.h"
#include "stm.h"
#include "usart.h"

int main(void)
{
    gpio_init();
    wms_init();
    wms_select_wash_type(WASH_WHITE);
    buffer_init();
    stm_init();
    usart_init();

    usart_send_str("Enter characters (# to stop)? ");
    char ch;
    do {
        ch = usart_get();
        usart_send(ch);
    } while (ch != '#');
}
