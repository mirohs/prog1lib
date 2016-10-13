/*
Compile: make input_output
Run: ./input_output
Compile & run: make input_output && ./input_output
*/
 
#include "base.h"
#include "string.h"

int main(void) {
    String s = s_input(100); // reads at most 100 characters

    prints("Your input was: ");
    printsln(s);

    prints("Number of characters: ");
    printiln(s_length(s));

    return 0;
}
