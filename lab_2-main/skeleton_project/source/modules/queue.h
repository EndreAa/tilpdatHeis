#pragma once
#define MAX_ORDERS 10
#include <stdbool.h>

#define FLOORS 4

typedef struct {
    int queue_first_pri;
    int queue_direction;
    int queue_current_floor;

    int queue_size; // comparing variable / max_orders
    int queue_list[MAX_ORDERS]; // må initialiserere alle til -1

    bool orders[FLOORS][3]; // 3 buttons, 4 floors, up, down, cab

} Queue;


void queue_add(int new_order, int button, Queue *q);
void queue_remove(int completed_order, Queue *q);
void queue_empty(Queue *q);
int queue_get_next(Queue *q, int current_floor);
void queue_sort(Queue *q);
// maybe add init?
void queue_init(Queue *q);
void print_queue(Queue *q);
