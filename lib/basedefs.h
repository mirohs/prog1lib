/** @file
Type definitions for Programming I Library. 

@author Michael Rohs
@date 15.10.2015, 26.09.2020, 25.09.2023
@copyright Apache License, Version 2.0
*/

#ifndef __BASEDEFS_H__
#define __BASEDEFS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

/** A Byte represents a single byte of memory. */
typedef unsigned char Byte;

/** A String is a sequence of characters. */
typedef char* String;

/** Represents a pointer to an unspecified type. A pointer to anything. */
typedef void* Any;

/** Represents a pointer to a function of an unspecified type. */
typedef void* AnyFn;

/** Represents a constant pointer to an unspecified type. A variable of this type cannot be modified. */
typedef const void* ConstAny;

/** 
A pair of integers.
@see make_int_pair
*/
typedef struct {
    int i;
    int j;
} IntPair;

/** 
Three integers. 
@see make_int_triple
*/
typedef struct {
    int i;
    int j;
    int k;
} IntTriple;

/** 
A pair of doubles. 
@see make_double_pair
*/
typedef struct {
    double i;
    double j;
} DoublePair;

/** 
Three doubles. 
@see make_double_triple
*/
typedef struct {
    double i;
    double j;
    double k;
} DoubleTriple;

/** 
A pair of Anys. 
@see make_any_pair
*/
typedef struct {
    Any a;
    Any b;
} AnyPair;

/** 
A tuple of three Anys.
@see make_any_triple
*/
typedef struct {
    Any a;
    Any b;
    Any c;
} AnyTriple;

/** 
A pair of Strings. 
@see make_string_pair
*/
typedef struct {
    String a;
    String b;
} StringPair;

/** 
A tuple of three Strings.
@see make_string_triple
*/
typedef struct {
    String a;
    String b;
    String c;
} StringTriple;

/** 
The possible results of comparing two comparable entities a and b: 
- a may be less than b (LT), 
- a may be equal to b (EQ), or 
- a may be greater than b (GT). 
*/
typedef enum {
    LT = -1,    // less than
    EQ = 0,     // equal
    GT = 1      // greater than
} CmpResult;

/**
A comparator is a function that take two (comparable) entities, compares them and returns a comparison result. It is often used with sorting algorithms to encode the sorting criteria.
@param[in] a first entity to compare
@param[in] b second entity to compare
@return comparison result
*/
typedef CmpResult (*Comparator)(ConstAny a, ConstAny b);

// Option types

/**
An integer option represents either an integer or nothing. Option types are typically used with functions that may return a value of the given type or nothing (i.e. the return value is optional). The @c none member is true if the value is not present. Otherwise the value can be accessed with the @c some member.

Example:
@code{.c}
IntOption op = index_of_int_value(array, 123); // index of value 123?
if (op.none) {
    printsln("value not found");
} else {
    printiln(op.some);
}
@endcode

@see make_int_none
@see make_int_some
*/
typedef struct IntOption {
    bool none;
    int some;
} IntOption;

/**
A byte option represents either a byte or nothing. Option types are typically used with functions that may return a value of the given type or nothing (i.e. the return value is optional). The @c none member is true if the value is not present. Otherwise the value can be accessed with the @c some member.

Example:
@code{.c}
ByteOption op = index_of_byte_value(array, 123); // index of value 123?
if (op.none) {
    printsln("value not found");
} else {
    printiln(op.some);
}
@endcode

@see make_byte_none
@see make_byte_some
*/
typedef struct ByteOption {
    bool none;
    Byte some;
} ByteOption;

/**
A double option represents either an double or nothing. Option types are typically used with functions that may return a value of the given type or nothing (i.e. the return value is optional). The @c none member is true if the value is not present. Otherwise the value can be accessed with the @c some member.

Example:
@code{.c}
DoubleOption op = index_of_double_value(array, 0.5, EPSILON); // index of value 0.5?
if (op.none) {
    printsln("value not found");
} else {
    printdln(op.some);
}
@endcode

@see make_double_none
@see make_double_some
*/
typedef struct DoubleOption {
    bool none;
    double some;
} DoubleOption;

/**
A String option represents either a String or nothing. Option types are typically used with functions that may return a value of the given type or nothing (i.e. the return value is optional). The @c none member is true if the value is not present. Otherwise the value can be accessed with the @c some member.

@see make_string_none
@see make_string_some
*/
typedef struct StringOption {
    bool none;
    String some;
} StringOption;

////////////////////////////////////////////////////////////////////////////
// Constructors

/** 
Creates a pair of integers (on the stack).
@param[in] i first element
@param[in] j second element
@return the pair
*/
IntPair make_int_pair(int i, int j);
    
/** 
Creates a pair of integers (on the stack).
@param[in] i first element
@param[in] j second element
@param[in] k third element
@return the triple
*/
IntTriple make_int_triple(int i, int j, int k);

/** 
Creates a pair of doubles (on the stack).
@param[in] i first element
@param[in] j second element
@return the pair
*/
DoublePair make_double_pair(double i, double j);

/** 
Creates a pair of doubles (on the stack).
@param[in] i first element
@param[in] j second element
@param[in] k third element
@return the triple
*/
DoubleTriple make_double_triple(double i, double j, double k);

/** 
Creates a pair of Anys (on the stack).
@param[in] a first element
@param[in] b second element
@return the pair
*/
AnyPair make_any_pair(Any a, Any b);

/** 
Creates a tuple of three Anys (on the stack).
@param[in] a first element
@param[in] b second element
@param[in] c third element
@return the triple
*/
AnyTriple make_any_triple(Any a, Any b, Any c);

/** 
Creates a pair of Strings (on the stack).
@param[in] a first element
@param[in] b second element
@return the pair
*/
StringPair make_string_pair(String a, String b);

/** 
Creates a tuple of three Strings (on the stack).
@param[in] a first element
@param[in] b second element
@param[in] c third element
@return the triple
*/
StringTriple make_string_triple(String a, String b, String c);

/**
Creates a non-present integer option (on the stack).
@return the option value
*/
IntOption make_int_none(void);

/**
Creates an integer option for some value (on the stack).
@param[in] some some value
@return the option value
*/
IntOption make_int_some(int some);

/**
Creates a non-present byte option (on the stack).
@return the option value
*/
ByteOption make_byte_none(void);

/**
Creates a byte option for some value (on the stack).
@param[in] some some value
@return the option value
*/
ByteOption make_byte_some(Byte some);

/**
Creates a non-present double option (on the stack).
@return the option value
*/
DoubleOption make_double_none(void);

/**
Creates a double option for some value (on the stack).
@param[in] some some value
@return the option value
*/
DoubleOption make_double_some(double some);

/**
Creates a non-present String option (on the stack).
@return the option value
*/
StringOption make_string_none(void);

/**
Creates a String option for some value (on the stack).
@param[in] some some value
@return the option value
*/
StringOption make_string_some(String some);

#endif

