#include "movement.h"


ElevatorState movement_get_to_floor(ElevatorSM *sm){
    if (sm->target_floor == elevio_floorSensor()){
        sm->elevator_direction = 0;
        return state_deliver;
    } else {
        elevio_motorDirection(sm->elevator_direction);
        return state_move;
    }
}

int movement_choose_direction(ElevatorSM *sm){
    if (sm->target_floor > sm->last_current_floor){ 
        return 1;
    } else if (sm->target_floor == sm->last_current_floor){
        return 0;
    } else if (sm->target_floor < sm->last_current_floor){
        return -1;
    }
}

