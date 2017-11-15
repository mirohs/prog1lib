/** @file
A list of integers. 
Stores an arbitrary number of integers. The prefix <code>il_</code> stands for <i>integer list</i>. Some operations are inherited from list.c. For example, <code>l_length</code> works with integer lists and any other kind of list.

@author Michael Rohs
@date 15.10.2015
@copyright Apache License, Version 2.0
*/

#ifndef __INT_LIST_H__
#define __INT_LIST_H__

#include "base.h"



/** 
Create an empty list of ints. 
@return empty list
*/
List il_create(void);

/**
Create a list of n ints by copying from buffer.
@param[in] buffer    the buffer to copy the elements from
@param[in] n        number of elements
@return list with n copied elements
*/
List il_of_buffer(Any buffer, int n);

#if 0
/** 
Create a list of n ints, all initialized to value. 
@param[in] n result length (number of values)
@param[in] value value to repeat
@return the initialized list
*/
List il_repeat(int n, int value);
#endif

/**
Create a list and set the elements to the interval [a,b) or (b,a], respectively. 
Index a is inclusive and index b is exclusive. 
- If a < b, then the result is an increasing range.
- If a > b, then the result is a decreasing range.
- If a == b, then the resulting list is empty.
@param[in] a first value of range (inclusive)
@param[in] b last value of range (exclusive)
@return the initialized list
*/
List il_range(int a, int b);

/**
Create a list from the given string.
Use ',' or ' ' as the separator.
Example: il_of_string("1, 3, -4") creates integer list [1, 3, -4].
@param[in] s string representation of int list
@return the initialized list
*/
List il_of_string(String s);

/** 
Create a list of n ints, each initialized with function init. 
@code{.c}
int init(int index, int x) {}
@endcode

@param[in] n    number of elements
@param[in] init    initialization function, will be called for each index [0, n-1)
@param[in] x    will be supplied to init

Example:
@code{.c}
static int two_i_plus_x(int index, int x) {
    return 2 * index + x;
}
...
List l = il_fn(3, two_i_plus_x, 10);
// set element 0 to init(0, 10) = 2 * 0 + 10 = 10
// set element 1 to init(1, 10) = 2 * 1 + 10 = 12
// set element 2 to init(2, 10) = 2 * 2 + 10 = 14
// now l is [10, 12, 14]
@endcode
*/
List il_fn(int n, IntIntToInt init, int x);

/**
Convert the list of doubles to a list of ints.
Each double of the input list is rounded to the nearest integer.
The input list is not modified.
@param[in] list list of doubles
@return the converted list of integers
*/
List il_of_dl(List list);

/**
Return list element at index.
@param[in] list int list
@param[in] index index of list element to return
@return list element
*/
int il_get(List list, int index);

/**
Set list element at index to value.
@param[in,out] list int list
@param[in] index index of list element to set
@param[in] value value to set
*/
void il_set(List list, int index, int value);

/**
Increment list element at index by value. Avoids common pattern: set(ls, i, get(ls, i) + v)
@param[in,out] list int list
@param[in] index index of list element to increment
@param[in] value value to increment
*/
void il_inc(List list, int index, int value);

/**
Return the next value.
@param[in,out] iter an iterator, iterator will be advanced to next element
@return next element
*/
int il_next(ListIterator *iter);

/**
Append value to end of list.
@param[in,out] list int list
@param[in] value value to append
*/
void il_append(List list, int value);

/**
Prepend value to front of list.
@param[in,out] list int list
@param[in] value value to prepend
*/
void il_prepend(List list, int value);

/**
Print the list.
@param[in] list int list
*/
void il_print(List list);

/**
Print the list, then print a line break.
@param[in] list int list
*/
void il_println(List list);

/**
Return true iff list contains value.
@param[in] list int list
@param[in] value value to look for
@return true if list contains value, false otherwise
*/
bool il_contains(List list, int value);

/**
Set each element of list to value.
@param[in,out] list int list
@param[in] value value to set
*/
void il_fill(List list, int value);

/**
Set a range of elements of list to value.
Index from is inclusive, index to is exclusive.
@param[in,out] list int list
@param[in] value value to set
@param[in] from start index (inclusive)
@param[in] to end index (exclusive)
*/
void il_fill_from_to(List list, int value, int from, int to);

/**
Return index of first occurrence of value in list. 
Return -1 if value is not in list.
@param[in] list int list
@param[in] value value to look for
@return index or -1
*/
int il_index(List list, int value);

/**
Return index of first occurrence of value in list at indices [from, n). 
Return -1 if value is not in list[from, n).
Index from is inclusive.
@param[in] list int list
@param[in] value value to look for
@param[in] from start index (inclusive)
@return index or -1
*/
int il_index_from(List list, int value, int from);

/**
Return index of first element for which the predicate function returns true.
Return -1 if predicate does not return true for any element.
@code{.c}
bool predicate(int element, int index, int x) {}
@endcode
@param[in] list int list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return index or -1
*/
int il_index_fn(List list, IntIntIntToBool predicate, int x);

/**
Sort the elements in increasing order.
Creates a new list. Does not modify the original list.
@param[in] list int list
@return new sorted list (increasing order).
*/
List il_sort(List list);

/**
Sort the elements in decreasing order.
Creates a new list. Does not modify the original list.
@param[in] list int list
@return new sorted list (decreasing order).
*/
List il_sort_dec(List list);

/**
Insert value into list at position index.
Does nothing if index is not valid, i.e., if not in interval [0,n].
@param[in,out] list int list
@param[in] index the position to insert at (index 0 means inserting at the front)
@param[in] value value to insert
*/
void il_insert(List list, int index, int value);

/**
Remove element at index in list.
Does nothing if index is not valid, i.e., if not in interval [0,n).
@param[in,out] list int list
@param[in] index index of element to remove
*/
void il_remove(List list, int index);

/**
Apply function f to each element of list. The original list is modified (if f modifies the element).
Function f is called once for each element and returns the transformed element.
@code{.c}
int f(int element, int index, int x) {}
@endcode

@param[in,out] list int list
@param[in] f a function that is called for each element of input list
@param[in] x provided to each invocation of f

<b>Step by step:</b><br/>
list[0] := f(list[0], 0, x)<br/>
list[1] := f(list[1], 1, x)<br/>
...<br/>
list[n-1] := f(list[n-1], n-1, x)
*/
void il_each(List list, IntIntIntToInt f, int x);

/**
Apply function f to each element of list. The original list is modified (if f modifies the element).
Function f is called once for each element and returns the transformed element.
@code{.c}
int f(int element, int index, int x, Any state) {}
@endcode

@param[in,out] list int list
@param[in] f a function that is called for each element of input list
@param[in] x provided to each invocation of f
@param[in] state provided to each invocation of f

<b>Step by step:</b><br/>
list[0] := f(list[0], 0, x, state)<br/>
list[1] := f(list[1], 1, x, state)<br/>
...<br/>
list[n-1] := f(list[n-1], n-1, x, state)
*/
void il_each_state(List list, IntIntIntAnyToInt f, int x, Any state);

/**
Apply function f to each element of list. 
The original list is not modified. A new list is created for the result.
Function f is called once for once for each element and returns the transformed element.
@code{.c}
int f(int element, int index, int x) {}
@endcode

@param[in] list int list
@param[in] f transformation function, called for each element of input list
@param[in] x provided to each invocation of f
@return the mapped list
*/
List il_map(List list, IntIntIntToInt f, int x);

/**
Apply function f to each element of list. 
The original list is not modified. A new list is created for the result.
Function f is called once for once for each element and returns the transformed element.
@code{.c}
int f(int element, int index, int x, Any state) {}
@endcode

@param[in] list int list
@param[in] f transformation function, called for each element of input list
@param[in] x provided to each invocation of f
@param[in] state provided to each invocation of f
@return the mapped list
*/
List il_map_state(List list, IntIntIntAnyToInt f, int x, Any state);

/**
Fold list from left to right, i.e., compute f(... f(f(init, l0), l1) ... ln).
@code{.c}
int f(int state, int element, int index) {}
@endcode

@param[in] list int list
@param[in] f a function that is called for each element of input list
@param[in] state provided to each invocation of f
@return the accumulated state

<b>Step by step:</b><br/>
state := f(state, list[0], 0)<br/>
state := f(state, list[1], 1)<br/>
... <br/>
state := f(state, list[n-1], n-1)
*/
int il_foldl(List list, IntIntIntToInt f, int state);

/**
Fold list from right to left. I.e., compute f(l0, f(l1,... f(ln, init)...)).
@code{.c}
int f(int element, int state, int index) {}
@endcode

@param[in] list int list
@param[in] f a function that is called for each element of input list
@param[in] state provided to each invocation of f
@return the accumulated state

<b>Step by step:</b><br/>
state := f(list[n-1], state, n-1)<br/>
... <br/>
state := f(list[1], state, 1)<br/>
state := f(list[0], state, 0)
*/
int il_foldr(List list, IntIntIntToInt f, int state);

/** 
Predicates
*/
bool il_even(int value, int index);
bool il_odd(int value, int index);
bool il_index_even(int value, int index);
bool il_index_odd(int value, int index);
bool il_pos(int value, int index);
bool il_neg(int value, int index);
bool il_ge0(int value, int index);
bool il_gt(int value, int index, int x);
bool il_ge(int value, int index, int x);
bool il_lt(int value, int index, int x);
bool il_le(int value, int index, int x);

/**
Functions for initialization, map, and fold
*/
int il_square(int value, int index);
int il_rnd(int index, int maximum);
int il_times(int value, int index, int x);
int int_plus(int x, int y, int index);
int int_minus(int x, int y, int index);
int int_mult(int x, int y, int index);
int int_div(int x, int y, int index);

/**
Create a new list with only those elements of list that satisfy the predicate.
The original list is not modified.
@code{.c}
bool predicate(int element, int index, int x) {}
@endcode

@param[in] list int list
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@return filtered list
*/
List il_filter(List list, IntIntIntToBool predicate, int x);

/**
Create a new list with only those elements of list that satisfy the predicate.
The original list is not modified.
@code{.c}
bool predicate(int element, int index, int x, Any state) {}
@endcode

@param[in] list int list
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return filtered list
*/
List il_filter_state(List list, IntIntIntAnyToBool predicate, int x, Any state);

/**
Filter and map list using f. The original list is not modified.
@code{.c}
IntOption f(int element, int index, int x) {}
@endcode

@param[in] list int list
@param[in] f mapping function, returns the mapped element or <code>none</code> if the element should not be included in the result
@param[in] x given to each invocation of predicate
@return filtered and mapped list

Example:
@code{.c}
IntOption evens_times_x(int element, int index, int x) {
    if ((element % 2) == 0) { // if element is even
        return make_int_some(x * element); // return int option with mapped element
    }
    return make_int_none(); // exclude this element
}
...
List a = il_of_string("1, 2, 3, 4, 5, 6");
List b = il_choose(a, evens_times_x, 3);
// b is [6, 12, 18]
@endcode
*/
List il_choose(List list, IntIntIntToIntOption f, int x);

/**
Filter and map list using f. The original list is not modified.
@code{.c}
IntOption f(int element, int index, int x, Any state) {}
@endcode

@param[in] list int list
@param[in] f mapping function, returns the mapped element or <code>none</code> if the element should not be included in the result
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return filtered and mapped list
*/
List il_choose_state(List list, IntIntIntAnyToIntOption f, int x, Any state);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(int element, int index, int x) {}
@endcode

@param[in] list int list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at least one element satisfies predicate
*/
bool il_exists(List list, IntIntIntToBool predicate, int x);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(int element, int index, int x, Any state) {}
@endcode

@param[in] list int list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return true iff at least one element satisfies predicate
*/
bool il_exists_state(List list, IntIntIntAnyToBool predicate, int x, Any state);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(int element, int index, int x) {}
@endcode

@param[in] list int list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at all the elements satisfy predicate
*/
bool il_forall(List list, IntIntIntToBool predicate, int x);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(int element, int index, int x, Any state) {}
@endcode

@param[in] list int list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@param[in] state given to each invocation of predicate (may be NULL)
@return true iff at all the elements satisfy predicate
*/
bool il_forall_state(List list, IntIntIntAnyToBool predicate, int x, Any state);

/**
Test for int lists.
@param[in] ac actual result list
@param[in] ex expected result list
@returns true iff actual equals expected list
*/
#define il_test_equal(ac, ex) \
    il_test_equal_file_line(__FILE__, __func__, __LINE__, ac, ex)

/**
Test for int lists.
@param[in] file source file name
@param[in] function function name
@param[in] line line number
@param[in] ac actual result list
@param[in] ex expected result list
@returns true iff actual equals expected list
*/
bool il_test_equal_file_line(const char *file, const char *function, int line, List ac, List ex);

/**
Checks if list has the right element size. Fails if not.
String s is debug output (e.g., function name).
*/
#ifdef CHECK_ELEMENT_SIZE
#define il_assert_element_size(list) \
    if ((list) != NULL && (list)->s != sizeof(int)) {\
        printf("%s: wrong element size %d, "\
            "this is not an int list\n", __func__, (list)->s);\
        exit(EXIT_FAILURE);\
    }
#else
#define il_assert_element_size(function_name, list) 
#endif

void il_test_all(void);

#endif
