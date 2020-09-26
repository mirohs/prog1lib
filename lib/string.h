/** @file
This file contains operations on @c String, such as comparing, getting characters, converting to lower case, etc. Several of these functions dynamically create new strings. The documentation of each function tells whether its result is dynamically allocated. Such strings need to be freed with @ref s_free when no longer needed.

@author Michael Rohs
@date 15.10.2015, 25.09.2020
@copyright Apache License, Version 2.0
*/

#ifndef __STRING_H__
#define __STRING_H__

#include "base.h"

/**
Creates a copy of the given string.
Memory of copy is dynamically allocated.
@param[in] s input string
@return copy of input string
*/
String s_create(String s);

/**
Creates a copy of the given string.
Memory of copy is dynamically allocated.
@param[in] s input string
@return copy of input string
*/
String s_copy(String s);

/**
Frees the memory of a dynamically allocated string.
Fails if s was not dynamically allocated.
@param[in,out] s input string, cannot be used thereafter
*/
void s_free(String s);

/**
Creates a string that consists of @c n copies of character @c c.
Memory of string is dynamically allocated.
@param[in] n length of the resulting string (in characters)
@param[in] c character with which to fill the string with
@return the initialized string
*/
String s_repeat(int n, char c);

/**
Creates a string with characters from the interval [a,b) or (b,a], respectively.
@c a is inclusive and @c b is exclusive. Memory of string is dynamically allocated.

- If a == b, then the resulting array is empty.
- If a < b, then the result is an increasing range.
- If a > b, then the result is a decreasing range.
@param[in] a first character of range (inclusive)
@param[in] b last character of range (exclusive)
@return the initialized string
*/
String s_range(char a, char b);

/**
Creates a substring consisting of s[i, j).
Index @c i is inclusive, index @c j is exclusive.
Memory of string is dynamically allocated.

@param[in] s string to be sub-stringed
@param[in] i start index (inclusive)
@param[in] j end index (exclusive)
@return the substring
*/
String s_sub(String s, int i, int j);

/**
Returns character at index @c i.
@param[in] s input string
@param[in] i index of character to return
@return character at index i
@pre "index in range", i >= 0 && i < length
*/
char s_get(String s, int i);

/** 
Sets s element at index i to value v.
@param[in,out] s input string
@param[in] i index of character to set
@param[in] c character to set
@pre "index in range", i >= 0 && i < length
*/
void s_set(String s, int i, char c);

/**
Copies count characters from @c source_index at @c source to @c destination_index at @c destination.
@param[in] source input string
@param[in] source_index start index at source
@param[out] destination output string
@param[in] destination_index start index at destination
@param[in] count number of characters to copy
*/
void s_blit(String source, int source_index, String destination, int destination_index, int count);

/**
Returns a new string that is the concatenation of @c x and @c y.
The characters of @c x come first, followed by the characters of @c y.
Memory of the new string is dynamically allocated.
@param[in] x first input string
@param[in] y second input string
@return the concatenation of x and y
*/
String s_concat(String x, String y);

/**
Returns the length of the string (number of characters).
@param[in] s input string
@return number of characters
*/
int s_length(String s);

/**
Converts s to lower case. Original string is not modified.
Memory of copy is dynamically allocated.
@param[in] s input string
@return copy of @c s with characters converted to lower case
*/
String s_lower_case(String s);

/**
Converts s to upper case. Original string is not modified.
Memory of copy is dynamically allocated.
@param[in] s input string
@return copy of s with characters converted to upper case
*/
String s_upper_case(String s);

/**
Compares @c s and @c t.
@param[in] s input string
@param[in] t input string
@return result of comparing @c s and @c t
*/
CmpResult s_compare(String s, String t);

/**
Compares @c s and @c t. Ignore case differences.
@param[in] s input string
@param[in] t input string
@return result of comparing @c s and @c t (ignoring case differences)
*/
CmpResult s_compare_ignore_case(String s, String t);

/**
Returns true iff @c s and @c t are equal.
@param[in] s input string
@param[in] t input string
@return true iff @c s and @c t are equal
*/
bool s_equals(String s, String t);

/**
Returns true iff s and t are equal. Ignore case differences.
@param[in] s input string
@param[in] t input string
@return true iff s and t are equal (ignoring case differences)
*/
bool s_equals_ignore_case(String s, String t);

/**
Returns true iff @c s contains part.
@param[in] s input string
@param[in] part input string
@return true iff s contains part
*/
bool s_contains(String s, String part);

/**
Returns true iff @c s starts with start.
@param[in] s input string
@param[in] start input string
@return true iff s starts with start
*/
bool s_starts_with(String s, String start);

/**
Returns true iff @c s ends with @c end.
@param[in] s input string
@param[in] end input string
@return true iff s ends with end
*/
bool s_ends_with(String s, String end);

/**
Returns the index of @c part in @c s. Return -1 if @c part is not in @c s.
@param[in] s input string
@param[in] part input string
@return index of part in s (or -1)
*/
int s_index(String s, String part);

/**
Returns the index of @c part in @c s, starting at @c from. Return -1 of @c part is not in @c s.
@param[in] s input string
@param[in] part input string
@param[in] from start index (inclusive)
@return index of @c part in @c s at or after @c from (or -1)
*/
int s_index_from(String s, String part, int from);

/**
Returns a new string that is @c s reversed.
Memory of new string is dynamically allocated.
@param[in] s input string
@return copy of @c s with characters in reversed order
*/
String s_reverse(String s);

/**
Returns the last index of @c part in @c s. Return -1 if @c part is not in @c s.
@param[in] s input string
@param[in] part string to look for
@return last index of @c part in @c s (or -1)
*/
int s_last_index(String s, String part);

/**
Returns true iff @c s is empty.
@param[in] s input string
@return true iff @c s is empty
*/
bool s_is_empty(String s);

/**
Copies @c s with whitespace removed on both sides.
Memory of new string is dynamically allocated.
@param[in] s input string
@return new string with whitespace removed on both sides
*/
String s_trim(String s);

/**
Returns a new string based on @c s, in which the first occurrence of @c part in @c s is replaced by @c replacement. If @c part does not occur in @c s, then the result is equal to @c s.
Always returns a new dynamically allocated string.
@param[in] s the string in which the replacement is to be made
@param[in] part the string to look for in @c s
@param[in] replacement the string to replace @c part with
*/
String s_replace(String s, String part, String replacement);

/**
Returns a new string based on @c s, in which all occurrences of @c part in @c s are replaced by @c replacement. If @c part does not occur in @c s, then the result is equal to @c s.
Always returns a new dynamically allocated string.
@param[in] s the string in which the replacements are to be made
@param[in] part the string to look for in @c s
@param[in] replacement the string to replace @c part with
*/
String s_replace_all(String s, String part, String replacement);

// split string --> see string list

/**
Returns a string representation of @c i.
Memory of new string is dynamically allocated.
@param[in] i value to convert to a string
@return string representation of @c i
*/
String s_of_int(int i);

/**
Returns a string representation of @c d.
Memory of new string is dynamically allocated.
@param[in] d value to convert to a string
@return string representation of @c d
*/
String s_of_double(double d);

/**
Returns a string representation of @c b.
Memory of new string is dynamically allocated.
@param[in] b value to convert to a string
@return string representation of @c b
*/
String s_of_boolean(bool b);

#endif
