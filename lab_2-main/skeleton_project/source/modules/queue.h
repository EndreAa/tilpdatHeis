#pragma once
#include "orders.h"
#include <vector>


typedef struct {
    int queue_first_pri;
    int queue_direction;
    int queue_current_floor;
    std::vector<int> queue_list;
} Queue;

