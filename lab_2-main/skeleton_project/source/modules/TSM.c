#include "TSM.h"


ElevatorState TSM_state_stop(StateEvent event)
{
    switch (event) {
    case event_enter:
        // Setup for stop state.
        break;
    case event_execute:
        // Example: remain in STOP state (or add logic to transition).
        return state_stop;
    case event_exit:
        // Cleanup for stop state.
        break;
    }
    return state_stop;
}

ElevatorState TSM_state_move(StateEvent event)
{
    switch (event) {
    case event_enter:
        // Start motor, etc.
        break;
    case event_execute:
        // Check if floor is reached. For demo, we immediately transition.
        return state_deliver;
    case event_exit:
        // Stop motor, etc.
        break;
    }
    return state_move;
}


ElevatorState TSM_state_deliver(StateEvent event)
{
    switch (event) {
    case event_enter:
        // Open doors, etc.
        break;
    case event_execute:
        // Stay in deliver state or decide next transition.
        return state_deliver;
    case event_exit:
        // Close doors, etc.
        break;
    }
    return state_deliver;
}

ElevatorState TSM_state_still(StateEvent event)
{
    switch (event) {
    case event_enter:
        // Open doors, etc.
        break;
    case event_execute:
        // Stay in deliver state or decide next transition.
        return state_deliver;
    case event_exit:
        // Close doors, etc.
        break;
    }
    return state_still;
}


void callExit(ElevatorState state)
{
    switch (state) {
    case state_stop:
        (void)TSM_state_stop(event_exit);
        break;
    case state_move:
        (void)TSM_state_move(event_exit);
        break;
    case state_deliver:
        (void)TSM_state_deliver(event_exit);
        break;
    case state_still:
        (void)TSM_state_still(event_exit);
        break;
    default:
        break;
    }
}


void callEnter(ElevatorState state)
{
    switch (state) {
    case state_stop:
        (void)TSM_state_stop(event_enter);
        break;
    case state_move:
        (void)TSM_state_move(event_enter);
        break;
    case state_deliver:
        (void)TSM_state_deliver(event_enter);
        break;
    default:
        break;
    }
}

/* Update function that dispatches the current state's EXECUTE event,
   and handles transitions by calling EXIT on the old state and ENTER on the new state */
ElevatorState TSM_update(ElevatorSM *sm)
{
    ElevatorState old_state = sm->current_state; // lagrer currentState som oldState
    ElevatorState next_state = old_state; // i tilfellet ingenting endrer seg

    switch (old_state) { // utfører logikken til staten
    case state_stop:
        next_state = TSM_state_stop(event_execute);
        break;
    case state_move:
        next_state = TSM_state_move(event_execute);
        break;
    case state_deliver:
        next_state = TSM_state_deliver(event_execute);
        break;
    default:
        break;
    }

    if (next_state != old_state) {
        callExit(old_state);
        callEnter(next_state);
        sm->current_state = next_state;
    }

    return next_state;
}
