/*
Compile: make read_write_file
Run: ./read_write_file
make read_write_file && ./read_write_file
*/

#include "base.h"

void upper_case(String s) {
    while (*s) {
        char c = *s;
        if (c >= 'a' && c <= 'z') {
            *s = c - 'a' + 'A';
        } else {
            *s = c;
        }
        s++;
    }
}

int main(void) {
    String s = s_read_file("read_write_file.c");
    printf("This file has %lu characters.\n", strlen(s));
    upper_case(s); // convert each character to upper case
    s_write_file("MY_UPPER.TXT", s);
    return 0;
}
