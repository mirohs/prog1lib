/*
Compile: make cast_pointer
Run: ./cast_pointer
*/

#include "base.h"

int main(void) {
    int i = 7; // at address 1000 (for example)
    int j = 8; // at address 1004 (for example)
    printiln(&i - &j); // 1 (j is further up runtime stack, which grows down)
    int *p = &j;
    printf("%lx\n", (long)&j);
    printf("%lx\n", (long)p);
    Byte *b = (Byte*) p; // type cast from int* to Byte*
    printiln(* b   ); // 8 (first byte of i)
    printiln(*(b+1)); // 0 (second byte of i)
    printiln(*(b+2)); // 0 (third byte of i)
    printiln(*(b+3)); // 0 (fourth byte of i)
    printiln(*(b+4)); // 7 (first byte of j)
    printiln(*(b+5)); // 0 (second byte of j)
    printiln(*(b+6)); // 0 (third byte of j)
    printiln(*(b+7)); // 0 (fourth byte of j)
    
    char messageArray[ ] = "now is the time";
    char *messagePointer = "now is the time";
    printsln(messageArray);
    printsln(messagePointer);
    messageArray[0] = 'N';
//    messagePointer[0] = 'N'; // bus error
    printsln(messageArray);
    // printsln(messagePointer); 

    char *s = "hello";
    char *t = "hello";
    printbln(s == t); // output: true
        
    char *u = "hello";
    char v[] = "hello";
    printbln(u == v); // output: false
    
    printbln(strcmp(u, v) == 0); // output: true
    v[0] = 'i';
    printiln(strcmp(u, v));
    
    return 0;
}
