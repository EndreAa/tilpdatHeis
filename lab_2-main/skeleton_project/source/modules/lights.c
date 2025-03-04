#include "lights.h"

void lights_last_floor_lamp(ElevatorSM *sm){
    elevio_floorIndicator(sm->last_current_floor);
} //muligens ubrukelig. Brukes ikke nå.
