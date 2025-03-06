#include "orders.h"

// Registers orders based on button presses
void orders_register_order(ElevatorSM *sm) {
    for (int floor = 0; floor < FLOORS; floor++) {
        for (int button = 0; button < BUTTONS; button++) {
            if (sensors_is_button_pressed(&(sm->sensors), floor, button)) {
                queue_add(floor, sm,); // endre på dette
            }
        }
    }
}