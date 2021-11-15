/*
Compile: make memory_leak
Run: ./memory_leak
make memory_leak && ./memory_leak
*/

#include "base.h"

int main(void) {
    report_memory_leaks(true);
    char *s = xmalloc(1);
    s[0] = '\0';
//    free(s);
    char *t = xmalloc(2);
    t[0] = 'x';
    t[1] = '\0';
//    free(s);
    
//    printf("s = %p\n", s);
//    s = 0x7ff9124028b0;
    
    return 0;
}
