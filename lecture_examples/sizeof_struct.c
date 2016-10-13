/*
Compile: make sizeof_struct
Run: ./sizeof_struct
make sizeof_struct && ./sizeof_struct
*/

#include "base.h"
#include "string.h"

struct S1 { char c; int i; }; // sizeof(struct S1) == 8
struct S2 { char c; double d; }; // sizeof(struct S2) == 16
struct S3 { char c; void *v; }; // sizeof(struct S3) == 16
struct S4 { char c1; char c2; void *v; }; // sizeof(struct S4) == 16
struct S5 { char c1; char c2; int i; void *v; }; // sizeof(struct S5) == 16


int main(void) {
    printiln(sizeof(struct S1));
    printiln(sizeof(struct S2));
    printiln(sizeof(struct S3));
    printiln(sizeof(struct S4));
    printiln(sizeof(struct S5));
    return 0;
}
