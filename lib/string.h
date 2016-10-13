/** @file
This file contains operations on @c String, such as comparing, getting characters, converting to lower case, etc. Several of these functions dynamically create new strings. The documentation of each function tells whether its result is dynamically allocated. Such strings need to be freed with @ref s_free when no longer needed.

@author Michael Rohs
@date 15.10.2015
@copyright Apache License, Version 2.0
*/

#ifndef __STRING_H__
#define __STRING_H__

#include "base.h"

/**
Create a copy of the given string.
Memory of copy is dynamically allocated.
@param[in] s input string
@return copy of input string
*/
String s_create(String s);

/**
Create a copy of the given string.
Memory of copy is dynamically allocated.
@param[in] s input string
@return copy of input string
*/
String s_copy(String s);

/**
Free the memory of a dynamically allocated string.
Fails if s was not dynamically allocated.
@param[in,out] s input string, cannot be used thereafter
*/
void s_free(String s);

/**
Create a string that consists of n copies of character c. 
Memory of string is dynamically allocated.
@param[in] n length of the resulting string (in characters)
@param[in] c character with which to fill the string with
@return the initialized string
*/
String s_repeat(int n, char c);

/**
Create a string with characters from the interval [a,b) or (b,a], respectively. 
a is inclusive and b is exclusive. Memory of string is dynamically allocated.

- If a == b, then the resulting array is empty.
- If a < b, then the result is an increasing range.
- If a > b, then the result is a decreasing range.
@param[in] a first character of range (inclusive)
@param[in] b last character of range (exclusive)
@return the initialized string
*/
String s_range(char a, char b);

/**
Create a substring consisting of s[i, j).
Index i is inclusive, index j is exclusive.
Memory of string is dynamically allocated.

@param[in] s string to be sub-stringed
@param[in] i start index (inclusive)
@param[in] j end index (exclusive)
@return the substring
*/
String s_sub(String s, int i, int j);

/**
Return character at index i.
@param[in] s input string
@param[in] i index of character to return
@return character at index i
*/
char s_get(String s, int i);

/** 
Set s element at index i to value v.
@param[in,out] s input string
@param[in] i index of character to set
@param[in] c character to set
*/
void s_set(String s, int i, char c);

/**
Copy count characters from source_index at source to destination_index at destination. 
@param[in] source input string
@param[in] source_index start index at source
@param[out] destination output string
@param[in] destination_index start index at destination
@param[in] count number of characters to copy
*/
void s_blit(String source, int source_index, String destination, int destination_index, int count);

/**
Return a new string that is the concatenation of x and y. 
The characters of x come first, followed by the characters of y.
Memory of the new string is dynamically allocated.
@param[in] x first input string
@param[in] y second input string
@return the concatenation of x and y
*/
String s_concat(String x, String y);

/**
Return the length of the string (number of characters).
@param[in] s input string
@return number of characters
*/
int s_length(String s);

/**
Convert s to lower case. Original s is not modified.
Memory of copy is dynamically allocated.
@param[in] s input string
@return copy of s with characters converted to lower case
*/
String s_lower_case(String s);

/**
Convert s to upper case. Original s is not modified.
Memory of copy is dynamically allocated.
@param[in] s input string
@return copy of s with characters converted to upper case
*/
String s_upper_case(String s);

/**
Compare s and t.
@param[in] s input string
@param[in] t input string
@return result of comparing s and t
*/
CmpResult s_compare(String s, String t);

/**
Compare s and t. Ignore case differences.
@param[in] s input string
@param[in] t input string
@return result of comparing s and t (ignoring case differences)
*/
CmpResult s_compare_ignore_case(String s, String t);

/**
Return true iff s and t are equal.
@param[in] s input string
@param[in] t input string
@return true iff s and t are equal
*/
bool s_equals(String s, String t);

/**
Return true iff s and t are equal. Ignore case differences.
@param[in] s input string
@param[in] t input string
@return true iff s and t are equal (ignoring case differences)
*/
bool s_equals_ignore_case(String s, String t);

/**
Return true iff s contains part.
@param[in] s input string
@param[in] part input string
@return true iff s contains part
*/
bool s_contains(String s, String part);

/**
Return true iff s starts with start.
@param[in] s input string
@param[in] start input string
@return true iff s starts with start
*/
bool s_starts_with(String s, String start);

/**
Return true iff s ends with end.
@param[in] s input string
@param[in] end input string
@return true iff s ends with end
*/
bool s_ends_with(String s, String end);

/**
Return the index of part in s. Return -1 of part is not in s.
@param[in] s input string
@param[in] part input string
@return index of part in s (or -1)
*/
int s_index(String s, String part);

/**
Return the index of part in s, starting at from. Return -1 of part is not in s.
@param[in] s input string
@param[in] part input string
@param[in] from start index (inclusive)
@return index of part in s at or after from (or -1)
*/
int s_index_from(String s, String part, int from);

/**
Return a new string that is s reversed.
Memory of new string is dynamically allocated.
@param[in] s input string
@return copy of s with characters in reversed order
*/
String s_reverse(String s);

/**
Return the last index of part in s. Return -1 of part is not in s.
@param[in] s input string
@param[in] part string to look for
@return last index of part in s (or -1)
*/
int s_last_index(String s, String part);

/**
Return true iff s is empty.
@param[in] s input string
@return true iff s is empty
*/
bool s_is_empty(String s);

/**
Copy s with whitespace removed on both sides.
Memory of new string is dynamically allocated.
@param[in] s input string
@return new string with whitespace removed on both sides
*/
String s_trim(String s);

// @todo: replace, replace_all

// split string --> see string list

/**
Return a string representation of i.
Memory of new string is dynamically allocated.
@param[in] i value to convert to a string
@return string representation of i
*/
String s_of_int(int i);

/**
Return a string representation of d.
Memory of new string is dynamically allocated.
@param[in] d value to convert to a string
@return string representation of d
*/
String s_of_double(double d);

/**
Return a string representation of b.
Memory of new string is dynamically allocated.
@param[in] b value to convert to a string
@return string representation of b
*/
String s_of_boolean(bool b);

#endif
