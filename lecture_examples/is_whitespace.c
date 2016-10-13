/*
Compile: make is_whitespace
Run: ./is_whitespace
Compile & run: make is_whitespace && ./is_whitespace
*/

#include "base.h"

bool is_whitespace(char c) {
    switch (c) {
        case ' ': // fallthrough
        case '\t': // fallthrough
        case '\r': // fallthrough
        case '\n':
            return true;
        default: // not whitespace
            return false;
    }
}

int main(void) {
    printbln(is_whitespace('a'));
    printbln(is_whitespace(' '));
    return 0;
}
