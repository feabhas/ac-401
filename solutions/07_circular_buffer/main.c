// main.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdio.h>
#include <assert.h>
#include "timer.h"
#include "gpio.h"
#include "wms.h"
#include "buffer.h"

int main(void)
{
    buffer_init();
    Buffer* buffer = buffer_create();

    const buffer_t value = 42;
    buffer_t item;

    // test empty and put/get
    assert(buffer_get(buffer, &item) == BUFFER_EMPTY);
    assert(buffer_put(buffer, &value) == BUFFER_OK);
    assert(buffer_size(buffer) == 1);
    assert(buffer_get(buffer, &item) == BUFFER_OK);
    assert(value == item);
    assert(buffer_size(buffer) == 0);
    assert(buffer_get(buffer, &item) == BUFFER_EMPTY);
    assert(buffer_size(buffer) == 0);

    // test buffer capacity
    for (int i=0; i<BUFFER_CAPACITY; i++) {
        assert(buffer_put(buffer, &i) == BUFFER_OK);
    }
    assert(buffer_size(buffer) == BUFFER_CAPACITY);
    assert(buffer_put(buffer, &value) == BUFFER_FULL);
    for (int i=0; i<BUFFER_CAPACITY; i++) {
        assert(buffer_get(buffer, &item) == BUFFER_OK);
        assert(i == item);
    }
    assert(buffer_get(buffer, &item) == BUFFER_EMPTY);

   // test buffer cycling
    for (int i=0; i<BUFFER_CAPACITY; i++) {
        assert(buffer_put(buffer, &i) == BUFFER_OK);
    }
    assert(buffer_get(buffer, &item) == BUFFER_OK);
    assert(0 == item);
    assert(buffer_put(buffer, &value) == BUFFER_OK);
    assert(buffer_size(buffer) == BUFFER_CAPACITY);
    assert(buffer_put(buffer, &value) == BUFFER_FULL);
    for (int i=1; i<BUFFER_CAPACITY; i++) {
        assert(buffer_get(buffer, &item) == BUFFER_OK);
        assert(i == item);
    }
    assert(buffer_get(buffer, &item) == BUFFER_OK);
    assert(value == item);
    assert(buffer_get(buffer, &item) == BUFFER_EMPTY);

    // gpio_init();
    // wms_init();
    // wms_select_wash_type(WASH_WHITE);
    // while (wms_next_wash_step())
    // { /* pass */ }
}

