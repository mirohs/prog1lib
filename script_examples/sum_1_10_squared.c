/*
Compile: make sum_1_10_squared
Run: ./sum_1_10_squared
make sum_1_10_squared && ./sum_1_10_squared
*/
 
#include "base.h"
 
int main(void) {
    // Iterate over the integers 1 to 10.
    // Compute the sum of squares of these values.
    int sum = 0; // <--- new variable for intermediary (and final) result
    int i = 1;
    while (i <= 10) {
        assert("while condition", i <= 10);
        assert("initialization and update", i >= 1);
        // printiln(i); // for inspecting the iteration variable
        sum = sum + i * i; // <--- adding the next squared value
        i = i + 1;
    }
    assert("inverse of while condition", !(i <= 10)); // inverse of condition (not)
    prints("sum = "); // <--- output
    printiln(sum);    // <--- output (value and newline)
    return 0;
}

/*
int main(void) {
    // purpose
    for (int i = initial_value; condition_on_i; update i) {
        assert(condition_on_i);
        assert(condition_on_i_init_update);
        printiln(i); // for inspecting the iteration variable
        // statements ...i...;
    }
    return 0;
}
*/
