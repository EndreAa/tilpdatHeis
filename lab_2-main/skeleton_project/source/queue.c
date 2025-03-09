#include <stdio.h>
#include <stdbool.h>
#include "elevator_types.h"
#include "queue.h"

void queue_init(Queue *queue) {
    queue->queue_count = 0;
    for (int i = 0; i < MAX_ORDERS; i++) {
        queue->queue_list[i] = NO_ORDER;
    }
}

int queue_find_floor(Queue *queue, int floor) {
    // FIX: Only search through active queue entries, not all MAX_ORDERS slots
    for (int i = 0; i < queue->queue_count; i++) {
        if (queue->queue_list[i] == floor) {
            return i;
        }
    }
    return -1;
}

static void queue_shift_right(Queue *queue, int position) {
    if (queue->queue_count >= MAX_ORDERS) {
        return;
    }
    
    for (int i = queue->queue_count; i > position; i--) {
        queue->queue_list[i] = queue->queue_list[i-1];
    }
    
    queue->queue_count++;
}

void queue_add(ElevatorSM *sm, int new_order, int button) {
    Queue *queue = &(sm->queue);
    
    if (queue_find_floor(queue, new_order) != -1) {
        printf("DEBUG: Skipping duplicate order for floor %d\n", new_order);
        return;
    }
    
    if (queue->queue_count >= MAX_ORDERS) {
        printf("DEBUG: Queue full, cannot add order for floor %d\n", new_order);
        return;
    }
    
    int current_direction = sm->elevator_direction;
    int current_floor = sm->sensors.sensor_current_floor;
    
    if (current_floor == -1) {
        current_floor = sm->last_current_floor;
    }
    
    printf("DEBUG: Adding order - Floor: %d, Button: %d, Current floor: %d, Direction: %d\n", 
           new_order, button, current_floor, current_direction);
    
    if (button == 2) {
        queue_shift_right(queue, 0);
        queue->queue_list[0] = new_order;
        printf("DEBUG: Cabin call - prioritized to front\n");
        queue_print(queue);
        return;
    }
    
    if (sm->current_state == state_move && current_direction != 0) {
        if ((current_direction > 0 && new_order > current_floor && new_order < sm->target_floor) ||
            (current_direction < 0 && new_order < current_floor && new_order > sm->target_floor)) {
            queue_shift_right(queue, 0);
            queue->queue_list[0] = new_order;
            printf("DEBUG: On-the-way call - prioritized to front\n");
            queue_print(queue);
            return;
        }
    }
    
    queue->queue_list[queue->queue_count] = new_order;
    queue->queue_count++;
    printf("DEBUG: Regular call - added to end\n");
    queue_print(queue);
}

int queue_remove(Queue *queue, int floor) {
    int index = queue_find_floor(queue, floor);
    
    if (index == -1) {
        printf("DEBUG: Failed to find floor %d in queue for removal\n", floor);
        return 0;
    }
    
    printf("DEBUG: Removing floor %d at index %d (queue count: %d)\n", 
           floor, index, queue->queue_count);
    
    for (int i = index; i < queue->queue_count - 1; i++) {
        queue->queue_list[i] = queue->queue_list[i+1];
    }
    
    queue->queue_list[queue->queue_count - 1] = NO_ORDER;
    queue->queue_count--;
    
    printf("DEBUG: After removal, queue count: %d\n", queue->queue_count);
    queue_print(queue);
    
    return 1;
}

void queue_empty(Queue *queue) {
    printf("DEBUG: Emptying queue\n");
    queue->queue_count = 0;
    for (int i = 0; i < MAX_ORDERS; i++) {
        queue->queue_list[i] = NO_ORDER;
    }
}

int queue_peek(Queue *queue) {
    if (queue->queue_count > 0) {
        return queue->queue_list[0];
    }
    return NO_ORDER;
}

void queue_print(Queue *queue) {
    printf("Queue (count=%d): ", queue->queue_count);
    for (int i = 0; i < queue->queue_count; i++) {
        printf("[Floor: %d] ", queue->queue_list[i]);
    }
    printf("\n");
}
