/*
Compile: make input
Run: ./input
make input && ./input
*/
 
#include "base.h"

int main(void) {
    int i = i_input();
    printiln(2 * i);
    double d = d_input();
    printdln(1.5 * d);
    return 0;
}
