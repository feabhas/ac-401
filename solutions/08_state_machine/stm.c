// stm.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdio.h>
#include "stm.h"
#include "wms.h"

typedef enum {
     IDLE, WASHING, PAUSED, NUM_STATES
} State;

typedef void(*Action)(void);
typedef void(*Behaviour)(void);

typedef struct {
    Action do_action;
    State next_state;
    Behaviour do_behaviour;
} StateCell;

static void idle_white_wash(void);
static void idle_color_wash(void);
static void idle_start_wash(void);
static void washing(void);
static void washing_complete(void);
static void washing_pause(void);
static void paused_resume(void);

// Meally state machine

static StateCell stm[NUM_STATES][STM_NUM_EVENTS] = {
    [IDLE][STM_WHITE_WASH] =   {idle_white_wash, IDLE, NULL},
    [IDLE][STM_COLOR_WASH] =   {idle_color_wash, IDLE, NULL},
    [IDLE][STM_START] =        {idle_start_wash, WASHING, washing},
    [WASHING][STM_COMPLETE] =  {washing_complete, IDLE, NULL},
    [WASHING][STM_PAUSE] =     {washing_pause, PAUSED, NULL},
    [PAUSED][STM_RESUME] =     {paused_resume, WASHING, washing},
};

static State current_state;
static Behaviour behaviour;

static Buffer* event_queue;

void stm_init(void)
{
    event_queue = buffer_create();
    current_state = IDLE;
}

void stm_run(void) {
    buffer_t item;
    if (buffer_get(event_queue, &item) == BUFFER_OK) {
        StmEvent event = (StmEvent)item;
        if (event != STM_NUM_EVENTS && stm[current_state][event].do_action) {
            stm[current_state][event].do_action();
            behaviour = stm[current_state][event].do_behaviour;
            current_state = stm[current_state][event].next_state;
        }
        else {
            printf("[**STM**] event %d ignored in state %d\n", event, current_state);
        }
    }
    if (behaviour) {
       behaviour();
    }
}

// Event injection

bool stm_send_event(StmEvent event)
{
    int item = (buffer_t)event;
    return buffer_put(event_queue, &item);
}

// Action handlers

static void idle_white_wash(void)
{
    printf("[**STM**] white wash\n");
    wms_select_wash_type(WASH_WHITE);
}

static void idle_color_wash(void)
{
    printf("[**STM**] white wash\n");
    wms_select_wash_type(WASH_COLOR);
}

static void idle_start_wash(void)
{
    printf("[**STM**] start wash\n");
    // stm_send_event(STM_NEXT_STEP);
}

static void washing(void)
{
    printf("[**STM**] next step\n");
    if (!wms_next_wash_step()) {
        stm_send_event(STM_COMPLETE);
    }
}

static void washing_complete(void)
{
    printf("[**STM**] wash complete\n");
}

static void washing_pause(void)
{
    printf("[**STM**] pause wash\n");
}

static void paused_resume(void)
{
    printf("[**STM**] resume wash\n");
}

// Moore state machine (outline)

typedef enum {
    MOORE_START, MOORE_NEXT_STEP, MOORE_COMPLETE,
    MOORE_PAUSE, MOORE_RESUME, MOORE_NUM_EVENTS
} MooreEvent;

static bool moore_event(MooreEvent event)
{
    int item = (buffer_t)event;
    return buffer_put(event_queue, &item);
}

// Action handlers

static State moore_start_wash(void) { return WASHING; }
static State moore_next_step(void)  {
    if (wms_next_wash_step()) {
        moore_event(MOORE_NEXT_STEP);
    }
    else {
        moore_event(MOORE_COMPLETE);
    }
    return WASHING;
}
static State moore_complete(void)   { return IDLE; }
static State moore_pause(void)      { return PAUSED; }
static State moore_resume(void)     { return WASHING; }

typedef State (*MooreAction)(void);

static MooreAction stm_moore[NUM_STATES][MOORE_NUM_EVENTS] = {
    [IDLE][MOORE_START] =        moore_start_wash,
    [WASHING][MOORE_NEXT_STEP] = moore_next_step,
    [WASHING][MOORE_COMPLETE] =  moore_complete,
    [WASHING][MOORE_PAUSE] =     moore_pause,
    [PAUSED][MOORE_RESUME] =     moore_resume,
};

void moore_run(void) {
    buffer_t item;
    if (buffer_get(event_queue, &item) == BUFFER_EMPTY) {
        return;
    }
    StmEvent event = (StmEvent)item;
    if (event != STM_NUM_EVENTS && stm_moore[current_state][event]) {
        current_state = stm_moore[current_state][event]();
   }
}
