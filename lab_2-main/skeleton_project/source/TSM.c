#include "TSM.h"


ElevatorState TSM_state_stop(ElevatorSM *sm, StateEvent event)
{
    switch (event) {
    case event_enter:
        queue_empty(&(sm->queue));
        sm->elevator_direction = 0;
        elevio_motorDirection(sm->elevator_direction);
        lights_turn_off_all_orders();
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
        printf("DEBUG [TSM %s]: Entering move state, target floor: %d\n", 
               get_timestamp(), sm->target_floor);
        
        if (sm->target_floor == NO_ORDER) {
            printf("DEBUG [TSM %s]: No target floor in move state! Returning to still\n", 
                   get_timestamp());
            return state_still;
        }
        
        sm->elevator_direction = movement_choose_direction(sm);
        printf("DEBUG [TSM %s]: Chosen direction: %d\n", 
               get_timestamp(), sm->elevator_direction);
        
        elevio_motorDirection(sm->elevator_direction); 
        break;
        
    case event_execute:
        printf("DEBUG [TSM %s]: Execute move state, target: %d, current: %d\n", 
               get_timestamp(), sm->target_floor, elevio_floorSensor());

        if (sm->target_floor == NO_ORDER) {
            printf("DEBUG [TSM %s]: No target floor in move state! Stopping motor\n", 
                   get_timestamp());
            elevio_motorDirection(DIRN_STOP);
            return state_still;
        }
        
        ElevatorState next_state = movement_get_to_floor(sm);
        printf("DEBUG [TSM %s]: movement_get_to_floor returned state: %d\n", 
               get_timestamp(), next_state);
        
        return next_state;
        
    case event_exit:
        printf("DEBUG [TSM %s]: Exiting move state\n", get_timestamp());
        break;
    }
    return state_move;
}

ElevatorState TSM_state_deliver(ElevatorSM *sm, StateEvent event)
{
    switch (event) {
    case event_enter:
        printf("DEBUG [TSM %s]: Entering deliver state for floor %d\n", 
               get_timestamp(), sm->target_floor);
        break;
        
    case event_execute:
        printf("DEBUG [TSM %s]: Execute deliver state for floor %d, queue count: %d\n", 
               get_timestamp(), sm->target_floor, sm->queue.queue_count);
        
        int door_result = door_deliver_to_floor(sm);
        printf("DEBUG [TSM %s]: door_deliver_to_floor returned: %d\n", 
               get_timestamp(), door_result);
        
        if (door_result == 0) {
            printf("DEBUG [TSM %s]: Delivery still in progress\n", get_timestamp());
            return state_deliver;
        } else {
            printf("DEBUG [TSM %s]: Delivery completed for floor %d\n", 
                   get_timestamp(), sm->target_floor);
            
            int remove_result = queue_remove(&(sm->queue), sm->target_floor);
            printf("DEBUG [TSM %s]: queue_remove returned: %d\n", 
                   get_timestamp(), remove_result);
            
            lights_turn_off(sm->target_floor);
            
            if (sm->queue.queue_count == 0) {
                printf("DEBUG [TSM %s]: Queue empty after delivery, moving to still state\n", 
                       get_timestamp());
                sm->target_floor = NO_ORDER;
                return state_still;
            } else {
                sm->target_floor = sm->queue.queue_list[0];
                printf("DEBUG [TSM %s]: Next target floor: %d\n", 
                       get_timestamp(), sm->target_floor);
                return state_move;
            }
        }
        
    case event_exit:
        printf("DEBUG [TSM %s]: Exiting deliver state\n", get_timestamp());
        break;
    }
    return state_deliver;
}

ElevatorState TSM_state_still(ElevatorSM *sm, StateEvent event)
{
    switch (event) {
    case event_enter:
        printf("DEBUG [TSM %s]: Entering still state\n", get_timestamp());
        elevio_motorDirection(DIRN_STOP);
        sm->elevator_direction = 0;
        break;
        
    case event_execute:
        printf("DEBUG [TSM %s]: Execute still state, target floor: %d, queue count: %d\n", 
               get_timestamp(), sm->target_floor, sm->queue.queue_count);
        
        if (sm->queue.queue_count > 0) {
            printf("DEBUG [TSM %s]: Orders in queue, moving to move state\n", 
                   get_timestamp());
            return state_move;
        } else {
            printf("DEBUG [TSM %s]: No orders, remaining in still state\n", 
                   get_timestamp());
            return state_still;
        }
        
    case event_exit:
        printf("DEBUG [TSM %s]: Exiting still state\n", get_timestamp());
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
    printf("\nDEBUG [TSM %s]: === Begin TSM Update ===\n", get_timestamp());
    printf("DEBUG [TSM %s]: Current state: %d, Target floor: %d, Last floor: %d, Direction: %d\n",
           get_timestamp(), sm->current_state, sm->target_floor, 
           sm->last_current_floor, sm->elevator_direction);
    printf("DEBUG [TSM %s]: Door status - Is open: %d\n", get_timestamp(), door_is_open);
    
    int sensor_floor = elevio_floorSensor();
    printf("DEBUG [TSM %s]: Current floor sensor: %d\n", get_timestamp(), sensor_floor);
    
    if (sensor_floor != -1) {
        sm->last_current_floor = sensor_floor;
    }
    lights_last_floor_lamp(sm);
    
    printf("DEBUG [TSM %s]: Stop button: %d\n", get_timestamp(), elevio_stopButton());
    if (elevio_stopButton() == 1) {
        if (sm->current_state != state_stop) {
            printf("DEBUG [TSM %s]: Stop button pressed - transitioning to stop state\n", 
                   get_timestamp());
            TSM_call_exit(sm, sm->current_state);
            TSM_call_enter(sm, state_stop);
            sm->current_state = state_stop;
        }
        return state_stop;
    } else {
        if (sm->current_state == state_stop) {
            printf("DEBUG [TSM %s]: Stop button released - transitioning to still state\n", 
                   get_timestamp());
            TSM_call_exit(sm, state_stop);
            TSM_call_enter(sm, state_still);
            sm->current_state = state_still;
        }
    }
    
    sensors_update(&(sm->sensors));
    orders_register_order(sm);
    
    printf("DEBUG [TSM %s]: Queue status: ", get_timestamp());
    queue_print(&(sm->queue));
    
    int old_target = sm->target_floor;
    if (sm->queue.queue_count > 0) {
        sm->target_floor = sm->queue.queue_list[0];
    } else {
        sm->target_floor = NO_ORDER;
    }
    
    if (old_target != sm->target_floor) {
        printf("DEBUG [TSM %s]: Target floor updated: %d -> %d\n", 
               get_timestamp(), old_target, sm->target_floor);
    }
    
    ElevatorState old_state = sm->current_state;
    ElevatorState next_state = old_state;
    
    printf("DEBUG [TSM %s]: Processing state: %d\n", get_timestamp(), old_state);
    
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
        printf("DEBUG [TSM %s]: State transition: %d -> %d\n", 
               get_timestamp(), old_state, next_state);
        TSM_call_exit(sm, old_state);
        TSM_call_enter(sm, next_state);
        sm->current_state = next_state;
    } else {
        printf("DEBUG [TSM %s]: Remaining in state: %d\n", get_timestamp(), old_state);
    }
    
    printf("DEBUG [TSM %s]: === End TSM Update ===\n\n", get_timestamp());
    return next_state;
}

static char* get_timestamp() {
    static char buffer[26];
    time_t timer;
    struct tm* tm_info;
    
    time(&timer);
    tm_info = localtime(&timer);
    
    strftime(buffer, 26, "%H:%M:%S", tm_info);
    return buffer;
}
