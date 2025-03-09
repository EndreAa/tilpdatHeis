#pragma once

#include "elevator_types.h"
#include "driver.h"
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

/**
 * @brief Opens the elevator door if the elevator is in a defined state (at a floor).
 * The elevator will not open if it is not at a floor.
 *
 * @param[in] sm Pointer to the elevator state machine
 * @return 1 if door was successfully opened, 0 if it failed
 * */
int door_open(ElevatorSM *sm);

/**
 * @brief Attempts to close the elevator door, checking for an obstruction. If
 * an obstruction occurs, the door will wait 3 more seconds before it closes.
 *
 * @param[in] sm Pointer to the elevator state machine
 * @return 1 if door was closed, 0 if not.
 */
int door_close(ElevatorSM *sm);

/**
 * @brief Starts a timer, used to hold the door open for 3 seconds.
 *
 * @param[in] sm Pointer to the elevator state machine
 */
// void door_timer_start(ElevatorSM *sm);

/**
 * @brief Checks if the door timer has expired.
 *
 * @param[in] sm Pointer to the elevator state machine
 * @return 1 if timer has expired, 0 otherwise
 */
// int door_timer_expired(ElevatorSM *sm);

/**
 * @brief Main door control function for floor delivery. To be called my the state
 * machine. Designed to be called continuously until it returns 1. 
 * 
 * @param[in] sm Pointer to the elevator state machine
 * @return 0 if door process still in progress, 1 if completed
 */
int door_deliver_to_floor(ElevatorSM *sm);
