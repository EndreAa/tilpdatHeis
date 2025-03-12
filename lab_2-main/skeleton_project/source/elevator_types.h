/**
* @file
* @brief Defines neccessary structs and enums
* containing information about the elevator.
*/

#pragma once
#include <stdio.h>
#include <stdbool.h>

#define FLOORS 4
#define BUTTONS 3
#define MAX_ORDERS 5

/**
 * @estruct Queue
 * @brief Contains the queue list for handling orders.
 */
typedef struct {
    int queue_count; /**< Amount of orders in the list */
    int queue_list[MAX_ORDERS];  /**< The queue list, containing orders for the elevator to service */
} Queue;

/**
 * @estruct Sensors
 * @brief Contains the sensor information for the elevator. Useful for debugging in case of hardware problems.
 */
typedef struct {
    bool sensor_stop_button; /**< Stop button status */
    int sensor_current_floor; /**< The elevators current floor */
    bool sensor_obstacle; /**< Obsutruction status */
    bool sensor_button_values[FLOORS][BUTTONS]; /**< A matrix containing the information for each of the order buttons in the elevator. */
} Sensors;

/**
 * @enum ElevatorState
 * @brief Enumeration of elevator states.
 */
typedef enum {
    state_stop,    /**< The stop button is pushed in */
    state_move,    /**< The elevator is moving towards an order */
    state_deliver, /**< The elevator stops at the floor to serive the order */
    state_still    /**< The elevator is awaing an order */
} ElevatorState;

/**
 * @enum StateEvent
 * @brief Enumeration of the different stages (substates) of each state.
 */
typedef enum {
    event_enter,   /**< Mechanisms while entering a state ("one-time" things) */
    event_execute, /**< Execute state event is repeatedly updated to continue carrying out the states actions. */
    event_exit     /**< The exit procedure for the state. */
} StateEvent;

/**
 * @struct ElevatorSM
 * @brief Structure containing the essential information about the elevator that 
 * the elevator itself cannot provide (since the current floor can be provided by the
 * driver, it is not included here.)
 */
typedef struct{
    ElevatorState current_state;
    int target_floor;
    int elevator_direction;
    int last_current_floor;
    Sensors sensors;
    Queue queue;
    int door_opened_at_stop;
} ElevatorSM;

