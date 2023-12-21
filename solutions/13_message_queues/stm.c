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

static feabhOS_QUEUE event_queue;

void stm_init(feabhOS_QUEUE queue)
{
    event_queue = queue;
    current_state = IDLE;
}

void stm_run(void) {
    StmEvent event;
    duration_mSec_t timeout = behaviour ? NO_WAIT : WAIT_FOREVER;
    if ( feabhOS_queue_get(&event_queue, &event, timeout) == ERROR_OK ) {
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
    return feabhOS_queue_post(&event_queue, &event, NO_WAIT) == ERROR_OK;
}

// Action handlers

static void idle_white_wash(void)
{
    printf("[**STM**] white wash\n");
    wms_select_wash_type(WASH_WHITE);
}

static void idle_color_wash(void)
{
    printf("[**STM**] colour wash\n");
    wms_select_wash_type(WASH_COLOR);
}

static void idle_start_wash(void)
{
    printf("[**STM**] start wash\n");
}

static void washing(void)
{
    printf("[**STM**] next step\n");
    if (wms_next_wash_step()){
    }
    else {
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
