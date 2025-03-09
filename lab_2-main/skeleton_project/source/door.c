#include "door.h"


const int DOOR_OPEN_TIME_MS = 3000;
const int TIMER_CHECK_INTERVAL_MS = 100;

static clock_t door_timer;
static int door_is_open = 0;
static int door_timer_active = 0;


int door_open(ElevatorSM *sm) {
    int current_floor = elevio_floorSensor();

    if (current_floor == -1) {
        return 0;
    }
    
    elevio_doorOpenLamp(1);
    door_is_open = 1;
    
    return 1;
}

int door_close(ElevatorSM *sm) {
    static int was_obstructed = 0;
    
    if (elevio_obstruction() == 1) {
        was_obstructed = 1;
        door_timer = clock();
        return 0;
    }

    if (was_obstructed) {
        door_timer = clock(); 
        was_obstructed = 0;   
        return 0;
    }
    
    elevio_doorOpenLamp(0);
    door_is_open = 0;
    door_timer_active = 0;
    
    return 1;
}


void door_timer_start(ElevatorSM *sm) {
    door_timer = clock();
    door_timer_active = 1;
}

int door_timer_expired(ElevatorSM *sm) {
    if (!door_timer_active) {
        return 0;
    }
    
    clock_t current_time = clock();
    double elapsed_ms = (double)(current_time - door_timer) * 1000 / CLOCKS_PER_SEC;
    
    return elapsed_ms >= DOOR_OPEN_TIME_MS;
}

int door_deliver_to_floor(ElevatorSM *sm) {

    if (!door_is_open) {
        if (door_open(sm)) {
            door_timer_start(sm);
        }
    }

    if (door_timer_expired(sm)) {
        if (door_close(sm)) {
            return 1;
        }
    }

    return 0;
}