/*
Compile: make function_pointer
Run: ./function_pointer
make function_pointer && ./function_pointer
*/
 
#include <stdio.h>

char *hello(void) { 
    return "hello"; 
}

int main(void) {
    printf ("%s\n", hello());   // calls function hello
                // output: hello
    printf("%#lx\n", (unsigned long)hello);     // address of function
                                                // output: 0x10fe4df10
    printf("%p\n", hello);                      // address of function
                                                // output: 0x10fe4df10
    return 0;
}
