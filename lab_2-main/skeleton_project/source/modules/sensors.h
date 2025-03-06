#pragma once
#include <stdbool.h>
#include "elevio.h"
#include "elevator_types.h"

void sensors_update(Sensors *sensors);

bool sensors_is_button_pressed(Sensors *sensors, int floor, int button);

int sensors_get_current_floor(Sensors *sensors);