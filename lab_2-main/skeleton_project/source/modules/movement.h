#pragma once
#include "TSM.h"

/**
 * @brief Moves the elevator to the target floor, making sure to only move when 
 * movement is permitted. 
 *
 * @param sm A pointer to the Elevator's state machine. The function needs access to this to 
 * update the elevator_direction and access the target_floor.
 * @return ElevatorState Returns state_move if the elevator isn't at the destination yet, and 
 * state_deliver when it is. 
 */ 
ElevatorState movement_get_to_floor(ElevatorSM *sm);

/**
 * @brief Decides the direction the elevator should move based on the current floor and the
 * target floor
 *
 * @param sm A pointer to the Elevator's state machine. The function needs access to this to 
 * update the elevator_direction and access the target_floor.
 * @return int Returns the chosen direction, either -1 (down), 0 (stop), or 1 (up).
 */ 
int movement_choose_direction(ElevatorSM *sm);


