/*
Compile: make divide
Run: ./divide
Compile & run: make divide && ./divide
*/

#include "base.h"

int main(void) {
    double x;

    x = 8 / 9;
    printdln(x);
    x = 10 / 9;
    printdln(x);
    x = -8 / 9;
    printdln(x);
    x = 3.0 / 2;
    printdln(x);
    x = 3.0 / 2.0;
    printdln(x);

    return 0;
}
