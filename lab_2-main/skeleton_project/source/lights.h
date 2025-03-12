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

/**
 * @brief Turns on the correct order light when a button is pressed.
 *
 * @param floor The floor at which the order is placed
 * @param button The type of button pressed (up, down, or cabin).
 */
void lights_turn_on(int floor, int button);

/**
 * @brief Turns off all order lights belonging to a give floor. To be used when order is
 * completed on said floor.
 *
 * @param floor The floor in which lights should be turned off.
 */
void lights_turn_off(int floor);

/**
 * @brief Turns off all order lights. To be used for initilsation and if the stop button
 * is pressed.
 */
void lights_turn_off_all_orders();
