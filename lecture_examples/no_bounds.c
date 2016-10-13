/*
Compile: make no_bounds
Run: ./no_bounds
make no_bounds && ./no_bounds
*/

#include "base.h"

/*
int* f(void) {
    int a[] = { 33, 11, 15, 7, -7 };
    return a;
}
*/

void f(/*in/out*/ int a[], /*in*/ int n) {
    for (int i = 0; i < n; i++) {
        a[i] *= -1;
    }
}

int g(int i) {
    prints("g");
    printiln(i);
    int j = i_input();
    int a[j];
    a[j-1] = 3;
    return a[j-1];
}

/*
void h(int i) {
    int a[1000000];
    a[0] = i;
    printiln(i);
    h(i + 1);
}
*/

int main(void) {
    int a[] = { 33, 11, 15, 7, -7 };
    printiln(a[0]);
    printiln(a[4]);
    int i = -1;
    printiln(a[i]);
    i = 5;
    printiln(a[i]);
    f(a, 5);
    printialn(a, 5);
    printiln(g(3));
//    h(1);
    return 0;
}
