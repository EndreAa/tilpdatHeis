#include "start.h"

bool start_find_floor(){
    while (elevio_floorSensor() == -1){
        elevio_motorDirection(DIRN_DOWN);
    }
    elevio_motorDirection(DIRN_STOP);
    return 1;
}
    

void start_init(){
    bool start_complete = start_find_floor();

    int initial_floor = elevio_floorSensor();
    ElevatorSM sm = {state_still, -1, 0, initial_floor};

    while (start_complete){
        ElevatorState new_state = TSM_update(&sm);
    }
}