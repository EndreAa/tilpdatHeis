#include "sensors.h"


void sensors_update(Sensors *sensors) {
    sensors->sensor_stop_button = elevio_stopButton();
    sensors->sensor_current_floor = elevio_floorSensor();

    for (int floor = 0; floor < FLOORS; floor++) {
        for (int button = 0; button < BUTTONS; button++) {
            sensors->sensor_button_values[floor][button] = elevio_callButton(floor, button);
        }
    }
}

bool sensors_is_button_pressed(Sensors *sensors, int floor, int button) {
    return sensors->sensor_button_values[floor][button];
}


int sensors_get_current_floor(Sensors *sensors) {
    return sensors->sensor_current_floor;
}
