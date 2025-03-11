#include "TSM.h"

ElevatorState TSM_state_stop(ElevatorSM *sm, StateEvent event)
{
    switch (event) {
    case event_enter:
        queue_empty(&(sm->queue));
        sm->elevator_direction = 0;
        elevio_motorDirection(sm->elevator_direction);
        lights_turn_off_all_orders();
        elevio_stopLamp(1);
        if (elevio_floorSensor() != -1) {
            door_open(sm);
        }
        break;
    case event_execute:
        elevio_motorDirection(sm->elevator_direction);
        elevio_stopLamp(1);
        return state_stop;
    case event_exit:
        elevio_stopLamp(0);
        door_timer_start(sm);
        if (elevio_floorSensor() == -1) {
            door_close(sm);
        }
        break;
    }
    return state_stop;
}

ElevatorState TSM_state_move(ElevatorSM *sm, StateEvent event)
{
    switch (event) {
    case event_enter:
        printf("Entering move state, target floor: %d\n", sm->target_floor);
        
        if (sm->target_floor == NO_ORDER) {
            printf("No target floor in move state! Returning to still\n");
            return state_still;
        }
        
        sm->elevator_direction = movement_choose_direction(sm);
        printf("Chosen direction: %d\n", sm->elevator_direction);
        
        elevio_motorDirection(sm->elevator_direction); 
        break;
        
    case event_execute:
        printf("Execute move state, target: %d, current: %d\n", sm->target_floor, elevio_floorSensor());

        if (sm->target_floor == NO_ORDER) {
            printf("No target floor in move state! Stopping motor\n");
            elevio_motorDirection(DIRN_STOP);
            return state_still;
        }
        
        ElevatorState next_state = movement_get_to_floor(sm);
        printf("movement_get_to_floor returned state: %d\n", next_state);
        
        return next_state;
        
    case event_exit:
        printf("Exiting move state\n");
        break;
    }
    return state_move;
}

ElevatorState TSM_state_deliver(ElevatorSM *sm, StateEvent event)
{
    switch (event) {
    case event_enter:
        printf("Entering deliver state for floor %d\n", sm->target_floor);
        break;
        
    case event_execute:
        printf("Execute deliver state for floor %d, queue count: %d\n", sm->target_floor, sm->queue.queue_count);
        
        int door_result = door_deliver_to_floor(sm);
        printf("door_deliver_to_floor returned: %d\n", door_result);
        
        if (door_result == 0) {
            printf("Delivery still in progress\n");
            return state_deliver;
        } else {
            printf("Delivery completed for floor %d\n", sm->target_floor);
            
            int remove_result = queue_remove(&(sm->queue), sm->target_floor);
            printf("queue_remove returned: %d\n", remove_result);
            
            lights_turn_off(sm->target_floor);
            
            if (sm->queue.queue_count == 0) {
                printf("Queue empty after delivery, moving to still state\n");
                sm->target_floor = NO_ORDER;
                return state_still;
            } else {
                sm->target_floor = sm->queue.queue_list[0];
                printf("Next target floor: %d\n", sm->target_floor);
                return state_move;
            }
        }
        
    case event_exit:
        printf("Exiting deliver state\n");
        break;
    }
    return state_deliver;
}

ElevatorState TSM_state_still(ElevatorSM *sm, StateEvent event)
{
    switch (event) {
    case event_enter:
        printf("Entering still state\n");
        elevio_motorDirection(DIRN_STOP);
        sm->elevator_direction = 0;
        break;
        
    case event_execute:
        printf("Execute still state, target floor: %d, queue count: %d\n", sm->target_floor, sm->queue.queue_count);
        
        if (door_is_open && !door_timer_expired(sm)) {
            return state_still;
        }
        
        if (door_is_open && door_timer_expired(sm)) {
            door_close(sm);
        }
        
        if (sm->queue.queue_count > 0) {
            printf("Orders in queue, moving to move state\n");
            return state_move;
        } else {
            printf("No orders, remaining in still state\n");
            return state_still;
        }
        
    case event_exit:
        printf("Exiting still state\n");
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
    printf("\n=== Begin TSM Update ===\n");
    printf("Current state: %d, Target floor: %d, Last floor: %d, Direction: %d\n",
           sm->current_state, sm->target_floor, sm->last_current_floor, sm->elevator_direction);
    
    int sensor_floor = elevio_floorSensor();
    printf("Current floor sensor: %d\n", sensor_floor);
    
    if (sensor_floor != -1) {
        sm->last_current_floor = sensor_floor;
    }
    lights_last_floor_lamp(sm);
    
    printf("Stop button: %d\n", elevio_stopButton());
    if (elevio_stopButton() == 1) {
        if (sm->current_state != state_stop) {
            printf("Stop button pressed - transitioning to stop state\n");
            TSM_call_exit(sm, sm->current_state);
            TSM_call_enter(sm, state_stop);
            sm->current_state = state_stop;
        }
        return state_stop;
    } else {
        if (sm->current_state == state_stop) {
            printf("Stop button released - transitioning to still state\n");
            TSM_call_exit(sm, state_stop);
            TSM_call_enter(sm, state_still);
            sm->current_state = state_still;
        }
    }
    
    sensors_update(&(sm->sensors));
    orders_register_order(sm);
    
    printf("Queue status: ");
    queue_print(&(sm->queue));
    
    int old_target = sm->target_floor;
    if (sm->queue.queue_count > 0) {
        sm->target_floor = sm->queue.queue_list[0];
    } else {
        sm->target_floor = NO_ORDER;
    }
    
    if (old_target != sm->target_floor) {
        printf("Target floor updated: %d -> %d\n", old_target, sm->target_floor);
    }
    
    ElevatorState old_state = sm->current_state;
    ElevatorState next_state = old_state;
    
    printf("Processing state: %d\n", old_state);
    
    switch (old_state) {
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
        printf("State transition: %d -> %d\n", old_state, next_state);
        TSM_call_exit(sm, old_state);
        TSM_call_enter(sm, next_state);
        sm->current_state = next_state;
    } else {
        printf("Remaining in state: %d\n", old_state);
    }
    
    printf("=== End TSM Update ===\n\n");
    return next_state;
}
