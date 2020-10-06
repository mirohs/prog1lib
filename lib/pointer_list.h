/** @file
A list of pointers. 
Stores an arbitrary number of pointers. The prefix @c pl_ stands for <i>pointer list</i>. Some operations are inherited from list.c. For example, \ref l_length works with pointer lists and any other kind of list.

<h4>Example 1: Creating a pointer list and printing its elements.</h4>
@code{.c}
List a = sl_of_string("alpha, beta, gamma, delta, epsilon");
sl_println(a);
sl_free(a);
@endcode

Output :
@code{.c}
[alpha, beta, gamma, delta, epsilon]
@endcode



See test functions in .c file for more examples.

@author Michael Rohs
@date 15.10.2015, 6.10.2020
@copyright Apache License, Version 2.0
*/

#ifndef __POINTER_LIST_H__
#define __POINTER_LIST_H__

#include "base.h"



/** 
Creates an empty list of pointers. 
@return empty list
*/
List pl_create();

#if 0
/** 
Creates a list of n pointers, all initialized to value. 
@param[in] n result length (number of values)
@param[in] value value to repeat
@return the initialized list
*/
List pl_repeat(int n, Any value);
#endif

/**
Frees the list itself and each element. Assumes that each element is dynamically allocated. If this is not the case or you do not want to free the elements, use @ref l_free.
@see l_free
@param[in,out] list to be freed, unusable thereafter
*/
void pl_free(List list);

/**
Frees the list itself and each element with a destructor function. Assumes that each element is dynamically allocated. If this is not the case or you do not want to free the elements, use @ref l_free
@code{.c}
void element_destructor(Any element) { }
@endcode
@param[in,out] list to be freed, unusable thereafter
@param[in] element_destructor function called for each element to destruct it, i.e., to release its memory
@pre "not null" element_destructor
@see l_free
*/
void pl_free_with_destructor(List list, AnyFn element_destructor);

/**
Returns list element at index.
@param[in] list pointer list
@param[in] index index of list element to return
@return list element
@pre "index in range", index >= 0 && index < length
*/
Any pl_get(List list, int index);

/**
Sets list element at index to value.
@param[in,out] list pointer list
@param[in] index index of list element to set
@param[in] value value to set
@pre "index in range", index >= 0 && index < length
*/
void pl_set(List list, int index, Any value);

/**
Returns the next value.
@param[in,out] iter an iterator, iterator will be advanced to next element
@return next element
@pre "iterator has more values", *iter
@see l_iterator, l_has_next
*/
Any pl_next(ListIterator *iter);

/**
Appends value to end of list.
@param[in,out] list pointer list
@param[in] value value to append
*/
void pl_append(List list, Any value);

/**
Prepends value to front of list.
@param[in,out] list pointer list
@param[in] value value to prepend
*/
void pl_prepend(List list, Any value);

/**
Prints the list using the function to print each element.
@code{.c}
void print_element(Any element) {}
@endcode
@param[in] list pointer list
@param[in] print_element function called for each element to print it
@pre "not null", print_element
*/
void pl_print(List list, AnyFn print_element);

/**
Prints the list using the function to print each element,
then print a line break.
@code{.c}
void print_element(Any element) {}
@endcode
@param[in] list pointer list
@param[in] print_element function called for each element to print it
@pre "not null", print_element
*/
void pl_println(List list, AnyFn print_element);

/**
Returns true iff list contains value. Compares pointers.
@param[in] list pointer list
@param[in] value value to look for
@return true if list contains value, false otherwise
*/
bool pl_contains(List list, Any value);

/**
Returns index of first occurrence of value in list (comparing pointers). 
Returns -1 if value is not in list.
@param[in] list pointer list
@param[in] value value to look for
@return index or -1
*/
int pl_index(List list, Any value);

/**
Returns index of first occurrence of value in list at indices [from, n). Compares pointers.
Returns -1 if value is not in list[from, n).
Index from is inclusive.
@param[in] list pointer list
@param[in] value value to look for
@param[in] from start index (inclusive)
@return index or -1
*/
int pl_index_from(List list, Any value, int from);

/**
Returns index of first element for which the predicate function returns true.
Returns -1 if predicate does not return true for any element.
@code{.c}
bool predicate(Any element, int index, Any x) {}
@endcode
@param[in] list pointer list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return index or -1
@pre "not null", predicate
*/
int pl_index_fn(List list, AnyFn predicate, Any x);

/**
Returns the first element for which the predicate function returns true.
Returns @c NULL if predicate does not return true for any element.
@code{.c}
bool predicate(Any element, int index, Any x) {}
@endcode
@param[in] list pointer list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return element or @c NULL
@pre "not null", predicate
*/
Any pl_find(List list, AnyFn predicate, Any x);

/**
Inserts value at index in list. 
Does nothing if index is not valid, i.e., if not in interval [0,n].
@param[in,out] list pointer list
@param[in] index the position to insert at (index 0 means inserting at the front)
@param[in] value value to insert
*/
void pl_insert(List list, int index, Any value);

/**
Removes element at index in list.
Does nothing if index is not valid, i.e., if not in interval [0,n).
@param[in,out] list String list
@param[in] index index of element to remove
*/
void pl_remove(List list, int index);

/**
Applies function f to each element of list. The original list is modified (if f modifies the element).
Function f is called once for each element and returns the transformed element.
@code{.c}
Any f(Any element, int index, Any x) {}
@endcode

@param[in,out] list pointer list
@param[in] f a function that is called for each element of input list
@param[in] x provided to each invocation of f
@pre "not null", f

<b>Step by step:</b><br/>
list[0] := f(list[0], 0, x)<br/>
list[1] := f(list[1], 1, x)<br/>
...<br/>
list[n-1] := f(list[n-1], n-1, x)

@see l_each
*/
void pl_each(List list, AnyFn f, Any x);

/**
Applies function f to each element of list. 
The original list is not modified. A new list is created for the result.
Function f is called once for once for each element and returns the transformed element.
@code{.c}
Any f(Any element, int index, Any x) {}
@endcode

@param[in] list pointer list
@param[in] f transformation function, called for each element of input list
@param[in] x provided to each invocation of f
@return the mapped list
@pre "not null", f

@see l_map
*/
List pl_map(List list, AnyFn f, Any x);

/**
Folds list from left to right, i.e., compute f(... f(f(init, l0), l1) ... ln).
@code{.c}
Any f(Any state, Any element, int index) {}
@endcode

@param[in] list pointer list
@param[in] f a function that is called for each element of input list
@param[in] state provided to each invocation of f
@return the accumulated state
@pre "not null", f

<b>Step by step:</b><br/>
state := f(state, list[0], 0)<br/>
state := f(state, list[1], 1)<br/>
... <br/>
state := f(state, list[n-1], n-1)

@see l_foldl
*/
Any pl_foldl(List list, AnyFn f, Any state);

/**
Folds list from right to left. I.e., compute f(l0, f(l1,... f(ln, init)...)).
@code{.c}
Any f(Any element, Any state, int index) {}
@endcode

@param[in] list pointer list
@param[in] f a function that is called for each element of input list
@param[in] state provided to each invocation of f
@return the accumulated state
@pre "not null", f

<b>Step by step:</b><br/>
state := f(list[n-1], state, n-1)<br/>
... <br/>
state := f(list[1], state, 1)<br/>
state := f(list[0], state, 0)

@see l_foldr
*/
Any pl_foldr(List list, AnyFn f, Any state);
#if 0
/** 
Predicates
*/
bool pl_even(Any value, int index);
bool pl_odd(Any value, int index);
bool pl_index_even(Any value, int index);
bool pl_index_odd(Any value, int index);
bool pl_pos(Any value, int index);
bool pl_neg(Any value, int index);
bool pl_ge0(Any value, int index);
bool pl_gt(Any value, int index, Any x);
bool pl_ge(Any value, int index, Any x);
bool pl_lt(Any value, int index, Any x);
bool pl_le(Any value, int index, Any x);

/**
Functions for initialization, map, and fold
*/
Any pl_square(Any value, int index);
Any pl_rnd(int index, Any maximum);
Any pl_times(Any value, int index, Any x);
Any Any_plus(Any x, Any y, int index);
Any Any_minus(Any x, Any y, int index);
Any Any_mult(Any x, Any y, int index);
Any Any_div(Any x, Any y, int index);
#endif

/**
Creates a new list with only those elements of list that satisfy the predicate.
The original list is not modified.
@code{.c}
bool predicate(Any element, int index, Any x) {}
@endcode

@param[in] list pointer list
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@return filtered list
@pre "not null", predicate
*/
List pl_filter(List list, AnyFn predicate, Any x);

#if 0
/**
True if element and state are the same memory address.
*/
bool pl_contains_pointer(Any element, int index, Any state);
#endif

/**
Filters and maps list using f. The original list is not modified.
@code{.c}
Any f(Any element, int index, Any x) {}
@endcode

@param[in] list pointer list
@param[in] f mapping function, returns the mapped element or <code>none</code> if the element should not be included in the result
@param[in] x given to each invocation of predicate
@return filtered and mapped list
@pre "not null", f

Example:
@code{.c}
static Any ends_width_0_append(Any element, int index, Any x) {
    if (s_ends_with(element, "0")) {
        Any s = s_concat(element, x);
        return make_Any_some(s);
    }
    return make_Any_none();
}
...
List a = sl_of_Any("10, 2, 30, 4, 50, 6");
List b = sl_choose(a, ends_width_0_append, "x");
// b is Any list ["10x", "30x", "50x"]
@endcode
*/
List pl_choose(List list, AnyFn f, Any x);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(Any element, int index, Any x) {}
@endcode

@param[in] list pointer list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at least one element satisfies predicate
@pre "not null", predicate
*/
bool pl_exists(List list, AnyFn predicate, Any x);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(Any element, int index, Any x) {}
@endcode

@param[in] list pointer list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at all the elements satisfy predicate
@pre "not null", predicate
*/
bool pl_forall(List list, AnyFn predicate, Any x);

/**
Test involving pointer lists.
@param[in] ac actual result list
@param[in] ex expected result list
@returns true iff actual equals expected list
*/
#define pl_test_equal(ac, ex) \
    pl_test_equal_file_line(__FILE__, __func__, __LINE__, ac, ex)

/**
Test involving pointer lists.
@param[in] file source file name
@param[in] function function name
@param[in] line line number
@param[in] ac actual result list
@param[in] ex expected result list
@returns true iff actual equals expected list
*/
bool pl_test_equal_file_line(const char *file, const char *function, int line, List ac, List ex);

/** 
Checks if list has the right element size. Fails if not.
String s is debug output (e.g., function name).
*/
#undef require_element_size_pointer
#ifdef NO_CHECK_ELEMENT_SIZE
#define require_element_size_pointer(list)
#else
#define require_element_size_pointer(list) \
    require_x("element size pointer", (list)->s == sizeof(Any), "size == %d", (list)->s)
#endif

void pl_test_all(void);

#endif
