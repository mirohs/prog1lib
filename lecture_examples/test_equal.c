/*
Compile: make test_equal
Run: ./test_equal
make test_equal && ./test_equal
*/
 
#include "base.h"

int main(void) {
    test_equal_i(strlen("hello"), 5);
    test_within_d(sqrt(2.0), 1.414, 0.01);
    test_equal_s("helloworld", "helloworld");
    test_equal_b(s_contains("world", "rl"), true);
    return 0;
}
