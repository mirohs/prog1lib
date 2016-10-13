/*
Compile: make snprintf
Run: ./snprintf
make snprintf && ./snprintf
*/

#include <stdio.h> // declaration sprintf function header
#include "base.h"
int main(void) {
    char *vehicle = "bike"; // typedef char* String;
    int wheel_count = 2;
    double top_speed = 25.9; // km/h
    char buffer[100]; // will store the formatted string
    snprintf(buffer, 100,
        "My %s has %d wheels and a top speed of %.1f km/h.\n", 
        vehicle, wheel_count, top_speed);
    printsln(buffer);
    return 0;
}
