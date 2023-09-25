/*
Compile: make assertions
Run: ./assertions
make assertions && ./assertions
*/

#include "base.h"

int main(void) {
    int i = 123;
    // assert("description", i < 0);
    // assert_x("description", i < 0, "i = %d", i);
    // panic("my panic: %d", i);
    // panic_if(i > 0, "panicing because i > 0: %d", i);
    PL;
    PLi(1);
    PLf("i = %d", 123);
    exit_if(i > 0, "exiting because i > 0: %d", i);
    return 0;
}
