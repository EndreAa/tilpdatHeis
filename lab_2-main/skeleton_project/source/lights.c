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
