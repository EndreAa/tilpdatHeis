#pragma once
#include <stdbool.h>
#include <stdio.h>
#include "elevator_types.h"
#include "driver.h"

void sensors_update(Sensors *sensors);

bool sensors_is_button_pressed(Sensors *sensors, int floor, int button);

int sensors_get_current_floor(Sensors *sensors);
