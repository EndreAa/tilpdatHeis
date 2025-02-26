#ifndef ELEVATOR_SM_H
#define ELEVATOR_SM_H

#include <stdbool.h>
#include <stdio.h>


typedef enum {
    state_stop,
    state_move,
    state_deliver,
    state_still
} ElevatorState;


typedef enum {
    event_enter,
    event_execute,
    event_exit
} StateEvent;


typedef struct {
    ElevatorState current_state;
    int current_floor;
    int target_floor; 
} ElevatorSM;

/* State function prototypes */
ElevatorState TSM_state_stop(StateEvent event);
ElevatorState TSM_state_move(StateEvent event);
ElevatorState TSM_state_deliver(StateEvent event);
ElevatorState TSM_state_still(StateEvent event);

/* Helper function prototypes for calling state entry/exit actions */
void callExit(ElevatorState state);
void callEnter(ElevatorState state);

/* Update function prototype */
ElevatorState TSM_update(ElevatorSM *sm);

#endif
