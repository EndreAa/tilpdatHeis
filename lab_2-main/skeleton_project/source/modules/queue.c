#include "queue.h"

// every method is public

void queue_add(int new_order, int direction, Queue *q){
    if(q->queue_size < MAX_ORDERS){
        q->queue_list[q->queue_size] = new_order; // setter siste element til ny bestilling
        q->queue_size++;
        q->queue_direction = direction; // oppdatere retning, dette må vi revurdere
        if (q->queue_size == 1){
            q->queue_first_pri = new_order;
            //size++?
        }
    }


}

// Virkemåte:
// queue_list = [2, 5, 3, 7, 1]  
// completed_order = 3;   
// queue_list = [2,5,7,1], --size


void queue_remove(int completed_order, Queue *q){
    for(int i = 0; i < q->queue_size; ++i){
        if (q->queue_list[i] == completed_order)
        // mangler logikk
    }
}

void queue_empty(Queue *q){
    // en easy fix her kan være:
    q->queue_size = 0;
}