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


    ElevatorSM sm = {
        .current_state = state_still,
        .target_floor = -1,
        .elevator_direction = 0,
        .last_current_floor = initial_floor,
        .sensors = {
            .sensor_stop_button = elevio_stopButton(),
            .sensor_current_floor = inital_floor,
            .sensor_obstacle = false,
            .sensor_button_values = { { false } }  // Initialiserer hele 2D-arrayen med false
        },
        .queue = {
            .queue_first_pri = 0,
            .queue_direction = 0,
            .queue_current_floor = 0,
            .queue_size = 0,
            .queue_list = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 }
        }
    };

    while (start_complete){
        ElevatorState new_state = TSM_update(&sm);
    }
}