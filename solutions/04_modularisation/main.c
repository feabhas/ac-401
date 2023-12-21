// main.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdio.h>
#include "wms.h"

int main(void)
{
    wms_init();
    while (wms_next_wash_step())
    { /* pass */ }
}
