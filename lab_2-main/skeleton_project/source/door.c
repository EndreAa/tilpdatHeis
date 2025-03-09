#include "door.h"

const int DOOR_OPEN_TIME_MS = 3000;
const int TIMER_CHECK_INTERVAL_MS = 100;

static struct timespec door_timer;
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
        clock_gettime(CLOCK_MONOTONIC, &door_timer);
        return 0;
    }

    if (was_obstructed) {
        clock_gettime(CLOCK_MONOTONIC, &door_timer); 
        was_obstructed = 0;   
        return 0;
    }
    
    elevio_doorOpenLamp(0);
    door_is_open = 0;
    door_timer_active = 0;
    
    return 1;
}

void door_timer_start(ElevatorSM *sm) {
    clock_gettime(CLOCK_MONOTONIC, &door_timer);
    door_timer_active = 1;
}

int door_timer_expired(ElevatorSM *sm) {
    if (!door_timer_active) {
        return 0;
    }
    
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    
    long elapsed_ms = (current_time.tv_sec - door_timer.tv_sec) * 1000 +
                      (current_time.tv_nsec - door_timer.tv_nsec) / 1000000;
    
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
