/*
Compile: make array_mean
Run: ./array_mean
make array_mean && ./array_mean
*/
 
#include "base.h"
#include "array.h"
#include "int_array.h"

/*
Compute a subarray of a given array.
*/

// Array, int, int -> Array
// Create a new array consisting of array[start_index, end_index).
// start_index is inclusive, end_index is exclusive.
Array ia_sub(Array array, int start_index, int end_index);

static void ia_sub_test(void) {
    Array array, array2, sub;
    array = ia_of_string("1 2 3 4");
 
    sub = ia_sub(array, 0, a_length(array));
    ia_test_equal(array, sub);
    a_free(sub);
 
    sub = ia_sub(array, -1, a_length(array) + 1);
    ia_test_equal(array, sub);
    a_free(sub);
 
    array2 = ia_of_string("2 3 4");
    sub = ia_sub(array, 1, a_length(array));
    ia_test_equal(array2, sub);
    a_free(array2);
    a_free(sub);
 
    array2 = ia_of_string("2 3");
    sub = ia_sub(array, 1, a_length(array) - 1);
    ia_test_equal(array2, sub);
    a_free(array2);
    a_free(sub);
 
    array2 = ia_of_string("");
    sub = ia_sub(array, 1, 1);
    ia_test_equal(array2, sub);
    a_free(sub);
 
    sub = ia_sub(array, 2, 1);
    ia_test_equal(array2, sub);
    a_free(array2);
    a_free(sub);
 
    a_free(array);
}

// Create a new array consisting of array[start_index, end_index).
// start_index is inclusive, end_index is exclusive.
Array ia_sub(Array array, int start_index, int end_index) {
    if (start_index >= end_index ||
        start_index >= a_length(array) ||
        end_index <= 0)
    {
        return ia_create(0, 0);
    }
    assert(start_index < end_index);
    assert(start_index < a_length(array));
    assert(end_index > 0);
    if (start_index < 0) start_index = 0;
    if (end_index > a_length(array)) end_index = a_length(array);
    int n = end_index - start_index;
    Array result = ia_create(n, 0);
    for (int i = start_index, j = 0; i < end_index; i++, j++) {
        assert(i < end_index);
        assert(i >= start_index);
        int value = ia_get(array, i); // read from input array
        ia_set(result, j, value); // write to output array
    }
    return result;
}

// Compute the sum of the elements in an array.
int ia_sum(Array array) {
    int sum = 0;
    for (int i = 0; i < a_length(array); i++) {
        sum = sum + ia_get(array, i);
    }
    return sum;
}

// Compute the average of the elements ina array.
int ia_average(Array array) {
    int n = a_length(array);
    if (n == 0) {
        printsln("Impossible. Bye.");
        exit(1);
    }
    int sum = ia_sum(array);
    return sum / n;
}



/*
Computing the average temperature of a selectable timespan over a day.
*/

// Array, int, int -> int
// Compute the average over the specified subarray.
// The start index is inclusive, the end index is exclusive.
int ia_sub_average(Array, int, int);

static void ia_sub_average_test(void) {
    Array array;
    array = ia_of_string("1 2 3 4");
    test_equal_i(ia_sub_average(array, 0, 1), 1);
    test_equal_i(ia_sub_average(array, 3, 4), 4);
    test_equal_i(ia_sub_average(array, 0, 4), 2);
    test_equal_i(ia_sub_average(array, 1, 4), 3);
    a_free(array);
}

// Compute the average over the specified subarray.
// The start index is inclusive, the end index is exclusive.
int ia_sub_average(Array array, int start_index, int end_index) {
    Array sub = ia_sub(array, start_index, end_index);
    int average = ia_average(sub);
    a_free(sub);
    return average;
}


// Compute the average over the specified subarray.
// The start index is inclusive, the end index is exclusive.
IntOption ia_sub_average2(Array array, int start_index, int end_index) {
	if (start_index >= end_index) {
		return make_int_none();
	}
    Array sub = ia_sub(array, start_index, end_index);
    int average = ia_average(sub);
    a_free(sub);
    return make_int_some(average);
}

int main(void) {
    printsln("ia_sub_test:");
    ia_sub_test();

    Array array = ia_of_string("2 2 3 1");
    printiln(ia_average(array));
    a_free(array);

    printsln("ia_sub_average_test:");
    ia_sub_average_test();

    Array temps = ia_of_string(
        "7, 6, 5, 6, 6, 5, 6, 6, 6, 7, 7, 9, 10,"
        "11, 11, 12, 13, 12, 10, 9, 9, 8, 7, 4");

    prints("Average morning temperature: ");
    printiln(ia_sub_average(temps, 6, 10));

    prints("Average forenoon temperature: ");
    printiln(ia_sub_average(temps, 10, 13));

    prints("Average afternoon temperature: ");
    printiln(ia_sub_average(temps, 13, 17));

    prints("Average evening temperature: ");
    printiln(ia_sub_average(temps, 17, 22));
	
	IntOption io = ia_sub_average2(temps, 17, 17);
	if (io.none) {
		printsln("none");
	} else {
		printiln(io.some);
	}
    
    a_free(temps);

    return 0;
}


