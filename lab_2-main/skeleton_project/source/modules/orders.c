#include "orders.h"
#include "elevator_types.h"

// Registers orders based on button presses
void orders_register_order(Sensors *sensors, Queue *queue) {
    for (int floor = 0; floor < FLOORS; floor++) {
        for (int button = 0; button < BUTTONS; button++) {
            if (sensors_is_button_pressed(sensors, floor, button)) {
                int direction = (button == 2) ? -1 : 1; 
                queue_add(floor, direction, queue);
            }
        }
    }
}

// Approves orders when the elevator reaches the target floor
// void orders_approve_order(Sensors *sensors, Queue *queue) {
//     int current_floor = sensors_get_current_floor(sensors);
//     if (current_floor != -1 && queue_contains(current_floor, queue)) {
//         queue_remove(current_floor, queue);
//     }
// }