/*
Compile: make fctable
Run: ./fctable
make fctable && ./fctable
*/

#include "base.h"

/*
int main(void) {
    double lower = 0.0;
    double upper = 300.0;
    double step = 20.0;
    double f = lower;
    while (f <= upper) {
    double celsius = (f - 32.0) * 5.0 / 9.0;
    printf("%3.0f %6.1f\n", f, celsius);
    f += step;
    }
    return 0;
}
*/

int main0(void) {
    double lower = 0.0;
    double upper = 300.0;
    double step = 20.0;
    for (double f = lower; f <= upper; f += step) {
        double c = (f - 32.0) * 5.0 / 9.0;
        printf("%3.0f %6.1f\n", f, c);
    }
    return 0;
}

int main(void) {
    double lower = 0.0;
    double upper = 300.0;
    double step = 20.0;
    for (double f = lower; f <= upper; f += step) {
        double c = (f - 32.0) * 5.0 / 9.0;
        printd(f);
        prints("   ");
        printd(c);
        println();
    }
    return 0;
}
