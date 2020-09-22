/*
Compile: make rec_sum_1_3_squared
Run: ./rec_sum_1_3_squared
make rec_sum_1_3_squared && ./rec_sum_1_3_squared
*/
 
#include "base.h"

// Iterate over the integers 1 to 3.
// Compute the sum of squares of these values.
// should only be called from sum_squares
int sum_squares_rec(int i) {
    if (i <= 3) {
        assert("if condition", i <= 3);
        // printiln(i); // for inspecting the iteration variable
        int contribution_of_i = i * i;
        return contribution_of_i + sum_squares_rec(i + 1);
    } else {
        assert("if condition", i > 3);
        return 0;
    }
}
 
int sum_squares(void) {
    return sum_squares_rec(1);
}
 
int main(void) {
    prints("sum = ");
    printiln(sum_squares());
    return 0;
}


/*
// purpose
// should only be called from repeat
int repeat_rec(int i) {
    if (condition) {
        assert(condition);
        // printiln(i); // for inspecting the iteration variable
        int contribution_of_i = ...;
        return conbribution_of_i <op> repeat_rec(update_value);
    } else {
        return initial_value_aggregation;
    }
}
 
int repeat(void) {
    return repeat_rec(initial_value);
}
 
int main(void) {
    printiln(repeat());
    return 0;
}
*/
