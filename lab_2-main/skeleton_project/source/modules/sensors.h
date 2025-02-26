#pragma once
#include <stdlib.h>

const int FLOORS = 4;
const int BUTTONS = 3;

typedef struct {
    bool sensor_stop_button;
    int sensor_current_floor;
    bool sensor_obstacle;
    bool sensor_button_values[FLOORS][BUTTONS];
} Sensors;  


void sensors_get_button_values(const Sensors *sensors);
