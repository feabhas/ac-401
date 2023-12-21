// main.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdio.h>
#include "timer.h"

int main(void)
{
    for(unsigned i=0; i< 5; ++i) {
        printf("tick...\n");
        sleep(1000);
    };
}
