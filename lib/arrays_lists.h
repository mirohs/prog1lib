/** @file
Simplified use of generic arrays and lists. See examples below.

<h3>Array Functions</h3>
array, set, get, a_length

<h3>List Functions</h3>
list, append, prepend, l_length, iterator, has_next, next_value

<h3>Array Example 1</h3>

@code{.c}
    Array a = array(int, 2); // create a two-element array
    printiln(a_length(a)); // print the length of the array (2)
    set(int, a, 0, 11); // set the first element to 11
    set(int, a, 1, 22); // set the second element to 22
    // set(int, a, 2, 33); // index out of range
    // set(a, 1, 22); // too few arguments (type missing)
    // set(int, 1, 22); // too few arguments (array missing)
    printiln(get(int, a, 0)); // get the first element (11)
    printiln(get(int, a, 1)); // get the second element (22)
    for (int i = 0; i < a_length(a); i++) { // output the array elements in a loop
        printf("a[%d] = %d\n", i, get(int, a, i));
    }
@endcode

Output:
@code{.c}
2
11
22
c[0] = 11
c[1] = 22
@endcode

<h3>Array Example 2</h3>

@code{.c}
typedef struct {
    int x, y;
} Point;

Point make_point(int x, int y) {
    Point p = { x, y };
    return p;
}

int main(void) {
    Array array = array(Point, 3); // create an array for three Points
    printiln(a_length(array)); // print the length of the array (3)
    for (int i = 0; i < a_length(array); i++) { // set the elements in a loop
        set(Point, array, i, make_point(i + 1, i + 2)); // set the elements (1, 2), (2, 3), (3, 4)
    }
    for (int i = 0; i < a_length(array); i++) {
        Point p = get(Point, array, i); // get the ith Point in the array
        printf("%d, %d\n", p.x, p.y); // print the point
    }
    return 0;
}
@endcode

Output:
@code{.c}
3
1, 2
2, 3
3, 4
@endcode

<h3>List Example 1</h3>

@code{.c}
    List ds = list(double); // create an empty list (for doubles)
    for (int i = 0; i < 5; i++) {
        append(double, ds, i / 2.0); // add a new element at the end
    }
    Iterator iter = iterator(ds); // for iterating through the list
    while (has_next(iter)) { // is there one more element?
        double d = next_value(double, iter); // get the next element
        printf("%.1f\n", d);
    }
@endcode

Output:
@code{.c}
0.0
0.5
1.0
1.5
2.0
@endcode

<h3>List Example 2</h3>

@code{.c}
    List my_list_of_strings = list(String);
    append(String, my_list_of_strings, "world"); // add new element at the end
    prepend(String, my_list_of_strings, "hello"); // add new element to the front
    append(String, my_list_of_strings, "!!!"); // add new element at the end
    Iterator iter = iterator(my_list_of_strings);
    while (has_next(iter)) {
        String value = next_value(String, iter);
        printsln(value);
    }
@endcode

Output:
@code{.c}
hello
world
!!!
@endcode

@author Michael Rohs
@date 21.11.2016
@copyright Apache License, Version 2.0
*/
#ifndef __ARRAYS_LISTS__
#define __ARRAYS_LISTS__

#include "base.h"

/*
Internal function to output a size error.
*/
Any _size_error(int sizeof_element, int sizeof_type, String type, String file, int line) {
    printf("%s, line %d: Wrong type: Size of element is %d bytes, but size of %s is %d bytes.\n",
        file, line, sizeof_element, type, sizeof_type);
    exit(0);
    return NULL;
}

/**
Create an array of the given type and length. Examples:
- Array a = array(double, 5); // create an array of 3 doubles (initialized to 0)
- Array cars = array(Car, 3); // create an array of 4 Cars (a structure)
*/
#define array(_type, _count) \
    (a_create(_count, sizeof(_type)))

/**
Set array element at index position to value. The element type must be given 
as the first argument. Examples:
- set(double, a, 0, 3.14); // set first array element to 3.14
- set(Car, cars, 2, make_car("Fiat 500", 10, 499, 105, 90)); // set third array element
*/
#define set(_type, _array, _index, _value) { \
    if (_array->s != sizeof(_type)) { \
        _size_error(_array->s, sizeof(_type), #_type, __FILE__, __LINE__); \
    } \
    _type _t = _value; \
    a_set(_array, _index, &_t); \
}

/**
Get array element at index. The element type must be given as the first 
argument. Examples:
- double d = get(double, a, 0); // get the first array element
- Car c = get(Car, cars, 2); // get the third array element
*/
#define get(_type, _array, _index) \
    ((_array->s == sizeof(_type)) ? \
    (*(_type*)a_get(_array, _index)) : \
    (*(_type*)_size_error(_array->s, sizeof(_type), #_type, __FILE__, __LINE__)))

/**
Create an empty list for elements of the given type. Examples:
- List a = list(double); // create a list for doubles
- List cars = list(Car); // create a list for Cars (a structure)
*/
#define list(_type) \
    (l_create(sizeof(_type)))

/**
Append a new element to the end of the list. The element type must be 
given as the first argument. Examples:
- append(double, a, 1.23);
- append(Car, cars, make_car("Fiat 500", 10, 499, 105, 90));
*/
#define append(_type, _list, _value) { \
    if (_list->s != sizeof(_type)) { \
        _size_error(_list->s, sizeof(_type), #_type, __FILE__, __LINE__); \
    } \
    _type _t = _value; \
    l_append(_list, &_t); \
}    

/**
Prepend a new element to the front of the list. The element type must be 
given as the first argument. Examples:
- prepend(double, a, 1.23);
- prepend(Car, cars, make_car("Fiat 500", 10, 499, 105, 90));
*/
#define prepend(_type, _list, _value) { \
    if (_list->s != sizeof(_type)) { \
        _size_error(_list->s, sizeof(_type), #_type, __FILE__, __LINE__); \
    } \
    _type _t = _value; \
    l_prepend(_list, &_t); \
}

/*
Internal structure.
*/
typedef struct {
    List list;
    ListIterator iterator;
} Iterator;

/**
Iterators allow iterating through lists. Use the following pattern:
@code{.c}
    Iterator iter = iterator(my_list_of_strings);
    while (has_next(iter)) {
        String value = next_value(String, iter);
        printsln(value);
    }
@endcode
*/
Iterator iterator(List list) {
    Iterator i = { list, l_iterator(list) };
    return i;
}
        
/**
Test whether there is at least one more element to be iterated.
*/
bool has_next(Iterator iterator) {
    return l_has_next(iterator.iterator);
}
        
/**
Return the next value from the iterator. The element type must be 
given as the first argument. Examples:
- double d = next_value(double, iter);
- String s = next_value(String, iter);
- Car c = next_value(Car, iter);
*/
#define next_value(_type, _iterator) \
    ((_iterator.list->s == sizeof(_type)) ? \
    (*(_type*)l_next(&(_iterator.iterator))) : \
    (*(_type*)_size_error(_iterator.list->s, sizeof(_type), #_type, __FILE__, __LINE__)))

///////////////////////////////////////////////////////////////////////////
/**
Eample error messages:

- Array a = array(int, 2);
  set(int, a, 2, 33); // index out of range
- set(a, 1, 22); // too few arguments (type missing)
- set(int, 1, 22); // too few arguments (array missing)
- Array ds = array(double, 4);
  printiln(get(int, ds, 0)); // type error: array contains doubles
- append(int, 10); // error: too few arguments (list missing)

- list = list(Point);
- append(int, list, p); // error: wrong type

Missing arguments:
my.c:101:33: fatal error: too few arguments provided to function-like macro invocation
    set(int, i, 10);
my.c:21:9: note: macro 'set' defined here
    #define set(type, array, index, value) { \

Missing closing bracket:
my.c:153:5: fatal error: unterminated function-like macro invocation
    set(Shape, shapes, 0, make_rect(0, 0, 100, 100, black);

*/
///////////////////////////////////////////////////////////////////////////

#endif
