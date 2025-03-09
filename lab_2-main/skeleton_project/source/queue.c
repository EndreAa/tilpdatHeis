#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "elevator_types.h"
#include "queue.h"

static char* get_timestamp() {
    static char buffer[26];
    time_t timer;
    struct tm* tm_info;
    
    time(&timer);
    tm_info = localtime(&timer);
    
    strftime(buffer, 26, "%H:%M:%S", tm_info);
    return buffer;
}

void queue_init(Queue *queue) {
    queue->queue_count = 0;
    for (int i = 0; i < MAX_ORDERS; i++) {
        queue->queue_list[i] = NO_ORDER;
    }
    printf("DEBUG [QUEUE %s]: Queue initialized\n", get_timestamp());
}

int queue_find_floor(Queue *queue, int floor) {
    printf("DEBUG [QUEUE %s]: Searching for floor %d in queue\n", 
           get_timestamp(), floor);
    
    for (int i = 0; i < queue->queue_count; i++) {
        if (queue->queue_list[i] == floor) {
            printf("DEBUG [QUEUE %s]: Found floor %d at index %d\n", 
                   get_timestamp(), floor, i);
            return i;
        }
    }
    
    printf("DEBUG [QUEUE %s]: Floor %d not found in queue\n", 
           get_timestamp(), floor);
    return -1;
}

static void queue_shift_right(Queue *queue, int position) {
    printf("DEBUG [QUEUE %s]: Shifting queue right from position %d\n", 
           get_timestamp(), position);
    
    if (queue->queue_count >= MAX_ORDERS) {
        printf("DEBUG [QUEUE %s]: Cannot shift - queue full\n", get_timestamp());
        return;
    }
    
    for (int i = queue->queue_count; i > position; i--) {
        queue->queue_list[i] = queue->queue_list[i-1];
    }
    
    queue->queue_count++;
    printf("DEBUG [QUEUE %s]: Queue shifted, new count: %d\n", 
           get_timestamp(), queue->queue_count);
}

void queue_add(ElevatorSM *sm, int new_order, int button) {
    Queue *queue = &(sm->queue);
    
    printf("\nDEBUG [QUEUE %s]: Adding order - Floor: %d, Button: %d\n", 
           get_timestamp(), new_order, button);
    printf("DEBUG [QUEUE %s]: Current state: %d, Direction: %d, Current floor: %d, Target: %d\n", 
           get_timestamp(), sm->current_state, sm->elevator_direction, 
           sm->sensors.sensor_current_floor, sm->target_floor);
    
    printf("DEBUG [QUEUE %s]: Queue before add: ", get_timestamp());
    queue_print(queue);
    
    if (queue_find_floor(queue, new_order) != -1) {
        printf("DEBUG [QUEUE %s]: Skipping duplicate order for floor %d\n", 
               get_timestamp(), new_order);
        return;
    }
    
    if (queue->queue_count >= MAX_ORDERS) {
        printf("DEBUG [QUEUE %s]: Queue full, cannot add order for floor %d\n", 
               get_timestamp(), new_order);
        return;
    }
    
    int current_direction = sm->elevator_direction;
    int current_floor = sm->sensors.sensor_current_floor;
    
    if (current_floor == -1) {
        current_floor = sm->last_current_floor;
    }
    
    if (button == 2) {
        queue_shift_right(queue, 0);
        queue->queue_list[0] = new_order;
        printf("DEBUG [QUEUE %s]: Cabin call - prioritized to front\n", get_timestamp());
    }
    else if (sm->current_state == state_move && current_direction != 0) {
        if ((current_direction > 0 && new_order > current_floor && new_order < sm->target_floor) ||
            (current_direction < 0 && new_order < current_floor && new_order > sm->target_floor)) {
            queue_shift_right(queue, 0);
            queue->queue_list[0] = new_order;
            printf("DEBUG [QUEUE %s]: On-the-way call - prioritized to front\n", get_timestamp());
        } else {
            queue->queue_list[queue->queue_count] = new_order;
            queue->queue_count++;
            printf("DEBUG [QUEUE %s]: Regular call - added to end\n", get_timestamp());
        }
    }
    else {
        queue->queue_list[queue->queue_count] = new_order;
        queue->queue_count++;
        printf("DEBUG [QUEUE %s]: Regular call - added to end\n", get_timestamp());
    }
    
    printf("DEBUG [QUEUE %s]: Queue after add: ", get_timestamp());
    queue_print(queue);
    printf("DEBUG [QUEUE %s]: Target floor after add: %d\n\n", 
           get_timestamp(), sm->target_floor);
}

int queue_remove(Queue *queue, int floor) {
    printf("\nDEBUG [QUEUE %s]: Attempting to remove floor %d\n", 
           get_timestamp(), floor);
    printf("DEBUG [QUEUE %s]: Queue before removal: ", get_timestamp());
    queue_print(queue);
    
    int index = queue_find_floor(queue, floor);
    
    if (index == -1) {
        printf("DEBUG [QUEUE %s]: Failed to find floor %d in queue for removal\n", 
               get_timestamp(), floor);
        return 0;
    }
    
    printf("DEBUG [QUEUE %s]: Removing floor %d at index %d (queue count: %d)\n", 
           get_timestamp(), floor, index, queue->queue_count);
    
    for (int i = index; i < queue->queue_count - 1; i++) {
        queue->queue_list[i] = queue->queue_list[i+1];
    }
    
    queue->queue_list[queue->queue_count - 1] = NO_ORDER;
    queue->queue_count--;
    
    printf("DEBUG [QUEUE %s]: After removal, queue count: %d\n", 
           get_timestamp(), queue->queue_count);
    printf("DEBUG [QUEUE %s]: Queue after removal: ", get_timestamp());
    queue_print(queue);
    printf("\n");
    
    return 1;
}

void queue_empty(Queue *queue) {
    printf("DEBUG [QUEUE %s]: Emptying queue\n", get_timestamp());
    queue->queue_count = 0;
    for (int i = 0; i < MAX_ORDERS; i++) {
        queue->queue_list[i] = NO_ORDER;
    }
    printf("DEBUG [QUEUE %s]: Queue emptied\n", get_timestamp());
}

int queue_peek(Queue *queue) {
    if (queue->queue_count > 0) {
        printf("DEBUG [QUEUE %s]: Peek - next floor is %d\n", 
               get_timestamp(), queue->queue_list[0]);
        return queue->queue_list[0];
    }
    printf("DEBUG [QUEUE %s]: Peek - queue is empty\n", get_timestamp());
    return NO_ORDER;
}

void queue_print(Queue *queue) {
    printf("Queue (count=%d): ", queue->queue_count);
    for (int i = 0; i < queue->queue_count; i++) {
        printf("[Floor: %d] ", queue->queue_list[i]);
    }
    printf("\n");
}
