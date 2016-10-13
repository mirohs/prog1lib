/*
Compile: make block_structure
Run: ./block_structure
*/

#include "base.h"

int i = 123;
// extern int i;

int f(void) {
    extern int i;
    return i;
}

int main(void) {
    int i = 5;
    if (i > 1) {
        int i = 1;
        printiln(i);
    }
    printiln(i);
    printiln(f());
    return 0;
}
