#include "queue.h"
// every method is public



void queue_add(int new_order, int direction, Queue *q){
    // sjekker om allerede eksisterer
    for(int i = 0; i < q->queue_size; i++){
        if(q->queue_list[i] == new_order){
            return;
        }
    }

    // Legg til bestilling
    if(q->queue_size < MAX_ORDERS){
        q->queue_list[q->queue_size] = new_order; // Ikke helt nødvendig, men gjør det skalerbart
        q->queue_size++;
        q->queue_direction = direction;
    }

    // Hvis første bestilling (eller eneste), gjør til prioritet
    if(q->queue_size == 1){
        q->queue_first_pri = new_order;
        return;
    }

    // sort the bitch
    queue_sort_prioritize(q);

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
        return; // stopper litt tidligere
    }
}

void queue_empty(Queue *q){
    // en easy fix her kan være:
    for(int i = 0; i < MAX_ORDERS; ++i){
        q->queue_list[i] = -1;
    }
    
    q->queue_size = 0; // the queue is emptied, the size is zero
    q->queue_first_pri = -1; // nothing is prioritized
    q->queue_direction = -1; // not moving
}

void queue_sort_prioritize(Queue *q){
    
    // Sortering - Tilfeller: bestilling på veien (ascending, descending)
    int temp;
    // Retning opp
    if(q->queue_direction == 1){
        // sorter fra nåværende etasje
        
    }

    // Retning ned

}