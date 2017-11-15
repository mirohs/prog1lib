/*
Compile: make test_equal
Run: ./test_equal
Compile & run: make test_equal && ./test_equal
*/
 
#include "base.h"
#include "string.h"

int main(void) {
    test_equal_i(s_length("hello"), 5);
    test_within_d(sqrt(2.0), 1.414, 0.01);
    test_equal_s(s_concat("hello", "world"), "helloworld");
    test_equal_b(s_contains("world", "rl"), true);
    return 0;
}
