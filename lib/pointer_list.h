/** @file
A list of pointers. 
Stores an arbitrary number of pointers. The prefix \c pl_ stands for <i>pointer list</i>. Some operations are inherited from list.c. For example, \ref l_length works with pointer lists and any other kind of list.

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
@date 15.10.2015
@copyright Apache License, Version 2.0
*/

#ifndef __POINTER_LIST_H__
#define __POINTER_LIST_H__

#include "base.h"



/** 
Create an empty list of pointers. 
@return empty list
*/
List pl_create();

#if 0
/** 
Create a list of n pointers, all initialized to value. 
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
@see l_free
*/
void pl_free_with_destructor(List list, AnyFn element_destructor);

/**
Return list element at index.
@param[in] list pointer list
@param[in] index index of list element to return
@return list element
*/
Any pl_get(List list, int index);

/**
Set list element at index to value.
@param[in,out] list pointer list
@param[in] index index of list element to set
@param[in] value value to set
*/
void pl_set(List list, int index, Any value);

/**
Return the next value.
@param[in,out] iter an iterator, iterator will be advanced to next element
@return next element
@see l_iterator, l_has_next
*/
Any pl_next(ListIterator *iter);

/**
Append value to end of list.
@param[in,out] list pointer list
@param[in] value value to append
*/
void pl_append(List list, Any value);

/**
Prepend value to front of list.
@param[in,out] list pointer list
@param[in] value value to prepend
*/
void pl_prepend(List list, Any value);

/**
Print the list using the function to print each element.
@code{.c}
void print_element(Any element) {}
@endcode
@param[in] list pointer list
@param[in] print_element function called for each element to print it
*/
void pl_print(List list, AnyFn print_element);

/**
Print the list using the function to print each element,
then print a line break.
@code{.c}
void print_element(Any element) {}
@endcode
@param[in] list pointer list
@param[in] print_element function called for each element to print it
*/
void pl_println(List list, AnyFn print_element);

/**
Return true iff list contains value. Compares pointers.
@param[in] list pointer list
@param[in] value value to look for
@return true if list contains value, false otherwise
*/
bool pl_contains(List list, Any value);

/**
Return index of first occurrence of value in list (comparing pointers). 
Return -1 if value is not in list.
@param[in] list pointer list
@param[in] value value to look for
@return index or -1
*/
int pl_index(List list, Any value);

/**
Return index of first occurrence of value in list at indices [from, n). Compares pointers.
Return -1 if value is not in list[from, n).
Index from is inclusive.
@param[in] list pointer list
@param[in] value value to look for
@param[in] from start index (inclusive)
@return index or -1
*/
int pl_index_from(List list, Any value, int from);

/**
Return index of first element for which the predicate function returns true.
Return -1 if predicate does not return true for any element.
@code{.c}
bool predicate(Any element, int index, Any x) {}
@endcode
@param[in] list pointer list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return index or -1
*/
int pl_index_fn(List list, AnyFn predicate, Any x);

/**
Return the first element for which the predicate function returns true.
Return \c NULL if predicate does not return true for any element.
@code{.c}
bool predicate(Any element, int index, Any x) {}
@endcode
@param[in] list pointer list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return element or \c NULL
*/
Any pl_find(List list, AnyFn predicate, Any x);

/**
Insert value at index in list. 
Does nothing if index is not valid, i.e., if not in interval [0,n].
@param[in,out] list pointer list
@param[in] index the position to insert at (index 0 means inserting at the front)
@param[in] value value to insert
*/
void pl_insert(List list, int index, Any value);

/**
Remove element at index in list.
Does nothing if index is not valid, i.e., if not in interval [0,n).
@param[in,out] list String list
@param[in] index index of element to remove
*/
void pl_remove(List list, int index);

/**
Apply function f to each element of list. The original list is modified (if f modifies the element).
Function f is called once for each element and returns the transformed element.
@code{.c}
Any f(Any element, int index, Any x) {}
@endcode

@param[in,out] list pointer list
@param[in] f a function that is called for each element of input list
@param[in] x provided to each invocation of f

<b>Step by step:</b><br/>
list[0] := f(list[0], 0, x)<br/>
list[1] := f(list[1], 1, x)<br/>
...<br/>
list[n-1] := f(list[n-1], n-1, x)

@see l_each
*/
void pl_each(List list, AnyFn f, Any x);

/**
Apply function f to each element of list. 
The original list is not modified. A new list is created for the result.
Function f is called once for once for each element and returns the transformed element.
@code{.c}
Any f(Any element, int index, Any x) {}
@endcode

@param[in] list pointer list
@param[in] f transformation function, called for each element of input list
@param[in] x provided to each invocation of f
@return the mapped list

@see l_map
*/
List pl_map(List list, AnyFn f, Any x);

/**
Fold list from left to right, i.e., compute f(... f(f(init, l0), l1) ... ln).
@code{.c}
Any f(Any state, Any element, int index) {}
@endcode

@param[in] list pointer list
@param[in] f a function that is called for each element of input list
@param[in] state provided to each invocation of f
@return the accumulated state

<b>Step by step:</b><br/>
state := f(state, list[0], 0)<br/>
state := f(state, list[1], 1)<br/>
... <br/>
state := f(state, list[n-1], n-1)

@see l_foldl
*/
Any pl_foldl(List list, AnyFn f, Any state);

/**
Fold list from right to left. I.e., compute f(l0, f(l1,... f(ln, init)...)).
@code{.c}
Any f(Any element, Any state, int index) {}
@endcode

@param[in] list pointer list
@param[in] f a function that is called for each element of input list
@param[in] state provided to each invocation of f
@return the accumulated state

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
Create a new list with only those elements of list that satisfy the predicate.
The original list is not modified.
@code{.c}
bool predicate(Any element, int index, Any x) {}
@endcode

@param[in] list pointer list
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@return filtered list
*/
List pl_filter(List list, AnyFn predicate, Any x);

#if 0
/**
True if element and state are the same memory address.
*/
bool pl_contains_pointer(Any element, int index, Any state);
#endif

/**
Filter and map list using f. The original list is not modified.
@code{.c}
Any f(Any element, int index, Any x) {}
@endcode

@param[in] list pointer list
@param[in] f mapping function, returns the mapped element or <code>none</code> if the element should not be included in the result
@param[in] x given to each invocation of predicate
@return filtered and mapped list

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
*/
bool pl_forall(List list, AnyFn predicate, Any x);

/**
Test for pointer lists.
@param[in] ac actual result list
@param[in] ex expected result list
@returns true iff actual equals expected list
*/
#define pl_test_equal(ac, ex) \
    pl_test_equal_file_line(__FILE__, __func__, __LINE__, ac, ex)

/**
Test for pointer lists.
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
#ifdef CHECK_ELEMENT_SIZE
#define pl_assert_element_size(list) \
    if ((list) != NULL && (list)->s != sizeof(Any)) {\
        printf("%s: wrong element size %d, "\
            "this is not a pointer list\n", __func__, (list)->s);\
        exit(EXIT_FAILURE);\
    }
#else
#define pl_assert_element_size(function_name, list) 
#endif

void pl_test_all(void);

#endif
