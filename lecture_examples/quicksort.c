/*
Compile: make quicksort
Run: ./quicksort
make quicksort && ./quicksort
*/

#include "base.h"
#include "array.h"
#include "int_array.h"

Array quicksort(Array v);

void quicksort_test() {
    Array a = ia_of_string("7 4 2 1 3");
    Array b = ia_of_string("1 2 3 4 7");
    Array as = quicksort(a);
    ia_test_equal(as, b);
    a_free(a); a_free(b); a_free(as);

    a = ia_of_string("7 -4 2 1 3 -4");
    b = ia_of_string("-4 -4 1 2 3 7");
    as = quicksort(a);
    ia_test_equal(as, b);
    a_free(a); a_free(b); a_free(as);
}

bool less_than(int element, int index, int x) {
    return element < x;
}

bool equal(int element, int index, int x) {
    return element == x;
}

bool greater_than(int element, int index, int x) {
    return element > x;
}
/*
Array quicksort(Array v) {
    int n = a_length(v);
    if (n <= 1) return a_copy(v);
    int p = ia_get(v, n / 2);
    printf("p = %d, v = ", p); ia_println(v);
    Array a = ia_filter(v, less_than, p);
    Array b = ia_filter(v, equal, p);
    Array c = ia_filter(v, greater_than, p);
    int na = a_length(a);
    int nb = a_length(b);
    int nc = a_length(c);
    Array a_sorted = quicksort(a);
    Array c_sorted = quicksort(c);
    Array v_sorted = ia_create(n, 0);
    a_blit(a_sorted, 0, v_sorted, 0, na);
    a_blit(b,        0, v_sorted, na, nb);
    a_blit(c_sorted, 0, v_sorted, na + nb, nc);
    a_free(a);
    a_free(b);
    a_free(c);
    a_free(a_sorted);
    a_free(c_sorted);
    return v_sorted;
}
*/
Array quicksort(Array v) {
    int n = a_length(v);
    if (n <= 1) return a_copy(v);
    int p = ia_get(v, n / 2);
    printf("p = %d, v = ", p); ia_println(v);
    Array l = ia_filter(v, less_than, p);
    Array e = ia_filter(v, equal, p);
    Array g = ia_filter(v, greater_than, p);
    Array l_sorted = quicksort(l);
    Array g_sorted = quicksort(g);
    Array le_sorted = a_concat(l_sorted, e);
    Array v_sorted = a_concat(le_sorted, g_sorted);
    a_free(l); a_free(e); a_free(g);
    a_free(l_sorted); a_free(g_sorted); a_free(le_sorted);
    return v_sorted;
}

void swap(int v[], int i, int j) {
    int a = v[i];
    v[i] = v[j];
    v[j] = a;
}

void quicksort2(int v[], int left, int right);

void quicksort2_test(void) {
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
    base_set_memory_check(true);
    quicksort_test();
    quicksort2_test();
    return 0;
}
