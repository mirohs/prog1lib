/*
Compile: make divide
Run: ./divide
make divide && ./divide
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
    
    printsln("-----------");
    for (int i = -10; i <= 10; i++) {
        printf("%d\t%d\t%d\n", i, i / 9, i % 9);
    }

    return 0;
}
