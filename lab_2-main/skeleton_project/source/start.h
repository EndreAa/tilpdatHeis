/**
* @file
* @brief A module that initialises and sustain the elevator state machine.
*/

#pragma once

#include "elevator_types.h"
#include "TSM.h"
#include "driver.h"
#include "queue.h"
#include "lights.h"
#include <stdbool.h>
#include <stdio.h>

/**
 * @brief Ensures the elevator enters a defined state before
 * the state machine is initialised. The elevator moves down until
 * it reaches a floor.
 *
 * @return bool Returns true when the elevator is in a defined state 
 * (at a floor).
 */
bool start_find_floor();

/**
 * @brief Instansiates the elevator state machine and updates the 
 * state machine in an endless while loop.
 */
void start_init();

