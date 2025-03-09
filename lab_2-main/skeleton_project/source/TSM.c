#include "TSM.h"


ElevatorState TSM_state_stop(ElevatorSM *sm, StateEvent event)
{
    switch (event) {
    case event_enter:
        queue_empty(&(sm->queue));
        sm->elevator_direction = 0;
        elevio_motorDirection(sm->elevator_direction);
        break;
    case event_execute:
        elevio_motorDirection(sm->elevator_direction);
        elevio_stopLamp(1);
        return state_stop;
    case event_exit:
        elevio_stopLamp(0);
        break;
    }
    return state_stop;
}

ElevatorState TSM_state_move(ElevatorSM *sm, StateEvent event)
{
    switch (event) {
    case event_enter:
            sm->elevator_direction = movement_choose_direction(sm);
            elevio_motorDirection(sm->elevator_direction); 
            break;
    case event_execute:
        movement_get_to_floor(sm);
    case event_exit:
        break;
    }
    return state_move;
}


ElevatorState TSM_state_deliver(ElevatorSM *sm, StateEvent event)
{
    switch (event) {
    case event_enter:
        break;
    case event_execute:
        if (door_deliver_to_floor(sm) == 0){
            return state_deliver;
        } else {
            queue_remove(&(sm->queue), sm->target_floor);
            lights_turn_off(sm->target_floor);
            return state_move;
        }
    case event_exit:
        break;
    }
    return state_deliver;
}

ElevatorState TSM_state_still(ElevatorSM *sm, StateEvent event)
{
    switch (event) {
    case event_enter:
        break;
    case event_execute:
        if (sm->target_floor != -1) { // A new order has been placed.
            return state_move;
        } else {
            return state_still;
        }
    case event_exit:
        break;
    }
    return state_still;
}


void TSM_call_exit(ElevatorSM *sm, ElevatorState state)
{
    switch (state) {
    case state_stop:
        (void)TSM_state_stop(sm, event_exit);
        break;
    case state_move:
        (void)TSM_state_move(sm, event_exit);
        break;
    case state_deliver:
        (void)TSM_state_deliver(sm, event_exit);
        break;
    case state_still:
        (void)TSM_state_still(sm, event_exit);
        break;
    default:
        break;
    }
}


void TSM_call_enter(ElevatorSM *sm, ElevatorState state)
{
    switch (state) {
    case state_stop:
        (void)TSM_state_stop(sm, event_enter);
        break;
    case state_move:
        (void)TSM_state_move(sm, event_enter);
        break;
    case state_deliver:
        (void)TSM_state_deliver(sm, event_enter);
        break;
    case state_still:
        (void)TSM_state_still(sm, event_enter);
        break;
    default:
        break;
    }
}


ElevatorState TSM_update(ElevatorSM *sm){
    int sensor_floor = elevio_floorSensor();
    if (sensor_floor != -1) {
        sm->last_current_floor = sensor_floor;
    }
    lights_last_floor_lamp(sm);

    if (elevio_stopButton() == 1) {
        if (sm->current_state != state_stop) {
            TSM_call_exit(sm, sm->current_state);
            TSM_call_enter(sm, state_stop);
            sm->current_state = state_stop;
        }
        return state_stop;
    } else {
        if (sm->current_state == state_stop) {
            TSM_call_exit(sm, state_stop);
            TSM_call_enter(sm, state_still);
            sm->current_state = state_still;
        }
    }
    

    sensors_update(&(sm->sensors));
    orders_register_order(sm);
    sm->target_floor = sm->queue.queue_list[0];

    ElevatorState old_state = sm->current_state; // lagrer currentState som oldState
    ElevatorState next_state = old_state; // i tilfellet ingenting endrer seg

    switch (old_state) { // utfører hovedfunksjonen til staten
    case state_still:
        next_state = TSM_state_still(sm, event_execute);
        break;
    case state_move:
        next_state = TSM_state_move(sm, event_execute);
        break;
    case state_deliver:
        next_state = TSM_state_deliver(sm, event_execute);
        break;
    default:
        break;
    }

    if (next_state != old_state) {
        TSM_call_exit(sm, old_state);
        TSM_call_enter(sm, next_state);
        sm->current_state = next_state;
    }

    return next_state;
}
