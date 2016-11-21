/*
Compile: make stdout_stderr
Run: ./stdout_stderr
./stdout_stderr 1> out.txt 2> err.txt
*/

#include "base.h"

int main(void) {
    printf("hello (implicitly on stdout)\n");
    fprintf(stdout, "hello (on stdout)\n");
    fprintf(stderr, "hello (on stderr)\n");
}
