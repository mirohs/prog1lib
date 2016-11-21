/*
Compile: make check_expect
Run: ./check_expect
Compile & run: make check_expect && ./check_expect
*/
 
#include "base.h"
#include "string.h"

int main(void) {
    check_expect_i(s_length("hello"), 5);
    check_within_d(sqrt(2.0), 1.414, 0.01);
    check_expect_s(s_concat("hello", "world"), "helloworld");
    check_expect_b(s_contains("world", "rl"), true);
    return 0;
}
