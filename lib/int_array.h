/** @file
An array of integers. 
It stores a fixed number of integers. The prefix @c ia_ stands for <i>integer array</i>. Some operations are inherited from array.h. For example, \ref a_length works with integer arrays and any other kind of array.

@author Michael Rohs
@date 15.10.2015
@copyright Apache License, Version 2.0
*/

#ifndef __INT_ARRAY_H__
#define __INT_ARRAY_H__

#include "base.h"
#include "double_array.h"

// tests serve as usage examples, so should remain in there, 
// but as static methods so they don't pollute the namespace

// use fixed-size arrays (length of an array never changes)
// closer to C array semantics, which will be introduced later

// do not use unsigned integers
// there are problems with unsigned (overflow and otherwise)

// do not use unsigned integers (generally, only use for arrays of raw bytes)
/*
Problems with unsigned ints: Scott Myers, Signed and Unsigned Types in Interfaces, 1995
Bjarne Stroustrup in "The C++ Programming Language":
"The unsigned integer types are ideal for uses that treat storage as a bit array. Using an unsigned instead of an int to gain one more bit to represent positive integers is almost never a good idea. Attempts to ensure that some values are positive by declaring variables unsigned will typically be defeated by the implicit conversion rules."
http://www.soundsoftware.ac.uk/c-pitfall-unsigned
*/
///////////////////////////////////////////////////////////////////////////////



/**
Creates an array of n integers, all initialized to value. 
@param[in] n number of elements
@param[in] value initialization value
@return the new array
@pre "non-negative length", n >= 0
*/
Array ia_create(int n, int value);

/**
Creates an array and sets the elements to the interval [a,b) or (b,a], respectively. 
Index a is inclusive and index b is exclusive. 
- If a < b, then the result is an increasing range.
- If a > b, then the result is a decreasing range.
- If a == b, then the resulting array is empty.
@param[in] a first value of range (inclusive)
@param[in] b last value of range (exclusive)
@return the initialized array
*/
Array ia_range(int a, int b);

/**
Creates an array from the given string.
Use ',' or ' ' as the separator.
Example: ia_of_string("1, 3, -4") creates integer array [1, 3, -4].
@param[in] s string representation of int array
@return the initialized array
*/
Array ia_of_string(String s);

/** 
Creates an array of n ints, each initialized with function init. 
@code{.c}
int init(int index, int x) {}
@endcode

@param[in] n    number of elements
@param[in] init    initialization function, will be called for each index [0, n-1)
@param[in] x    will be supplied to init
@pre "non-negative length", n >= 0
@pre "not null", init

Example:
@code{.c}
static int two_i_plus_x(int index, int x) {
    return 2 * index + x;
}
...
Array a = ia_fn(3, two_i_plus_x, 10);
// set element 0 to init(0, 10) = 2 * 0 + 10 = 10
// set element 1 to init(1, 10) = 2 * 1 + 10 = 12
// set element 2 to init(2, 10) = 2 * 2 + 10 = 14
// now a is [10, 12, 14]
@endcode
*/
Array ia_fn(int n, IntIntToInt init, int x);

/**
Converts the array of doubles to an array of ints.
Each double of the input array is rounded to the nearest integer.
The input array is not modified.
@param[in] array array of doubles
@return the converted array of integers
*/
Array ia_of_da(Array array);

/**
Returns array element at index.
@param[in] array int array
@param[in] index index of array element to return
@return array element
@pre "index in range", index >= 0 && index < length
*/
#ifdef NO_GET_SET
#define ia_get(array, i) ((int*)((array)->a))[i]
#else
int ia_get(Array array, int index);
#endif

/**
Sets array element at index to value.
@param[in,out] array int array
@param[in] index index of array element to set
@param[in] value value to set
@pre "index in range", index >= 0 && index < length
*/
#ifdef NO_GET_SET
#define ia_set(array, index, value) ((int*)((array)->a))[index] = value;
#else
void ia_set(Array array, int index, int value);
#endif

/**
Increments array element at index by value. Avoids common pattern: set(a, i, get(a, i) + v)
@param[in,out] array int array
@param[in] index index of array element to increment
@param[in] value value to increment
@return the incremented value
@pre "index in range", index >= 0 && index < length
*/
#ifdef NO_GET_SET
#define ia_inc(array, index, value) ia_set(array, index, ia_get(array, index) + (value));
#else
int ia_inc(Array array, int index, int value);
#endif

/**
Prints the array.
@param[in] array int array
*/
void ia_print(Array array);

/**
Prints the array, then prints a line break.
@param[in] array int array
*/
void ia_println(Array array);

/**
Returns true iff array contains value.
@param[in] array int array
@param[in] value value to look for
@return true if array contains value, false otherwise
*/
bool ia_contains(Array array, int value);

/**
Sets each element of array to value.
@param[in,out] array int array
@param[in] value value to set
*/
void ia_fill(Array array, int value);

/**
Sets a range of elements of array to value.
Index from is inclusive, index to is exclusive.
@param[in,out] array int array
@param[in] value value to set
@param[in] from start index (inclusive)
@param[in] to end index (exclusive)
*/
void ia_fill_from_to(Array array, int value, int from, int to);

/**
Returns index of first occurrence of value in array. 
Returns -1 if value is not in array.
@param[in] array int array
@param[in] value value to look for
@return index or -1
*/
int ia_index(Array array, int value);

/**
Returns index of first occurrence of value in array at indices [from, n). 
Returns -1 if value is not in array[from, n).
Index from is inclusive.
@param[in] array int array
@param[in] value value to look for
@param[in] from start index (inclusive)
@return index or -1
*/
int ia_index_from(Array array, int value, int from);

/**
Returns index of first element for which the predicate function returns true.
Returns -1 if predicate does not return true for any element.
@code{.c}
bool predicate(int element, int index, int x) {}
@endcode
@param[in] array int array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return index or -1
@pre "not null", predicate
*/
int ia_index_fn(Array array, IntIntIntToBool predicate, int x);

/**
Returns index of last occurrence of value in array. 
Returns -1 if value is not in array.
@param[in] array int array
@param[in] value value to look for
@return index or -1
*/
int ia_last_index(Array array, int value);

/**
Returns index of last occurrence of value in array at or before index from.
Returns -1 if value is not in array.
@param[in] array int array
@param[in] value value to look for
@param[in] from starting index (inclusive)
@return index or -1
*/
int ia_last_index_from(Array array, int value, int from);

/**
Returns index of last element for which the predicate function returns true.
Returns -1 if predicate does not return true for any element.
@code{.c}
bool predicate(int element, int index, int x) {}
@endcode
@param[in] array int array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return index or -1
@pre "not null", predicate
*/
int ia_last_index_fn(Array array, IntIntIntToBool predicate, int x);

/**
Sorts the elements in increasing order. The input array is modified.
@param[in,out] array int array
*/
void ia_sort(Array array);

/**
Sorts the elements in decreasing order.The input array is modified.
@param[in,out] array int array
*/
void ia_sort_dec(Array array);

#if 0
/**
Inserts value at index in array. 
Shift up everything above (and including) index. Old element at index (n-1) falls off.
Does nothing if index is not valid, i.e., if not in interval [0,n).
@param[in,out] array int array
@param[in] index the position to insert at (index 0 means inserting at the front)
@param[in] value value to insert
*/
void ia_insert(Array array, int index, int value);

/**
Removes element at index in array.
Shift down everything above (and including) i. Set v at index n-1.
Does nothing if index is not valid, i.e., if not in interval [0,n).
@param[in,out] array int array
@param[in] index index of element to remove
@param[in] value value to set at index n-1
*/
void ia_remove(Array array, int index, int value);
#endif

/**
Applies function f to each element of array. The original array is modified (if f modifies the element).
Function f is called once for each element and returns the transformed element.
@code{.c}
int f(int element, int index, int x) {}
@endcode

@param[in,out] array int array
@param[in] f a function that is called for each element of input array
@param[in] x provided to each invocation of f
@pre "not null", f

<b>Step by step:</b><br/>
array[0] := f(array[0], 0, x)<br/>
array[1] := f(array[1], 1, x)<br/>
...<br/>
array[n-1] := f(array[n-1], n-1, x)
*/
void ia_each(Array array, IntIntIntToInt f, int x); 

/**
Applies function f to each element of array. The original array is modified (if f modifies the element).
Function f is called once for each element and returns the transformed element.
@code{.c}
int f(int element, int index, int x, Any state) {}
@endcode

@param[in,out] array int array
@param[in] f a function that is called for each element of input array
@param[in] x provided to each invocation of f
@param[in] state provided to each invocation of f
@pre "not null", f

<b>Step by step:</b><br/>
array[0] := f(array[0], 0, x, state)<br/>
array[1] := f(array[1], 1, x, state)<br/>
...<br/>
array[n-1] := f(array[n-1], n-1, x, state)
*/
void ia_each_state(Array array, IntIntIntAnyToInt f, int x, Any state);

/**
Applies function f to each element of array. 
The original array is not modified. A new array is created for the result.
Function f is called once for each element and returns the transformed element.
@code{.c}
int f(int element, int index, int x) {}
@endcode

@param[in] array int array
@param[in] f transformation function, called for each element of input array
@param[in] x provided to each invocation of f
@return the mapped array
@pre "not null", f
*/
Array ia_map(Array array, IntIntIntToInt f, int x);

/**
Applies function f to each element of array. 
The original array is not modified. A new array is created for the result.
Function f is called once for each element and returns the transformed element.
@code{.c}
int f(int element, int index, int x, Any state) {}
@endcode

@param[in] array int array
@param[in] f transformation function, called for each element of input array
@param[in] x provided to each invocation of f
@param[in] state provided to each invocation of f
@return the mapped array
@pre "not null", f
*/
Array ia_map_state(Array array, IntIntIntAnyToInt f, int x, Any state);

/**
Folds array from left to right, i.e., compute f(... f(f(init, a0), a1) ... an).
@code{.c}
int f(int state, int element, int index) {}
@endcode

@param[in] array int array
@param[in] f a function that is called for each element of input array
@param[in] state provided to each invocation of f
@return the accumulated state
@pre "not null", f

<b>Step by step:</b><br/>
state := f(state, array[0], 0)<br/>
state := f(state, array[1], 1)<br/>
... <br/>
state := f(state, array[n-1], n-1)
*/
int ia_foldl(Array array, IntIntIntToInt f, int state);

/**
Folds array from right to left. I.e., compute f(a0, f(a1,... f(an, init)...)).
@code{.c}
int f(int element, int state, int index) {}
@endcode

@param[in] array int array
@param[in] f a function that is called for each element of input array
@param[in] state provided to each invocation of f
@return the accumulated state
@pre "not null", f

<b>Step by step:</b><br/>
state := f(array[n-1], state, n-1)<br/>
... <br/>
state := f(array[1], state, 1)<br/>
state := f(array[0], state, 0)
*/
int ia_foldr(Array array, IntIntIntToInt f, int state);

/**
Predicates
*/
bool ia_even(int value, int index, int x);
bool ia_odd(int value, int index, int x);
bool ia_gt(int value, int index, int x);
bool ia_ge(int value, int index, int x);
bool ia_lt(int value, int index, int x);
bool ia_le(int value, int index, int x);

/**
Functions for initialization, map, and fold
*/
int ia_square(int value, int index);
int ia_rnd(int index, int maximum);
int ia_times(int value, int index, int x);
int int_plus(int x, int y, int index);
int int_minus(int x, int y, int index);
int int_mult(int x, int y, int index);
int int_div(int x, int y, int index);

/**
Creates a new array with only those elements of array that satisfy the predicate.
The original array is not modified.
@code{.c}
bool predicate(int element, int index, int x) {}
@endcode

@param[in] array int array
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@return filtered array
@pre "not null", predicate
*/
Array ia_filter(Array array, IntIntIntToBool predicate, int x);

/**
Creates a new array with only those elements of array that satisfy the predicate.
The original array is not modified.
@code{.c}
bool predicate(int element, int index, int x, Any state) {}
@endcode

@param[in] array int array
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return filtered array
@pre "not null", predicate
*/
Array ia_filter_state(Array array, IntIntIntAnyToBool predicate, int x, Any state);

/**
Filters and maps array using f. The original array is not modified.
@code{.c}
IntOption f(int element, int index, int x) {}
@endcode

@param[in] array int array
@param[in] f mapping function, returns the mapped element or @c none if the element should not be included in the result
@param[in] x given to each invocation of predicate
@return filtered and mapped array
@pre "not null", f

Example:
@code{.c}
IntOption evens_times_x(int element, int index, int x) {
    if ((element % 2) == 0) { // if element is even
        return make_int_some(x * element); // return int option with mapped element
    }
    return make_int_none(); // exclude this element
}
...
Array a = ia_of_string("1, 2, 3, 4, 5, 6");
Array b = ia_choose(a, evens_times_x, 3);
// b is [6, 12, 18]
@endcode
*/
Array ia_choose(Array array, IntIntIntToIntOption f, int x);

/**
Filters and maps array using f. The original array is not modified.
@code{.c}
IntOption f(int element, int index, int x, Any state) {}
@endcode

@param[in] array int array
@param[in] f mapping function, returns the mapped element or @c none if the element should not be included in the result
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return filtered and mapped array
@pre "not null", f
*/
Array ia_choose_state(Array array, IntIntIntAnyToIntOption f, int x, Any state);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(int element, int index, int x) {}
@endcode

@param[in] array int array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at least one element satisfies predicate
@pre "not null", predicate
*/
bool ia_exists(Array array, IntIntIntToBool predicate, int x);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(int element, int index, int x, Any state) {}
@endcode

@param[in] array int array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return true iff at least one element satisfies predicate
@pre "not null", predicate
*/
bool ia_exists_state(Array array, IntIntIntAnyToBool predicate, int x, Any state);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(int element, int index, int x) {}
@endcode

@param[in] array int array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at all the elements satisfy predicate
@pre "not null", predicate
*/    
bool ia_forall(Array array, IntIntIntToBool predicate, int x);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(int element, int index, int x, Any state) {}
@endcode

@param[in] array int array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return true iff at all the elements satisfy predicate
@pre "not null", predicate
*/
bool ia_forall_state(Array array, IntIntIntAnyToBool predicate, int x, Any state);

/*
Tests for int arrays.
@param[in] ac actual result array
@param[in] ex expected result array
@returns true iff actual equals expected array
*/
#define ia_test_equal(ac, ex) \
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, ac, (ex)->a, (ex)->n)

/**
Tests for int arrays.
@param[in] file source file name
@param[in] function function name
@param[in] line line number
@param[in] ac actual result array
@param[in] ex expected result C-array
@param[in] exn length of expected result C-array (number of elements)
@returns true iff actual equals expected array
*/
bool ia_test_equal_file_line(const char *file, const char *function, int line, Array ac, int *ex, int exn);

/*
Checks if array has the right element size. Fails if not.
String s is debug output (e.g., function name).
*/
#undef require_element_size_int
#ifdef NO_CHECK_ELEMENT_SIZE
#define require_element_size_int(array)
#else
#define require_element_size_int(array) \
    require_x("element size int", (array)->s == sizeof(int), "size == %d", (array)->s)
#endif

void ia_test_all(void);
void da_test_all(void);

#endif
