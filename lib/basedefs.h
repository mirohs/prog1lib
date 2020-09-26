/** @file
Type definitions for Programming I Library. 

@author Michael Rohs
@date 15.10.2015, 26.09.2020
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
Array array = ia_of_string("1, 2, 3, 4, 5, 6");
IntOption op = ia_index_option(array, 0); // index of value 0?
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
Array array = ba_of_string("1, 2, 3, 4, 5, 6");
ByteOption op = ba_index_option(array, 0); // index of value 0?
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
Array array = da_of_string("1, 2, 3, 4, 5, 6");
DoubleOption op = da_index_option(array, 0.5, EPSILON); // index of value 0.5?
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

// Function type declarations

typedef Any (*AnyAnyIntToAny)(Any, Any, int);
typedef Any (*AnyIntAnyAnyToAny)(Any, int, Any, Any);
typedef Any (*AnyIntAnyToAny)(Any, int, Any);
typedef Any (*AnyIntToAny)(Any, int);
typedef bool (*AnyIntAnyAnyToBool)(Any, int, Any, Any);
typedef bool (*AnyIntAnyToBool)(Any, int, Any);
typedef bool (*AnyIntToBool)(Any, int);
typedef bool (*ByteIntAnyToBool)(Byte, int, Any);
typedef bool (*ByteIntByteAnyToBool)(Byte, int, Byte, Any);
typedef bool (*ByteIntByteToBool)(Byte, int, Byte);
typedef bool (*DoubleDoubleDoubleToBool)(double, double, double);
typedef bool (*DoubleDoubleToBool)(double, double);
typedef bool (*DoubleIntDoubleAnyToBool) (double, int, double, Any);
typedef bool (*DoubleIntDoubleToBool)(double, int, double);
typedef bool (*DoubleIntToBool)(double, int);
typedef bool (*DoubleToBool)(double);
typedef bool (*IntIntIntAnyToBool)(int, int, int, Any);
typedef bool (*IntIntIntToBool)(int, int, int);
typedef bool (*IntIntToBool)(int, int);
typedef bool (*IntToBool)(int);
typedef bool (*StringIntAnyToBool)(String, int, Any);
typedef bool (*StringIntStringAnyToBool)(String, int, String, Any);
typedef bool (*StringIntStringToBool)(String, int, String);
typedef bool (*StringIntToBool)(String, int);
typedef Byte (*ByteByteByteIntAnyToByte)(Byte, Byte, Byte, int, Any);
typedef Byte (*ByteByteIntAnyToByte)(Byte, Byte, int, Any);
typedef Byte (*ByteByteIntToByte)(Byte, Byte, int);
typedef Byte (*ByteIntAnyToByte)(Byte, int, Any);
typedef Byte (*ByteIntByteAnyToByte)(Byte, int, Byte, Any);
typedef Byte (*ByteIntByteToByte)(Byte, int, Byte);
typedef Byte (*IntAnyToByte)(int, Any);
typedef Byte (*IntByteToByte)(int, Byte);
typedef ByteOption (*ByteIntByteAnyToByteOption)(Byte, int, Byte, Any);
typedef ByteOption (*ByteIntByteToByteOption)(Byte, int, Byte);
typedef double (*DoubleDoubleDoubleToDouble)(double, double, double);
typedef double (*DoubleDoubleIntToDouble)(double, double, int);
typedef double (*DoubleDoubleToDouble)(double, double);
typedef double (*DoubleIntDoubleAnyToDouble)(double, int, double, Any);
typedef double (*DoubleIntDoubleToDouble)(double, int, double);
typedef double (*DoubleIntToDouble)(double, int);
typedef double (*DoubleToDouble)(double);
typedef double (*IntDoubleToDouble)(int, double);
typedef DoubleOption (*DoubleIntDoubleAnyToDoubleOption) (double, int, double, Any);
typedef DoubleOption (*DoubleIntDoubleToDoubleOption)(double, int, double);
typedef DoubleOption (*DoubleIntToDoubleOption)(double, int);
typedef int (*IntIntAnyToInt)(int, int, Any);
typedef int (*IntIntIntAnyToInt)(int, int, int, Any);
typedef int (*IntIntIntToInt)(int, int, int);
typedef int (*IntIntToInt)(int, int);
typedef int (*IntToInt)(int);
typedef IntOption (*IntIntIntAnyToIntOption)(int, int, int, Any);
typedef IntOption (*IntIntIntToIntOption)(int, int, int);
typedef IntOption (*IntIntToIntOption)(int, int);
typedef String (*IntStringToString)(int, String);
typedef String (*StringIntAnyToString)(String, int, Any);
typedef String (*StringIntStringAnyToString)(String, int, String, Any);
typedef String (*StringIntStringToString)(String, int, String);
typedef String (*StringIntToString)(String, int);
typedef String (*StringStringIntToString)(String, String, int);
typedef String (*StringStringToString)(String, String);
typedef StringOption (*StringIntStringAnyToStringOption)(String, int, String, Any);
typedef StringOption (*StringIntStringToStringOption)(String, int, String);
typedef StringOption (*StringIntToStringOption)(String, int);
typedef void (*AnyAnyAnyAnyIntToVoid)(Any, Any, Any, Any, int);
typedef void (*AnyAnyAnyIntAnyAnyToVoid)(Any, Any, Any, int, Any, Any);
typedef void (*AnyAnyAnyIntAnyToVoid)(Any, Any, Any, int, Any);
typedef void (*AnyAnyAnyIntToVoid)(Any, Any, Any, int);
typedef void (*AnyAnyIntAnyAnyToVoid)(Any, Any, int, Any, Any);
typedef void (*AnyAnyIntAnyToVoid)(Any, Any, int, Any);
typedef void (*AnyAnyIntToVoid)(Any, Any, int);
typedef void (*AnyByteByteByteIntToVoid)(Any, Byte, Byte, Byte, int);
typedef void (*AnyByteByteIntToVoid)(Any, Byte, Byte, int);
typedef void (*AnyByteIntToVoid)(Any, Byte, int);
typedef void (*AnyIntAnyAnyToVoid)(Any, int, Any, Any);
typedef void (*AnyIntAnyToVoid)(Any, int, Any);
typedef void (*AnyIntIntToVoid)(Any, int, int);
typedef void (*AnyIntToVoid)(Any, int);
typedef void (*AnyStringIntToVoid)(Any, String, int);
typedef void (*AnyStringStringToVoid)(Any, String, String);
typedef void (*AnyToVoid)(Any);
typedef void (*ByteAnyIntToVoid)(Byte, Any, int);

/**
Contains information about an array.
*/
typedef struct ArrayHead {
    int n; ///< number of elements
    int s; ///< element size (in bytes)
    Any a; ///< pointer to actual data
} ArrayHead;

typedef struct ArrayHead * Array;

/**
Represents an unspecified list node. The next pointer is the first element of list nodes of any type. @c ListNode does not further specify the kind of data that the node holds.
*/
typedef struct ListNode { 
    struct ListNode *next; ///< next list node (or NULL)
} ListNode;

/**
Contains information about a list.
@see ListNode, IntListNode, DoubleListNode, StringListNode, PointerListNode
*/
typedef struct ListHead { 
    int s; ///< element size (in bytes)
    Any first; ///< pointer to first list node (or NULL); type is ListNode*, IntListNode*, etc.
    Any last; ///< pointer to last list node (or NULL); type is ListNode*, IntListNode*, etc.
} ListHead;

typedef struct ListHead * List;

/**
Represents the state for iterating through a list.
*/
typedef ListNode* ListIterator;

/**
Represents a list node that holds an integer.
*/
typedef struct IntListNode {
    struct IntListNode *next; ///< next list node (or NULL)
    int value; ///< value that the node holds
} IntListNode;

/**
Represents list node that holds a double.
*/
typedef struct DoubleListNode {
    struct DoubleListNode *next; ///< next list node (or NULL)
    double value; ///< value that the node holds
} DoubleListNode;

/**
Represents a list node that holds a String.
*/
typedef struct StringListNode {
    struct StringListNode *next; ///< next list node (or NULL)
    String value; ///< value that the node holds
} StringListNode;

/**
Represents a list node that holds a pointer. The type of the data pointed to is not specified.
*/
typedef struct PointerListNode {
    struct PointerListNode *next; ///< next list node (or NULL)
    Any value; ///< value that the node holds
} PointerListNode;



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

