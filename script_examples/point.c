/*
Compile: make point
Run: ./point
make point && ./point
*/
 
#include "base.h"

/*
Objects are located somewhere on a 2D plane. Design a function that computes
the distance of the center of an object to the origin of the coordinate system.
*/

struct Point {
    double x; // represents the x-coordinate of a point
    double y; // represents the y-coordinate of a point
};

// constructor for Point
struct Point make_point(double x, double y) {
    struct Point p = { x, y };
    return p;
}

// struct Point -> double
// Computes the distance from the given point
// to the origin of the coordinate system.
double distance_to_origin(struct Point p);

static void distance_to_origin_test() {
    test_within_d(distance_to_origin(make_point(0, 0)),
        0.0, EPSILON);
    test_within_d(distance_to_origin(make_point(1, 0)),
        1.0, EPSILON);
    test_within_d(distance_to_origin(make_point(-1, 0)),
        1.0, EPSILON);
    test_within_d(distance_to_origin(make_point(0, 2)),
        2.0, EPSILON);
    test_within_d(distance_to_origin(make_point(0, -2)),
        2.0, EPSILON);
    test_within_d(distance_to_origin(make_point(3, 4)),
        5.0, EPSILON);
    test_within_d(distance_to_origin(make_point(3, -4)),
        5.0, EPSILON);
}

// Computes the distance from the given point
// to the origin of the coordinate system.
double distance_to_origin(struct Point p) {
    return sqrt(p.x * p.x + p.y * p.y); // square root math.h
}

int main(void) {
    distance_to_origin_test();
    return 0;
}



/*
1. Problem statement (given)
Objects are located somewhere on a 2D plane. Design a function that computes
the distance of the center of an object to the origin of the coordinate system.

2. Write a data definition
struct Point {
    double x; // represents the x-coordinate of a point
    double y; // represents the y-coordinate of a point
};

// constructor for Point
struct Point make_point(double x, double y) {
    struct Point p = { x, y };
    return p;
}

3. Write a function header
// struct Point -> double
double distance_to_origin(struct Point p);

4. Write a purpose statement for the function
Computes the distance from the given point
to the origin of the coordinate system.

5. Give examples and expected results
For point (0,0) expect a distance to origin of 0.
For point (1,0) expect a distance to origin of 1.
For point (-1,0) expect a distance to origin of 1.
For point (3,4) expect a distance to origin of 5.
etc.

6. Implement the function body
// Computes the distance from the given point
// to the origin of the coordinate system.
double distance_to_origin(struct Point p) {
    return sqrt(p.x * p.x + p.y * p.y); // square root math.h
}

7. Test the function
static void distance_to_origin_test() {
    test_within_d(distance_to_origin(make_point(0, 0)),
        0.0, EPSILON);
    test_within_d(distance_to_origin(make_point(1, 0)),
        1.0, EPSILON);
    test_within_d(distance_to_origin(make_point(-1, 0)),
        1.0, EPSILON);
    test_within_d(distance_to_origin(make_point(0, 2)),
        2.0, EPSILON);
    test_within_d(distance_to_origin(make_point(0, -2)),
        2.0, EPSILON);
    test_within_d(distance_to_origin(make_point(3, 4)),
        5.0, EPSILON);
    test_within_d(distance_to_origin(make_point(3, -4)),
        5.0, EPSILON);
}

int main(void) {
    distance_to_origin_test();
    return 0;
}

8. Review and revise the function
no revision required
*/
