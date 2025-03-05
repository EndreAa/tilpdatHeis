#pragma once
#define MAX_ORDERS 5


typedef struct {
    int queue_first_pri;
    int queue_direction;
    int queue_current_floor;
    int queue_size; // comparing variable / max_orders
    int queue_list[MAX_ORDERS]; // må initialiserere alle til -1
} Queue;

void queue_add(int new_order, int direction, Queue *q);
void queue_remove(int completed_order, Queue *q);
void queue_empty(Queue *q);
int queue_get_next(Queue *q, int current_floor);
void queue_sort(Queue *q);
// maybe add init?