/*
Compile: make argv
Run: ./argv
make argv && ./argv
*/

#include "base.h"
#include "string.h"

// int main(int argc, char *argv[]) {
int main(int argc, String argv[]) {
    for (int i = 0; i <= argc; i++) {
        printsln(argv[i]);
    }
    return 0;
}
