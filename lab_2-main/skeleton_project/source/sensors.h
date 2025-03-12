#pragma once
#include <stdbool.h>
#include <stdio.h>
#include "elevator_types.h"
#include "driver.h"

/**
 * @brief Iterates through all order button values. Neccessary for regestering orders. 
 * Updates sensor information for the stop button and floor sensor as well.
 *
 * The function is continuously called and this nested vector containing the order button information
 * updated. This nested vector is then used in processing orders.
 *
 * @param *sensors A pointer to the sensors struct instance for the elevator state machine. The function needs access 
 * to this to update the sensors information in the instance of the elevator state machine.
 */ 
void sensors_update(Sensors *sensors);

/**
 * @brief Used to know if a button in the sensors vector is pressed. Useful for regestering orders.
 *
 *
 * @param *sensors A pointer to the sensors struct instance for the elevator state machine. 
 * @param floor The floor to be checked.
 * @param button The type of button to be checked.
 * @return bool Return true (1) if the button is pressed, and false (0) if not.
 */ 
bool sensors_is_button_pressed(Sensors *sensors, int floor, int button);

/**
 * @brief Used to get sensor information about the current floor.
 *
 * @param *sensors A pointer to the sensors struct instance for the elevator state machine. 
 * @return int The current floor.
 */ 
int sensors_get_current_floor(Sensors *sensors);
