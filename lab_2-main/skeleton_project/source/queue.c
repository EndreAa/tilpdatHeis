#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "elevator_types.h"
#include "queue.h"


void queue_init(Queue *queue) {
    queue->queue_count = 0;
    for (int i = 0; i < MAX_ORDERS; i++) {
        queue->queue_list[i] = NO_ORDER;
    }
    printf("Queue initialized\n");
}

int queue_find_floor(Queue *queue, int floor) {
    printf("Searching for floor %d in queue\n", floor);
    
    for (int i = 0; i < queue->queue_count; i++) {
        if (queue->queue_list[i] == floor) {
            printf("Found floor %d at index %d\n", floor, i);
            return i;
        }
    }
    
    printf("Floor %d not found in queue\n", floor);
    return -1;
}

static void queue_shift_right(Queue *queue, int position) {
    printf("Shifting queue right from position %d\n", position);
    
    if (queue->queue_count >= MAX_ORDERS) {
        printf("Cannot shift - queue full\n");
        return;
    }
    
    for (int i = queue->queue_count; i > position; i--) {
        queue->queue_list[i] = queue->queue_list[i-1];
    }
    
    queue->queue_count++;
    printf("Queue shifted, new count: %d\n", queue->queue_count);
}

void queue_add(ElevatorSM *sm, int new_order, int button) {
    Queue *queue = &(sm->queue);
    
    printf("\nAdding order - Floor: %d, Button: %d\n", new_order, button);
    printf("Current state: %d, Direction: %d, Current floor: %d, Target: %d\n", 
           sm->current_state, sm->elevator_direction, 
           sm->sensors.sensor_current_floor, sm->target_floor);
    
    printf("Queue before add: ");
    queue_print(queue);
    
    if (queue->queue_count >= MAX_ORDERS) {
        printf("Queue full, cannot add order for floor %d\n", new_order);
        return;
    }
    
    int current_direction = sm->elevator_direction;
    int current_floor = sm->sensors.sensor_current_floor;
    
    if (current_floor == -1) {
        current_floor = sm->last_current_floor;
    }

    int existing_index = queue_find_floor(queue, new_order);
    
    if (sm->current_state == state_move && current_direction != 0) {
        bool is_on_the_way = (current_direction > 0 && new_order > current_floor && new_order < sm->target_floor) ||
                             (current_direction < 0 && new_order < current_floor && new_order > sm->target_floor);
                             
        bool same_direction = (current_direction > 0 && button == BUTTON_HALL_UP) || 
                              (current_direction < 0 && button == BUTTON_HALL_DOWN) ||
                              (button == BUTTON_CAB);

        if (is_on_the_way && same_direction) {
            if (existing_index != -1) {
                queue_remove(queue, new_order);
            }
            
            queue_shift_right(queue, 0);
            queue->queue_list[0] = new_order;
            printf("On-the-way call in same direction - prioritized to front\n");
        } else {
            if (existing_index == -1) {
                queue->queue_list[queue->queue_count] = new_order;
                queue->queue_count++;
                printf("Regular call or opposite direction - added to end\n");
            } else {
                printf("Skipping duplicate order for floor %d\n", new_order);
            }
        }
    } else {
        if (existing_index == -1) {
            queue->queue_list[queue->queue_count] = new_order;
            queue->queue_count++;
            printf("Regular call - added to end\n");
        } else {
            printf("Skipping duplicate order for floor %d\n", new_order);
        }
    }
    
    printf("Queue after add: ");
    queue_print(queue);
    printf("Target floor after add: %d\n\n", sm->target_floor);
}

int queue_remove(Queue *queue, int floor) {
    printf("\nAttempting to remove floor %d\n", floor);
    printf("Queue before removal: ");
    queue_print(queue);
    
    int index = queue_find_floor(queue, floor);
    
    if (index == -1) {
        printf("Failed to find floor %d in queue for removal\n", floor);
        return 0;
    }
    
    printf("Removing floor %d at index %d (queue count: %d)\n", floor, index, queue->queue_count);
    
    for (int i = index; i < queue->queue_count - 1; i++) {
        queue->queue_list[i] = queue->queue_list[i+1];
    }
    
    queue->queue_list[queue->queue_count - 1] = NO_ORDER;
    queue->queue_count--;
    
    printf("After removal, queue count: %d\n", queue->queue_count);
    printf("Queue after removal: ");
    queue_print(queue);
    printf("\n");
    
    return 1;
}

void queue_empty(Queue *queue) {
    printf("Emptying queue\n");
    queue->queue_count = 0;
    for (int i = 0; i < MAX_ORDERS; i++) {
        queue->queue_list[i] = NO_ORDER;
    }
    printf("Queue emptied\n");
}

int queue_peek(Queue *queue) {
    if (queue->queue_count > 0) {
        printf("Peek - next floor is %d\n", queue->queue_list[0]);
        return queue->queue_list[0];
    }
    printf("Peek - queue is empty\n");
    return NO_ORDER;
}

void queue_print(Queue *queue) {
    printf("Queue (count=%d): ", queue->queue_count);
    for (int i = 0; i < queue->queue_count; i++) {
        printf("[Floor: %d] ", queue->queue_list[i]);
    }
    printf("\n");
}
