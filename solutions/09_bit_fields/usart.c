// usart.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdint.h>
#include "usart_utils.h"
#include "usart.h"

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

#define USART3_BASE 0x40004800u

static volatile UsartStatus *const sr = (UsartStatus*)(USART3_BASE);
static volatile UsartData *const dr = (UsartData*)(USART3_BASE + 0x04u);

void usart_init(void)
{
    usart_utils_init();
}

void usart_send(char c)
{
    while (sr->TXE == 0)
    { /* pass */}
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
    if (sr->RXNE == 0) {
        return false;
    }
    *holder = (char)dr->rx;
    return true;
}
