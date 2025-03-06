#pragma once

#define FLOORS 4
#define BUTTONS 3
#define MAX_ORDERS 5

// inkludert i denne header filen for å unngå circular dependancy
typedef struct {
    int queue_first_pri;
    int queue_direction;
    int queue_current_floor;
    int queue_size; // comparing variable / max_orders
    int queue_list[MAX_ORDERS]; // må initialiserere alle til -1
} Queue;

// inkludert i denne header filen for å unngå circular dependancy
typedef struct {
    bool sensor_stop_button;
    int sensor_current_floor;
    bool sensor_obstacle;
    bool sensor_button_values[FLOORS][BUTTONS]; 
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
} ElevatorSM;

