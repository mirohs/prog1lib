/*
Compile: make array_temperatures
Run: ./array_temperatures
make array_temperatures && ./array_temperatures
*/
 
#include "base.h"
#include "array.h"
#include "int_array.h"

/*
Find out whether the array contains any freezing temperatures. 
*/

// Array(int) -> bool
// Returns true if the array contains negative values,
// otherwise returns false.
bool contains_negative(Array array);

static void contains_negative_test(void) {
    Array array;
 
    array = ia_of_string("10, 20, 30");
    test_equal_b(contains_negative(array), false);
    a_free(array);
 
    array = ia_of_string("0, 0, 1, 1, 1");
    test_equal_b(contains_negative(array), false);
    a_free(array);
 
    array = ia_of_string("-1, -3");
    test_equal_b(contains_negative(array), true);
    a_free(array);
 
    array = ia_of_string("1, 3, -99");
    test_equal_b(contains_negative(array), true);
    a_free(array);
 
    array = ia_of_string("-1, 3, 99");
    test_equal_b(contains_negative(array), true);
    a_free(array);
 
    array = ia_of_string("");
    test_equal_b(contains_negative(array), false);
    a_free(array);
}

bool contains_negative(Array array) {
    // Iterate over the elements of the array (any order).
    // For each array element, check whether it is negative.
    // If so return true, otherwise continue
    // until all of the elements have been inspected.
    // If there is no negative value, answer false.
    int n = a_length(array);
    for (int i = 0; i < n; i++) {
        assert(i < n);
        assert(i >= 0);
        if (ia_get(array, i) < 0) {
            return true;
        }
    }
    return false;
}

/*
Subsample the data and provide every second element of the 
original array. It should start with the first element.
*/

// Array(int) -> Array(int)
// Return an array that contains every other element of
// the provided array, i.e., the result array contains
// the elements at the even indices of the provided array.
Array every_other(Array array);

static void every_other_test(void) {
    Array a, ac, ex;
    a = ia_of_string("1 2 3");
    ac = every_other(a);
    ex = ia_of_string("1 3");
    ia_test_equal(ac, ex);
    a_free(a);
    a_free(ac);
    a_free(ex);

    a = ia_of_string("");
    ac = every_other(a);
    ex = ia_of_string("");
    ia_test_equal(ac, ex);
    a_free(a);
    a_free(ac);
    a_free(ex);

    a = ia_of_string("-2");
    ac = every_other(a);
    ex = ia_of_string("-2");
    ia_test_equal(ac, ex);
    a_free(a);
    a_free(ac);
    a_free(ex);

    a = ia_of_string("1 2");
    ac = every_other(a);
    ex = ia_of_string("1");
    ia_test_equal(ac, ex);
    a_free(a);
    a_free(ac);
    a_free(ex);

    a = ia_of_string("1 2 3 4");
    ac = every_other(a);
    ex = ia_of_string("1 3");
    ia_test_equal(ac, ex);
    a_free(a);
    a_free(ac);
    a_free(ex);
}

// Return an array that contains every other element of
// the provided array, i.e., the result array contains
// the elements at the even indices of the provided array.
Array every_other(Array array) {
    int n = a_length(array);
    int m = (n + 1) / 2;
    Array result = ia_create(m, 0);
    // Iterate over the even indices in the original order.
    // Copy each element at an even index to the result array.
    for (int i = 0; i < n; i += 2) {
        assert(i < n);
        assert(i >= 0 && (i % 2) == 0);
        // printiln(i);
        ia_set(result, i / 2, ia_get(array, i));
    }
    // return ia_create(0, 0);
    return result;
}

int main(void) {
    // every_other_test();
    // return 0;
    
    Array temps = ia_of_string(
        "7, 6, 5, 6, 6, 5, 6, 6, 6, 7, 7, 9, 10,"
        "11, 11, 12, 13, 12, 10, 9, 9, 8, 7, 4");
    ia_println(temps);
    printiln(a_length(temps));
    printiln(ia_get(temps, 0));
    printiln(ia_get(temps, 23));
    
    printsln("contains_negative_test:");
    contains_negative_test();

    printsln("every_other_test:");
    every_other_test();
    
    a_free(temps);
    return 0;
}
