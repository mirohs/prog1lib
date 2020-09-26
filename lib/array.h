/** @file
A generic array. 
It stores a fixed number of elements. Each element has a size of s bytes. The number of elements and element size have to be specified at creation time. The elements are treated as opaque byte sequences. The prefix @c a_ stands for <i>array</i>.

There are more specific arrays for integers, doubles, strings, and pointers. 
They are more convenient for these types. Some functions are shared between array implementations. For example, \ref a_length works with any kind of array.

@author Michael Rohs
@date 15.10.2015
@copyright Apache License, Version 2.0
*/

#ifndef __ARRAY_H__
#define __ARRAY_H__

#include "base.h"

/**
Creates an array of n elements of size s, all initialized to 0.
The array occupies a block of n * s bytes of memory.
@param[in] n number of elements
@param[in] s element size in bytes
@return zero-initialized array
@pre "non-negative length", n >= 0
@pre "positive size", s > 0
*/
Array a_create(int n, int s);

/**
Creates an array of n elements, each of size s, by copying n * s bytes from buffer.
The array occupies a new dynamically allocated block of n * s bytes of memory.
@param[in] buffer the buffer to copy the elements from
@param[in] n number of elements
@param[in] s element size in bytes
@return array of n copied elements, each of size s
@pre "non-negative length", n >= 0
@pre "positive size", s > 0
*/
Array a_of_buffer(Any buffer, int n, int s);

/**
Creates an array of n elements of size s, each initialized with function init.
The array occupies a block of n * s bytes of memory.

@code{.c}
void init(Any element, int index, Any state) {}
@endcode

@param[in] n number of elements
@param[in] s element size in bytes
@param[in] init initialization function, will be called for each index [0, n-1)
@param[in] state state will be supplied to init
@pre "non-negative length", n >= 0
@pre "positive size", s > 0

Example:
@code{.c}
static void x_and_xsquare(Any element, int index, Any state) {
    IntPair *ip = element;
    ip->i = index;
    ip->j = indexindex;
}
...
Array ac = a_fn(4, sizeof(IntPair), x_and_xsquare, NULL);
// set element 0 to (0, 0)
// set element 1 to (1, 1)
// set element 2 to (2, 4)
// set element 3 to (3, 9)
@endcode
*/
Array a_fn(int n, int s, AnyFn init, Any state);

/**
Creates a copy of the given array.
The bytes of the array elements are copied.
@param[in] array to be copied
@return the copy of the array
*/
Array a_copy(Array array);

/**
Creates a new subarray consisting of array[i, j).
Index i is inclusive, index j is exclusive.
@param[in] array to be sub-arrayed
@param[in] i start index (inclusive)
@param[in] j end index (exclusive)
@return the subarray
*/
Array a_sub(Array array, int i, int j);

/**
Creates a new array by copying the elements of the list.
@param[in] list the elements of the list will be copied
@return the new array
*/
Array a_of_l(List list);

/**
Copies count elements from source to destination starting from source_index and destination_index, respectively.
@param[in] source source array
@param[in] source_index start index in source array
@param[in,out] destination destination array
@param[in] destination_index start index in destination array
@param[in] count number of elements (not bytes!) to copy
@pre "equal element sizes", source->s == destination->s
@pre "source_index in range", source_index >= 0 && source_index < source->n
@pre "destination_index in range", destination_index >= 0 && destination_index < destination->n
@pre "(source_index + count) in range", source_index + count <= source->n
@pre "(destination_index + count) in range", destination_index + count <= destination->n
*/
void a_blit(Array source, int source_index, Array destination, int destination_index, int count);

/**
Frees the memory of the array.
@param[in,out] array to be freed, unusable thereafter
*/
void a_free(Array array);

/**
Returns the memory address of the array element at index.
@param[in] array input array
@param[in] index index of array element to return
@return address of array element
@pre "index in range", index >= 0 && index < array->n
*/
Any a_get(Array array, int index);

/**
Sets array element at index to value.
Copies a_element_size(array) bytes from value.
@param[in,out] array input array
@param[in] index index of array element to set
@param[in] value address of the memory to copy
@pre "index in range", index >= 0 && index < array->n
*/
void a_set(Array array, int index, Any value);
    
/**
Returns the number of elements of the array (not the number of bytes!).
@param[in] array input array
@return number of elements of array
*/
int a_length(Array array);

/**
Returns the size in bytes of an element of array.
@param[in] array input array
@return size of array elements
*/
int a_element_size(Array array);

/**
Prints the array using the function to print each element.
@code{.c}
void print_element(Any element) {}
@endcode
@param[in] array input array
@param[in] print_element function to print a single element, will be called for each element
*/
void a_print(Array array, AnyFn print_element);

/**
Prints the array using the function to print each element, then print a line break.
@code{.c}
void print_element(Any element) {}
@endcode
@param[in] array input array
@param[in] print_element function to print a single element, will be called for each element
*/
void a_println(Array array, AnyFn print_element);

/**
Returns true iff the given arrays are equal. Performs a bytewise comparison on the data.
@param[in] a first input array
@param[in] b second input array
@return true if arrays are equal, false otherwise
*/
bool a_equals(Array a, Array b);

/**
Returns a new array that is the concatenation of x and y. Does not modify x or y.
The elements of x come first, followed by the elements of y.
@param[in] x first input array
@param[in] y second input array
@return the concatenation of x and y
@pre "equal element sizes", x->s == y->s
*/
Array a_concat(Array x, Array y);

/**
Returns index of first element for which the predicate function returns true.
Returns -1 if predicate does not return true for any element.
@code{.c}
bool predicate(Any element, int index, Any state) {}
@endcode
@param[in] array input array
@param[in] predicate predicate function
@param[in] state given to each invocation of predicate (may be NULL)
@return index or -1
*/
int a_index_fn(Array array, AnyFn predicate, Any state);

/**
Returns index of last element for which the predicate function returns true.
Returns -1 if predicate does not return true for any element.
@code{.c}
bool predicate(Any element, int index, Any state) {}
@endcode
@param[in] array input array
@param[in] predicate predicate function
@param[in] state given to each invocation of predicate (may be NULL)
@return index or -1
*/
int a_last_index_fn(Array array, AnyFn predicate, Any state);

/*
Returns the address of the first element for which the predicate function returns true.
Returns NULL if predicate does not return true for any element.
@code{.c}
bool predicate(Any element, int index, Any state) {}
@endcode

@param[in] array input array
@param[in] predicate predicate function
@param[in] state given to each invocation of predicate (may be NULL)
@return element address or NULL
*/
//@todo: Any l_find(List array, AnyIntAnyToBool predicate, Any state);

/**
Reverses the array. Modifies the array.
@param[in,out] array input array
*/
void a_reverse(Array array);

/**
Randomly rearranges the elements of array. Modifies the array.
@param[in,out] array input array
*/
void a_shuffle(Array array);

/**
Sorts the elements using the given comparator function. Modifies the array.

@param[in,out] array input array
@param[in] c comparator function to compare two elements

Example:
@code{.c}
static CmpResult my_int_comparator(ConstAny a, ConstAny b) {
    int x = *(int*)a;
    int y = *(int*)b;
    return (x < y) ? LT : ((x > y) ? GT : EQ);
}
...
Array a = ia_of_string("4, 1, 3, 2, 5");
a_sort(a, my_int_comparator);
ia_println(a);
a_free(a);
@endcode
*/
void a_sort(Array array, Comparator c);

/**
Applies function f to each element of array. The original array is not modified.
Function f is called once for each element from first to last.
@code{.c}
void f(Any element, int index, Any state, Any mapped_element) {}
@endcode

@param[in] array input array
@param[in] f transformation function, called for each element of input array
@param[in] mapped_element_size size of elements in the mapped array (in bytes)
@param[in] state provided to each invocation of f
@return the mapped array
@pre "positive size", mapped_element_size > 0
*/
Array a_map(Array array, AnyFn f, int mapped_element_size, Any state);

#if 0
/**
Applies function f to each element of arrays. The original arrays are not modified.
Function f is called once for each index from first to last.
@code{.c}
void f(Any element1, Any element2, int index, Any state, Any mapped_element) {}
@endcode

@param[in] a1 input array 1
@param[in] a2 input array 2
@param[in] f transformation function, called for each element of input arrays
@param[in] mapped_element_size size of elements in the mapped array (in bytes)
@param[in] state provided to each invocation of f
@return the mapped array
*/
Array a_map2(Array a1, Array a2, AnyFn f, int mapped_element_size, Any state);

/**
Applies function f to each element of arrays. The original arrays are not modified.
Function f is called once for each index from first to last.
@code{.c}
void f(Any element1, Any element2, Any element3, int index, Any state, Any mapped_element) {}
@endcode

@param[in] a1 input array 1
@param[in] a2 input array 2
@param[in] a3 input array 3
@param[in] f transformation function, called for each element of input arrays
@param[in] mapped_element_size size of elements in the mapped array (in bytes)
@param[in] state provided to each invocation of f
@return the mapped array
*/
Array a_map3(Array a1, Array a2, Array a3, AnyFn f, int mapped_element_size, Any state);
#endif

/**
Applies function f to each element of array. The original array is modified (if f modifies the element).
Function f is called once for each element from first to last.
@code{.c}
void f(Any element, int index, Any state) {}
@endcode

@param[in,out] array input array
@param[in] f a function that is called for each element of input array
@param[in] state provided to each invocation of f

<b>Step by step:</b><br/>
f(array[0], 0, state)<br/>
f(array[1], 1, state)<br/>
...<br/>
f(array[n-1], n-1, state)
*/
void a_each(Array array, AnyFn f, Any state);

#if 0
/**
Applies function f to each element of array. The original array is modified (if f modifies the elements).
Function f is called once for each index from first to last.
@code{.c}
void f(Any element1, Any element2, int index, Any state) {}
@endcode
@param[in,out] a1 input array 1
@param[in,out] a2 input array 2
@param[in] f a function that is called for each element of input arrays
@param[in] state provided to each invocation of f

<b>Step by step:</b><br/>
f(a1[0], a2[0], 0, state)<br/>
f(a1[1], a2[1], 1, state)<br/>
... <br/>
f(a1[n-1], a2[n-1], n-1, state)
*/
void a_each2(Array a1, Array a2, AnyFn f, Any state);

/**
Applies function f to each element of array. The original array is modified (if f modifies the elements).
Function f is called once for each index from first to last.
@code{.c}
void f(Any element1, Any element2, Any element3, int index, Any state) {}
@endcode

@param[in,out] a1 input array 1
@param[in,out] a2 input array 2
@param[in,out] a3 input array 3
@param[in] f a function that is called for each element of input arrays
@param[in] state provided to each invocation of f

<b>Step by step:</b><br/>
f(a1[0], a2[0], a3[0], 0, state)<br/>
f(a1[1], a2[1], a3[1], 1, state)<br/>
... <br/>
f(a1[n-1], a2[n-1], a3[n-1], n-1, state)
*/
void a_each3(Array a1, Array a2, Array a3, AnyFn f, Any state);
#endif

/**
Folds the array from left to right.
@code{.c}
void f(Any state, Any element, int index) {}
@endcode

@param[in] array input array
@param[in] f a function that is called for each element of input array
@param[in,out] state provided to each invocation of f

<b>Step by step:</b><br/>
f(state, array[0], 0) // modify state<br/>
f(state, array[1], 1) // modify state <br/>
... <br/>
f(state, array[n-1], n-1) // modify state
*/
void a_foldl(Array array, AnyFn f, Any state);

#if 0
/**
Folds the arrays from left to right.
@code{.c}
void f(Any state, Any element1, Any element2, int index) {}
@endcode

@param[in] a1 input array 1
@param[in] a2 input array 2
@param[in] f a function that is called for each element of input arrays
@param[in,out] state provided to each invocation of f

<b>Step by step:</b><br/>
f(state, a1[0], a2[0], 0) // modify state<br/>
f(state, a1[1], a2[1], 1) // modify state <br/>
... <br/>
f(state, a1[n-1], a2[n-1], n-1) // modify state
*/
void a_foldl2(Array a1, Array a2, AnyFn f, Any state);

/**
Folds the arrays from left to right.
@code{.c}
void f(Any state, Any element1, Any element2, Any element3, int index) {}
@endcode

@param[in] a1 input array 1
@param[in] a2 input array 2
@param[in] a3 input array 3
@param[in] f a function that is called for each element of input arrays
@param[in,out] state provided to each invocation of f

<b>Step by step:</b><br/>
f(state, a1[0], a2[0], a3[0], 0) // modify state<br/>
f(state, a1[1], a2[1], a3[1], 1) // modify state <br/>
... <br/>
f(state, a1[n-1], a2[n-1], a3[n-1], n-1) // modify state
*/
void a_foldl3(Array a1, Array a2, Array a3, AnyFn f, Any state);
#endif

/**
Folds the array from right to left.
@code{.c}
void f(Any element, Any state, int index) {}
@endcode

@param[in] array input array
@param[in] f a function that is called for each element of input array
@param[in,out] state provided to each invocation of f

<b>Step by step:</b><br/>
f(array[n-1], state, n-1) // modify state<br/>
... <br/>
f(array[1], state, 1) // modify state <br/>
f(array[0], state, 0) // modify state
*/
void a_foldr(Array array, AnyFn f, Any state);

/**
Creates a new array with only those elements that satisfy the predicate.
The original array is not modified.
@code{.c}
bool predicate(Any element, int index, Any state) {}
@endcode

@param[in] array input array
@param[in] predicate predicate function, returns true iff element should be included
@param[in] state given to each invocation of predicate (may be NULL)
@return filtered array
*/
Array a_filter(Array array, AnyFn predicate, Any state);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(Any element, int index, Any state) {}
@endcode

@param[in] array input array
@param[in] predicate predicate function
@param[in] state given to each invocation of predicate (may be NULL)
@return true iff at least one element satisfies predicate
*/
bool a_exists(Array array, AnyFn predicate, Any state);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(Any element, int index, Any state) {}
@endcode

@param[in] array input array
@param[in] predicate predicate function
@param[in] state given to each invocation of predicate (may be NULL)
@return true iff at all the elements satisfy predicate
*/
bool a_forall(Array array, AnyFn predicate, Any state);

/**
Tests for inline arrays.
@param[in] ac actual result array
@param[in] ex expected result array
@returns true iff actual equals expected array
*/
#define a_test_equal(ac, ex) \
    a_test_equal_file_line(__FILE__, __func__, __LINE__, ac, ex)

/**
Tests for inline arrays. Use macro \ref a_test_equal.
@param[in] file source file name
@param[in] function function name
@param[in] line line number
@param[in] ac actual result array
@param[in] ex expected result array
@returns true iff actual equals expected array
*/
bool a_test_equal_file_line(const char *file, const char *function, int line, Array ac, Array ex);

void a_test_all(void);

#endif
