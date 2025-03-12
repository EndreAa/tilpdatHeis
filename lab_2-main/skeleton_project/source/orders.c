#include "orders.h"

void orders_register_order(ElevatorSM *sm) {
    for (int floor = 0; floor < FLOORS; floor++) {
        for (int button = 0; button < BUTTONS; button++) {
            if (sensors_is_button_pressed(&(sm->sensors), floor, button)) {
                queue_add(sm, floor, button); // button can be 0, 1, or 2
                lights_turn_on(floor, button);
            }
        }
    }
}
