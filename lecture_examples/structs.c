/*
Compile: make structs
Run: ./structs
make structs && ./structs
*/

#include "base.h"

struct Point {
    int x;
    int y;
};

typedef struct Point Point;

bool points_equal(Point a, Point b) {
    return a.x == b.x && a.y == b.y;
}

int main(void) {
    Point a = { 15, 26 };
    Point b = a;
    // printbln(a == b); // error: "invalid operands to binary expression"
    printbln(points_equal(a, b));
    
    
    return 0;
}
