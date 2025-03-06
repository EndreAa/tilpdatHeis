#include "queue.h"
#include <stdio.h>

// Simple helper function to print queue status
void print_test_result(const char* test_name, Queue *q) {
    printf("\n-- %s --\n", test_name);
    print_queue(q);
}

// Test case: Initialize the queue and print it
void test_queue_init() {
    Queue q;
    queue_init(&q);
    print_test_result("Initial Queue", &q);
}

// Test case: Add an order and print the queue
void test_queue_add_order() {
    Queue q;
    queue_init(&q);
    
    queue_add(3, 0, &q);  // Add an order to go up at floor 3
    print_test_result("Queue after adding order 3 (up)", &q);
    
    queue_add(1, 1, &q);  // Add an order to go down at floor 1
    print_test_result("Queue after adding order 1 (down)", &q);
    
    queue_add(2, 0, &q);  // Add an order to go up at floor 2
    print_test_result("Queue after adding order 2 (up)", &q);
    
    queue_add(0, 2, &q);  // Add a "cab" order at floor 0
    print_test_result("Queue after adding cab order 0", &q);
}

// Test case: Add orders, remove an order, and print the queue
void test_queue_remove_order() {
    Queue q;
    queue_init(&q);
    
    queue_add(3, 0, &q);
    queue_add(1, 1, &q);
    queue_add(2, 0, &q);
    queue_add(0, 2, &q);
    
    print_test_result("Queue after adding orders", &q);
    
    // Remove an order
    queue_remove(2, &q);  // Remove order 2
    print_test_result("Queue after removing order 2", &q);
    
    queue_remove(0, &q);  // Remove order 0
    print_test_result("Queue after removing order 0", &q);
}

// Test case: Test the sorting functionality when the elevator is moving up
void test_queue_sort_up_direction() {
    Queue q;
    queue_init(&q);
    
    // Adding some orders to simulate a moving elevator
    queue_add(1, 0, &q);  // Order to go up at floor 1
    queue_add(4, 0, &q);  // Order to go up at floor 4
    queue_add(2, 0, &q);  // Order to go up at floor 2
    queue_add(3, 0, &q);  // Order to go up at floor 3
    
    // Set the direction to UP
    q.queue_direction = 1;
    q.queue_current_floor = 0;
    
    print_test_result("Queue before sorting (moving up)", &q);
    
    queue_sort(&q);  // Sorting the queue
    
    print_test_result("Queue after sorting (moving up)", &q);
}

// Test case: Test the sorting functionality when the elevator is moving down
void test_queue_sort_down_direction() {
    Queue q;
    queue_init(&q);
    
    // Adding some orders to simulate a moving elevator
    queue_add(3, 1, &q);  // Order to go down at floor 3
    queue_add(0, 1, &q);  // Order to go down at floor 0
    queue_add(1, 1, &q);  // Order to go down at floor 1
    queue_add(2, 1, &q);  // Order to go down at floor 2
    
    // Set the direction to DOWN
    q.queue_direction = -1;
    q.queue_current_floor = 3;
    
    print_test_result("Queue before sorting (moving down)", &q);
    
    queue_sort(&q);  // Sorting the queue
    
    print_test_result("Queue after sorting (moving down)", &q);
}

// Test case: Test adding duplicate orders to the queue
void test_queue_duplicate_orders() {
    Queue q;
    queue_init(&q);
    
    queue_add(3, 0, &q);  // Add order at floor 3
    queue_add(3, 0, &q);  // Try to add the same order again
    
    print_test_result("Queue after adding duplicate order at floor 3", &q);
    
    queue_add(2, 0, &q);  // Add order at floor 2
    queue_add(2, 0, &q);  // Try to add the same order again
    
    print_test_result("Queue after adding duplicate order at floor 2", &q);
}

// Test case: Test emptying the queue
void test_queue_empty() {
    Queue q;
    queue_init(&q);
    
    queue_add(3, 0, &q);
    queue_add(1, 1, &q);
    
    print_test_result("Queue before emptying", &q);
    
    queue_empty(&q);  // Empty the queue
    print_test_result("Queue after emptying", &q);
}

int main() {
    // Run the tests
    test_queue_init();
    test_queue_add_order();
    test_queue_remove_order();
    test_queue_sort_up_direction();
    test_queue_sort_down_direction();
    test_queue_duplicate_orders();
    test_queue_empty();
    
    return 0;
}