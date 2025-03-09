#include "door.h"
#include <unistd.h>

const int DOOR_OPEN_TIME_MS = 3000;
const int TIMER_CHECK_INTERVAL_MS = 100;

static int door_is_open = 0;

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
        printf("DEBUG [DOOR]: Door obstructed, cannot close\n");
        return 0;
    }
    
    elevio_doorOpenLamp(0);
    door_is_open = 0;
    printf("DEBUG [DOOR]: Door closed successfully\n");
    
    return 1;
}

int door_deliver_to_floor(ElevatorSM *sm) {
    printf("DEBUG [DOOR]: door_deliver_to_floor - door_is_open: %d\n", door_is_open);

    if (!door_is_open) {
        printf("DEBUG [DOOR]: Door not open, attempting to open\n");
        if (!door_open(sm)) {
            printf("DEBUG [DOOR]: Failed to open door\n");
            return 0;
        }
    }
    
    sleep(3);
    
    printf("DEBUG [DOOR]: Attempting to close door after sleep(3)\n");
    if (door_close(sm)) {
        printf("DEBUG [DOOR]: Door closed, delivery complete\n");
        return 1;
    } else {
        printf("DEBUG [DOOR]: Could not close door, delivery continuing...\n");
        return 0;
    }
}

