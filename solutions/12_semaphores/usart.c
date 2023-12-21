// usart.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <assert.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "usart_utils.h"
#include "usart.h"
#include "buffer.h"
#include "feabhOS_semaphore.h"

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

feabhOS_SEMAPHORE rx_data;

// defined in stm32f4xx.h
// #define USART3_BASE 0x40004800u

static volatile UsartStatus *const sr = (UsartStatus*)(USART3_BASE);
static volatile UsartData *const dr = (UsartData*)(USART3_BASE + 0x04u);

void usart_init(void)
{
    NVIC_DisableIRQ(USART3_IRQn);
    buffer = buffer_create();
    feabhOS_semaphore_create(&rx_data, BUFFER_CAPACITY, 0);

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
	int value;
    feabhOS_semaphore_take(&rx_data, WAIT_FOREVER);
    NVIC_DisableIRQ(USART3_IRQn);
    buffer_get(buffer, &value);
    NVIC_EnableIRQ(USART3_IRQn);
	return (char)value;
}

bool usart_try_get(char *const holder)
{
	bool status = false;
    NVIC_DisableIRQ(USART3_IRQn);
	if (feabhOS_semaphore_take(&rx_data, NO_WAIT) == ERROR_OK) {
		int value;
		buffer_get(buffer, &value);
		*holder = (char)value;
		status = true;
	}
    NVIC_EnableIRQ(USART3_IRQn);
    return status;
}

void USART3_IRQHandler(void)
{
    if (sr->RXNE != 0) {
        int value = (int)dr->rx;
        if (buffer_put(buffer, &value) == BUFFER_OK) {
            feabhOS_semaphore_give_ISR(&rx_data);
        }
    }
}


