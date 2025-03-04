#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "queue.h"
#include "driver.h"
#include "movement.h"

/**
 * @enum ElevatorState
 * @brief Enumeration of possible elevator states.
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
typedef struct {
    ElevatorState current_state;
    int target_floor;
    int elevator_direction;
    int last_current_floor;
} ElevatorSM;

/**
 * @brief Handles state actions for the stop state. The elevator is in the stop
 * state when the stop button is held in.
 *
 * @param sm A pointer to the ElevatorSM variable sm. The function needs access 
 * to be able to change member varaibles of sm.
 * @param event The state event to process.
 * @return ElevatorState The new state after processing the event.
 */
ElevatorState TSM_state_stop(ElevatorSM *sm, StateEvent event);

/**
 * @brief Handles state actions for the move state. The elevator is in the move state
 * when moving towards an order.
 *
 * @param sm A pointer to the ElevatorSM variable sm. The function needs access 
 * to be able to change member varaibles of sm.
 * @param event The state event to process.
 * @return ElevatorState The new state after processing the event.
 */
ElevatorState TSM_state_move(ElevatorSM *sm, StateEvent event);

/**
 * @brief Handles state actions for the deliver state. The elevator is in the deliver state
 * when servicing an order at a floor.
 *
 * @param sm A pointer to the ElevatorSM variable sm. The function needs access 
 * to be able to change member varaibles of sm.
 * @param event The state event to process.
 * @return ElevatorState The new state after processing the event.
 */
ElevatorState TSM_state_deliver(ElevatorSM *sm, StateEvent event);

/**
 * @brief Handles state actions for the still state. The elevator is in the still
 * state when there are no orders to be serviced.
 *
 * @param sm A pointer to the ElevatorSM variable sm. The function needs access 
 * to be able to change member varaibles of sm.
 * @param event The state event to process.
 * @return ElevatorState The new state after processing the event.
 */
ElevatorState TSM_state_still(ElevatorSM *sm, StateEvent event);

/**
 * @brief Executes the exit action for a given state.
 *
 * @param sm A pointer to the ElevatorSM variable sm. The function needs to be 
 * able to pass sm to the functions it calls.
 * @param state The state for which to call the exit function.
 */
void TSM_call_exit(ElevatorSM *sm, ElevatorState state);

/**
 * @brief Executes the enter action for a given state.
 *
 * @param sm A pointer to the ElevatorSM variable sm. The function needs to be
 * able to pass sm to the functions it calls.
 * @param state The state for which to call the enter function.
 */
void TSM_call_enter(ElevatorSM *sm, ElevatorState state);

/**
 * @brief Updates the state machine.
 *
 * Dispatches the current state's execute event and handles transitions by 
 * calling exit on the old state and enter on the new state. Stop-events are also
 * first priority, and if the stop button is pressed in, it it handeled immediately.
 *
 * @param sm Pointer to the ElevatorSM instance.
 * @return ElevatorState The new state after the update.
 */
ElevatorState TSM_update(ElevatorSM *sm);



