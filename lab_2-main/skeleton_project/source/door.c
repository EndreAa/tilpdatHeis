#include "door.h"
#include <time.h>
#include <stdio.h>

const int DOOR_OPEN_TIME_MS = 3000;

static long door_timer = 0;
static int door_timer_active = 0;
static int door_is_open = 0;

static long get_current_time_ms() {
    struct timespec now;
    timespec_get(&now, TIME_UTC);
    return now.tv_sec * 1000L + now.tv_nsec / 1000000L;
}

void door_timer_start(ElevatorSM *sm) {
    door_timer = get_current_time_ms();
    door_timer_active = 1;
    printf("DEBUG [DOOR]: Timer started at %ld ms\n", door_timer);
}

int door_timer_expired(ElevatorSM *sm) {
    if (!door_timer_active) {
        return 0;
    }
    long current_time = get_current_time_ms();
    long elapsed_ms = current_time - door_timer;
    printf("DEBUG [DOOR]: Timer check - Elapsed: %ld ms of %d ms\n", elapsed_ms, DOOR_OPEN_TIME_MS);
    return (elapsed_ms >= DOOR_OPEN_TIME_MS);
}

int door_open(ElevatorSM *sm) {
    int current_floor = elevio_floorSensor();
    printf("DEBUG [DOOR]: Attempting to open door. Current floor: %d\n", current_floor);
    if (current_floor == -1) {
        printf("DEBUG [DOOR]: Cannot open door - not at valid floor\n");
        return 0;
    }
    elevio_doorOpenLamp(1);
    door_is_open = 1;
    printf("DEBUG [DOOR]: Door opened successfully at floor %d\n", current_floor);
    return 1;
}

int door_close(ElevatorSM *sm) {
    if (elevio_obstruction() == 1 || elevio_stopButton() == 1) {
        printf("DEBUG [DOOR]: Cannot close door due to obstruction or stop button\n");
        return 0;
    }
    elevio_doorOpenLamp(0);
    door_is_open = 0;
    door_timer_active = 0;
    printf("DEBUG [DOOR]: Door closed successfully\n");
    return 1;
}

int door_deliver_to_floor(ElevatorSM *sm) {
    printf("DEBUG [DOOR]: door_deliver_to_floor - door_is_open: %d\n", door_is_open);
    
    if (!door_is_open) {
        if (!door_open(sm)) {
            return 0;
        }
        door_timer_start(sm);
    }
    
    if (elevio_stopButton() == 1 || elevio_obstruction() == 1) {
        printf("DEBUG [DOOR]: Stop button pressed or obstruction detected, resetting timer\n");
        door_timer_start(sm);
        return 0;
    }
    
    if (door_timer_expired(sm)) {
        printf("DEBUG [DOOR]: Timer expired, attempting to close door\n");
        if (door_close(sm)) {
            printf("DEBUG [DOOR]: Door closed, delivery complete\n");
            return 1;
        } else {
            printf("DEBUG [DOOR]: Could not close door, delivery continuing...\n");
        }
    }
    
    return 0;
}
