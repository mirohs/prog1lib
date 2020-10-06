/** @file
An array of bytes. 
It stores a fixed number of bytes. The prefix @c ba_ stands for <i>byte array</i>. Some operations are inherited from array.h. For example, \ref a_length works with byte arrays and any other kind of array.

@author Michael Rohs
@date 15.10.2015, 6.10.2020
@copyright Apache License, Version 2.0
*/

#ifndef __BYTE_ARRAY_H__
#define __BYTE_ARRAY_H__

#include "base.h"
#include "double_array.h"

// tests serve as usage examples, so should remain in there, 
// but as static methods so they don't pollute the namespace

// use fixed-size arrays (length of an array never changes)
// closer to C array semantics, which will be introduced later

// do not use unsigned bytes
// there are problems with unsigned (overflow and otherwise)
/*
Problems with unsigned ints: Scott Myers, Signed and Unsigned Types in Interfaces, 1995
Bjarne Stroustrup in "The C++ Programming Language":
"The unsigned byte types are ideal for uses that treat storage as a bit array. Using an unsigned instead of an int to gain one more bit to represent positive bytes is almost never a good idea. Attempts to ensure that some values are positive by declaring variables unsigned will typically be defeated by the implicit conversion rules."
http://www.soundsoftware.ac.uk/c-pitfall-unsigned
*/
///////////////////////////////////////////////////////////////////////////////



/**
Creates an array of n bytes, all initialized to value. 
@param[in] n number of elements
@param[in] value initialization value
@return the new array
@pre "non-negative length", n >= 0
*/
Array ba_create(int n, Byte value);

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
Array ba_range(Byte a, Byte b);

/**
Creates an array from the given string.
Use ',' or ' ' as the separator.
Example: ba_of_string("1, 3, 4") creates byte array [1, 3, 4].
@param[in] s string representation of byte array
@return the initialized array
*/
Array ba_of_string(String s);

/** 
Creates an array of n bytes, each initialized with function init. 
@code{.c}
Byte init(int index, Byte x) {}
@endcode

@param[in] n    number of elements
@param[in] init    initialization function, will be called for each index [0, n-1)
@param[in] x    will be supplied to init
@pre "non-negative length", n >= 0
@pre "not null", init

Example:
@code{.c}
static Byte two_i_plus_x(int index, Byte x) {
    return 2 * index + x;
}
...
Array a = ba_fn(3, two_i_plus_x, 10);
// set element 0 to init(0, 10) = 2 * 0 + 10 = 10
// set element 1 to init(1, 10) = 2 * 1 + 10 = 12
// set element 2 to init(2, 10) = 2 * 2 + 10 = 14
// now a is [10, 12, 14]
@endcode
*/
Array ba_fn(int n, IntByteToByte init, Byte x);

/**
Returns array element at index.
@param[in] array Byte array
@param[in] index index of array element to return
@return array element
@pre "index in range", index >= 0 && index < length
*/
#ifdef NO_GET_SET
#define ba_get(array, index) ((Byte*)((array)->a))[index]
#else
Byte ba_get(Array array, int index);
#endif

/**
Sets array element at index to value.
@param[in,out] array Byte array
@param[in] index index of array element to set
@param[in] value value to set
@pre "index in range", index >= 0 && index < length
*/
#ifdef NO_GET_SET
#define ba_set(array, index, value) ((Byte*)((array)->a))[index] = value;
#else
void ba_set(Array array, int index, Byte value);
#endif

/**
Prints the array.
@param[in] array Byte array
*/
void ba_print(Array array);

/**
Prints the array, then print a line break.
@param[in] array Byte array
*/
void ba_println(Array array);

/**
Returns true iff array contains value.
@param[in] array Byte array
@param[in] value value to look for
@return true if array contains value, false otherwise
*/
bool ba_contains(Array array, Byte value);

/**
Sets each element of array to value.
@param[in,out] array Byte array
@param[in] value value to set
*/
void ba_fill(Array array, Byte value);

/**
Sets a range of elements of array to value.
Index from is inclusive, index to is exclusive.
@param[in,out] array Byte array
@param[in] value value to set
@param[in] from start index (inclusive)
@param[in] to end index (exclusive)
*/
void ba_fill_from_to(Array array, Byte value, int from, int to);

/**
Returns index of first occurrence of value in array. 
Returns -1 if value is not in array.
@param[in] array Byte array
@param[in] value value to look for
@return index or -1
*/
int ba_index(Array array, Byte value);

/**
Returns index of first occurrence of value in array at indices [from, n). 
Returns -1 if value is not in array[from, n).
Index from is inclusive.
@param[in] array Byte array
@param[in] value value to look for
@param[in] from start index (inclusive)
@return index or -1
*/
int ba_index_from(Array array, Byte value, int from);

/**
Returns index of first element for which the predicate function returns true.
Returns -1 if predicate does not return true for any element.
@code{.c}
bool predicate(Byte element, int index, Byte x) {}
@endcode
@param[in] array Byte array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return index or -1
@pre "not null", predicate
*/
int ba_index_fn(Array array, ByteIntByteToBool predicate, Byte x);

/**
Returns index of last occurrence of value in array. 
Returns -1 if value is not in array.
@param[in] array Byte array
@param[in] value value to look for
@return index or -1
*/
int ba_last_index(Array array, Byte value);

/**
Returns index of last occurrence of value in array at or before index from.
Returns -1 if value is not in array.
@param[in] array Byte array
@param[in] value value to look for
@param[in] from starting index (inclusive)
@return index or -1
*/
int ba_last_index_from(Array array, Byte value, int from);

/**
Returns index of last element for which the predicate function returns true.
Returns -1 if predicate does not return true for any element.
@code{.c}
bool predicate(Byte element, int index, Byte x) {}
@endcode
@param[in] array Byte array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return index or -1
@pre "not null", predicate
*/
int ba_last_index_fn(Array array, ByteIntByteToBool predicate, Byte x);

/**
Sorts the elements in increasing order. The input array is modified.
@param[in,out] array Byte array
*/
void ba_sort(Array array);

/**
Sorts the elements in decreasing order.The input array is modified.
@param[in,out] array Byte array
*/
void ba_sort_dec(Array array);

/**
Inserts value at index in array. 
Shift up everything above (and including) index. Old element at index (n-1) falls off.
Does nothing if index is not valid, i.e., if not in interval [0,n).
@param[in,out] array Byte array
@param[in] index the position to insert at (index 0 means inserting at the front)
@param[in] value value to insert
*/
void ba_insert(Array array, int index, Byte value);

/**
Removes element at index in array.
Shift down everything above (and including) i. Set v at index n-1.
Does nothing if index is not valid, i.e., if not in interval [0,n).
@param[in,out] array Byte array
@param[in] index index of element to remove
@param[in] value value to set at index n-1
*/
void ba_remove(Array array, int index, Byte value);

/**
Applies function f to each element of array. The original array is modified (if f modifies the element).
Function f is called once for each element and returns the transformed element.
@code{.c}
Byte f(Byte element, int index, Byte x) {}
@endcode

@param[in,out] array Byte array
@param[in] f a function that is called for each element of input array
@param[in] x provided to each invocation of f
@pre "not null", f

<b>Step by step:</b><br/>
array[0] := f(array[0], 0, x)<br/>
array[1] := f(array[1], 1, x)<br/>
...<br/>
array[n-1] := f(array[n-1], n-1, x)
*/
void ba_each(Array array, ByteIntByteToByte f, Byte x); 

/**
Applies function f to each element of array. The original array is modified (if f modifies the element).
Function f is called once for each element and returns the transformed element.
@code{.c}
Byte f(Byte element, int index, Byte x, Any state) {}
@endcode

@param[in,out] array Byte array
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
void ba_each_state(Array array, ByteIntByteAnyToByte f, Byte x, Any state);

/**
Applies function f to each element of array. 
The original array is not modified. A new array is created for the result.
Function f is called once for each element and returns the transformed element.
@code{.c}
Byte f(Byte element, int index, Byte x) {}
@endcode

@param[in] array Byte array
@param[in] f transformation function, called for each element of input array
@param[in] x provided to each invocation of f
@return the mapped array
@pre "not null", f
*/
Array ba_map(Array array, ByteIntByteToByte f, Byte x);

/**
Applies function f to each element of array. 
The original array is not modified. A new array is created for the result.
Function f is called once for each element and returns the transformed element.
@code{.c}
int f(Byte element, int index, Byte x, Any state) {}
@endcode

@param[in] array Byte array
@param[in] f transformation function, called for each element of input array
@param[in] x provided to each invocation of f
@param[in] state provided to each invocation of f
@return the mapped array
@pre "not null", f
*/
Array ba_map_state(Array array, ByteIntByteAnyToByte f, Byte x, Any state);

/**
Folds array from left to right, i.e., compute f(... f(f(init, a0), a1) ... an).
@code{.c}
Byte f(Byte state, Byte element, int index) {}
@endcode

@param[in] array Byte array
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
int ba_foldl(Array array, ByteByteIntToByte f, Byte state);

/**
Folds array from right to left. I.e., compute f(l0, f(l1,... f(ln, init)...)).
@code{.c}
Byte f(Byte element, Byte state, int index) {}
@endcode

@param[in] array Byte array
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
int ba_foldr(Array array, ByteByteIntToByte f, Byte state);

/**
Predicates
*/
bool ba_even(Byte value, int index, Byte x);
bool ba_odd(Byte value, int index, Byte x);
bool ba_gt(Byte value, int index, Byte x);
bool ba_ge(Byte value, int index, Byte x);
bool ba_lt(Byte value, int index, Byte x);
bool ba_le(Byte value, int index, Byte x);

/**
Functions for initialization, map, and fold
*/
Byte ba_square(Byte value, int index);
Byte ba_rnd(int index, Byte maximum);
Byte ba_times(Byte value, int index, Byte x);
Byte ba_plus(Byte x, Byte y, int index);
Byte ba_minus(Byte x, Byte y, int index);
Byte ba_mult(Byte x, Byte y, int index);
Byte ba_div(Byte x, Byte y, int index);

/**
Creates a new array with only those elements of array that satisfy the predicate.
The original array is not modified.
@code{.c}
bool predicate(Byte element, int index, Byte x) {}
@endcode

@param[in] array Byte array
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@return filtered array
@pre "not null", predicate
*/
Array ba_filter(Array array, ByteIntByteToBool predicate, Byte x);

/**
Creates a new array with only those elements of array that satisfy the predicate.
The original array is not modified.
@code{.c}
bool predicate(Byte element, int index, Byte x, Any state) {}
@endcode

@param[in] array Byte array
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return filtered array
@pre "not null", predicate
*/
Array ba_filter_state(Array array, ByteIntByteAnyToBool predicate, Byte x, Any state);

/**
Filters and maps array using f. The original array is not modified.
@code{.c}
ByteOption f(Byte element, int index, Byte x) {}
@endcode

@param[in] array Byte array
@param[in] f mapping function, returns the mapped element or @c none if the element should not be included in the result
@param[in] x given to each invocation of predicate
@return filtered and mapped array
@pre "not null", f

Example:
@code{.c}
ByteOption evens_times_x(Byte element, int index, Byte x) {
    if ((element % 2) == 0) { // if element is even
        return make_byte_some(x * element); // return byte option with mapped element
    }
    return make_byte_none(); // exclude this element
}
...
Array a = ba_of_string("1, 2, 3, 4, 5, 6");
Array b = ba_choose(a, evens_times_x, 3);
// b is [6, 12, 18]
@endcode
*/
Array ba_choose(Array array, ByteIntByteToByteOption f, Byte x);

/**
Filters and maps array using f. The original array is not modified.
@code{.c}
ByteOption f(Byte element, int index, Byte x, Any state) {}
@endcode

@param[in] array Byte array
@param[in] f mapping function, returns the mapped element or @c none if the element should not be included in the result
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return filtered and mapped array
@pre "not null", f
*/
Array ba_choose_state(Array array, ByteIntByteAnyToByteOption f, Byte x, Any state);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(Byte element, int index, Byte x) {}
@endcode

@param[in] array Byte array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at least one element satisfies predicate
@pre "not null", predicate
*/
bool ba_exists(Array array, ByteIntByteToBool predicate, Byte x);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(Byte element, int index, Byte x, Any state) {}
@endcode

@param[in] array Byte array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return true iff at least one element satisfies predicate
@pre "not null", predicate
*/
bool ba_exists_state(Array array, ByteIntByteAnyToBool predicate, Byte x, Any state);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(Byte element, int index, Byte x) {}
@endcode

@param[in] array Byte array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at all the elements satisfy predicate
@pre "not null", predicate
*/
bool ba_forall(Array array, ByteIntByteToBool predicate, Byte x);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(Byte element, int index, Byte x, Any state) {}
@endcode

@param[in] array Byte array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return true iff at all the elements satisfy predicate
@pre "not null", predicate
*/
bool ba_forall_state(Array array, ByteIntByteAnyToBool predicate, Byte x, Any state);

/*
Tests involving Byte arrays.
@param[in] ac actual result array
@param[in] ex expected result array
@returns true iff actual equals expected array
*/
#define ba_test_equal(ac, ex) \
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, ac, (ex)->a, (ex)->n)

/**
Tests involving Byte arrays.
@param[in] file source file name
@param[in] function function name
@param[in] line line number
@param[in] ac actual result array
@param[in] ex expected result C-array
@param[in] exn length of expected result C-array (number of elements)
@returns true iff actual equals expected array
*/
bool ba_test_equal_file_line(const char *file, const char *function, int line, Array ac, Byte *ex, int exn);

/*
Checks if array has the right element size. Fails if not.
*/
#undef require_element_size_byte
#ifdef NO_CHECK_ELEMENT_SIZE
#define require_element_size_byte(array)
#else
#define require_element_size_byte(array) \
    require_x("element size byte", (array)->s == sizeof(Byte), "size == %d", (array)->s)
#endif

void ba_test_all(void);

#endif
