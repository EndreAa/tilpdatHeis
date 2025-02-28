#ifndef DOOR_H
#define DOOR_H

#include <stdbool.h>
#include <stdio.h>
 

void door_open();

void door_close();

void door_count();

/* 
uses the 3 functions above to open the doors, 
hold them open for 3 seconds, and then close them again
*/

void door_deliver_to_floor();

#endif