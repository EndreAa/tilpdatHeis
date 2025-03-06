#include <stdio.h>
#include "queue.h"

void test_queue_sort_up_direction() {
    printf("Testing queue sort when elevator is moving up...\n");

    Queue q;
    queue_init(&q);

    // Set elevator to current floor 2
    q.queue_current_floor = 2;
    q.queue_direction = 1;  // Elevator moving up

    // Add some orders
    queue_add(3, 0, &q);  // Up order for floor 3
    queue_add(1, 0, &q);  // Up order for floor 1
    queue_add(4, 1, &q);  // Down order for floor 4

    print_queue(&q);  // Print the queue before sorting
    queue_sort(&q);   // Sort the queue
    print_queue(&q);  // Print the queue after sorting
}

void test_queue_sort_down_direction() {
    printf("Testing queue sort when elevator is moving down...\n");

    Queue q;
    queue_init(&q);

    // Set elevator to current floor 2
    q.queue_current_floor = 2;
    q.queue_direction = -1;  // Elevator moving down

    // Add some orders
    queue_add(3, 0, &q);  // Up order for floor 3
    queue_add(1, 0, &q);  // Up order for floor 1
    queue_add(4, 1, &q);  // Down order for floor 4

    print_queue(&q);  // Print the queue before sorting
    queue_sort(&q);   // Sort the queue
    print_queue(&q);  // Print the queue after sorting
}

int main() {
    // Test queue sorting in different directions
    test_queue_sort_up_direction();
    test_queue_sort_down_direction();

    return 0;
}