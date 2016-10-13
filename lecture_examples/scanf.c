/*
Compile: make scanf
Run: ./scanf
make scanf && ./scanf
*/

#include <stdio.h> // declaration scanf function header

int main(void) {
    int day = 0;
    int month = 0;
    int year = 0;
    int matches = scanf("%d.%d.%d", &day, &month, &year);
    printf("%d: %d %d %d\n", matches, day, month, year);
    return 0; 
}
