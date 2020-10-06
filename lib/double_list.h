/** @file
A list of doubles. 
Stores an arbitrary number of doubles. The prefix <code>dl_</code> stands for <i>double list</i>. Some operations are inherited from list.c. For example, <code>l_length</code> works with double lists and any other kind of list.

@author Michael Rohs
@date 10.8.2015
@copyright Apache License, Version 2.0
*/

#ifndef __DOUBLE_LIST_H__
#define __DOUBLE_LIST_H__

#include "base.h"



/** 
Creates an empty list of doubles. 
@return empty list
*/
List dl_create(void);

/**
Creates a list of n doubles by copying from buffer.
@param[in] buffer    the buffer to copy the elements from
@param[in] n        number of elements
@return list with n copied elements
@pre "not null", buffer
@pre "not negative", n >= 0
*/
List dl_of_buffer(Any buffer, int n);

#if 0
/** 
Creates a list of n doubles, all initialized to value. 
@param[in] n result length (number of values)
@param[in] value value to repeat
@return the initialized list
@pre "not negative", n >= 0
*/
List dl_repeat(int n, double value);
#endif

/**
Creates a list and sets the elements to the interval [a,b) or (b,a], respectively. 
Index a is inclusive and index b is exclusive. 
- If a < b, then the result is an increasing range.
- If a > b, then the result is a decreasing range.
- If a == b, then the resulting list is empty.
@param[in] a first value of range (inclusive)
@param[in] b last value of range (exclusive)
@param[in] step step size
@return the initialized list
@pre "step not too small"
*/
List dl_range(double a, double b, double step);

/**
Creates a list from the given string.
Use ',' or ' ' as the separator.
Example: <code>dl_of_string("1, 2.5, -3.2, 49.1")</code> creates double list <code>[1.0 2.5 -3.2 49.1]</code>
@param[in] s string representation of double list
@return the initialized list
@pre "not null", s
*/
List dl_of_string(String s);

/** 
Creates a list of n doubles, each initialized with function init. 
@code{.c}
double init(int index, double x) {}
@endcode

@param[in] n    number of elements
@param[in] init    initialization function, will be called for each index [0, n-1)
@param[in] x    will be supplied to init
@pre "not negative", n >= 0
@pre "not null", init

Example:
@code{.c}
static double two_i_plus_x(double index, double x) {
    return 2 * index + x;
}
...
List l = dl_fn(3, two_i_plus_x, 10.1);
// set element 0 to init(0, 10) = 2 * 0 + 10.1 = 10.1
// set element 1 to init(1, 10) = 2 * 1 + 10.1 = 12.1
// set element 2 to init(2, 10) = 2 * 2 + 10.1 = 14.1
// now l is [10.1, 12.1, 14.1]
@endcode
*/
List dl_fn(int n, IntDoubleToDouble init, double x);

/**
Converts the list of ints to a list of doubles.
The input list is not modified.
@param[in] list list of integers
@return the converted list of doubles
@pre "not null", list
*/
List dl_of_il(List list);

/**
Returns list element at index.
@param[in] list double list
@param[in] index index of list element to return
@return list element
@pre "index in range", index >= 0 && index < length
*/
double dl_get(List list, int index);

/**
Sets list element at index to value.
@param[in,out] list double list
@param[in] index index of list element to set
@param[in] value value to set
@pre "index in range", index >= 0 && index < length
*/
void dl_set(List list, int index, double value);

/**
Increments list element at index by value. Avoids common pattern: set(ls, i, get(ls, i) + v)
@param[in,out] list double list
@param[in] index index of list element to increment
@param[in] value value to increment
@pre "index in range", index >= 0 && index < length
*/
void dl_inc(List list, int index, double value);

/**
Returns the next value.
@param[in,out] iter an iterator, iterator will be advanced to next element
@return next element
@pre "iterator has more values", *iter
*/
double dl_next(ListIterator *iter);

/**
Appends value to end of list.
@param[in,out] list double list
@param[in] value value to append
*/
void dl_append(List list, double value);

/**
Prepends value to front of list.
@param[in,out] list double list
@param[in] value value to prepend
*/
void dl_prepend(List list, double value);

/**
Prints the list.
@param[in] list double list
*/
void dl_print(List list);

/**
Prints the list, then print a line break.
@param[in] list double list
*/
void dl_println(List list);

/**
Returns true iff list contains value.
The list is considered to contain value iff it has an element 
whose absolute difference from value is less than epsilon.

@param[in] list double list
@param[in] value value to look for
@param[in] epsilon tolerance around value
@return true if list contains value, false otherwise
@pre "positive", epsilon > 0
*/
bool dl_contains(List list, double value, double epsilon);

/**
Sets each element of list to value.
@param[in,out] list double list
@param[in] value value to set
*/
void dl_fill(List list, double value);

/**
Sets a range of elements of list to value.
Index from is inclusive, index to is exclusive.
@param[in,out] list double list
@param[in] value value to set
@param[in] from start index (inclusive)
@param[in] to end index (exclusive)
*/
void dl_fill_from_to(List list, double value, int from, int to);

/**
Returns index of first occurrence of value in list. 
Returns -1 if value is not in list.
The list is considered to contain value iff it has an element 
whose absolute difference from value is less than epsilon.
@param[in] list double list
@param[in] value value to look for
@param[in] epsilon tolerance around value
@return index or -1
@pre "positive", epsilon > 0
*/
int dl_index(List list, double value, double epsilon);

/**
Returns index of first occurrence of value in list at indices [from, n). 
Returns -1 if value is not in list[from, n).
Index from is inclusive.
The list is considered to contain value iff it has an element 
whose absolute difference from value is less than epsilon.
@param[in] list double list
@param[in] value value to look for
@param[in] from start index (inclusive)
@param[in] epsilon tolerance around value
@return index or -1
@pre "positive", epsilon > 0
*/
int dl_index_from(List list, double value, int from, double epsilon);

/**
Returns index of first element for which the predicate function returns true.
Returns -1 if predicate does not return true for any element.
@code{.c}
bool predicate(double element, int index, double x) {}
@endcode
@param[in] list double list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return index or -1
@pre "not null", predicate
*/
int dl_index_fn(List list, DoubleIntDoubleToBool predicate, double x);

/**
Sorts the elements in increasing order.
Creates a new list. Does not modify the original list.
@param[in] list double list
@return new sorted list (increasing order).
*/
List dl_sort(List list);

/**
Sorts the elements in decreasing order.
Creates a new list. Does not modify the original list.
@param[in] list double list
@return new sorted list (decreasing order).
*/
List dl_sort_dec(List list);

/**
Inserts value at index in list. 
Does nothing if index is not valid, i.e., if not in interval [0,n].
@param[in,out] list double list
@param[in] index the position to insert at (index 0 means inserting at the front)
@param[in] value value to insert
*/
void dl_insert(List list, int index, double value);

/**
Removes element at index in list.
Does nothing if index is not valid, i.e., if not in interval [0,n).
@param[in,out] list double list
@param[in] index index of element to remove
*/
void dl_remove(List list, int index);

/**
Applies function f to each element of list. The original list is modified (if f modifies the element).
Function f is called once for each element and returns the transformed element.
@code{.c}
double f(double element, int index, double x) {}
@endcode

@param[in,out] list double list
@param[in] f a function that is called for each element of input list
@param[in] x provided to each invocation of f
@pre "not null", f

<b>Step by step:</b><br/>
list[0] := f(list[0], 0, x)<br/>
list[1] := f(list[1], 1, x)<br/>
...<br/>
list[n-1] := f(list[n-1], n-1, x)
*/
void dl_each(List list, DoubleIntDoubleToDouble f, double x);

/**
Applies function f to each element of list. The original list is modified (if f modifies the element).
Function f is called once for each element and returns the transformed element.
@code{.c}
double f(double element, int index, double x, Any state) {}
@endcode

@param[in,out] list double list
@param[in] f a function that is called for each element of input list
@param[in] x provided to each invocation of f
@param[in] state provided to each invocation of f
@pre "not null", f

<b>Step by step:</b><br/>
list[0] := f(list[0], 0, x, state)<br/>
list[1] := f(list[1], 1, x, state)<br/>
...<br/>
list[n-1] := f(list[n-1], n-1, x, state)
*/
void dl_each_state(List list, DoubleIntDoubleAnyToDouble f, double x, Any state);

/**
Applies function f to each element of list. 
The original list is not modified. A new list is created for the result.
Function f is called once for once for each element and returns the transformed element.
@code{.c}
double f(double element, int index, double x) {}
@endcode

@param[in] list double list
@param[in] f transformation function, called for each element of input list
@param[in] x provided to each invocation of f
@return the mapped list
@pre "not null", f
*/
List dl_map(List list, DoubleIntDoubleToDouble f, double x);

/**
Applies function f to each element of list. 
The original list is not modified. A new list is created for the result.
Function f is called once for once for each element and returns the transformed element.
@code{.c}
double f(double element, int index, double x) {}
@endcode

@param[in] list double list
@param[in] f transformation function, called for each element of input list
@param[in] x provided to each invocation of f
@param[in] state provided to each invocation of f
@return the mapped list
@pre "not null", f
*/
List dl_map_state(List list, DoubleIntDoubleAnyToDouble f, double x, Any state);

/**
Folds list from left to right, i.e., compute f(... f(f(init, l0), l1) ... ln).
@code{.c}
double f(double state, double element, int index) {}
@endcode

@param[in] list double list
@param[in] f a function that is called for each element of input list
@param[in] state provided to each invocation of f
@return the accumulated state
@pre "not null", f

<b>Step by step:</b><br/>
state := f(state, list[0], 0)<br/>
state := f(state, list[1], 1)<br/>
... <br/>
state := f(state, list[n-1], n-1)
*/
double dl_foldl(List list, DoubleDoubleIntToDouble f, double state);

/**
Folds list from right to left. I.e., compute f(l0, f(l1,... f(ln, init)...)).
@code{.c}
double f(double element, double state, int index) {}
@endcode

@param[in] list double list
@param[in] f a function that is called for each element of input list
@param[in] state provided to each invocation of f
@return the accumulated state
@pre "not null", f

<b>Step by step:</b><br/>
state := f(list[n-1], state, n-1)<br/>
... <br/>
state := f(list[1], state, 1)<br/>
state := f(list[0], state, 0)
*/
double dl_foldr(List list, DoubleDoubleIntToDouble f, double state);

/** 
Predicates
*/
bool dl_even(double value, int index);
bool dl_odd(double value, int index);
bool dl_index_even(double value, int index);
bool dl_index_odd(double value, int index);
bool dl_pos(double value, int index);
bool dl_neg(double value, int index);
bool dl_ge0(double value, int index);
bool dl_gt(double value, int index, double x);
bool dl_ge(double value, int index, double x);
bool dl_lt(double value, int index, double x);
bool dl_le(double value, int index, double x);

/**
Functions for initialization, map, and fold
*/
double dl_square(double value, int index);
double dl_rnd(int index, double maximum);
double dl_times(double value, int index, double x);
double double_plus(double x, double y, int index);
double double_minus(double x, double y, int index);
double double_mult(double x, double y, int index);
double double_div(double x, double y, int index);

/**
Creates a new list with only those elements of list that satisfy the predicate.
The original list is not modified.
@code{.c}
bool predicate(double element, int index, double x) {}
@endcode

@param[in] list double list
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@return filtered list
@pre "not null", predicate
*/
List dl_filter(List list, DoubleIntDoubleToBool predicate, double x);

/**
Creates a new list with only those elements of list that satisfy the predicate.
The original list is not modified.
@code{.c}
bool predicate(double element, int index, double x, Any state) {}
@endcode

@param[in] list double list
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return filtered list
@pre "not null", predicate
*/
List dl_filter_state(List list, DoubleIntDoubleAnyToBool predicate, double x, Any state);

/**
Filters and maps list using f. The original list is not modified.
@code{.c}
DoubleOption f(double element, int index, double x) {}
@endcode

@param[in] list double list
@param[in] f mapping function, returns the mapped element or <code>none</code> if the element should not be included in the result
@param[in] x given to each invocation of predicate
@return filtered and mapped list
@pre "not null", f

Example:
@code{.c}
DoubleOption gt3_times_x(double element, int index, double x) {
    if (element > 3) {
        return make_double_some(x * element);
    }
    return make_double_none();
}

Liat a = dl_of_string("1, 2, 3, 4, 5, 6");
List b = dl_choose(a, gt3_times_x, 4);
// b is double list [16.0, 20.0, 24.0]
@endcode
*/
List dl_choose(List list, DoubleIntDoubleToDoubleOption f, double x);

/**
Filters and maps list using f. The original list is not modified.
@code{.c}
DoubleOption f(double element, int index, double x) {}
@endcode

@param[in] list double list
@param[in] f mapping function, returns the mapped element or <code>none</code> if the element should not be included in the result
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return filtered and mapped list
@pre "not null", f

Example:
@code{.c}
DoubleOption gt3_times_x(double element, int index, double x) {
    if (element > 3) {
        return make_double_some(x * element);
    }
    return make_double_none();
}

Liat a = dl_of_string("1, 2, 3, 4, 5, 6");
List b = dl_choose(a, gt3_times_x, 4);
// b is double list [16.0, 20.0, 24.0]
@endcode
*/
List dl_choose_state(List list, DoubleIntDoubleAnyToDoubleOption f, double x, Any state);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(double element, int index, double x) {}
@endcode

@param[in] list double list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at least one element satisfies predicate
@pre "not null", predicate
*/
bool dl_exists(List list, DoubleIntDoubleToBool predicate, double x);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(double element, int index, double x, Any state) {}
@endcode

@param[in] list double list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return true iff at least one element satisfies predicate
@pre "not null", predicate
*/
bool dl_exists_state(List list, DoubleIntDoubleAnyToBool predicate, double x, Any state);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(double element, int index, double x) {}
@endcode

@param[in] list double list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at all the elements satisfy predicate
@pre "not null", predicate
*/
bool dl_forall(List list, DoubleIntDoubleToBool predicate, double x);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(double element, int index, double x, Any state) {}
@endcode

@param[in] list double list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return true iff at all the elements satisfy predicate
@pre "not null", predicate
*/
bool dl_forall_state(List list, DoubleIntDoubleAnyToBool predicate, double x, Any state);

/**
Test involving double lists.
@param[in] ac actual result list
@param[in] ex expected result list
@returns true iff actual equals expected list
*/
#define dl_test_within(ac, ex) \
    dl_test_within_file_line(__FILE__, __func__, __LINE__, ac, ex, EPSILON)

/**
Test involving double lists.
@param[in] file source file name
@param[in] function function name
@param[in] line line number
@param[in] ac actual result list
@param[in] ex expected result list
@param[in] epsilon tolerance around expected values
@returns true iff actual equals expected list
@pre "positive", epsilon > 0
*/
bool dl_test_within_file_line(const char *file, const char *function, int line, List ac, List ex, double epsilon);

/**
Checks if list has the right element size. Fails if not.
String s is debug output (e.g., function name).
*/
#undef require_element_size_double
#ifdef NO_CHECK_ELEMENT_SIZE
#define require_element_size_double(list)
#else
#define require_element_size_double(list) \
    require_x("element size double", (list)->s == sizeof(double), "size == %d", (list)->s)
#endif


void dl_test_all(void);

#endif
