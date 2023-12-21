// main.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdio.h>
#include <stdbool.h>

typedef enum {
    No_step, Fill, Empty, Heat, Wash, Rinse, Spin, Complete
} Step;

static const char *const step_names[] = {
    "No_step", "Fill", "Empty", "Heat",
    "Wash", "Rinse", "Spin",  "Complete",
};

static Step white_wash[] = {
    Fill, Heat, Wash, Empty, Fill, Rinse, Empty, Spin, Complete
};

static Step* next = white_wash;

bool next_wash_step(void)
{
    printf("Step %d: action %s\n", (next-white_wash), step_names[*next]);
    if (*next != Complete) {
		++next;
		return true;
    }
	next = white_wash;
	return false;
}

int main(void)
{
    while (next_wash_step())
    { /* pass */ }
}
