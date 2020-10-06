/** @file
A list of strings. 
Stores an arbitrary number of strings. The prefix <code>sl_</code> stands for <i>String list</i>. Some operations are inherited from list.c. For example, <code>l_length</code> works with String lists and any other kind of list.

<b>Example 1: Creating a String list and printing its elements.</b>
@code{.c}
List a = sl_of_string("alpha, beta, gamma, delta, epsilon");
sl_println(a);
sl_free(a);
@endcode

Output 1:
@code{.c}
[alpha, beta, gamma, delta, epsilon]
@endcode

<b>Example 2: Appending and prepending.</b>
@code{.c}
List a2 = sl_create();
sl_append(a2, "b");
sl_append(a2, "c");
sl_prepend(a2, "a");
sl_println(a2);
l_free(a2);
@endcode

Output 2:
@code{.c}
[a, b, c]
@endcode

<b>Example 3: Iteration with loops and get. Inefficient!</b>
@code{.c}
List a3 = sl_of_string("alpha, beta, gamma, delta, epsilon");
int n = l_length(a3);
for (int i = 0; i < n; i++) {
    printsln(sl_get(a3, i));
}
sl_free(a3);
@endcode

Output 3:
@code{.c}
alpha
beta
gamma
delta
epsilon
@endcode

<b>Example 4: Iteration with iterators.</b>
@code{.c}
List a5 = sl_of_string("alpha, beta, gamma, delta, epsilon");
ListIterator iter = l_iterator(a5);
while (l_has_next(iter)) {
    printsln(sl_next(&iter));
}
sl_free(a5);
@endcode

Output 4:
@code{.c}
alpha
beta
gamma
delta
epsilon
@endcode

<b>Example 5: Iteration with loops and list nodes.</b>
@code{.c}
List a4 = sl_of_string("alpha, beta, gamma, delta, epsilon");
for (StringListNode *node = a4->first; node != NULL; node = node->next) {
    printsln(node->value);
}
sl_free(a4);
@endcode

Output 5:
@code{.c}
alpha
beta
gamma
delta
epsilon
@endcode

<b>Example 6: Sorting.</b>
@code{.c}
List a6 = sl_of_string("alpha, beta, gamma, delta, epsilon");
List a7 = sl_sort_dec(a6);
l_free(a6); // free list, but not elements
sl_println(a7);
sl_free(a7); // free list and elements
@endcode

Output 6:
@code{.c}
[gamma, epsilon, delta, beta, alpha]
@endcode

<b>Example 7: Map String to its length (as a String).</b>
@code{.c}
String string2length(String element, int index, String x) {
    // index and x are not used here
    int n = s_length(element);
    return s_of_int(n);
}
...
List a8 = sl_of_string("alpha, beta, gamma, delta, epsilon");
List a9 = sl_map(a8, string2length, NULL);
sl_free(a8); // free list and elements
sl_println(a9);
sl_free(a9); // free list and elements
@endcode

Output 7:
@code{.c}
[5, 4, 5, 5, 7]
@endcode

<b>Example 8: Map String to its length (as an int).</b>
@code{.c}
void string2length2(Any element, int index, Any state, Any mapped_element) {
    // index and state are not used
    String s = *(String*)element;
    int *result = mapped_element;
    *result = s_length(s);
}
...
List a10 = sl_of_string("alpha, beta, gamma, delta, epsilon");
List a11 = l_map(a10, string2length2, sizeof(int), NULL); // result element size is sizeof(int)
sl_free(a10); // free list and elements
il_println(a11); // a11 is an int list
l_free(a11); // free list
@endcode

Output 8:
@code{.c}
[5, 4, 5, 5, 7]
@endcode

<b>Example 9: Total number of characters of all Strings in the list.</b>
@code{.c}
void sum_lengths(Any state, Any element, int index) {
    // index is not used
    int *n = state;
    String s = *(String*)element;
    *n += s_length(s);
}
...
List a12 = sl_of_string("alpha, beta, gamma, delta, epsilon");
int sum = 0;
l_foldl(a12, sum_lengths, &sum);
sl_free(a12); // free list and elements
printiln(sum);
@endcode

Output 9:
@code{.c}
26
@endcode

<b>Example 10: Example 10: Total number of characters of all Strings in the list (with iterator).</b>
@code{.c}
List a13 = sl_of_string("alpha, beta, gamma, delta, epsilon");
int sum = 0;
ListIterator it = l_iterator(a13);
while (l_has_next(it)) {
    String s = sl_next(&it);
    sum += s_length(s);
}
sl_free(a13);
printiln(sum);
@endcode

Output 10:
@code{.c}
26
@endcode

<b>Example 11: Filter Strings starting with "al" and convert them to upper case.</b>
@code{.c}
StringOption starts_with_toupper(String element, int index, String x) {
    if (s_starts_with(element, x)) {
        String s = s_upper_case(element);
        return make_string_some(s);
    }
    return make_string_none();
}
...
List a14 = sl_of_string("alpha, alfred, beta, gamma, alf");
List a15 = sl_choose(a14, starts_with_toupper, "al");
sl_free(a14); // free list and elements
sl_println(a15);
sl_free(a15); // free list and elements
@endcode

Output 11:
@code{.c}
[ALPHA, ALFRED, ALF]
@endcode



See test functions in .c file for more examples.

@author Michael Rohs
@date 15.10.2015, 6.10.2020
@copyright Apache License, Version 2.0
*/

#ifndef __STRING_LIST_H__
#define __STRING_LIST_H__

#include "base.h"


 
/** 
Creates an empty list of Strings. 
@return empty list
*/
List sl_create(void);

#if 0
/** 
Creates a list of n Strings, all initialized to value. 
@param[in] n result length (number of values)
@param[in] value value to repeat
@return the initialized list
@pre "not negative", n >= 0
@pre "not null", value
*/
List sl_repeat(int n, String value);
#endif

/**
Creates a list from the given string.
Use ',' as the separator. The separator may be followed by whitespace.
All the elements in the list are new dynamically allocated Strings.
Example: 
<code>sl_of_string("hello, world, hello")</code> 
creates String list <code>["hello", "world", "hello"]</code>

@param[in] s string representation of String list
@return the initialized list
@pre "not null", s
*/
List sl_of_string(String s);

/**
Creates a list from the given string.
Use separator as the separator.
All the elements in the list are new dynamically allocated Strings.
Example: <code>sl_split("hello+world+moin moin", '+')</code>
creates String list <code>["hello", "world", "moin moin"]</code>

@param[in] s string representation of String list
@param[in] separator this character separates parts of the string
@return the initialized list
@pre "valid separator", separator != '\0'
*/
List sl_split(String s, char separator);

/**
Joins the elements of the String list to one large String, using the given character. The result is dynamically allocated.
@param[in] list string list
@param[in] joiner this character joins the list elements
@return the joined string
@pre "valid joiner", joiner != '\0'
*/
String s_join(List list, char joiner);

/**
Frees the list itself and each element. Assumes that each element is dynamically allocated. If this is not the case or you do not want to free the elements, use @ref l_free
@see l_free
@param[in,out] list to be freed, unusable thereafter
*/
void sl_free(List list);

/**
Returns list element at index.
@param[in] list String list
@param[in] index index of list element to return
@return list element
@pre "valid index", index >= 0 && index < length
*/
String sl_get(List list, int index);

/**
Sets list element at index to value.
@param[in,out] list String list
@param[in] index index of list element to set
@param[in] value value to set
@pre "valid index", index >= 0 && index < length
@pre "not null", value
*/
void sl_set(List list, int index, String value);

/**
Returns the next value.
@param[in,out] iter an iterator, iterator will be advanced to next element
@return next element
@see l_iterator, l_has_next
@pre "iterator has more values", *iter
*/
String sl_next(ListIterator *iter);

/**
Appends value to end of list.
@param[in,out] list String list
@param[in] value value to append
@pre "not null", value
*/
void sl_append(List list, String value);

/**
Prepends value to front of list.
@param[in,out] list String list
@param[in] value value to prepend
@pre "not null", value
*/
void sl_prepend(List list, String value);

/**
Prints the list.
@param[in] list String list
*/
void sl_print(List list);

/**
Prints the list, then print a line break.
@param[in] list String list
*/
void sl_println(List list);

/**
Returns true iff list contains value.
@param[in] list String list
@param[in] value value to look for
@return true if list contains value, false otherwise
@pre "not null", value
*/
bool sl_contains(List list, String value);

/**
Sets each element of list to value.
@param[in,out] list String list
@param[in] value value to set
@pre "not null", value
*/
void sl_fill(List list, String value);

/**
Sets a range of elements of list to value.
Index from is inclusive, index to is exclusive.
@param[in,out] list String list
@param[in] value value to set
@param[in] from start index (inclusive)
@param[in] to end index (exclusive)
@pre "not null", value
*/
void sl_fill_from_to(List list, String value, int from, int to);

/**
Returns index of first occurrence of value in list. 
Returns -1 if value is not in list.
@param[in] list pointer list
@param[in] value value to look for
@return index or -1
@pre "not null", value
*/
int sl_index(List list, String value);

/**
Returns index of first occurrence of value in list at indices [from, n). 
Returns -1 if value is not in list[from, n).
Index from is inclusive.
@param[in] list String list
@param[in] value value to look for
@param[in] from start index (inclusive)
@return index or -1
@pre "not null", value
*/
int sl_index_from(List list, String value, int from);

/**
Returns index of first element for which the predicate function returns true.
Returns -1 if predicate does not return true for any element.
@code{.c}
bool predicate(String element, int index, String x) {}
@endcode
@param[in] list String list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return index or -1
@pre "not null", predicate
*/
int sl_index_fn(List list, StringIntStringToBool predicate, String x);

/**
Returns first element for which the predicate function returns true.
Returns @c NULL if predicate does not return true for any element.
@code{.c}
bool predicate(String element, int index, String x) {}
@endcode
@param[in] list String list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return element or @c NULL
@pre "not null", predicate
*/
String sl_find(List list, StringIntStringToBool predicate, String x);

/**
Sorts the elements in increasing order.
Creates a new list. Does not modify the original list.
@param[in] list String list
@return new sorted list (increasing order).
*/
List sl_sort(List list);

/**
Sorts the elements in decreasing order.
Creates a new list. Does not modify the original list.
@param[in] list String list
@return new sorted list (decreasing order).
*/
List sl_sort_dec(List list);

/**
Inserts value at index in list. 
Does nothing if index is not valid, i.e., if not in interval [0,n].
@param[in,out] list String list
@param[in] index the position to insert at (index 0 means inserting at the front)
@param[in] value value to insert
@pre "not null", value
*/
void sl_insert(List list, int index, String value);

/**
Removes element at index in list.
Does nothing if index is not valid, i.e., if not in interval [0,n).
@param[in,out] list String list
@param[in] index index of element to remove
*/
void sl_remove(List list, int index);

/**
Applies function f to each element of list. The original list is modified (if f modifies the element).
Function f is called once for each element and returns the transformed element.
@code{.c}
String f(String element, int index, String x) {}
@endcode

@param[in,out] list String list
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
void sl_each(List list, StringIntStringToString f, String x);

/**
Applies function f to each element of list. The original list is modified (if f modifies the element).
Function f is called once for each element and returns the transformed element.
@code{.c}
String f(String element, int index, String x, Any state) {}
@endcode

@param[in,out] list String list
@param[in] f a function that is called for each element of input list
@param[in] x provided to each invocation of f
@param[in] state provided to each invocation of f
@pre "not null", f

<b>Step by step:</b><br/>
list[0] := f(list[0], 0, x, state)<br/>
list[1] := f(list[1], 1, x, state)<br/>
...<br/>
list[n-1] := f(list[n-1], n-1, x, state)

@see l_each
*/
void sl_each_state(List list, StringIntStringAnyToString f, String x, Any state);

/**
Applies function f to each element of list. 
The original list is not modified. A new list is created for the result.
Function f is called once for once for each element and returns the transformed element.
@code{.c}
String f(String element, int index, String x) {}
@endcode

@param[in] list String list
@param[in] f transformation function, called for each element of input list
@param[in] x provided to each invocation of f
@return the mapped list
@pre "not null", f

@see l_map
*/
List sl_map(List list, StringIntStringToString f, String x);

/**
Applies function f to each element of list. 
The original list is not modified. A new list is created for the result.
Function f is called once for once for each element and returns the transformed element.
@code{.c}
String f(String element, int index, String x, Any state) {}
@endcode

@param[in] list String list
@param[in] f transformation function, called for each element of input list
@param[in] x provided to each invocation of f
@param[in] state provided to each invocation of f
@return the mapped list
@pre "not null", f

@see l_map
*/
List sl_map_state(List list, StringIntStringAnyToString f, String x, Any state);

/**
Folds list from left to right, i.e., compute f(... f(f(init, l0), l1) ... ln).
@code{.c}
String f(String state, String element, int index) {}
@endcode

@param[in] list String list
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
String sl_foldl(List list, StringStringIntToString f, String state);

/**
Folds list from right to left. I.e., compute f(l0, f(l1,... f(ln, init)...)).
@code{.c}
String f(String element, String state, int index) {}
@endcode

@param[in] list String list
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
String sl_foldr(List list, StringStringIntToString f, String state);

/**
Creates a new list with only those elements of list that satisfy the predicate.
The original list is not modified.
@code{.c}
bool predicate(String element, int index, String x) {}
@endcode

@param[in] list String list
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@return filtered list
@pre "not null", predicate
*/
List sl_filter(List list, StringIntStringToBool predicate, String x);

/**
Creates a new list with only those elements of list that satisfy the predicate.
The original list is not modified.
@code{.c}
bool predicate(String element, int index, String x, Any state) {}
@endcode

@param[in] list String list
@param[in] predicate predicate function, returns true iff element should be included
@param[in] x given to each invocation of predicate
@param[in] state provided to each invocation of predicate
@return filtered list
@pre "not null", predicate
*/
List sl_filter_state(List list, StringIntStringAnyToBool predicate, String x, Any state);

/**
Filters and maps list using f. The original list is not modified.
@code{.c}
StringOption f(String element, int index, String x) {}
@endcode

@param[in] list String list
@param[in] f mapping function, returns the mapped element or <code>none</code> if the element should not be included in the result
@param[in] x given to each invocation of predicate
@return filtered and mapped list
@pre "not null", f

Example:
@code{.c}
static StringOption ends_width_0_append(String element, int index, String x) {
    if (s_ends_with(element, "0")) {
        String s = s_concat(element, x);
        return make_string_some(s);
    }
    return make_string_none();
}
...
List a = sl_of_string("10, 2, 30, 4, 50, 6");
List b = sl_choose(a, ends_width_0_append, "x");
// b is String list ["10x", "30x", "50x"]
@endcode
*/
List sl_choose(List list, StringIntStringToStringOption f, String x);

/**
Filters and maps list using f. The original list is not modified.
@code{.c}
StringOption f(String element, int index, String x, Any state) {}
@endcode

@param[in] list String list
@param[in] f mapping function, returns the mapped element or <code>none</code> if the element should not be included in the result
@param[in] x given to each invocation of predicate
@param[in] state provided to each invocation of f
@return filtered and mapped list
@pre "not null", f
*/
List sl_choose_state(List list, StringIntStringAnyToStringOption f, String x, Any state);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(String element, int index, String x) {}
@endcode

@param[in] list String list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at least one element satisfies predicate
@pre "not null", predicate
*/
bool sl_exists(List list, StringIntStringToBool predicate, String x);

/**
Returns true iff there is at least one element that satisfies the predicate.
@code{.c}
bool predicate(String element, int index, String x) {}
@endcode

@param[in] list String list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@param[in] state provided to each invocation of predicate
@return true iff at least one element satisfies predicate
@pre "not null", predicate
*/
bool sl_exists_state(List list, StringIntStringAnyToBool predicate, String x, Any state);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(String element, int index, String x) {}
@endcode

@param[in] list String list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@return true iff at all the elements satisfy predicate
@pre "not null", predicate
*/
bool sl_forall(List list, StringIntStringToBool predicate, String x);

/**
Returns true iff all the elements satisfy the predicate.
@code{.c}
bool predicate(String element, int index, String x, Any state) {}
@endcode

@param[in] list String list
@param[in] predicate predicate function
@param[in] x given to each invocation of predicate
@param[in] state provided to each invocation of predicate
@return true iff at all the elements satisfy predicate
@pre "not null", predicate
*/
bool sl_forall_state(List list, StringIntStringAnyToBool predicate, String x, Any state);

/**
Test involving String lists.
@param[in] ac actual result list
@param[in] ex expected result list
@returns true iff actual equals expected list
*/
#define sl_test_equal(ac, ex) \
    sl_test_equal_file_line(__FILE__, __func__, __LINE__, ac, ex)

/**
Test involving String lists.
@param[in] file source file name
@param[in] function function name
@param[in] line line number
@param[in] ac actual result list
@param[in] ex expected result list
@returns true iff actual equals expected list
*/
bool sl_test_equal_file_line(const char *file, const char *function, int line, List ac, List ex);

/** 
Checks if list has the right element size. Fails if not.
String s is debug output (e.g., function name).
*/
#undef require_element_size_string
#ifdef NO_CHECK_ELEMENT_SIZE
#define require_element_size_string(list)
#else
#define require_element_size_string(list) \
    require_x("element size string", (list)->s == sizeof(String), "size == %d", (list)->s)
#endif

void sl_test_all(void);

#endif
