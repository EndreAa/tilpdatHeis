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
    for (int i = 0; i < MAX_ORDERS; i++) {
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
        return;
    }
    
    if (queue->queue_count >= MAX_ORDERS) {
        return;
    }
    
    int current_direction = sm->elevator_direction;
    int current_floor = sm->sensors.sensor_current_floor;
    
    if (current_floor == -1) {
        current_floor = sm->last_current_floor;
    }
  
    if (sm->current_state == state_move) {
        if (current_direction > 0 && button == 0) {
            if (new_order > current_floor && new_order < sm->target_floor) {
                queue_shift_right(queue, 0);
                queue->queue_list[0] = new_order;
                return;
            }
        }
        else if (current_direction < 0 && button == 1) {
            if (new_order < current_floor && new_order > sm->target_floor) {
                queue_shift_right(queue, 0);
                queue->queue_list[0] = new_order;
                return;
            }
        }
    }
    
    queue->queue_list[queue->queue_count] = new_order;
    queue->queue_count++;
}

int queue_remove(Queue *queue, int floor) {
    int index = queue_find_floor(queue, floor);
    
    if (index == -1) {
        return 0;
    }
    
    for (int i = index; i < queue->queue_count - 1; i++) {
        queue->queue_list[i] = queue->queue_list[i+1];
    }
    
    queue->queue_list[queue->queue_count - 1] = NO_ORDER;
    queue->queue_count--;
    
    return 1;
}

void queue_empty(Queue *queue) {
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
