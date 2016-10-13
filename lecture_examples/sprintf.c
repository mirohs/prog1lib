/*
Compile: make sprintf
Run: ./sprintf
make sprintf && ./sprintf
*/

#include <stdio.h> // sprintf
#include "base.h"

int main(void) {
    char *vehicle = "bike"; // typedef char* String;
    int wheel_count = 2;
    double max_speed = 25.9; // km/h
    char buffer[100]; // will store the formatted string
    sprintf(buffer, "My %s has %d wheels and a maximum speed of %.1f km/h.\n", 
        vehicle, wheel_count, max_speed);
    printsln(buffer);
    return 0;
}
