#pragma once
#include "sensors.h"
#include "queue.h"

void orders_register_order(Sensors *sensors, Queue *queue);
void orders_approve_order(Sensors *sensors, Queue *queue);