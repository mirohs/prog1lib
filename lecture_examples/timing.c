/*
Compile: make timing
Run: ./timing
make timing && ./timing
*/

#include "base.h"

int main(void) {
    timespec start = time_now();
    int n = 10000;
    int x = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            x += i - j;
        }
    }
    double ms = time_ms_since(start);
    printiln(x);
    printdln(ms);
    return 0;
}

