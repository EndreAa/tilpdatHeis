#include <stdio.h>
#include "queue.h"
#include <assert.h>


void print_queue(Queue *q) {
    printf("Queue: [");
    for (int i = 0; i < q->queue_size; i++) {
        printf("%d", q->queue_list[i]);
        if (i < q->queue_size - 1) printf(", ");
    }
    printf("], Size: %d, Direction: %d, First Priority: %d\n", 
            q->queue_size, q->queue_direction, q->queue_first_pri);
}

void test_queue_add() {
    Queue q = { .queue_size = 0, .queue_direction = 0, .queue_first_pri = -1 };
    
    print_queue(&q);
    // Add orders in arbitrary order
    queue_add(5, 1, &q);  // Direction is up
    queue_add(3, 1, &q);
    queue_add(2, 1, &q);

    // After adding orders, the queue should be sorted in ascending order for up direction
    print_queue(&q);
    assert(q.queue_size == 3);
    assert(q.queue_list[0] == 2);
    assert(q.queue_list[1] == 3);
    assert(q.queue_list[2] == 5);
    
    // Add another order (duplicate)
    queue_add(3, 1, &q);
    assert(q.queue_size == 3);  // Ensure no duplicate orders are added

    // Change direction to down and add new orders
    queue_add(7, -1, &q);  // Direction is down
    queue_add(6, -1, &q);
    print_queue(&q);
    
    // The queue should now be sorted in descending order (since direction is down)
    assert(q.queue_list[0] == 7);
    assert(q.queue_list[1] == 6);
    assert(q.queue_list[2] == 5);
    assert(q.queue_list[3] == 3);
    assert(q.queue_list[4] == 2);

    printf("test_queue_add passed!\n");
    fflush(stdout);
}

void test_queue_remove() {
    Queue q = { .queue_list = {2, 3, 5}, .queue_size = 3, .queue_direction = 1, .queue_first_pri = 2 };

    queue_remove(3, &q);
    assert(q.queue_size == 2);
    assert(q.queue_list[0] == 2);
    assert(q.queue_list[1] == 5);

    queue_remove(2, &q);
    assert(q.queue_size == 1);
    assert(q.queue_list[0] == 5);

    queue_remove(5, &q);
    assert(q.queue_size == 0);

    printf("test_queue_remove passed!\n");
}

void test_queue_get_next() {
    Queue q = { .queue_list = {1, 4, 6}, .queue_size = 3, .queue_direction = 1, .queue_first_pri = 1 };
    
    int next = queue_get_next(&q, 2);
    assert(next == 4); // Should go to 4 first

    next = queue_get_next(&q, 5);
    assert(next == 6); // Should go to 6

    printf("test_queue_get_next passed!\n");
}

void test_queue_empty() {
    Queue q = { .queue_list = {1, 2, 3}, .queue_size = 3, .queue_direction = 1, .queue_first_pri = 1 };

    queue_empty(&q);
    assert(q.queue_size == 0);
    assert(q.queue_first_pri == -1);
    assert(q.queue_direction == -1);

    printf("test_queue_empty passed!\n");
}

int main() {
    test_queue_add();
    test_queue_remove();
    test_queue_get_next();
    test_queue_empty();
    printf("All tests passed!\n");
    return 0;
}