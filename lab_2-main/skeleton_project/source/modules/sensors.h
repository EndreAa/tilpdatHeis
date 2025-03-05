#pragma once
#include <stdbool.h>
#include "elevator_types.h"

#define FLOORS 4
#define BUTTONS 3 // Inside, outside_up, and outside_down


typedef struct {
    bool sensor_stop_button;
    int sensor_current_floor;
    bool sensor_obstacle;
    bool sensor_button_values[FLOORS][BUTTONS]; 
} Sensors;  


void sensors_update(Sensors *sensors);
bool sensors_is_button_pressed(Sensors *sensors, int floor, int button);
int sensors_get_current_floor(Sensors *sensors);