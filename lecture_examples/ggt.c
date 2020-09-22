/*
Compile: make ggt
Run: ./ggt
make ggt && ./ggt
*/
 
#include "base.h"

/*
Design a function that computes the greatest common divisor of numbers x and y.
*/

// int, int -> int
// Computes the greatest common divisor of numbers x and y.
int ggt(int x, int y);

static void ggt_test(void) {
    test_equal_i(ggt(28, 20), 4);
    test_equal_i(ggt(11, 12), 1);
}

int ggt(int x, int y) {
    if (y == 0) return x;
    else return ggt(y, x % y);
}

int main(void) {
    ggt_test();
    return 0;
}
