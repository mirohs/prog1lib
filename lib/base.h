/*
base +
string +
array +
  int_array + 
  double_array +
  string_array +
  pointer_array +
  byte_array +
list +
  int_list +
  double_list +
  string_list +
  pointer_list +
*/

/** @file
Base functions for Programming I Library. Here you find basic functions for output, input, files, random numbers, dynamic memory allocation, timing, assertions, and testing.

<h3>Naming Conventions</h3>

Types names are written in upper camel case, such as @c String or @c StringList, except for predefined type names such as @c int, @c double, and @c bool. This library does not cover type @c float. This library does not deal with @c unsigned types.

Function names are written in lower case. Parts within function names are separated with an underscore character, such as in @c base_init. This makes function names distinguishable from type names.

Constants, such as @c NULL or @c EQ, are written in upper case. This is independent of whether they have been defined as C constants or with preprocessor @c define statements.

<h3>Type Declarations</h3>

The type @c String is simply defined as a pointer to a character:
@code{.c}
typedef char* String;
@endcode

There are several type declarations for functions. Example: 
@code{.c}
typedef String (*IntStringToString)(int, String);
@endcode
The type @c IntStringToString represents a pointer to a function that takes an integer and a String and returns a String.

<h3>Dynamic Memory</h3>

Memory is allocated using @ref xmalloc or @ref xcalloc and released with @ref free. These functions (in fact: macros) keep track of allocated memory and report memory leaks. Make sure to always use @ref xmalloc or @ref xcalloc rather than @c malloc and @c calloc in your code.

<h3>Examples</h3>

<h4>Example 1: Printing an integer</h4>
@code{.c}
printiln(123); // print an integer followed by a line break
@endcode

Output:
@code{.c}
123
@endcode


<h4>Example 2: Reading a text file into a list of strings</h4>
Let @c example.txt be
@code{.c}
line 1
line II
my line 3
last line
@endcode
To read this file into a list of strings:
@code{.c}
String s = s_read_file("example.txt"); // read the complete file
List sl = sl_split(s, '\n'); // split file contents into lines
s_free(s); // free the string
sl_println(sl); // print the list of strings
sl_free(sl); // free the list and its elements
@endcode

Output:
@code{.c}
[line 1, line II, my line 3, last line]
@endcode

<h4>Example 2: Write a list of strings to a text file</h4>
@code{.c}
List list = sl_create();
sl_append(list, "line 1");
sl_append(list, "line II");
sl_append(list, "my line 3");
sl_append(list, "last line");
String s = s_join(list, '\n');
l_free(list); // free list
s_write_file("example.txt", s);
s_free(s); // free joined string
@endcode

Output (into file @c example.txt):
@code{.c}
line 1
line II
my line 3
last line
@endcode

<h4>Example 3: Write 100 random double values between 0 and 10 to a file</h4>
@code{.c}
List list = sl_create();
for (int i = 0; i < 100; i++) {
    double d = d_rnd(10);
    sl_append(list, s_of_double(d));
}
String s = s_join(list, '\n');
sl_free(list); // free list and elements
s_write_file("random-doubles.txt", s);
s_free(s); // joined string
@endcode

Output (into file @c random-doubles.txt):
@code{.c}
8.650181371707539
...
2.228865998398248
@endcode



See test functions in .c file for more examples.

@author Michael Rohs
@date 15.10.2015, 22.09.2020
@copyright Apache License, Version 2.0
*/

#ifndef __BASE_H__
#define __BASE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include "basedefs.h"


////////////////////////////////////////////////////////////////////////////
// Initialization

/** 
Initialization function should be the first statement in main.
Enables memory allocation tracking.
*/
void base_init(void);

/**     
Switch on or off memory checking at termination of the programx.
@param[in] do_check if \c true, then memory is checked
*/
void base_set_memory_check(bool do_check);



////////////////////////////////////////////////////////////////////////////
// Constructors

/** 
Create a pair of integers (on the stack).
@param[in] i first element
@param[in] j second element
@return the pair
*/
IntPair make_int_pair(int i, int j);
    
/** 
Create a pair of integers (on the stack).
@param[in] i first element
@param[in] j second element
@param[in] k third element
@return the triple
*/
IntTriple make_int_triple(int i, int j, int k);

/** 
Create a pair of doubles (on the stack).
@param[in] i first element
@param[in] j second element
@return the pair
*/
DoublePair make_double_pair(double i, double j);

/** 
Create a pair of doubles (on the stack).
@param[in] i first element
@param[in] j second element
@param[in] k third element
@return the triple
*/
DoubleTriple make_double_triple(double i, double j, double k);

/** 
Create a pair of Anys (on the stack).
@param[in] a first element
@param[in] b second element
@return the pair
*/
AnyPair make_any_pair(Any a, Any b);

/** 
Create a tuple of three Anys (on the stack).
@param[in] a first element
@param[in] b second element
@param[in] c third element
@return the triple
*/
AnyTriple make_any_triple(Any a, Any b, Any c);

/** 
Create a pair of Strings (on the stack).
@param[in] a first element
@param[in] b second element
@return the pair
*/
StringPair make_string_pair(String a, String b);

/** 
Create a tuple of three Strings (on the stack).
@param[in] a first element
@param[in] b second element
@param[in] c third element
@return the triple
*/
StringTriple make_string_triple(String a, String b, String c);

/**
Create a non-present integer option (on the stack).
@return the option value
*/
IntOption make_int_none(void);

/**
Create an integer option for some value (on the stack).
@param[in] some some value
@return the option value
*/
IntOption make_int_some(int some);

/**
Create a non-present byte option (on the stack).
@return the option value
*/
ByteOption make_byte_none(void);

/**
Create a byte option for some value (on the stack).
@param[in] some some value
@return the option value
*/
ByteOption make_byte_some(Byte some);

/**
Create a non-present double option (on the stack).
@return the option value
*/
DoubleOption make_double_none(void);

/**
Create a double option for some value (on the stack).
@param[in] some some value
@return the option value
*/
DoubleOption make_double_some(double some);

/**
Create a non-present String option (on the stack).
@return the option value
*/
StringOption make_string_none(void);

/**
Create a String option for some value (on the stack).
@param[in] some some value
@return the option value
*/
StringOption make_string_some(String some);



////////////////////////////////////////////////////////////////////////////
// Assertions

/**
Switching assertion checking on and off.
If @cn NO_ASSERT is defined, then assertions will not be compiled.
*/
#define NO_ASSERT_DOC

/**
Switching assertion checking on and off.
If @cn NO_REQUIRE is defined, then preconditions will not be compiled.
*/
#define NO_REQUIRE_DOC

/**
Switching assertion checking on and off.
If @cn NO_ENSURE is defined, then postconditions will not be compiled.
*/
#define NO_ENSURE_DOC



#ifdef NO_ASSERT
#define assert(description, condition)
#else
/**
Check the given condition. If the condition is true, do nothing. If the condition is false, report the file and line of the assertion  and stop the program. Assertions are used to check for conditions that have to be valid at a particular point.

Example use of an assertion:
@code{.c}
    ...
    assert("not too large", x < 3);
    ...
@endcode

Example output of failed preconditions:

    myfile.c, line 18: assertion "not too large" (x < 3) violated
    myfile.c, line 18: assertion "sorted" (forall(int i = 0, i < n-1, i++, a[i] <= a[i+1])) violated

@param[in] description String a description of the condition that has to be valid
@param[in] condition boolean the condition to check
*/
#define assert(description, condition) \
if (!(condition)) {\
    fprintf(stderr, "%s, line %d: assertion \"%s\" (%s) violated\n", __FILE__, __LINE__, description, #condition);\
    exit(EXIT_FAILURE);\
}
#endif

#ifdef NO_ASSERT
#define assert_x(description, condition)
#else
/**
Check the given condition. If the condition is true, do nothing. If the condition is false, report the file and line of the assertion  and stop the program. Assertions are used to check for conditions that have to be valid at a particular point.

Example use of an assertion:
@code{.c}
    ...
    assert_x("not too large", x < 3, "x == %d", x);
    ...
@endcode

Example output of failed preconditions:

    myfile.c, line 18: assertion "not too large" violated: x == 3
    myfile.c, line 18: assertion "sorted" violated: a[2] == 5 && a[3] == 4

@param[in] description String a description of the condition that has to be valid
@param[in] condition boolean the condition to check
*/
#define assert_x(description, condition, ...) \
if (!(condition)) {\
    fprintf(stderr, "%s, line %d: assertion \"%s\" violated: ", __FILE__, __LINE__, description);\
    fprintf(stderr, __VA_ARGS__);\
    fprintf(stderr, "\n");\
    exit(EXIT_FAILURE);\
}
#endif



#ifdef NO_REQUIRE
#define require(description, condition)
#else
/**
Check the given precondition. If the condition is true, do nothing. If the condition is false, report the location of the precondition and stop the program. A precondition is a special type of assertion that has to be valid at the beginning of a function.

Example use of a precondition:
@code{.c}
    int myfunction(int x) {
        require("not too large", x < 3);
        ...
    }
@endcode

Example output of failed preconditions:

    myfile.c, line 18: myfunction's precondition "not too large" (x < 3) violated
    myfile.c, line 18: myfunction's precondition "sorted" (forall(int i = 0, i < n-1, i++, a[i] <= a[i+1])) violated

@param[in] description String a description of the condition that has to be valid
@param[in] condition boolean the condition to check
*/
#define require(description, condition) \
if (!(condition)) {\
    fprintf(stderr, "%s, line %d: %s's precondition \"%s\" (%s) violated\n", __FILE__, __LINE__, __func__, description, #condition);\
    exit(EXIT_FAILURE);\
}
#endif

#ifdef NO_REQUIRE
#define require_x(description, condition)
#else
/**
Check the given precondition. If the condition is true, do nothing. If the condition is false, report the location of the precondition and stop the program. A precondition is a special type of assertion that has to be valid at the beginning of a function.

Example use of a precondition:
@code{.c}
    int myfunction(int x) {
        require_x("not too large", x < 3, "x == %d", x);
        ...
    }
@endcode

Example output of failed preconditions:

    myfile.c, line 18: myfunction's precondition "not too large" violated: x == 3
    myfile.c, line 18: myfunction's precondition "sorted" violated: a[2] == 5 && a[3] == 4

@param[in] description String a description of the condition that has to be valid
@param[in] condition boolean the condition to check
*/
#define require_x(description, condition, ...) \
if (!(condition)) {\
    fprintf(stderr, "%s, line %d: %s's precondition \"%s\" violated: ", __FILE__, __LINE__, __func__, description);\
    fprintf(stderr, __VA_ARGS__);\
    fprintf(stderr, "\n");\
    exit(EXIT_FAILURE);\
}
#endif

#ifdef NO_REQUIRE
#define require_not_null(argument)
#else
/**
Check that the given argument is not NULL. If so, do nothing. Otherwise report the location of the precondition and stop the program. A precondition is a special type of assertion that has to be valid at the beginning of a function.

Example use of a precondition:
@code{.c}
    int myfunction(String s) {
        require_not_null(s);
        ...
    }
@endcode

Example output of failed preconditions:

    myfile.c, line 18: myfunction's precondition "not null" (s) violated

@param[in] argument pointer a pointer that must not be null
*/
#define require_not_null(argument) \
if (argument == NULL) {\
    fprintf(stderr, "%s, line %d: %s's precondition \"not null\" (" #argument ") violated\n", __FILE__, __LINE__, __func__);\
    exit(EXIT_FAILURE);\
}
#endif



#ifdef NO_ENSURE
#define ensure(description, condition)
#else
/**
Check the given postcondition. If the condition is true, do nothing. If the condition is false, report the location of the postcondition and stop the program. A postcondition is a special type of assertion that has to be valid before returning from a function.

Example use of a postcondition:
@code{.c}
    int myfunction(...) {
        int result = 0;
        ...
        ensure("not negative", result >= 0);
        return result;
    }
@endcode

Example output of failed postconditions:

    myfile.c, line 18: myfunction's postcondition "not negative" (result >= 0) violated
    myfile.c, line 18: myfunction's postcondition "sorted" (forall(int i = 0, i < n-1, i++, a[i] <= a[i+1])) violated

@param[in] description String a description of the condition that has to be valid
@param[in] condition boolean the condition to check
*/
#define ensure(description, condition) \
if (!(condition)) {\
    fprintf(stderr, "%s, line %d: %s's postcondition \"%s\" (%s) violated\n", __FILE__, __LINE__, __func__, description, #condition);\
    exit(EXIT_FAILURE);\
}
#endif

#ifdef NO_ENSURE
#define ensure_x(description, condition)
#else
/**
Check the given postcondition. If the condition is true, do nothing. If the condition is false, report the location of the postcondition and stop the program. A postcondition is a special type of assertion that has to be valid before returning from a function.

Example use of a postcondition:
@code{.c}
    int myfunction(...) {
        int result = 0;
        ...
        ensure_x("not negative", result >= 0, "result == %d", result);
        return result;
    }
@endcode

Example output of failed postconditions:

    myfile.c, line 18: myfunction's postcondition "not negative" (result >= 0) violated
    myfile.c, line 18: myfunction's postcondition "sorted" violated: a[2] == 5 && a[3] == 4


@param[in] description String a description of the condition that has to be valid
@param[in] condition boolean the condition to check
*/
#define ensure_x(description, condition, ...) \
if (!(condition)) {\
    fprintf(stderr, "%s, line %d: %s's postcondition \"%s\" violated: ", __FILE__, __LINE__, __func__, description);\
    fprintf(stderr, __VA_ARGS__);\
    fprintf(stderr, "\n");\
    exit(EXIT_FAILURE);\
}
#endif

/**
Write code that is later used in a postcondition. Indicates that this code is used for a postcondition. It is removed if NO_ENSURE is defined.

Example:
@code{.c}
    int inc(int x) {
        ensure_code(int old_x = x); // save old value for postcondition
        x = x + 1;
        ensure("incremented", x == old_x + 1); // check whether new value is as expected
        return x;
    }
@endcode
*/
#ifdef NO_ENSURE
#define ensure_code(x)
#else
#define ensure_code(x) x
#endif


/**
Check that the given condition is true for all steps of an iteration. Primarily for use in assertions, preconditions, and postconditions.

Example: Checking that an int-array of n elements is sorted:
@code{.c}
    bool is_sorted = forall(int i = 0, i < n-1, i++, a[i] <= a[i+1]);
@endcode
 */
#define forall(init, has_more_steps, do_step, condition) ({\
   bool result = true;\
   for (init; has_more_steps; do_step) { if (!(condition)) { result = false; break; } }\
   result;\
})

/**
Check that the given condition is true for all steps of an iteration. Primarily for use in assertions, preconditions, and postconditions.

Example: Checking whether an int-array of n elements contains negative elements:
@code{.c}
    bool has_negative_elements = exists(int i = 0, i < n, i++, a[i] < 0);
@endcode
 */
#define exists(init, has_more_steps, do_step, condition) ({\
   bool result = false;\
   for (init; has_more_steps; do_step) { if (condition) { result = true; break; } }\
   result;\
})



////////////////////////////////////////////////////////////////////////////
// Timing

/**
Switching timing measurements on and off.
If @c NO_TIMING is defined, then timing code will not be compiled.
@see time_function
*/
#define NO_TIMING_DOC

#ifdef NO_TIMING
#define time_function(f);
#else
/**
Print the execution time of a function (in milliseconds).
@param[in] f function to time
*/
#define time_function(f) {\
    clock_t t = clock();\
    f;\
    t = clock() - t;\
    printf("time: %g ms\n", t * 1000.0 / CLOCKS_PER_SEC);\
}
#endif


////////////////////////////////////////////////////////////////////////////
// Memory

// http://www.gnu.org/software/libc/manual/html_node/Malloc-Examples.html

/**
Allocate a block of size bytes with @c malloc. Exit with error message on failure. The content of the allocated memory block is not initialized (i.e., the memory block contains arbitrary values). In addition to size, takes file name and line number for error reporting. For zero-initialized memory use @ref xcalloc.

@param[in] file file name of source code
@param[in] function function name of source code
@param[in] line line number in source code
@param[in] size number of bytes to allocate
@return address of the allocated memory block
@see xmalloc
*/
Any base_malloc(const char *file, const char *function, int line, size_t size);

/**
Allocate a block of size bytes with xmalloc. Exit with error message on failure. The content of the allocated memory block is not initialized (i.e., the memory block contains arbitrary values).
Convenience macro for calling @ref base_malloc.
@param[in] size number of bytes to allocate
@return address of the allocated memory block
@see xcalloc, free
*/
#define xmalloc(size) base_malloc(__FILE__, __func__, __LINE__, size)

/**
Allocate a block of (num * size) bytes with @c calloc. Exit with error message on failure. The allocated bytes are set to zero (the initial @c c in @c calloc stands for <i>clear<i>).
In addition, takes file name and line number for error reporting.
@param[in] file file name of source code
@param[in] function function name
@param[in] line line number in source code
@param[in] num number of elements
@param[in] size size (in bytes) of each element
@return address of the allocated memory block
@see xcalloc
*/
Any base_calloc(const char *file, const char *function, int line, size_t num, size_t size);

/**
Allocate a block of (num * size) bytes with @c calloc. Exit with error message on failure. The allocated bytes are set to zero (the initial @c c in @c calloc stands for <i>clear<i>).
Convenience macro for calling @ref base_calloc.
@param[in] num number of elements
@param[in] size size (in bytes) of each element
@return address of the allocated memory block
@see xmalloc, free
*/
#define xcalloc(num, size) base_calloc(__FILE__, __func__, __LINE__, num, size)

/**
Our own version of free. Keeps track of allocated blocks for error reporting.
@param[in] p pointer to memory block to free
*/
void base_free(Any p);

/**
Free memory blocks allocated with @ref xmalloc or @ref xcalloc.
free(p) in client code will be @c base_free
@param[in] p pointer to memory block to free
*/
#define free base_free



////////////////////////////////////////////////////////////////////////////
// Conversion

/** Convert a String to an integer. */
int i_of_s(String s);

/** Convert a String to a double. */
double d_of_s(String s);

/** Convert part of a String to a double. Index start is inclusive, index end is exclusive. */
double d_of_s_sub(String s, int start, int end);

////////////////////////////////////////////////////////////////////////////
// Output

/** Print an integer. */
void printi(int i);

/** Print an integer followed by a line break. */
void printiln(int i);

/** Print a double. */
void printd(double d);

/** Print a double followed by a line break. */
void printdln(double d);

/** Print a character. */
void printc(char c);

/** Print a character followed by a line break. */
void printcln(char c);

/** Print a String. */
void prints(String s);

/** Print a String followed by a line break. */
void printsln(String s);

/** Print a boolean value. */
void printb(bool b);

/** Print a boolean value followed by a line break. */
void printbln(bool b);

/** Print a line break. */
void println();

/** Print a C-array of n integers. */
void printia(int *a, int n);

/** Print a C-array of n integers followed by a line break. */
void printialn(int *a, int n);

/** Print a C-array of n doubles. */
void printda(double *a, int n);

/** Print a C-array of n doubles followed by a line break. */
void printdaln(double *a, int n);

/** Print a C-array of n Strings. */
void printsa(String *a, int n); 

/** Print a C-array of n Strings followed by a line break. */
void printsaln(String *a, int n); 

/** Print a C-array of n characters. */
void printca(char *a, int n);

/** Print a C-array of n characters followed by a line break. */
void printcaln(char *a, int n);

/** Print a C-array of n Bytes. */
void printba(Byte *a, int n);

/** Print a C-array of n Bytes followed by a line break. */
void printbaln(Byte *a, int n);

/** Print a C-array of n booleans. */
void printboa(bool *a, int n);

/** Print a C-array of n booleans followed by a line break. */
void printboaln(bool *a, int n);



////////////////////////////////////////////////////////////////////////////
// Input

/**
Read at most n-1 characters into an existing buffer. 
Stop reading if newline or end-of-file is reached.
Ensure that n is greater than or equal to 8.
@param[in,out] line buffer to write into (size of buffer at least n bytes)
@param[in] n maximum number of bytes to read, has to be greater than or equal to 8
@see s_input
*/
void get_line(char *line, int n);

/**
Read at most n-1 characters into a newly allocated string.
Stop reading if newline or end-of-file is reached.
Ensure that n is greater than or equal to 8.
@param[in] n maximum number of bytes to read, has to be greater than or equal to 8
@return newly allocated String with bytes read, length is according to number of bytes actually read
@see get_line
*/
String s_input(int n);

/**
Read an integer from standard input. The integer is terminated by a line break.
*/
int i_input(void);

/**
Read a double from standard input. The double is terminated by a line break.
*/
double d_input(void);


////////////////////////////////////////////////////////////////////////////
// Files

/**
Read the contents of a file into a String.
@param[in] name file name (including path)
@return newly allocated String with data read from file
@see s_write_file, write_file_data
*/
String s_read_file(String name);

/**
Write a String to a file. An existing file of the same name will be overwritten.
@param[in] name file name (including path)
@param[in] data the data to write to the file
@see s_read_file, write_file_data
*/
void s_write_file(String name, String data);

/**
Write a memory block to a file. An existing file of the same name will be overwritten.
@param[in] name file name (including path)
@param[in] data the data to write to the file
@param[in] n the number of bytes to write to the file
@see s_read_file, s_write_file
*/
void write_file_data(String name, Byte *data, int n);



////////////////////////////////////////////////////////////////////////////
// Random numbers

/**
Return a random int between in the interval [0,i).
0 is inclusive, i is exclusive.
@param[in] i upper boundary (exclusive)
@return an integer value between 0 (inclusive) and i (exclusive)
*/
int i_rnd(int i);

/**
Return a random double between in the interval [0,i).
0 is inclusive, i is exclusive.
@param[in] i upper boundary (exclusive)
@return a double value between 0 (inclusive) and i (exclusive)
*/
double d_rnd(double i);

/**
Return a random Boolean between (true or false) with 50% probability.
@return true or false
*/
bool b_rnd(void);



////////////////////////////////////////////////////////////////////////////
// Array and List types

/**
Switching element size checking for arrays and lists on and off.
If @c NO_CHECK_ELEMENT_SIZE is defined, then element size checking will not be performed.
*/
#define NO_CHECK_ELEMENT_SIZE_DOC

/**
If @c NO_GET_SET is defined, then use direct array access instead of accessor functions. Accessor functions perform bounds checking, but are less efficient than unchecked array element access.
*/
#define NO_GET_SET_DOC



////////////////////////////////////////////////////////////////////////////
// Testing

/** A very small positive value.*/
#define EPSILON 0.00000001

bool base_test_equal_b(const char *file, int line, bool a, bool e);
bool base_test_equal_i(const char *file, int line, int a, int e);
bool base_test_within_d(const char *file, int line, double a, double e, double epsilon);
bool base_test_within_i(const char *file, int line, int a, int e, int epsilon);
bool base_test_equal_c(const char *file, int line, char a, char e);
bool base_test_equal_s(const char *file, int line, String a, String e);

bool base_test_equal_ca(const char *file, int line, Array a, char *e, int ne);
bool base_test_equal_boa(const char *file, int line, Array a, bool *e, int ne);

bool base_test_equal_struct(const char *file, int line, 
        Any actual, Any expected, Any predicate);



/** Check whether the actual boolean (first argument) is equal to the expected boolean (second argument). */
#define test_equal_b(a, e) base_test_equal_b(__FILE__, __LINE__, a, e)

/** Check whether the actual int (first argument) is equal to the expected int (second argument). */
#define test_equal_i(a, e) base_test_equal_i(__FILE__, __LINE__, a, e)

/** Check whether the actual double (first argument) is within +/-epsilon (third argument) of the expected double (second argument). */
#define test_within_d(a, e, epsilon) base_test_within_d(__FILE__, __LINE__, a, e, epsilon)

/** Check whether the actual int (first argument) is within +/-epsilon (third argument) of the expected int (second argument). */
#define test_within_i(a, e, epsilon) base_test_within_i(__FILE__, __LINE__, a, e, epsilon)

/** Check whether the actual character (first argument) is equal to the expected character (second argument). */
#define test_equal_c(a, e) base_test_equal_c(__FILE__, __LINE__, a, e)

/** Check whether the actual String (first argument) is equal to the expected String (second argument). */
#define test_equal_s(a, e) base_test_equal_s(__FILE__, __LINE__, a, e)



/** Check whether the actual array (first argument) is equal to the expected array (second argument). */
#define test_equal_ca(a, e, ne) base_test_equal_ca(__FILE__, __LINE__, a, e, ne)

/** Check whether the actual array (first argument) is equal to the expected array (second argument). */
#define test_equal_boa(a, e, ne) base_test_equal_ba(__FILE__, __LINE__, a, e, ne)

/** Check whether the actual struct (first argument) is equal to the expected struct (second argument). */
#define test_equal_struct(a, e, p) base_test_equal_struct(__FILE__, __LINE__, a, e, p)



/** Call this from within test_* functions to count the number of checks. */
void base_count_check(void);

/** Call this from within test_* functions to count the number of successful checks. */
void base_count_success(void);

////////////////////////////////////////////////////////////////////////////
// Convenience includes (so users just need to include base.h)

#include "string.h"
#include "array.h"
#include "int_array.h"
#include "double_array.h"
#include "string_array.h"
#include "pointer_array.h"
#include "byte_array.h"
#include "list.h"
#include "int_list.h"
#include "double_list.h"
#include "string_list.h"
#include "pointer_list.h"

#endif
