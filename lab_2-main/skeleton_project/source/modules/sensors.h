#pragma once
#include <stdbool.h>

#define FLOORS 4
#define BUTTONS 3 // Inside, outside, and ... ?

typedef struct {
    bool sensor_stop_button;
    int sensor_current_floor;
    bool sensor_obstacle;
    int sensor_button_values[FLOORS][BUTTONS]; // [floor1 -> [0,0,1], floor2 ...] 
} Sensors;  


void sensors_get_button_values(Sensors *sensors); // private method, how?
