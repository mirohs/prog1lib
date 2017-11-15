/*
Compile: make stars
Run: ./stars
make stars && ./stars
*/

#include "base.h"
#include "string.h"

// typedef char* String;

String stars(int n) {
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
    test_equal_s(s = stars(0), "");
    s_free(s);
    test_equal_s(s = stars(1), "*");
    s_free(s);
    test_equal_s(s = stars(2), "**");
    s_free(s);
    test_equal_s(s = stars(3), "***");
    s_free(s);
}

int main(void) {
    base_set_memory_check(true);
    stars_test();
    return 0;
}
