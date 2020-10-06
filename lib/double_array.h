/** @file
An array of doubles. 
It stores a fixed number of doubles. The prefix @c da_ stands for <i>double array</i>. Some operations are inherited from array.h. For example, \ref a_length works with double arrays and any other kind of array.

@author Michael Rohs
@date 15.10.2015, 6.10.2020
@copyright Apache License, Version 2.0
*/

#ifndef __DOUBLE_ARRAY_H__
#define __DOUBLE_ARRAY_H__

#include "base.h"
#include "int_array.h"

/**
Creates an array of n doubles, all initialized to value. 
@param[in] n number of elements
@param[in] value initialization value
@return the new array
@pre "non-negative length", n >= 0
*/
Array da_create(int n, double value);

/**
Creates an array and sets the elements to the interval [a,b) or (b,a], respectively. 
Index a is inclusive and index b is exclusive. 
- If a < b, then the result is an increasing range.
- If a > b, then the result is a decreasing range.
- If a == b, then the resulting array is empty.
@param[in] a first value of range (inclusive)
@param[in] b last value of range (exclusive)
@param[in] step step size
@return the initialized array
@pre "step not too small", fabs(step) > 0.000001
*/
Array da_range(double a, double b, double step);

/**
Creates an array from the given string.
Use ',' or ' ' as the separator.
Example: ia_of_string("1, 3, -4") creates integer array [1, 3, -4].
@param[in] s string representation of double array
@return the initialized array
*/
Array da_of_string(String s);

/** 
Creates an array of n ints, each initialized with function init. 
@code{.c}
double init(int index, double x) {}
@endcode

@param[in] n number of elements
@param[in] init initialization function, will be called for each index [0, n-1)
@param[in] x will be supplied to init
@pre "non-negative length", n >= 0
@pre "not null", init

Example:
@code{.c}
static int two_i_plus_x(int index, double x) {
    return 2.0 * index + x;
}
...
Array a = ia_fn(3, two_i_plus_x, 10.0);
// set element 0 to init(0, 10.0) = 2.0 * 0 + 10.0 = 10.0
// set element 1 to init(1, 10.0) = 2.0 * 1 + 10.0 = 12.0
// set element 2 to init(2, 10.0) = 2.0 * 2 + 10.0 = 14.0
// now a is [10, 12, 14]
@endcode
*/
Array da_fn(int n, IntDoubleToDouble init, double x);

/**
Converts the array of ints to an array of doubles.
The input array is not modified.
@param[in] array array of integers
@return the converted array of doubles
*/
Array da_of_ia(Array array);

/**
Returns array element at index.
@param[in] array double array
@param[in] index index of array element to return
@return array element
@pre "index in range", i >= 0 && i < length
*/
#ifdef NO_GET_SET
#define da_get(array, index) ((double*)array.a)[index]
#else
double da_get(Array array, int index);
#endif

/**
Sets array element at index to value.
@param[in,out] array double array
@param[in] index index of array element to set
@param[in] value value to set
@pre "index in range", i >= 0 && i < length
*/
#ifdef NO_GET_SET
#define da_set(array, index, value) ((double*)array->a)[index] = value;
#else
void da_set(Array array, int index, double value);
#endif

/**
Increments array element at index by value. Avoids common pattern: set(a, i, get(a, i) + v)
@param[in,out] array double array
@param[in] index index of array element to increment
@param[in] value value to increment
@pre "index in range", i >= 0 && i < length
*/
#ifdef NO_GET_SET
#define da_inc(array, index, value) ia_set(array, index, ia_get(array, index) + (value));
#else
void da_inc(Array array, int index, double value);
#endif

/**
Prints the array.
@param[in] array double array
*/
void da_print(Array array);

/**
Prints the array, then prints a line break.
@param[in] array double array
*/
void da_println(Array array);

/**
Returns true iff array contains value.
@param[in] array double array
@param[in] value value to look for
@param[in] epsilon (small positive value) allowed tolerance around value
@return true if array contains value, false otherwise
@pre "positive", epsilon > 0
*/
bool da_contains(Array array, double value, double epsilon);

/**
Sets each element of array to value.
@param[in,out] array double array
@param[in] value value to set
*/
void da_fill(Array array, double value);

/**
Sets a range of elements of array to value.
Index from is inclusive, index to is exclusive.
@param[in,out] array double array
@param[in] value value to set
@param[in] from start index (inclusive)
@param[in] to end index (exclusive)
*/
void da_fill_from_to(Array array, double value, int from, int to);

/**
Returns index of first occurrence of value in array. 
Returns -1 if value is not in array.
@param[in] array double array
@param[in] value value to look for
@param[in] epsilon (small positive value) allowed tolerance around value
@return index or -1
@pre "positive", epsilon > 0
*/
int da_index(Array array, double value, double epsilon);

/**
Returns index of first occurrence of value in array at indices [from, n). 
Returns -1 if value is not in array[from, n).
Index from is inclusive.
@param[in] array double array
@param[in] value value to look for
@param[in] from start index (inclusive)
@param[in] epsilon (small positive value) allowed tolerance around value
@return index or -1
@pre "positive", epsilon > 0
*/
int da_index_from(Array array, double value, int from, double epsilon);

/**
Returns index of first element for which the predicate function returns true.
Returns -1 if predicate does not return true for any element.
@code{.c}
bool predicate(double element, int index, double x) {}
@endcode
@param[in] array double array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return index or -1
*/
int da_index_fn(Array array, DoubleIntDoubleToBool predicate, double x);

/**
Returns index of last occurrence of value in array. 
Returns -1 if value is not in array.
@param[in] array double array
@param[in] value value to look for
@param[in] epsilon (small positive value) allowed tolerance around value
@return index or -1
@pre "positive", epsilon > 0
*/
int da_last_index(Array array, double value, double epsilon);

DoubleOption da_index_option(Array array, double value, double epsilon);

/**
Returns index of last occurrence of value in array at or before index from.
Returns -1 if value is not in array.
@param[in] array double array
@param[in] value value to look for
@param[in] from starting index (inclusive)
@param[in] epsilon (small positive value) allowed tolerance around value
@return index or -1
@pre "positive", epsilon > 0
*/
int da_last_index_from(Array array, double value, int from, double epsilon);

/**
Returns index of last element for which the predicate function returns true.
Returns -1 if predicate does not return true for any element.
@code{.c}
bool predicate(double element, int index, double x) {}
@endcode
@param[in] array double array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return index or -1
*/
int da_last_index_fn(Array array, DoubleIntDoubleToBool predicate, double x);

/**
Sorts the elements in increasing order. The input array is modified.
@param[in,out] array double array
*/
void da_sort(Array array);

/**
Sorts the elements in decreasing order. The input array is modified.
@param[in,out] array double array
*/
void da_sort_dec(Array array);

#if 0
/**
Inserts value at index in array. 
Shift up everything above (and including) index. Old element at index (n-1) falls off.
Does nothing if index is not valid, i.e., if not in interval [0,n).
@param[in,out] array double array
@param[in] index the position to insert at (index 0 means inserting at the front)
@param[in] value value to insert
*/
void da_insert(Array array, int index, double value);

/**
Removes element at index in array.
Shift down everything above (and including) i. Set v at index n-1.
Does nothing if index is not valid, i.e., if not in interval [0,n).
@param[in,out] array double array
@param[in] index index of element to remove
@param[in] value value to set at index n-1
*/
void da_remove(Array array, int index, double value);
#endif

/**
Applies function f to each element of array. The original array is modified (if f modifies the element).
Function f is called once for each element and returns the transformed element.
@code{.c}
double f(double element, int index, double x) {}
@endcode

@param[in,out] array double array
@param[in] f a function that is called for each element of input array
@param[in] x provided to each invocation of f

<b>Step by step:</b><br/>
array[0] := f(array[0], 0, x)<br/>
array[1] := f(array[1], 1, x)<br/>
...<br/>
array[n-1] := f(array[n-1], n-1, x)
*/
void da_each(Array array, DoubleIntDoubleToDouble f, double x);

/**
Applies function f to each element of array. The original array is modified (if f modifies the element).
Function f is called once for each element and returns the transformed element.
@code{.c}
double f(double element, int index, double x, Any state) {}
@endcode

@param[in,out] array double array
@param[in] f a function that is called for each element of input array
@param[in] x provided to each invocation of f
@param[in] state provided to each invocation of f

<b>Step by step:</b><br/>
array[0] := f(array[0], 0, x, state)<br/>
array[1] := f(array[1], 1, x, state)<br/>
...<br/>
array[n-1] := f(array[n-1], n-1, x, state)
*/
void da_each_state(Array array, DoubleIntDoubleAnyToDouble f, double x, Any state);

/**
Applies function f to each element of array. 
The original array is not modified. A new array is created for the result.
Function f is called once for each element and returns the transformed element.
@code{.c}
double f(double element, int index, double x) {}
@endcode

@param[in] array double array
@param[in] f transformation function, called for each element of input array
@param[in] x provided to each invocation of f
@return the mapped array
*/
Array da_map(Array array, DoubleIntDoubleToDouble f, double x);

/**
Applies function f to each element of array. 
The original array is not modified. A new array is created for the result.
Function f is called once for each element and returns the transformed element.
@code{.c}
double f(int element, int index, double x, Any state) {}
@endcode

@param[in] array double array
@param[in] f transformation function, called for each element of input array
@param[in] x provided to each invocation of f
@param[in] state provided to each invocation of f
@return the mapped array
*/
Array da_map_state(Array array, DoubleIntDoubleAnyToDouble f, double x, Any state);

/**
Folds array from left to right, i.e., computes f(... f(f(init, a0), a1) ... an).
@code{.c}
double f(double state, double element, int index) {}
@endcode

@param[in] array double array
@param[in] f a function that is called for each element of input array
@param[in] state provided to each invocation of f
@return the accumulated state

<b>Step by step:</b><br/>
state := f(state, array[0], 0)<br/>
state := f(state, array[1], 1)<br/>
... <br/>
state := f(state, array[n-1], n-1)
*/

double da_foldl(Array array, DoubleDoubleIntToDouble f, double state);

/**
Folds array from right to left. I.e., computes f(l0, f(l1,... f(ln, init)...)).
@code{.c}
int f(int element, int state, int index) {}
@endcode

@param[in] array double array
@param[in] f a function that is called for each element of input array
@param[in] state provided to each invocation of f
@return the accumulated state

<b>Step by step:</b><br/>
state := f(array[n-1], state, n-1)<br/>
... <br/>
state := f(array[1], state, 1)<br/>
state := f(array[0], state, 0)
*/
double da_foldr(Array array, DoubleDoubleIntToDouble f, double state);

/**
Predicates
*/
bool da_index_even(double value, int index);
bool da_index_odd(double value, int index);
bool da_gt(double value, int index, double x);
bool da_ge(double value, int index, double x);
bool da_lt(double value, int index, double x);
bool da_le(double value, int index, double x);

/**
Functions for initialization, map, and fold
*/
double da_square(double value, int index);
double da_rnd(int index, double maximum);
double da_times(double value, int index, double x);
double double_plus(double x, double y, int index);
double double_minus(double x, double y, int index);
double double_mult(double x, double y, int index);
double double_div(double x, double y, int index);

/**
Creates a new array with only those elements of array that satisfy the predicate.
The original array is not modified.
@code{.c}
bool predicate(double element, int index, double x) {}
@endcode

@param[in] array double array
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@return filtered array
*/
Array da_filter(Array array, DoubleIntDoubleToBool predicate, double x);

/**
Creates a new array with only those elements of array that satisfy the predicate.
The original array is not modified.
@code{.c}
bool predicate(double element, int index, double x, Any state) {}
@endcode

@param[in] array double array
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return filtered array
*/
Array da_filter_state(Array array, DoubleIntDoubleAnyToBool predicate, double x, Any state);

// @todo: add choose

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(double element, int index, double x) {}
@endcode

@param[in] array double array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at least one element satisfies predicate
*/
bool da_exists(Array array, DoubleIntDoubleToBool predicate, double x);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(double element, int index, double x, Any state) {}
@endcode

@param[in] array double array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return true iff at least one element satisfies predicate
*/
bool da_exists_state(Array array, DoubleIntDoubleAnyToBool predicate, double x, Any state);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(double element, int index, double x) {}
@endcode

@param[in] array double array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at all the elements satisfy predicate
*/    
bool da_forall(Array array, DoubleIntDoubleToBool predicate, double x);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(double element, int index, double x, Any state) {}
@endcode

@param[in] array double array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return true iff at all the elements satisfy predicate
*/
bool da_forall_state(Array array, DoubleIntDoubleAnyToBool predicate, double x, Any state);

/*
Tests involving double arrays.
@param[in] ac actual result array
@param[in] ex expected result array
@returns true iff actual equals expected array
*/
#define da_test_within(ac, ex) \
    da_test_within_file_line(__FILE__, __func__, __LINE__, ac, (ex)->a, (ex)->n, EPSILON)

#define da_test_within2(ac, ex, exn) \
    da_test_within_file_line(__FILE__, __func__, __LINE__, ac, ex, exn, EPSILON)

/**
Tests involving double arrays.
@param[in] file source file name
@param[in] function function name
@param[in] line line number
@param[in] ac actual result array
@param[in] ex expected result C-array
@param[in] exn length of expected result C-array (number of doubles)
@param[in] epsilon (small positive value) allowed tolerance
@returns true iff actual equals expected array
@pre "positive", epsilon > 0
*/
bool da_test_within_file_line(const char *file, const char *function, int line, Array ac, double *ex, int exn, double epsilon);

/**
Checks if array has the right element size. Fails if not.
@param[in] array array to check
*/
#undef require_element_size_double
#ifdef NO_CHECK_ELEMENT_SIZE
#define require_element_size_double(array)
#else
#define require_element_size_double(array) \
    require_x("element size double", (array)->s == sizeof(double), "size == %d", (array)->s)
#endif

void da_test_all(void);

#endif
