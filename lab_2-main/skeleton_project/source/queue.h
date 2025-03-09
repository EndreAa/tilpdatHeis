#pragma once

#include "elevator_types.h"
#include "driver.h"
#include <stdio.h>
#include <stdbool.h>

// Special value to indicate no order
#define NO_ORDER -1

/**
 * @brief Initialize the queue
 * @param queue Pointer to the Queue structure to initialize
 */
void queue_init(Queue *queue);

/**
 * @brief Check if a floor is already in the queue
 * @param queue Pointer to the Queue structure
 * @param floor Floor number to find
 * @return Index of the floor in the queue if found, -1 otherwise
 */
int queue_find_floor(Queue *queue, int floor);

/**
 * @brief Add an order to the queue with proper prioritization
 * @param sm Pointer to the ElevatorSM structure
 * @param new_order Floor number of the new order
 * @param button Button type: 0=up, 1=down, 2=cabin
 * 
 * Priorities:
 * 1. If elevator is moving and a new order from outside is in the same direction
 *    and within the current range of motion, it becomes the next order
 * 2. All other orders go to the end of the queue
 * 3. Duplicate floor orders are ignored
 */
void queue_add(ElevatorSM *sm, int new_order, int button);

/**
 * @brief Remove a specific floor from the queue
 * @param queue Pointer to the Queue structure
 * @param floor Floor number to remove
 * @return 1 if successful, 0 if floor not found
 */
int queue_remove(Queue *queue, int floor);

/**
 * @brief Empty the entire queue
 * @param queue Pointer to the Queue structure
 */
void queue_empty(Queue *queue);

/**
 * @brief Get the next order from the queue without removing it
 * @param queue Pointer to the Queue structure
 * @return Next floor in queue or NO_ORDER if queue is empty
 */
int queue_peek(Queue *queue);

/**
 * @brief Debug function to print the current queue state
 * @param queue Pointer to the Queue structure
 */
void queue_print(Queue *queue);
