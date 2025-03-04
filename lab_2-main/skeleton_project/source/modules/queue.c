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


// Bestemmer neste etasje for heisen basert på køen
int queue_get_next(Queue *q, int current_floor){
    if(q->queue_size == 0) return -1; // Ingen bestillinger

    int next = -1;

    // 1. Sjekk bestillinger i bevegelsesretningen
    for(int i = 0; i < q->queue_size; i++){
        if ((q->queue_direction == 1 && q->queue_list[i] > current_floor) ||
            (q->queue_direction == -1 && q->queue_list[i] < current_floor)) {
            next = q->queue_list[i];
            break;
        }
    }

    // 2. Hvis ingen funnet, ta nærmeste bestilling
    if (next == -1) {
        next = q->queue_list[0]; // Prioriterer første i køen
        q->queue_direction = (next > current_floor) ? 1 : -1; // Sett ny retning
    }

    return next;
}

// Sorterer køen for å prioritere bestillinger i riktig retning
void queue_sort_prioritize(Queue *q){
    int temp;
    if(q->queue_direction == 1){
        // Sorter stigende (laveste først)
        for(int i = 0; i < q->queue_size - 1; i++){
            for(int j = 0; j < q->queue_size - i - 1; j++){
                if(q->queue_list[j] > q->queue_list[j + 1]){
                    temp = q->queue_list[j];
                    q->queue_list[j] = q->queue_list[j + 1];
                    q->queue_list[j + 1] = temp;
                }
            }
        }
    } else if(q->queue_direction == -1){
        // Sorter synkende (høyeste først)
        for(int i = 0; i < q->queue_size - 1; i++){
            for(int j = 0; j < q->queue_size - i - 1; j++){
                if(q->queue_list[j] < q->queue_list[j + 1]){
                    temp = q->queue_list[j];
                    q->queue_list[j] = q->queue_list[j + 1];
                    q->queue_list[j + 1] = temp;
                }
            }
        }
    }
}