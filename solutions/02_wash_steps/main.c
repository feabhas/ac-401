// main.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdio.h>
#include <stdbool.h>

typedef enum {
    No_step, Fill, Empty, Heat, Wash, Rinse, Spin, Complete
} Step;

static Step white_wash[] = {
    Fill, Heat, Wash, Empty, Fill, Rinse, Empty, Spin, Complete
};

static bool next_wash_step(void)
{
    static int next;
    printf("Step %d: action %d\n", next, white_wash[next]);
    // Optional
    const char* name = NULL;
    switch(white_wash[next]) {
        case Fill:     name = "Fill";  break;
        case Empty:    name = "Empty";    break;
        case Heat:     name = "Heat";     break;
        case Wash:     name = "Wash";     break;
        case Rinse:    name = "Rinse";    break;
        case Spin:     name = "Spin";     break;
        case Complete: name = "Complete"; break;
        default:       name = "No_step";  break;
    }
    printf("  %s\n", name);

    if (white_wash[next] == Complete) {
        next = 0;
        return false;
    }
    ++next;
    return true;
}

int main(void)
{
    while (next_wash_step())
    { /* pass */ }
}
