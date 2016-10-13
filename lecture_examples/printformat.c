/*
Compile: make printformat
Run: ./printformat
make printformat && ./printformat
*/

#include "base.h"

int main(void) {
    int base = 3;
    double factor = 1.5;
    double total = factor * base;
    String unit = "kg";
    printf("%d %s times %g is %g %s.\n", base, unit, factor, total, unit);

    printi(base);
    prints(" ");
    prints(unit);
    prints(" times ");
    printd(factor);
    prints(" is ");
    printd(total);
    prints(" ");
    prints(unit);
    printsln(".");

    return 0;
}
