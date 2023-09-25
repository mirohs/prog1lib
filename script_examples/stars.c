/*
Compile: make stars
Run: ./stars
make stars && ./stars
*/

#include "base.h"

// typedef char* String;

String stars(int n) {
    require("not negative", n >= 0);
    char *s = xmalloc(n + 1);
//  char *s = xcalloc(n + 1, 1);
    for (int i = 0; i < n; i++) {
        s[i] = '*';
    }
    s[n] = '\0';
    return s;
}

void stars_test(void) {
    String s;
    // would violate precondition:
    // test_equal_s(s = stars(-1), "");
    test_equal_s(s = stars(0), "");
    free(s);
    test_equal_s(s = stars(1), "*");
    free(s);
    test_equal_s(s = stars(2), "**");
    free(s);
    test_equal_s(s = stars(3), "***");
    free(s);
}

int main(void) {
    report_memory_leaks(true);
    stars_test();
    return 0;
}
