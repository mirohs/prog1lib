/** @file
An array of strings. 
It stores a fixed number of strings. The prefix @c sa_ stands for <i>string array</i>. Some operations are inherited from array.h. For example, \ref a_length works with string arrays and any other kind of array.

@author Michael Rohs
@date 15.10.2015, 6.10.2020
@copyright Apache License, Version 2.0
*/

#ifndef __STRING_ARRAY_H__
#define __STRING_ARRAY_H__

#include "base.h"



/**
Creates an array of n elements, all initialized to value. The value is not copied.
@param[in] n number of elements
@param[in] value initialization value
@return the new array
@pre "non-negative length", n >= 0
*/
Array sa_create(int n, String value);

/**
Creates an array from the given string.
Use "," (with optional surrounding whitespace) as the separator.
Example: sa_of_string("hello, world, hello") creates String array ["hello", "world", "hello"].
@param[in] s string representation of String array
@return the initialized array
*/
Array sa_of_string(String s);

/**
Creates an array from the given string.
Uses `separator` as the separator.
All the elements in the array are new dynamically allocated Strings.
Example: <code>sa_split("hello+world+moin moin", '+')</code>
creates a String array <code>["hello", "world", "moin moin"]</code>

@param[in] s string representation of String array
@param[in] separator this character separates parts of the string
@return the initialized array
*/
Array sa_split(String s, char separator);

/**
Frees the array itself and each element. If you do not intend to free each element, use \ref a_free instead.
@param[in,out] array to be freed, unusable thereafter
*/
void sa_free(Array array);

/**
Returns array element at index.
@param[in] array String array
@param[in] index index of array element to return
@return array element
@pre "index in range", index >= 0 && index < length
*/
#ifdef NO_GET_SET
#define sa_get(array, index) ((String*)((array)->a))[index]
#else
String sa_get(Array array, int index);
#endif

/**
Sets array element at index to value.
@param[in,out] array String array
@param[in] index index of array element to set
@param[in] value value to set
@pre "index in range", index >= 0 && index < length
*/
#ifdef NO_GET_SET
#define sa_set(array, index, value) ((String*)((array)->a))[index] = value;
#else
void sa_set(Array array, int index, String value);
#endif

/**
Prints the array.
@param[in] array String array
*/
void sa_print(Array array);

/**
Prints the array, then print a line break.
@param[in] array String array
*/
void sa_println(Array array);

/**
Returns true iff array contains value.
@param[in] array String array
@param[in] value value to look for
@return true if array contains value, false otherwise
*/
bool sa_contains(Array array, String value);

/**
Returns index of first occurrence of value in array. 
Returns -1 if value is not in array.
@param[in] array String array
@param[in] value value to look for
@return index or -1
*/
int sa_index(Array array, String value);

/**
Returns index of first occurrence of value in array at indices [from, n). 
Returns -1 if value is not in array[from, n).
Index from is inclusive.
@param[in] array String array
@param[in] value value to look for
@param[in] from start index (inclusive)
@return index or -1
*/
int sa_index_from(Array array, String value, int from);

/**
Returns index of first element for which the predicate function returns true.
Returns -1 if predicate does not return true for any element.
@code{.c}
bool predicate(String element, int index, String x) {}
@endcode
@param[in] array String array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return index or -1
@pre "not null", predicate
*/
int sa_index_fn(Array array, StringIntStringToBool predicate, String x);

/**
Returns index of last occurrence of value in array. 
Returns -1 if value is not in array.
@param[in] array String array
@param[in] value value to look for
@return index or -1
*/
int sa_last_index(Array array, String value);

/**
Returns index of last occurrence of value in array at or before index from.
Returns -1 if value is not in array.
@param[in] array String array
@param[in] value value to look for
@param[in] from starting index (inclusive)
@return index or -1
*/
int sa_last_index_from(Array array, String value, int from);

/**
Returns index of last element for which the predicate function returns true.
Returns -1 if predicate does not return true for any element.
@code{.c}
bool predicate(int element, int index, String x) {}
@endcode
@param[in] array String array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return index or -1
@pre "not null" predicate
*/
int sa_last_index_fn(Array array, StringIntStringToBool predicate, String x);

/**
Sorts the elements in increasing order. The input array is modified.
@param[in,out] array String array
*/
void sa_sort(Array array);

/**
Sorts the elements in increasing order, ignoring lower/upper case. The input array is modified.
@param[in,out] array String array
*/
void sa_sort_ignore_case(Array array);

/**
Sortis the elements in decreasing order.The input array is modified.
@param[in,out] array String array
*/
void sa_sort_dec(Array array);

/**
Sorts the elements in decreasing order, ignoring lower/upper case. The input array is modified.
@param[in,out] array String array
*/
void sa_sort_dec_ignore_case(Array array);

/**
Applies function f to each element of array. The original array is modified (if f modifies the element).
Function f is called once for each element and returns the transformed element.
@code{.c}
String f(String element, int index, String x) {}
@endcode

@param[in,out] array String array
@param[in] f a function that is called for each element of input array
@param[in] x provided to each invocation of f
@pre "not null", f

<b>Step by step:</b><br/>
array[0] := f(array[0], 0, x)<br/>
array[1] := f(array[1], 1, x)<br/>
...<br/>
array[n-1] := f(array[n-1], n-1, x)
*/
void sa_each(Array array, StringIntStringToString f, String x); 

/**
Applies function f to each element of array. 
The original array is not modified. A new array is created for the result.
Function f is called once for each element and returns the transformed element.
@code{.c}
int f(int element, int index, int x) {}
@endcode

@param[in] array String array
@param[in] f transformation function, called for each element of input array
@param[in] x provided to each invocation of f
@return the mapped array
@pre "not null", f
*/
Array sa_map(Array array, StringIntStringToString f, String x);

#if 0
/*
 * Applies function f to each element of array. The original array is modified.
 * Function f is called once for each element and returns the transformed element.
 * String f(String element, int index, Any p) {}
 */
Array sa_map_p(Array array, StringIntAnyToString f, Any p);
#endif

/**
Fold array from left to right, i.e., compute f(... f(f(init, a0), a1) ... an).
@code{.c}
String f(String state, String element, int index) {}
@endcode

@param[in] array String array
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
String sa_foldl(Array array, StringStringIntToString f, String state);

/**
Folds array from right to left. I.e., compute f(l0, f(l1,... f(ln, init)...)).
@code{.c}
String f(String element, String state, int index) {}
@endcode

@param[in] array String array
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
String sa_foldr(Array array, StringStringIntToString f, String state);

/**
Predicates
*/
bool sa_index_even(String value, int index);
bool sa_index_odd(String value, int index);
bool sa_element_empty(String value, int index);

/*
 * Functions for initialization, map, and fold
 */
// see String functions

/**
Creates a new array with only those elements of array that satisfy the predicate.
The original array is not modified.
@code{.c}
bool predicate(String element, int index, String x) {}
@endcode

@param[in] array String array
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@return filtered array
@pre "not null", predicate
*/
Array sa_filter(Array array, StringIntStringToBool predicate, String x);

/**
Creates a new array with only those elements of array that satisfy the predicate.
The original array is not modified.
@code{.c}
bool predicate(String element, int index, String x, Any state) {}
@endcode

@param[in] array String array
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return filtered array
@pre "not null", predicate
*/
Array sa_filter_state(Array array, StringIntStringAnyToBool predicate, String x, Any state);

/**
Filters and maps array using f. The original array is not modified.
@code{.c}
StringOption f(String element, int index, String x) {}
@endcode

@param[in] array String array
@param[in] f mapping function, returns the mapped element or @c none if the element should not be included in the result
@param[in] x given to each invocation of predicate
@return filtered and mapped array
@pre "not null", f

Example:
@code{.c}
static StringOption starts_with_toupper(String element, int index, String x) {
    if (s_starts_with(element, x)) {
        String u = s_upper_case(element);
        return make_string_some(u);
    }
    return make_string_none(); // exclude this element
}
...
Array a = sa_of_string("a1, a2, b3, b4, a5, b6");
Array b = sa_choose(a, starts_with_toupper, "a");
// b is ["A1", "A2", "A5"]
@endcode
*/
Array sa_choose(Array array, StringIntStringToStringOption f, String x);

// @todo: add tests

/**
Filters and maps array using f. The original array is not modified.
@code{.c}
StringOption f(String element, int index, String x, Any state) {}
@endcode

@param[in] array String array
@param[in] f mapping function, returns the mapped element or @c none if the element should not be included in the result
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return filtered and mapped array
@pre "not null", f
*/
Array sa_choose_state(Array array, StringIntStringAnyToStringOption f, String x, Any state);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(String element, int index, String x) {}
@endcode

@param[in] array String array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at least one element satisfies predicate
@pre "not null", predicate
*/
bool sa_exists(Array array, StringIntStringToBool predicate, String x);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(String element, int index, String x, Any state) {}
@endcode

@param[in] array String array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return true iff at least one element satisfies predicate
@pre "not null", predicate
*/
bool sa_exists_state(Array array, StringIntStringAnyToBool predicate, String x, Any state);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(String element, int index, String x) {}
@endcode

@param[in] array String array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at all the elements satisfy predicate
@pre "not null", predicate
*/
bool sa_forall(Array array, StringIntStringToBool predicate, String x);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(String element, int index, String x, Any state) {}
@endcode

@param[in] array String array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return true iff at all the elements satisfy predicate
@pre "not null", predicate
*/
bool sa_forall_state(Array array, StringIntStringAnyToBool predicate, String x, Any state);

/*
Tests involving String arrays.
@param[in] ac actual result array
@param[in] ex expected result array
@returns true iff actual equals expected array
*/
#define sa_test_equal(ac, ex) \
    sa_test_equal_file_line(__FILE__, __func__, __LINE__, ac, (ex)->a, (ex)->n)

/**
Tests involving int arrays.
@param[in] file source file name
@param[in] function function name
@param[in] line line number
@param[in] ac actual result array
@param[in] ex expected result C-array
@param[in] exn length of expected result C-array (number of elements)
@returns true iff actual equals expected array
*/
bool sa_test_equal_file_line(const char *file, const char *function, int line, Array ac, String *ex, int exn);

/*
* Checks if array has the right element size. Fails if not.
* String s is debug output (e.g., function name).
*/
#undef require_element_size_string
#ifdef NO_CHECK_ELEMENT_SIZE
#define require_element_size_string(array)
#else
#define require_element_size_string(array) \
    require_x("element size string", (array)->s == sizeof(String), "size == %d", (array)->s)
#endif

void sa_test_all(void);

#endif
