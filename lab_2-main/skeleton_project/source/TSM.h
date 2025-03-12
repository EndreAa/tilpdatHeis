/**
* @file
* @brief Main functionality for the elevator state machine.
* Handles updating and switching between states.
*/

#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "elevator_types.h"
#include "driver.h"
#include "movement.h"
#include "door.h"
#include "sensors.h"
#include "orders.h"


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



