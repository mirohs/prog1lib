/*
Compile: make argv
Run: ./argv
make argv && ./argv 1 2 3
*/

#include "base.h"

// int main(int argc, char** argv) {
// int main(int argc, char * argv[]) {
int main(int argc, String argv[]) {
    for (int i = 0; i <= argc; i++) {
        String s = argv[i];
        if (s == NULL) {
            printsln("(NULL)");
        } else {
            printsln(argv[i]);
        }
    }
    return 0;
}
