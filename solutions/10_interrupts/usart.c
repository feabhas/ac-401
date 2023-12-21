// usart.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdint.h>
#include "stm32f4xx.h"
#include "usart_utils.h"
#include "usart.h"
#include "buffer.h"

// Status register
typedef struct
{
  uint32_t          : 5;
  uint32_t RXNE     : 1;
  uint32_t          : 1;
  uint32_t TXE      : 1;
} UsartStatus;

// Data register
typedef union
{
  uint32_t tx;
  uint32_t rx;
} UsartData;

static Buffer* buffer;

// defined in stm32f4xx.h
// #define USART3_BASE 0x40004800u

static volatile UsartStatus *const sr = (UsartStatus*)(USART3_BASE);
static volatile UsartData *const dr = (UsartData*)(USART3_BASE + 0x04u);

void usart_init(void)
{
    NVIC_DisableIRQ(USART3_IRQn);
    buffer = buffer_create();
    usart_utils_init();
    usart_utils_enable_rx_interrupts();
    NVIC_EnableIRQ(USART3_IRQn);
}

void usart_send(char c)
{
    while (sr->TXE == 0) {
        // pass
    }
    dr->tx = c;
}

void usart_send_str (const char* str)
{
    while (*str) {
        usart_send(*str);
        str++;
    }
}

char usart_get(void)
{
    char ch;
    while (!usart_try_get(&ch)) {
        // pass
    }
    return ch;
}

bool usart_try_get(char *const holder)
{
    NVIC_DisableIRQ(USART3_IRQn);
    int value;
    BufferStatus status = buffer_get(buffer, &value);
    NVIC_EnableIRQ(USART3_IRQn);
    if (status == BUFFER_OK) {
        *holder = (char)value;
        return true;
    }
    return false;
}

void USART3_IRQHandler(void)
{
    if (sr->RXNE != 0) {
        int value = (int)dr->rx;
        buffer_put(buffer, &value);
    }
}
