#include "door.h"


const int DOOR_OPEN_TIME_MS = 3000;
const int TIMER_CHECK_INTERVAL_MS = 100;

static clock_t door_timer;
static int door_is_open = 0;
static int door_timer_active = 0;


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
    static int was_obstructed = 0;
    
    printf("DEBUG [DOOR]: Attempting to close door. Obstruction: %d, Was obstructed: %d\n", 
           elevio_obstruction(), was_obstructed);
    
    if (elevio_obstruction() == 1) {
        printf("DEBUG [DOOR]: Door obstructed, resetting timer\n");
        was_obstructed = 1;
        door_timer = clock();
        return 0;
    }

    if (was_obstructed) {
        printf("DEBUG [DOOR]: Previously obstructed, resetting timer\n");
        door_timer = clock(); 
        was_obstructed = 0;   
        return 0;
    }
    
    elevio_doorOpenLamp(0);
    door_is_open = 0;
    door_timer_active = 0;
    printf("DEBUG [DOOR]: Door closed successfully\n");
    
    return 1;
}


void door_timer_start(ElevatorSM *sm) {
    door_timer = clock();
    door_timer_active = 1;
    printf("DEBUG [DOOR]: Timer started at %ld ms\n", (long)door_timer);
}

int door_timer_expired(ElevatorSM *sm) {
    if (!door_timer_active) {
        printf("DEBUG [DOOR]: Timer not active\n");
        return 0;
    }
    
    clock_t current_time = clock();
    double elapsed_ms = (double)(current_time - door_timer) * 1000 / CLOCKS_PER_SEC;
    
    printf("DEBUG [DOOR]: Timer check - Elapsed: %.2f ms of %d ms\n", 
           elapsed_ms, DOOR_OPEN_TIME_MS);
    
    if (elapsed_ms >= DOOR_OPEN_TIME_MS) {
        printf("DEBUG [DOOR]: Timer EXPIRED\n");
        return 1;
    }
    
    return 0;
}

int door_deliver_to_floor(ElevatorSM *sm) {
    printf("DEBUG [DOOR]: door_deliver_to_floor - door_is_open: %d, timer_active: %d\n", 
           door_is_open, door_timer_active);

    if (!door_is_open) {
        printf("DEBUG [DOOR]: Door not open, attempting to open\n");
        if (door_open(sm)) {
            printf("DEBUG [DOOR]: Door opened, starting timer\n");
            door_timer_start(sm);
        } else {
            printf("DEBUG [DOOR]: Failed to open door\n");
        }
    }

    if (door_timer_expired(sm)) {
        printf("DEBUG [DOOR]: Timer expired, attempting to close door\n");
        if (door_close(sm)) {
            printf("DEBUG [DOOR]: Door closed, delivery complete\n");
            return 1;  // Delivery completed
        } else {
            printf("DEBUG [DOOR]: Could not close door, delivery continuing...\n");
        }
    } else {
        printf("DEBUG [DOOR]: Waiting for timer to expire\n");
    }

    return 0;  // Delivery still in progress
}
