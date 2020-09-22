/*
Compile: make search
Run: ./search
make search && ./search
*/

#include "base.h"

IntOption linear_search(Array a, int x);

bool test_equal_io(int line, IntOption actual, IntOption expected) {
    base_test_equal_b(__FILE__, line, actual.none, expected.none);
    base_test_equal_i(__FILE__, line, actual.some, expected.some);
    return actual.none == expected.none && actual.some == expected.some;
}

void linear_search_test(void) {
    Array a = ia_of_string("3, 5, 6, -7, 11, 12, 14, 16, 17");
    test_equal_io(__LINE__, linear_search(a, 3), make_int_some(0));
    test_equal_io(__LINE__, linear_search(a, 17), make_int_some(8));
    test_equal_io(__LINE__, linear_search(a, 20), make_int_none());
    a_free(a);
}

IntOption linear_search(Array a, int x) {
    int n = a_length(a);
    for (int i = 0; i < n; i++) {
        if (ia_get(a, i) == x) {
            return make_int_some(i); // found match
        }
    }
    return make_int_none(); // no match
}

IntOption binary_search(Array a, int x);

void binary_search_test(void) {
    Array a = ia_of_string("3, 5, 6, 11, 12, 14, 16, 17");
    test_equal_io(__LINE__, binary_search(a, 3), make_int_some(0));
    test_equal_io(__LINE__, binary_search(a, 17), make_int_some(7));
    test_equal_io(__LINE__, binary_search(a, 20), make_int_none());
    a_free(a);
}

IntOption binary_search(Array a, int x) {
    int low = 0;
    int high = a_length(a) - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        int v = ia_get(a, mid);
        if (x == v) {
            return make_int_some(mid); // found match
        } else if (x < v) {
            high = mid - 1; // search low..mid-1
        } else if (x > v) {
            low = mid + 1; // search mid+1..high
        }
    }
    return make_int_none(); // no match
}

int binary_search2(int a[], int n, int x);

void binary_search_test2(void) {
    int a[] = { 3, 5, 6, 11, 12, 14, 16, 17 };
    int n = sizeof(a) / sizeof(int);
    printiln(n);
    test_equal_i(binary_search2(a, n, 3), 0);
    test_equal_i(binary_search2(a, n, 17), 7);
    test_equal_i(binary_search2(a, n, 20), -1);
    test_equal_i(binary_search2(a, 1, 2), -1);
    test_equal_i(binary_search2(a, 1, 3), 0);
    test_equal_i(binary_search2(a, 1, 4), -1);
    test_equal_i(binary_search2(a, 2, 2), -1);
    test_equal_i(binary_search2(a, 2, 3), 0);
    test_equal_i(binary_search2(a, 2, 4), -1);
    test_equal_i(binary_search2(a, 2, 5), 1);
    test_equal_i(binary_search2(a, 2, 6), -1);
    test_equal_i(binary_search2(a, 8, 12), 4);
}

#if 1
int binary_search2(int a[], int n, int x) {
    int low = 0;
    int high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        int v = a[mid];
        if (x == v) {
            return mid; // found match
        } else if (x < v) {
            high = mid - 1; // search low..mid-1
        } else if (x > v) {
            low = mid + 1; // search mid+1..high
        }
    }
    return -1; // no match
}
#endif
#if 0
int binary_search2(int a[], int n, int x) {
    int low = 0;
    int high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        int v = a[mid];
        if (x < v) {
            high = mid - 1; // search low..mid-1
        } else if (x > v) {
            low = mid + 1; // search mid+1..high
        } else {
            return mid; // found match
        }
    }
    return -1; // no match
}
#endif
#if 0
int binary_search2(int a[], int n, int x) {
    if (a == NULL || n <= 0) return -1;
    int low = 0; // inclusive
    int high = n; // exclusive, n >= 1
    int mid = 0;
    while (low < high) {
        mid = (low + high) / 2;
        if (x < a[mid]) {
            high = mid; // search low..mid-1
        } else {
            low = mid + 1; // search mid+1..high-1
        }
    }
    if (mid < n && a[mid] == x) return mid;
    return -1; // no match
}
#endif
#if 0
// Wirth: Programming in Oberon, 2014
// WRONG
int binary_search2(int a[], int n, int x) {
    if (a == NULL || n <= 0) return -1;
    int i = 0;
    int j = n;
    do {
        int mid = (i + j) / 2;
        if (x < a[mid]) {
            j = mid;
        } else {
            i = mid + 1;
        }
    } while (i < j);
    if (j < n && a[j] == x) return j;
    return -1; // no match
}
#endif

int main(void) {
    linear_search_test();
    binary_search_test();
    binary_search_test2();

    int n = 10000000;
    int* a = xmalloc(n * sizeof(int));
    for (int i = 0; i < n; i++) a[i] = i;

    clock_t t = clock();
	int sum = 0;
    for (int i = 0; i < n; i++) {
        int j = binary_search2(a, n, i);
        sum += j;
    }
    printiln(sum);
    t = clock() - t;
    printf("time: %g ms\n", t * 1000.0 / CLOCKS_PER_SEC);
    
    return 0;
}
