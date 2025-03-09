#pragma once

#include "elevator_types.h"
#include "driver.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Moves the elevator to the target floor, making sure to only move when 
 * movement is permitted.
 *
 * @param sm A pointer to the Elevator's state machine. The function needs access to this to 
 * update the elevator_direction and access the target_floor.
 * @return bool Returns true when the elevator has reached the target floor.
 */
void lights_last_floor_lamp(ElevatorSM *sm);

void lights_turn_on(int floor, int button);

void lights_turn_off(int floor);

void lights_turn_off_all_orders();
