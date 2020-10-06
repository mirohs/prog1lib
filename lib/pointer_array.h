/** @file
An array of pointers. 
It stores a fixed number of pointers. The prefix @c pa_ stands for <i>pointer array</i>. Some operations are inherited from array.h. For example, \ref a_length works with pointer arrays and any other kind of array.

@author Michael Rohs
@date 15.10.2015, 6.10.2020
@copyright Apache License, Version 2.0
*/

#ifndef __POINTER_ARRAY_H__
#define __POINTER_ARRAY_H__

#include "base.h"

/**
Creates an array of n elements, all initialized to value. The value is not copied.
@param[in] n number of elements
@param[in] value initialization value
@return the new array
@pre "non-negative length", n >= 0
*/
Array pa_create(int n, Any value);

/**
Frees the array itself and each element. If you do not intend to free each element, use \ref a_free instead.
@param[in,out] array to be freed, unusable thereafter
*/
void pa_free(Array array);

/**
Returns array element at index.
@param[in] array pointer array
@param[in] index index of array element to return
@return array element
@pre "index in range", index >= 0 && index < length
*/
#ifdef NO_GET_SET
#define pa_get(array, index) ((Any*)((array)->a))[index]
#else
Any pa_get(Array array, int index);
#endif

/**
Sets array element at index to value.
@param[in,out] array pointer array
@param[in] index index of array element to set
@param[in] value value to set
@pre "index in range", index >= 0 && index < length
*/
#ifdef NO_GET_SET
#define pa_set(array, index, value) ((Any*)((array)->a))[index] = value;
#else
void pa_set(Array array, int index, Any value);
#endif

#if 0
/**
Prints the array.
@param[in] array pointer array
*/
void pa_print(Array array);

/**
Prints the array, then print a line break.
@param[in] array pointer array
*/
void pa_println(Array array);
#endif

/**
Returns true iff array contains value. Compares pointers.
@param[in] array pointer array
@param[in] value value to look for
@return true if array contains value, false otherwise
*/
bool pa_contains(Array array, Any value);

/**
Returns index of first occurrence of value in array. Compares pointers.
Returns -1 if value is not in array.
@param[in] array pointer array
@param[in] value value to look for
@return index or -1
*/
int pa_index(Array array, Any value);

/**
Returns index of first occurrence of value in array at indices [from, n). Compares pointers.
Returns -1 if value is not in array[from, n).
Index from is inclusive.
@param[in] array pointer array
@param[in] value value to look for
@param[in] from start index (inclusive)
@return index or -1
*/
int pa_index_from(Array array, Any value, int from);

/**
Returns index of first element for which the predicate function returns true.
Returns -1 if predicate does not return true for any element.
@code{.c}
bool predicate(Any element, int index, Any x) {}
@endcode
@param[in] array pointer array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return index or -1
@pre "not null", predicate
*/
int pa_index_fn(Array array, AnyFn predicate, Any x);

/**
Returns index of last occurrence of value in array. Compares pointers.
Returns -1 if value is not in array.
@param[in] array pointer array
@param[in] value value to look for
@return index or -1
*/
int pa_last_index(Array array, Any value);

/**
Returns index of last occurrence of value in array at or before index from. Compares pointers.
Returns -1 if value is not in array.
@param[in] array pointer array
@param[in] value value to look for
@param[in] from starting index (inclusive)
@return index or -1
*/
int pa_last_index_from(Array array, Any value, int from);

/**
Returns index of last element for which the predicate function returns true.
Returns -1 if predicate does not return true for any element.
@code{.c}
bool predicate(int element, int index, Any x) {}
@endcode
@param[in] array pointer array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return index or -1
@pre "not null", predicate
*/
int pa_last_index_fn(Array array, AnyFn predicate, Any x);

/**
Applies function f to each element of array. The original array is modified (if f modifies the element).
Function f is called once for each element and returns the transformed element.
@code{.c}
Any f(Any element, int index, Any x) {}
@endcode

@param[in,out] array pointer array
@param[in] f a function that is called for each element of input array
@param[in] x provided to each invocation of f
@pre "not null", f

<b>Step by step:</b><br/>
array[0] := f(array[0], 0, x)<br/>
array[1] := f(array[1], 1, x)<br/>
...<br/>
array[n-1] := f(array[n-1], n-1, x)
*/
void pa_each(Array array, AnyFn f, Any x); 

/**
Applies function f to each element of array. 
The original array is not modified. A new array is created for the result.
Function f is called once for each element and returns the transformed element.
@code{.c}
int f(int element, int index, int x) {}
@endcode

@param[in] array pointer array
@param[in] f transformation function, called for each element of input array
@param[in] x provided to each invocation of f
@return the mapped array
@pre "not null", f
*/
Array pa_map(Array array, AnyFn f, Any x);

/**
Folds array from left to right, i.e., compute f(... f(f(init, a0), a1) ... an).
@code{.c}
Any f(Any state, Any element, int index) {}
@endcode

@param[in] array pointer array
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
Any pa_foldl(Array array, AnyFn f, Any state);

/**
Folds array from right to left. I.e., compute f(l0, f(l1,... f(ln, init)...)).
@code{.c}
Any f(Any element, Any state, int index) {}
@endcode

@param[in] array pointer array
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
Any pa_foldr(Array array, AnyFn f, Any state);

/**
Predicates
*/
bool pa_index_even(Any value, int index);
bool pa_index_odd(Any value, int index);
bool pa_element_empty(Any value, int index);

/*
 * Functions for initialization, map, and fold
 */
// see Any functions

/**
Creates a new array with only those elements of array that satisfy the predicate.
The original array is not modified.
@code{.c}
bool predicate(Any element, int index, Any x) {}
@endcode

@param[in] array pointer array
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@return filtered array
@pre "not null", predicate
*/
Array pa_filter(Array array, AnyFn predicate, Any x);

/**
Creates a new array with only those elements of array that satisfy the predicate.
The original array is not modified.
@code{.c}
bool predicate(Any element, int index, Any x, Any state) {}
@endcode

@param[in] array pointer array
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return filtered array
@pre "not null", predicate
*/
Array pa_filter_state(Array array, AnyFn predicate, Any x, Any state);

/**
Filters and maps array using f. The original array is not modified.
@code{.c}
Any f(Any element, int index, Any x) {}
@endcode

@param[in] array pointer array
@param[in] f mapping function, returns the mapped element or @c none if the element should not be included in the result
@param[in] x given to each invocation of predicate
@return filtered and mapped array
@pre "not null", f

Example:
@code{.c}
static Any starts_with_toupper(Any element, int index, Any x) {
    if (s_starts_with(element, x)) {
        Any u = s_upper_case(element);
        return make_Any_some(u);
    }
    return make_Any_none(); // exclude this element
}
...
Array a = pa_of_Any("a1, a2, b3, b4, a5, b6");
Array b = pa_choose(a, starts_with_toupper, "a");
// b is ["A1", "A2", "A5"]
@endcode
*/
Array pa_choose(Array array, AnyFn f, Any x);

// @todo: add tests

/**
Filters and maps array using f. The original array is not modified.
@code{.c}
Any f(Any element, int index, Any x, Any state) {}
@endcode

@param[in] array pointer array
@param[in] f mapping function, returns the mapped element or @c none if the element should not be included in the result
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return filtered and mapped array
@pre "not null", f
*/
Array pa_choose_state(Array array, AnyFn f, Any x, Any state);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(Any element, int index, Any x) {}
@endcode

@param[in] array pointer array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at least one element satisfies predicate
@pre "not null", predicate
*/
bool pa_exists(Array array, AnyFn predicate, Any x);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(Any element, int index, Any x, Any state) {}
@endcode

@param[in] array pointer array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return true iff at least one element satisfies predicate
@pre "not null", predicate
*/
bool pa_exists_state(Array array, AnyFn predicate, Any x, Any state);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(Any element, int index, Any x) {}
@endcode

@param[in] array pointer array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at all the elements satisfy predicate
@pre "not null", predicate
*/    
bool pa_forall(Array array, AnyFn predicate, Any x);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(Any element, int index, Any x, Any state) {}
@endcode

@param[in] array pointer array
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return true iff at all the elements satisfy predicate
@pre "not null", predicate
*/
bool pa_forall_state(Array array, AnyFn predicate, Any x, Any state);

/*
Tests involving pointer arrays.
@param[in] ac actual result array
@param[in] ex expected result array
@returns true iff actual equals expected array
*/
#define pa_test_equal(ac, ex) \
    pa_test_equal_file_line(__FILE__, __func__, __LINE__, ac, (ex)->a, (ex)->n)

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
bool pa_test_equal_file_line(const char *file, const char *function, int line, Array ac, Any *ex, int exn);

/*
* Checks if array has the right element size. Fails if not.
* Any s is debug output (e.g., function name).
*/
#undef require_element_size_pointer
#ifdef NO_CHECK_ELEMENT_SIZE
#define require_element_size_pointer(array)
#else
#define require_element_size_pointer(array) \
    require_x("element size pointer", (array)->s == sizeof(Any), "size == %d", (array)->s)
#endif

void pa_test_all(void);

#endif
