/*
Compile: make breaker
Run: ./breaker
*/

#include "base.h"
#include "string.h"

/*
int main(void) {
    while (true) {
        prints("> ");
        String s = s_input(100);
        if (s_contains(s, "exit")) {
            break;
        }
        prints("executing ");
        printsln(s);
    }
    printsln("finished");
    return 0;
}
*/

int main(void) {
    prints("> ");
    String s = s_input(100);
    while (!s_contains(s, "exit")) {
        prints("executing ");
        printsln(s);
        prints("> ");
        s = s_input(100);
    }
    printsln("finished");
    return 0;
}
