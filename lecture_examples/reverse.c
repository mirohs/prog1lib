/*
Compile: make reverse
Run: ./reverse
make reverse && ./reverse
*/

#include "base.h"

int length(char s[]) {
    int i = 0;
    while (s[i] != '\0') i++;
    return i;
}

void reverse(char s[]) {
    int n = length(s);
    for (int i = 0, j = n - 1; i < j; i++, j--) {
        char c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

int main(void) {
    char message[] = "Hannover";
    printsln(message);
    reverse(message);
    printsln(message);
    return 0;
}
