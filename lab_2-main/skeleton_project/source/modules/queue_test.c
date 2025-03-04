#include <stdio.h>
#include "queue.h"


void print_queue(const Queue *q) {
    printf("Queue (size = %d): [ ", q->queue_size);
    for (int i = 0; i < q->queue_size; i++) {
        printf("%d ", q->queue_list[i]);
    }
    printf("]\n");
}

int main() {
    Queue q = {0, -1, -1, {0}};  // Initierer køen

    printf("=== TEST: Legge til bestillinger ===\n");
    queue_add(2, 1, &q);
    queue_add(5, 1, &q);
    queue_add(3, -1, &q);
    print_queue(&q);  // Forventet: [2, 5, 3]

    printf("\n=== TEST: Fjerne bestilling ===\n");
    queue_remove(5, &q);
    print_queue(&q);  // Forventet: [2, 3]

    printf("\n=== TEST: Fjerne bestilling som ikke finnes ===\n");
    queue_remove(9, &q);
    print_queue(&q);  // Forventet: [2, 3] (ingen endring)

    printf("\n=== TEST: Legge til flere og prioritere ===\n");
    queue_add(7, 1, &q);
    queue_add(1, -1, &q);
    print_queue(&q);  // Forventet: [2, 3, 7, 1]
    
    printf("\n=== TEST: Tømme køen ===\n");
    queue_empty(&q);
    print_queue(&q);  // Forventet: []

    printf("\n=== TEST: Er køen tom? %s\n", queue_is_empty(&q) ? "JA" : "NEI");

    return 0;
}