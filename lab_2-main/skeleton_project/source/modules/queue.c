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
    int temp_queue[MAX_ORDERS];  // Midlertidig liste for opp-bestillinger
    int temp_size = 0;

    if (q->queue_direction == 0) return;  // Hvis heisen er på vent, trenger vi ikke sortere

    // Håndtering av opp-bestillinger hvis retningen er opp (1)
    if (q->queue_direction == 1) {
        // Legg til bestillinger med type 2 (cab orders) og større enn current_floor i starten av lista
        for (int i = 0; i < FLOORS; i++) {
            if (q->orders[i][2] && i > q->queue_current_floor) {  // Cab orders som er over current_floor
                temp_queue[temp_size++] = i;
            }
        }

        // Legg til opp-bestillinger som er over current_floor og ikke finnes i lista
        for (int i = 0; i < FLOORS; i++) {
            if (q->orders[i][0] && i > q->queue_current_floor) {  // Opp-bestillinger som er over current_floor
                int found = 0;
                for (int j = 0; j < temp_size; j++) {
                    if (temp_queue[j] == i) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    temp_queue[temp_size++] = i;  // Legg til bestillingen
                }
            }
        }

        // Sorter opp-bestillingene i stigende rekkefølge
        for (int i = 0; i < temp_size - 1; i++) {
            for (int j = i + 1; j < temp_size; j++) {
                if (temp_queue[i] > temp_queue[j]) {
                    int temp = temp_queue[i];
                    temp_queue[i] = temp_queue[j];
                    temp_queue[j] = temp;
                }
            }
        }
    }
    
    // Håndtering av ned-bestillinger hvis retningen er ned (-1)
    int temp_down[MAX_ORDERS];
    int temp_down_size = 0;

    if (q->queue_direction == -1) {
        // Legg til ned-bestillinger som er under current_floor og ikke finnes i lista
        for (int i = FLOORS - 1; i >= 0; i--) {
            if (q->orders[i][1] && i < q->queue_current_floor) {  // Ned-bestillinger som er under current_floor
                int found = 0;
                for (int j = 0; j < temp_down_size; j++) {
                    if (temp_down[j] == i) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    temp_down[temp_down_size++] = i;  // Legg til bestillingen
                }
            }
        }
        printf("current floor: %d\n", q->queue_current_floor);
        print_list(temp_down, temp_down_size);
        // Sorter ned-bestillingene i synkende rekkefølge
        for (int i = 0; i < temp_down_size - 1; i++) {
            for (int j = i + 1; j < temp_down_size; j++) {
                if (temp_down[i] < temp_down[j]) {
                    int temp = temp_down[i];
                    temp_down[i] = temp_down[j];
                    temp_down[j] = temp;
                }
            }
        }
    }   


    // Slå sammen opp- og ned-bestillinger
    int final_queue[MAX_ORDERS];
    int final_size = 0;

    //legg til opp-bestillingene 
    for (int i = 0; i < temp_size; i++) {
        final_queue[final_size++] = temp_queue[i];
    }


    //legg til ned-bestillingene 
    for (int i = 0; i < temp_down_size; i++) {
        final_queue[final_size++] = temp_down[i];
    }

    // Kopier den endelige listen tilbake til den originale køen
    for (int i = 0; i < final_size; i++) {
        q->queue_list[i] = final_queue[i];
    }


    q->queue_size = final_size;  // Oppdater køens størrelse
}

void print_queue(Queue *q) {
    printf("Queue (size %d): ", q->queue_size);
    for (int i = 0; i < q->queue_size; i++) {
        printf("%d ", q->queue_list[i]);
    }
    printf("\n");
}

void print_list(int *list, int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", list[i]);
    }
    printf("\n");
}