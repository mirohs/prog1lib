/*
Compile: make while_30_3_squared
Run: ./while_30_3_squared
make while_30_3_squared && ./while_30_3_squared
*/
 
#include "base.h"
 
int main(void) {
    // Iterate over the multiples of 3, from 30 down to 3.
    // Output the square of each of these values.
    int i = 30;
    while (i >= 3) {
        assert("while condition", i >= 3);
        assert("initialization and update", i <= 30);
        // printiln(i); // for inspecting the iteration variable
        printiln(i * i); // square the iterated values
        i = i - 3;
    }
    assert("inverse of while condition", !(i >= 3)); // inverse of condition (not)
    return 0;
}

/*
int main(void) {
    // purpose
    int i = initial_value;
    while (condition_on_i) {
        assert(condition_on_i);
        assert(condition_on_i_init_update);
        printiln(i); // for inspecting the iteration variable
        // statements ...i...;
        update i;
    }
    assert(!(condition_on_i)); // inverse of condition (not)
    return 0;
}
*/
