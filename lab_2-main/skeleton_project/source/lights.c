#include "lights.h"

void lights_last_floor_lamp(ElevatorSM *sm){
    elevio_floorIndicator(sm->last_current_floor);
}

void lights_turn_on(int floor, int button){
    elevio_buttonLamp(floor, button, 1);
}

void lights_turn_off(int floor){
    for (int i = 0; i < 3; ++i){
        elevio_buttonLamp(floor, i, 0);
    }
}

void lights_turn_off_all_orders(){
    for (int i = 0; i < 3; ++i){
        for (int j = 0; j < 4; ++j){
            elevio_buttonLamp(j, i, 0);
        }
    }
}