/*
Compile: make check_point
Run: ./check_point
make check_point && ./check_point
*/

#include "base.h"

typedef struct {
    int x, y;
} Point;

Point make_point(int x, int y) {
    Point p = { x, y };
    return p;
}

bool test_equal_point(int line, Point a, Point b) {
    bool x_equal = base_test_equal_i(__FILE__, line, a.x, b.x);
    bool y_equal = base_test_equal_i(__FILE__, line, a.y, b.y);
    return x_equal && y_equal;
}

int main(void) {
    test_equal_point(__LINE__, make_point(10, 20), make_point(5 + 5, 2 * 10));
    test_equal_point(__LINE__, make_point(10, 20), make_point(11, 21)); // this test fails
    return 0;
}
