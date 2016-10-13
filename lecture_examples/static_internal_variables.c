/*
Compile: make static_internal_variables
Run: ./static_internal_variables
make static_internal_variables && ./static_internal_variables
*/

#include "base.h"

int inc(void) {
    // int i = 3;
    static int i = 3;
    i++;
    return i;
}

int main(void) {
    printiln(inc());
    printiln(inc());
    printiln(inc());
    return 0;
}
