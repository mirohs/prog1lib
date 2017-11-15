/*
Compile: make search
Run: ./search
make search && ./search
*/

#include "base.h"
#include "array.h"
#include "int_array.h"

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

int main(void) {
    linear_search_test();
    binary_search_test();
    return 0;
}
