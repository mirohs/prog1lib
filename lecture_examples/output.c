/*
Compile: make output
Run: ./output
make output && ./output
*/
 
#include "base.h"

int main(void) {
    printi(123);
    printiln(456); 
    printdln(3.141592654);
    printsln("hello world");
    
    printiln(i_rnd(100));
    printdln(d_rnd(3.14));
    
    return 0;
}
