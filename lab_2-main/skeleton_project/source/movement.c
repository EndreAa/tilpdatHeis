#include "movement.h"


ElevatorState movement_get_to_floor(ElevatorSM *sm){
    if (sm->target_floor == elevio_floorSensor()){
        sm->elevator_direction = 0;
        elevio_motorDirection(sm->elevator_direction);
        return state_deliver;
    } else {
        elevio_motorDirection(sm->elevator_direction);
        return state_move;
    }
}

int movement_choose_direction(ElevatorSM *sm){
    if (sm->target_floor > sm->last_current_floor){ 
        return 1;
    } else if (sm->target_floor < sm->last_current_floor){
        return -1;
    } else {
        return 0;
    }
}

