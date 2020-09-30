/** @file
A generic inline list. 
It stores an arbitrary number of elements. Each element has a size of s bytes. The element size has to be specified at list creation time. Elements are stored inline in the nodes. The elements are treated as opaque byte sequences.

There are more specific lists for integers, doubles, strings, and pointers. 
They are more convenient for these types. Some functions are shared between list implementations. For example, <code>l_length</code> works with any kind of list.

@author Michael Rohs
@date 15.10.2015
@copyright Apache License, Version 2.0
*/

#ifndef __LIST_H__
#define __LIST_H__

#include "base.h"

/**
Creates an empty list of elements of size s. 
@param[in] s element size in bytes
@return empty list
@pre "positive size", s > 0
*/
List l_create(int s);

/**
Creates a list of n elements, each of size s, by copying n * s bytes from buffer.
@param[in] buffer    the buffer to copy the elements from
@param[in] n            number of elements
@param[in] s            element size in bytes
@return list with n copied elements, each of size s
@pre "non-negative length", n >= 0
@pre "positive size", s > 0
*/
List l_of_buffer(Any buffer, int n, int s);

/**
Creates a list of n elements of size s, each initialized with function init. 

@code{.c}
void init(Any element, int index, Any state) {}
@endcode

@param[in] n        number of elements
@param[in] s        element size in bytes
@param[in] init    initialization function, will be called for each index [0, n-1)
@param[in] state    state will be supplied to init
@pre "non-negative length", n >= 0
@pre "positive size", s > 0
@pre "not null", init

Example:
@code{.c}
 static void x_and_xsquare(IntPair *element, int index, Any state) {
     element->i = index;
     element->j = indexindex;
 }
 ...
 List ac = l_fn(4, sizeof(IntPair), x_and_xsquare, NULL);
 // set element 0 to (0, 0)
 // set element 1 to (1, 1)
 // set element 2 to (2, 4)
 // set element 3 to (3, 9)
@endcode
*/
List l_fn(int n, int s, AnyFn init, Any state);

/**
Creates a copy of the given list.
The bytes of the list elements are copied.
@param[in] list    to be copied
@return the copy of the list
*/
List l_copy(List list);

/**
Creates a sublist consisting of list[i, j).
Index i is inclusive, index j is exclusive.
@param[in] list    to be sub-listed
@param[in] i start index (inclusive)
@param[in] j end index (exclusive)
@return the sublist
*/
List l_sub(List list, int i, int j);

/**
Creates a new list by copying the elements of the array.
@param[in] array the elements of the array will be copied
@return the new list
*/
List l_of_a(Array array);

/**
Free the memory of the list.
@param[in,out] list to be freed, unusable thereafter
*/
void l_free(List list);

/**
Returns the memory address of the list element at index i.
@param[in] list input list
@param[in] index index of list element to return
@return address of list element
@pre "index in range"
*/
Any l_get(List list, int index);

/**
Sets list element at index to value.
Copies l_element_size(list) bytes from value.
@param[in,out] list input list
@param[in] index index of list element to set
@param[in] value address of the memory to copy
@pre "index in range"
*/
void l_set(List list, int index, Any value);

/**
Returns a fresh iterator for this list. Usable with any kind of list.
@param[in] list input list
@return list iterator
*/
ListIterator l_iterator(List list);

/**
Returns true iff there are one or more values left. Usable with any kind of list.
@param[in] iter an iterator
@return true iff there are one or more values left
*/
bool l_has_next(ListIterator iter);

/**
Returns the next element.
There are more specific variants: il_next, dl_next, etc.
@param[in,out] iter an iterator, iterator will be advanced to next element
@return address of next element
@pre "iterator has more values", *iter
*/
Any l_next(ListIterator *iter);

/**
Appends value to end of list.
Copies l_element_size(list) bytes from value.
@param[in,out] list input list
@param[in] value address of value to append (size is determined by element size)
*/
void l_append(List list, Any value);
    
/**
Prepends value to front of list.
Copies l_element_size(list) bytes from value.
@param[in,out] list input list
@param[in] value address of value to prepend (size is determined by element size)
*/
void l_prepend(List list, Any value);
    
/**
Returns the number of elements of the list (not the number of bytes!).
@param[in] list input list
@return number of elements in list
*/
int l_length(List list);

/**
Returns the size in bytes of an element of list.
@param[in] list input list
@return size of list elements
*/
int l_element_size(List list);

/**
Prints the list using the function to print each element.
@code{.c}
void print_element(Any element) {}
@endcode
@param[in] list input list
@param[in] print_element function to print a single element, will be called for each element
@pre "not null", print_element
*/
void l_print(List list, AnyFn print_element);

/**
Checks if corresponding elements of @c a and @c b are equal. Performs a bytewise comparison on the data.
@param[in] a a list
@param[in] b another list
@return true iff the lengths of both lists are equal, their element sizes are equal, and for all positions i the i-th element of @c a is equal to i-th element of @c b
*/
bool l_equals(List a, List b);

/**
Prints the list using the function to print each element,
then print a line break.
@code{.c}
void print_element(Any element) {}
@endcode
@param[in] list input list
@param[in] print_element function to print a single element, will be called for each element
@pre "not null", print_element
*/
void l_println(List list, AnyFn print_element);

/**
Returns a new list that is the concatenation of x and y. Does not modify x or y.
The elements of x come first, followed by the elements of y.
@param[in] x first input list
@param[in] y second input list
@return the concatenation of x and y
@pre "equal element sizes", x->s == y->s
*/
List l_concat(List x, List y);

/**
Returns index of first element for which the predicate function returns true.
Returns -1 if predicate does not return true for any element.
@code{.c}
bool predicate(Any element, int index, Any state) {}
@endcode
@param[in] list input list
@param[in] predicate predicate function
@param[in] state given to each invocation of predicate (may be NULL)
@return index or -1
@pre "not null", predicate
*/
int l_index_fn(List list, AnyFn predicate, Any state);

/**
Returns the address of the first element for which the predicate function returns true.
Returns NULL if predicate does not return true for any element.
@code{.c}
bool predicate(Any element, int index, Any state) {}
@endcode

@param[in] list input list
@param[in] predicate predicate function
@param[in] state given to each invocation of predicate (may be NULL)
@return element address or NULL
@pre "not null", predicate
*/
Any l_find(List list, AnyFn predicate, Any state);

/**
Creates a reversed list. Does not modify the original list.
@param[in] list input list
@return copy of list with element order reversed
*/
List l_reverse(List list);

/**
Randomly rearranges the elements of list. Does not modify the original list.
@param[in] list input list
@return copy of list with element order randomized
*/
List l_shuffle(List list);

/**
Sorts the elements using the given comparator function. 
Creates a new list. Does not modify the original list.

@param[in] list input list
@param[in] c comparator function to compare two elements
@return new list with sorted order

Example:
@code{.c}
static CmpResult my_int_comparator(ConstAny a, ConstAny b) {
    int x = *(int*)a;
    int y = *(int*)b;
    return (x < y) ? LT : ((x > y) ? GT : EQ);
}
...
List a = il_of_string("1, 2, 3, 4, 5");
l_sort(a, my_int_comparator);
il_free(a);
@endcode
*/
List l_sort(List list, Comparator c);

/**
Inserts value at index in list. 
Does nothing if index is not a valid index, i.e. if not interval [0,n].
@param[in,out] list input list
@param[in] index the position to insert at (index 0 means inserting at the front)
@param[in] value address of element to insert
*/
void l_insert(List list, int index, Any value);

/**
Removes element at index in list.
Does nothing if index is not a valid index, i.e. if not interval [0,n).
@param[in,out] list input list
@param[in] index index of element to remove
*/
void l_remove(List list, int index);

/**
Applies function f to each element of list. The original list is not modified.
Function f is called once for each element from first to last.
@code{.c}
void f(Any element, int index, Any state, Any mapped_element) {}
@endcode

@param[in] f transformation function, called for each element of input list
@param[in] mapped_element_size size of elements in the mapped list (in bytes)
@param[in] state provided to each invocation of f
@param[in] list input list
@return the mapped list
@pre "not null", f
@pre "positive size", mapped_element_size > 0
*/
List l_map(List list, AnyFn f, int mapped_element_size, Any state);

#if 0
/**
Applies function f to each element of lists. The original lists are not modified.
Function f is called once for each index from first to last.
@code{.c}
void f(Any element1, Any element2, int index, Any state, Any mapped_element) {}
@endcode

@param[in] f transformation function, called for each element of input lists
@param[in] mapped_element_size size of elements in the mapped list (in bytes)
@param[in] state provided to each invocation of f
@param[in] l1 input list 1
@param[in] l2 input list 2
@return the mapped list
@pre "not null", f
@pre "positive size", mapped_element_size > 0
*/
List l_map2(AnyAnyIntAnyAnyToVoid f, int mapped_element_size, 
            Any state, List l1, List l2);

/**
Applies function f to each element of lists. The original lists are not modified.
Function f is called once for each index from first to last.
@code{.c}
void f(Any element1, Any element2, Any element3, int index, Any state, Any mapped_element) {}
@endcode

@param[in] f transformation function, called for each element of input lists
@param[in] mapped_element_size size of elements in the mapped list (in bytes)
@param[in] state provided to each invocation of f
@param[in] l1 input list 1
@param[in] l2 input list 2
@param[in] l3 input list 3
@return the mapped list
@pre "not null", f
@pre "positive size", mapped_element_size > 0
*/
List l_map3(AnyAnyAnyIntAnyAnyToVoid f, int mapped_element_size, Any state, 
            List l1, List l2, List l3);
#endif

/**
Applies function f to each element of list. The original list is modified (if f modifies the element).
Function f is called once for each element from first to last.
@code{.c}
void f(Any element, int index, Any state) {}
@endcode

@param[in] f a function that is called for each element of input list
@param[in] state provided to each invocation of f
@param[in,out] list input list

<b>Step by step:</b><br/>
f(list[0], 0, state)<br/>
f(list[1], 1, state)<br/>
...<br/>
f(list[n-1], n-1, state)
*/
void l_each(List list, AnyFn f, Any state);

#if 0
/**
Applies function f to each element of list. The original list is modified (if f modifies the elements).
Function f is called once for each index from first to last.
@code{.c}
void f(Any element1, Any element2, int index, Any state) {}
@endcode

@param[in] f a function that is called for each element of input lists
@param[in] state provided to each invocation of f
@param[in,out] l1 input list 1
@param[in,out] l2 input list 2

<b>Step by step:</b><br/>
f(l1[0], l2[0], 0, state)<br/>
f(l1[1], l2[1], 1, state)<br/>
... <br/>
f(l1[n-1], l2[n-1], n-1, state)
*/
void l_each2(AnyAnyIntAnyToVoid f, Any state, List l1, List l2);

/**
Applies function f to each element of list. The original list is modified (if f modifies the elements).
Function f is called once for each index from first to last.
@code{.c}
void f(Any element1, Any element2, Any element3, int index, Any state) {}
@endcode

@param[in] f a function that is called for each element of input lists
@param[in] state provided to each invocation of f
@param[in,out] l1 input list 1
@param[in,out] l2 input list 2
@param[in,out] l3 input list 3

<b>Step by step:</b><br/>
f(l1[0], l2[0], l3[0], 0, state)<br/>
f(l1[1], l2[1], l3[1], 1, state)<br/>
... <br/>
f(l1[n-1], l2[n-1], l3[n-1], n-1, state)
*/
void l_each3(AnyAnyAnyIntAnyToVoid f, Any state, List l1, List l2, List l3);
#endif

/**
Folds the list from left to right.
@code{.c}
void f(Any state, Any element, int index) {}
@endcode

@param[in] f a function that is called for each element of input list
@param[in,out] state provided to each invocation of f
@param[in] list input list
@pre "not null", f

<b>Step by step:</b><br/>
f(state, list[0], 0) // modify state<br/>
f(state, list[1], 1) // modify state <br/>
... <br/>
f(state, list[n-1], n-1) // modify state
*/
void l_foldl(List list, AnyFn f, Any state);

#if 0
/**
Folds the lists from left to right.
@code{.c}
void f(Any state, Any element1, Any element2, int index) {}
@endcode

@param[in] f a function that is called for each element of input lists
@param[in,out] state provided to each invocation of f
@param[in] l1 input list 1
@param[in] l2 input list 2

<b>Step by step:</b><br/>
f(state, l1[0], l2[0], 0) // modify state<br/>
f(state, l1[1], l2[1], 1) // modify state <br/>
... <br/>
f(state, l1[n-1], l2[n-1], n-1) // modify state
*/
void l_foldl2(AnyAnyAnyIntToVoid f, Any state, List l1, List l2);

/**
Folds the lists from left to right.
@code{.c}
void f(Any state, Any element1, Any element2, Any element3, int index) {}
@endcode

@param[in] f a function that is called for each element of input lists
@param[in,out] state provided to each invocation of f
@param[in] l1 input list 1
@param[in] l2 input list 2
@param[in] l3 input list 3

<b>Step by step:</b><br/>
f(state, l1[0], l2[0], l3[0], 0) // modify state<br/>
f(state, l1[1], l2[1], l3[1], 1) // modify state <br/>
... <br/>
f(state, l1[n-1], l2[n-1], l3[n-1], n-1) // modify state
*/
void l_foldl3(AnyAnyAnyAnyIntToVoid f, Any state, List l1, List l2, List l3);
#endif

/**
Folds the list from right to left.
@code{.c}
void f(Any element, Any state, int index) {}
@endcode

@param[in] f a function that is called for each element of input list
@param[in,out] state provided to each invocation of f
@param[in] list input list
@pre "not null", f

<b>Step by step:</b><br/>
f(list[n-1], state, n-1) // modify state<br/>
... <br/>
f(list[1], state, 1) // modify state <br/>
f(list[0], state, 0) // modify state
*/
void l_foldr(List list, AnyFn f, Any state);

/**
Creates a new list with only those elements that satisfy the predicate.
The original list is not modified.
@code{.c}
bool predicate(Any element, int index, Any state) {}
@endcode

@param[in] predicate predicate function, returns true iff element should be included
@param[in] state given to each invocation of predicate (may be NULL)
@param[in] list input list
@return filtered list
@pre "not null", predicate
*/
List l_filter(List list, AnyFn predicate, Any state);

// @todo: add l_choose

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(Any element, int index, Any state) {}
@endcode

@param[in] predicate predicate function
@param[in] state given to each invocation of predicate (may be NULL)
@param[in] list input list
@return true iff at least one element satisfies predicate
@pre "not null", predicate
*/
bool l_exists(List list, AnyFn predicate, Any state);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(Any element, int index, Any state) {}
@endcode

@param[in] predicate predicate function
@param[in] state given to each invocation of predicate (may be NULL)
@param[in] list input list
@return true iff at all the elements satisfy predicate
@pre "not null", predicate
*/
bool l_forall(List list, AnyFn predicate, Any state);

/**
Test for inline lists.
@param[in] ac actual result list
@param[in] ex expected result list
@returns true iff actual equals expected list
*/
#define l_test_equal(ac, ex) \
    l_test_equal_file_line(__FILE__, __func__, __LINE__, ac, ex)

/**
Test for inline lists. Use macro \ref l_test_equal.
@param[in] file source file name
@param[in] function function name
@param[in] line line number
@param[in] ac actual result list
@param[in] ex expected result list
@returns true iff actual equals expected list
*/
bool l_test_equal_file_line(const char *file, const char *function, int line, List ac, List ex);

void l_test_all(void);

#endif
