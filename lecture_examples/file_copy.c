/*
Compile: make file_copy
Run: ./file_copy
Compile & run: make file_copy && ./file_copy
*/

#include "base.h"

int main(void) {
    int c;
    while ((c = getchar()) != EOF) {
        putchar(c);
    }
    return 0;
}
