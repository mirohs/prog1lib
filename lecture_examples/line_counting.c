/*
Compile: make line_counting
Run: ./line_counting
make line_counting && ./line_counting
*/

#include "base.h"

int main(void) {
    int c;
    int lines = 0;
    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            lines++;
        }
    }
    printf("%d lines\n", lines);
    return 0;
}
