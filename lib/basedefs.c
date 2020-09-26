/*
@author Michael Rohs
@date 26.09.2020
@copyright Apache License, Version 2.0
*/

#include "basedefs.h"

////////////////////////////////////////////////////////////////////////////
// Types (and type constructors)

IntOption make_int_none(void) {
    IntOption op = { true, 0 };
    return op;
}

IntOption make_int_some(int some) {
    IntOption op = { false, some };
    return op;
}

ByteOption make_byte_none(void) {
    ByteOption op = { true, 0 };
    return op;
}

ByteOption make_byte_some(Byte some) {
    ByteOption op = { false, some };
    return op;
}

DoubleOption make_double_none(void) {
    DoubleOption op = { true, 0.0 };
    return op;
}

DoubleOption make_double_some(double some) {
    DoubleOption op = { false, some };
    return op;
}

StringOption make_string_none(void) {
    StringOption op = { true, NULL };
    return op;
}

StringOption make_string_some(String some) {
    StringOption op = { false, some };
    return op;
}

IntPair make_int_pair(int i, int j) {
    IntPair result = { i, j };
    return result;
}

IntTriple make_int_triple(int i, int j, int k) {
    IntTriple result = { i, j, k };
    return result;
}

DoublePair make_double_pair(double i, double j) {
    DoublePair result = { i, j };
    return result;
}

DoubleTriple make_double_triple(double i, double j, double k) {
    DoubleTriple result = { i, j, k };
    return result;
}

AnyPair make_any_pair(Any a, Any b) {
    AnyPair result = { a, b };
    return result;
}

AnyTriple make_any_triple(Any a, Any b, Any c) {
    AnyTriple result = { a, b, c };
    return result;
}

StringPair make_string_pair(String a, String b) {
    StringPair result = { a, b };
    return result;
}

StringTriple make_string_triple(String a, String b, String c) {
    StringTriple result = { a, b, c };
    return result;
}

ListHead make_list_head(int s, ListNode *first, ListNode *last) {
    ListHead result = { s, first, last };
    return result;
}

