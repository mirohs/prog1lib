/*
Compile: make quicksort
Run: ./quicksort
make quicksort && ./quicksort
*/

#include "base.h"

void swap(int v[], int i, int j) {
    int a = v[i];
    v[i] = v[j];
    v[j] = a;
}

void quicksort2(int v[], int left, int right);

void quicksort_test(void) {
    int a[] = { 7, 4, 2, 1, 3 };
    int na = sizeof(a) / sizeof(int); // sizeof(a): number of bytes
    quicksort2(a, 0, na - 1);
    printialn(a, na);

    int b[] = { -4, -4, 1, 2, 3, 7 };
    int nb = sizeof(b) / sizeof(int); // sizeof(a): number of bytes
    quicksort2(b, 0, nb - 1);
    printialn(b, nb);
}

void quicksort2(int v[], int left, int right) {
    if (left >= right) return; // 0 or 1 elements, recursion end
//    printialn(v + left, right - left + 1);
    swap(v, left, (left + right) / 2); // move pivot element to left
//    printialn(v + left, right - left + 1);
    int j = left;
    for (int i = left + 1; i <= right; i++) {
        if (v[i] < v[left]) {
            swap(v, ++j, i);
        }
        // assert: v[i] < v[left] for i = left+1..j
    }
//    printialn(v + left, right - left + 1);
    swap(v, left, j); // move back pivot element
//    printialn(v + left, right - left + 1);
//    exit(1);
    quicksort2(v, left, j-1); // assert: v[i] < v[j] for i = left..j-1
    quicksort2(v, j+1, right); // assert: v[i] >= v[j] for i = j+1..right
}

int main(void) {
    report_memory_leaks(true);
    quicksort_test();
    return 0;
}
