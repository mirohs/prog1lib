/*
Compile: make traffic_light
Run: ./traffic_light
make traffic_light && ./traffic_light
*/
 
#include "base.h"

/*
Design a function that returns the next color of a traffic light 
given the current color of the traffic light.
*/

enum TrafficLight {
    RED,
    GREEN,
    YELLOW
};

// enum TrafficLight -> enum TrafficLight
// Return the amount of tax for the given price.
enum TrafficLight traffic_light_next(enum TrafficLight);

static void traffic_light_next_test() {
    test_equal_i(traffic_light_next(RED), GREEN);
    test_equal_i(traffic_light_next(GREEN), YELLOW);
    test_equal_i(traffic_light_next(YELLOW), RED);
}

// Produces the next color of a traffic light
// given the current color of the traffic light.
enum TrafficLight traffic_light_next(enum TrafficLight tl) {
    if (tl == RED) {
        return GREEN;
    } else if (tl == GREEN) {
        return YELLOW;
    } else if (tl == YELLOW) {
        return RED;
    }
    return RED;
}

int main(void) {
    traffic_light_next_test();
    return 0;
}



/*
1. Problem statement (given)
Design a function that returns the next color of a traffic light 
given the current color of the traffic light.

2. Write a data definition
enum TrafficLight {
    RED,
    GREEN,
    YELLOW
};

3. Write a function header
// enum TrafficLight -> enum TrafficLight
enum TrafficLight traffic_light_next(enum TrafficLight);

4. Write a purpose statement for the function
Produces the next color of a traffic light 
given the current color of the traffic light.

5. Give examples and expected results
If the traffic light is RED, expect GREEN as the next state.
If the traffic light is GREEN, expect YELLOW as the next state.
If the traffic light is YELLOW, expect RED as the next state.

6. Implement the function body
enum TrafficLight traffic_light_next(enum TrafficLight tl) {
    if (tl == RED) {
        return GREEN;
    } else if (tl == GREEN) {
        return YELLOW;
    } else if (tl == YELLOW) {
        return RED;
    }
    return RED;
}

7. Test the function
static void traffic_light_next_test() {
    test_equal_i(traffic_light_next(RED), GREEN);
    test_equal_i(traffic_light_next(GREEN), YELLOW);
    test_equal_i(traffic_light_next(YELLOW), RED);
}

int main(void) {
    traffic_light_next_test();
    return 0;
}

8. Review and revise the function
no revision required
*/
