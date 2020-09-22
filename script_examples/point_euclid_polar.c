/*
Compile: make point_euclid_polar
Run: ./point_euclid_polar
make point_euclid_polar && ./point_euclid_polar
*/
 
#include "base.h"

/*
Points on the 2D plane may be given either in Euclidean coordinates or in
polar coordinates. Design a function that computes the distance of such a
point to the origin of the coordinate system.
*/

struct Point {
    enum { TPointEuclid, TPointPolar } tag; // indicate which variant follows
    union {
        struct {                 // Euclidean variant
            double x;
            double y;
        };
        struct {                 // polar variant
            double theta;
            double magnitude;
        };
    };
};

// constructor for PointEuclid
struct Point make_point_euclid(double x, double y) {
    struct Point p;
    p.tag = TPointEuclid;
    p.x = x;
    p.y = y;
    return p;
}
 
// constructor for PointPolar
struct Point make_point_polar(double t, double m) {
    require("valid angle", 0 <= t && t <= 360);
    require("non-negative magnitude", m >= 0);
    struct Point p;
    p.tag = TPointPolar;
    p.theta = t;
    p.magnitude = m;
    return p;
}

// struct Point -> double
// Computes the distance from the given point
// to the origin of the coordinate system.
double distance_to_origin(struct Point p);

static void distance_to_origin_test() {
    // test cases for polar variant
    test_within_d(
        distance_to_origin(make_point_polar(0.0, 0.0)), 
        0.0, EPSILON);
    test_within_d(
        distance_to_origin(make_point_polar(0.0, 0.0)), 
        0.0, EPSILON);
    test_within_d(
         distance_to_origin(make_point_polar(0.0, 1.0)), 
        1.0, EPSILON);
    test_within_d(
        distance_to_origin(make_point_polar(2.3, 2.0)), 
        2.0, EPSILON);

    // would fail precondition:
    // struct Point p = make_point_polar(360.1, 0.0);
    // struct Point p = make_point_polar(-0.1, 0.0);
    // struct Point p = make_point_polar(45, -0.1);
    // printiln(p.tag);

    // test cases for Euclidean variant
    test_within_d(
        distance_to_origin(make_point_euclid(0.0, -2.0)), 
        2.0, EPSILON);
    test_within_d( 
        distance_to_origin(make_point_euclid(2.0, 0.0)), 
        2.0, EPSILON);
    test_within_d( 
        distance_to_origin(make_point_euclid(1.0, 1.0)), 
        sqrt(2.0), EPSILON); // square root
}

// Computes the distance from the given point
// to the origin of the coordinate system.
double distance_to_origin(struct Point p) {
    switch (p.tag) {
        case TPointEuclid:
            return sqrt(p.x * p.x + p.y * p.y); // square root
        case TPointPolar:
            return p.magnitude;
    }
    return 0;
}

int main(void) {
    distance_to_origin_test();
    return 0;
}



/*
1. Problem statement (given)
Points on the 2D plane may be given either in Euclidean coordinates or in
polar coordinates. Design a function that computes the distance of such a
point to the origin of the coordinate system.

2. Write a data definition
enum PointTag {
    TPointEuclid, // tag for a point in Euclidean coordinates
    TPointPolar   // tag for a point in polar coordinates
};

struct Point {
    enum PointTag tag;           // indicate which variant follows
    union {
        struct {                 // Euclidean variant
            double x;
            double y;
        };
        struct {                 // polar variant
            double theta;
            double magnitude;
        };
    };
};

// constructor for PointEuclid
struct Point make_point_euclid(double x, double y) {
    struct Point p;
    p.tag = TPointEuclid;
    p.x = x;
    p.y = y;
    return p;
}
 
// constructor for PointPolar
struct Point make_point_polar(double t, double m) {
    struct Point p;
    p.tag = TPointPolar;
    p.theta = t;
    p.magnitude = m;
    return p;
}

3. Write a function header
// struct Point -> double
double distance_to_origin(struct Point p);

4. Write a purpose statement for the function
Computes the distance from the given point
to the origin of the coordinate system.

5. Give examples and expected results
see below

6. Implement the function body
// Computes the distance from the given point
// to the origin of the coordinate system.
double distance_to_origin(struct Point p) {
    switch (p.tag) {
        case TPointEuclid:
            return sqrt(p.x * p.x + p.y * p.y); // square root
        case TPointPolar:
            return p.magnitude;
    }
    return 0;
}

7. Test the function
static void distance_to_origin_test() {
    // test cases for polar variant
    test_within_d(
        distance_to_origin(make_point_polar(0.0, 0.0)), 
        0.0, EPSILON);
    test_within_d(
         distance_to_origin(make_point_polar(0.0, 1.0)), 
        1.0, EPSILON);
    test_within_d(
        distance_to_origin(make_point_polar(2.3, 2.0)), 
        2.0, EPSILON);

    // test cases for Euclidean variant
    test_within_d(
        distance_to_origin(make_point_euclid(0.0, -2.0)), 
        2.0, EPSILON);
    test_within_d( 
        distance_to_origin(make_point_euclid(2.0, 0.0)), 
        2.0, EPSILON);
    test_within_d( 
        distance_to_origin(make_point_euclid(1.0, 1.0)), 
        sqrt(2.0), EPSILON); // square root
}

int main(void) {
    distance_to_origin_test();
    return 0;
}

8. Review and revise the function
no revision required
*/


