/*
Compile: make for_30_3_squared
Run: ./for_30_3_squared
make for_30_3_squared && ./for_30_3_squared
*/
 
#include "base.h"
 
int main(void) {
    // Iterate over the multiples of 3, from 30 down to 3.
    // Output the square of each of these values.
    for (int i = 30; i >= 3; i = i - 3) {
        assert("loop condition", i >= 3);
        assert("loop initialization", i <= 30);
        // printiln(i); // for inspecting the iteration variable
        printiln(i * i); // square the iterated values
    }
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
