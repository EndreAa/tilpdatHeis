#include "queue.h"
#include <stdio.h>  
// every method is public

// Vi prøver på nytt

// initialiserer køen
void queue_init(Queue *q) {
    for (int i = 0; i < FLOORS; i++) {
        for (int j = 0; j < 3; j++) {
            q->orders[i][j] = false;
        }
    }
    for (int i = 0; i < MAX_ORDERS; i++) {
        q->queue_list[i] = -1;
    }
    q->queue_size = 0;
    q->queue_direction = 0; // potensielt hente disse fra en plass?
    q->queue_current_floor = 0; // ...
}


// Legger til bestilling i køen
void queue_add(int new_order, int button, Queue *q) {
    if (new_order < 0 || new_order >= FLOORS) return;

    // Set the corresponding order type (up, down, or cab)
    q->orders[new_order][button] = true;

    // Avoid dups
    for (int i = 0; i < q->queue_size; i++) {
        if (q->queue_list[i] == new_order) return;
    }

    // Add the order
    if (q->queue_size < MAX_ORDERS) {
        q->queue_list[q->queue_size++] = new_order;
    }

    queue_sort(q);  // Sort
}



void queue_remove(int completed_order, Queue *q){

    for(int i = 0; i < q->queue_size; ++i){
        if(q->queue_list[i] == completed_order){ // hvis gjennomført
            for(int j = i; j < q->queue_size -1; j++){  // hold i mente, j < queue_size - 1 fikser bounds
                q->queue_list[j] = q->queue_list[j+1]; // shift mf
            }
            q->queue_list[q->queue_size-1] = -1; // siste element blir -1
            q->queue_size--; // reduserer størrelsen, ikke sånn faktisk
        }
    }
}


void queue_empty(Queue *q){
    queue_init(q);
}

// sorting logic, ta hensyn til retning til heisen, hvor heisen er, og hvor bestillingene er

void queue_sort(Queue *q) {
    int temp_queue[MAX_ORDERS];
    int temp_size = 0;

    if (q->queue_direction == 0) return;  // If idle, no sorting needed

    // If moving up (1)
    if (q->queue_direction == 1) {
        // Add up orders that are above the current floor
        for (int i = 0; i < FLOORS; i++) {
            if (q->orders[i][0] && i > q->queue_current_floor) {  // Up orders above current floor
                temp_queue[temp_size++] = i;
            }
        }

        // Add down orders that are below the current floor
        for (int i = 0; i < FLOORS; i++) {
            if (q->orders[i][1] && i < q->queue_current_floor) {  // Down orders below current floor
                temp_queue[temp_size++] = i;
            }
        }

        // Add cab orders
        for (int i = 0; i < FLOORS; i++) {
            if (q->orders[i][2]) {  // Cab orders
                temp_queue[temp_size++] = i;
            }
        }
    }
    // If moving down (-1)
    else if (q->queue_direction == -1) {
        // Add down orders that are below the current floor
        for (int i = FLOORS - 1; i >= 0; i--) {
            if (q->orders[i][1] && i < q->queue_current_floor) {  // Down orders below current floor
                temp_queue[temp_size++] = i;
            }
        }

        // Add up orders that are above the current floor
        for (int i = FLOORS - 1; i >= 0; i--) {
            if (q->orders[i][0] && i > q->queue_current_floor) {  // Up orders above current floor
                temp_queue[temp_size++] = i;
            }
        }

        // Add cab orders
        for (int i = FLOORS - 1; i >= 0; i--) {
            if (q->orders[i][2]) {  // Cab orders
                temp_queue[temp_size++] = i;
            }
        }
    }

    // Copy the sorted queue back to the original queue
    for (int i = 0; i < temp_size; i++) {
        q->queue_list[i] = temp_queue[i];
    }
    q->queue_size = temp_size;  // Update queue size
}

void print_queue(Queue *q) {
    printf("Queue (size %d): ", q->queue_size);
    for (int i = 0; i < q->queue_size; i++) {
        printf("%d ", q->queue_list[i]);
    }
    printf("\n");
}