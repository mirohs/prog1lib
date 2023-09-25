/*
Compile: make input_output
Run: ./input_output
make input_output && ./input_output
*/
 
#include "base.h"

int main(void) {
    String s = s_input(100); // reads at most 100 characters

    prints("Your input was: ");
    printsln(s);

    prints("Number of characters: ");
    printiln(strlen(s));

    return 0;
}
