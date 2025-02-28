#ifndef MOVEMENT_H
#define MOVEMENT_H

#include "TSM.h"

/*
Her er målet at funskjonene her kan brukes til å få heisen til en spesifikk etasje. 
TSM skal kunne bruke en funskjon, og all funskjonalitet skal innebygges her.
*/ 

bool movement_get_to_floor(ElevatorSM current_floor, ElevatorSM target_floor);

#endif