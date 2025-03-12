/**
* @file
* @brief A module that registers orders.
*/

#pragma once
#include "sensors.h"
#include "queue.h"
#include "driver.h"
#include "lights.h"
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Iterates through the nested vector contaning the order button information and registers
 * orders if they are placed.
 *
 *
 * @param *sm A pointer to elevator state machine. Needed to give access to sensor information.
 */ 
void orders_register_order(ElevatorSM *sm);
