/*
Compile: make rec_30_3_squared
Run: ./rec_30_3_squared
make rec_30_3_squared && ./rec_30_3_squared
*/
 
#include "base.h"

// Iterate over the multiples of 3, from 30 down to 3.
// Output the square of each of these values.
// should only be called from repeat (or from itself)
void repeat_rec(int i) {
    if (i >= 3) {
        assert("if condition", i >= 3);
        // printiln(i); // for inspecting the iteration variable
        printiln(i * i);
        repeat_rec(i - 3); // function calls itself
                           // with another argument
    }
}
 
void repeat(void) {
    repeat_rec(30);
}
 
int main(void) {
    repeat();
    return 0;
}



/*
// purpose
// should only be called from repeat (or from itself)
void repeat_rec(int i) {
    if (condition_on_i) {
        assert(condition_on_i);
        printiln(i); // for inspecting the iteration variable
        // statements ...i...;
        repeat_rec(update_i); // function calls itself
                              // with another argument
    }
}
 
void repeat(void) {
    repeat_rec(initial_value);
}
 
int main(void) {
    repeat();
    return 0;
}
*/
