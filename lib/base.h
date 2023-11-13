/*
base +
*/

/** @file
Base functions for Programming I Library. Here you find basic functions for output, input, files, random numbers, dynamic memory allocation, timing, assertions, and testing.

<h3>Naming Conventions</h3>

Types names are written in upper camel case, such as @c String, except for predefined type names such as @c int, @c double, and @c bool. This library does not cover type @c float. This library does not deal with @c unsigned types.

Function names are written in lower case. Parts within function names are separated with an underscore character, such as in @c base_init. This makes function names distinguishable from type names.

Constants, such as @c NULL, are written in upper case. This is independent of whether they have been defined as C constants or with preprocessor @c define statements.

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


<h4>Example 2: Reading a text file</h4>
Let @c example.txt be
@code{.c}
line 1
line II
my line 3
last line
@endcode
To read this file:
@code{.c}
String s = s_read_file("example.txt"); // read the complete file
printsln(s);
free(s); // free the string
@endcode

Output:
@code{.c}
line 1
line II
my line 3
last line
@endcode


See test functions in .c file for more examples.

@author Michael Rohs
@date 15.10.2015, 22.09.2020, 25.09.2023
@copyright Apache License, Version 2.0
*/

#ifndef __BASE_H__
#define __BASE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
// #include <execinfo.h> // stack traces
#include "basedefs.h"


////////////////////////////////////////////////////////////////////////////
// Initialization

/**
Initialization function of the library. Enables memory allocation tracking. Users of the library do not need to call this function.
@private
*/
void base_init(void);

/**
Switches memory checking on or off. If on, checks for memory leaks when thr program terminates.
@param[in] do_check if @c true, then memory is checked
*/
void report_memory_leaks(bool do_check);



////////////////////////////////////////////////////////////////////////////
// Assertions

/**
Switching assertion checking on and off.
If @c NO_ASSERT is defined, then assertions will not be compiled.
*/
#define NO_ASSERT_DOC

/**
Switching assertion checking on and off.
If @c NO_REQUIRE is defined, then preconditions will not be compiled.
*/
#define NO_REQUIRE_DOC

/**
Switching assertion checking on and off.
If @c NO_ENSURE is defined, then postconditions will not be compiled.
*/
#define NO_ENSURE_DOC

#ifdef NO_ASSERT
#define assert(description, condition)
#else
/**
Checks the given condition. If the condition is true, does nothing. If the condition is false, reports the file and line of the assertion and stops the program. Assertions are conditions that have to be valid at a particular point. If an assertion is false this indicates a bug in the program.

Example use of an assertion:
@code{.c}
    ...
    assert("not too large", x < 3);
    ...
@endcode

Example output of failed assertions:

    myfile.c, line 12: assertion "not too large" (x < 3) violated
    myfile.c, line 12: assertion "sorted" (forall(int i = 0, i < n-1, i++, a[i] <= a[i+1])) violated

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
#define assert_x(description, condition, ...)
#else
/**
Checks the given condition. If the condition is true, does nothing. If the condition is false, reports the file and line of the assertion and stops the program. Assertions are conditions that have to be valid at a particular point. If an assertion is false this indicates a bug in the program.

Example use of an assertion:
@code{.c}
    ...
    assert_x("not too large", x < 3, "x == %d", x);
    ...
@endcode

Example output of failed assertions:

    myfile.c, line 12: assertion "not too large" violated: x == 3
    myfile.c, line 12: assertion "sorted" violated: a[2] == 5 && a[3] == 4

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

#ifdef NO_ASSERT
#define assert_not_null(pointer)
#else
/**
Checks that the given pointer is not NULL. If so, does nothing. Otherwise reports the location of the assertion and stops the program. Assertions are conditions that have to be valid at a particular point. If an assertion is false this indicates a bug in the program.

Example use of an assertion:
@code{.c}
    ...
    assert_not_null(s);
    ...
@endcode

Example output of failed assertion:

    myfile.c, line 12: assertion "not null" (s) violated

@param[in] pointer a pointer that must not be null
*/
#define assert_not_null(pointer) \
if (pointer == NULL) {\
    fprintf(stderr, "%s, line %d: assertion \"not null\" (" #pointer ") violated\n", __FILE__, __LINE__);\
    exit(EXIT_FAILURE);\
}
#endif

/**
Allows writing code that is meant for use in an assertion. The code is removed if NO_ASSERT is defined.

Example:
@code{.c}
    assert_code(int old_x = x); // save old value for assertion
    x = x + 1;
    assert("incremented", x == old_x + 1); // check whether new value is as expected
@endcode
*/
#ifdef NO_ASSERT
#define assert_code(x)
#else
#define assert_code(x) x
#endif



#ifdef NO_REQUIRE
#define require(description, condition)
#else
/**
Checks the given precondition. If the condition is true, does nothing. If the condition is false, reports the location of the precondition and stops the program. A precondition is a special type of assertion that has to be valid at the beginning of a function.

Example use of a precondition:
@code{.c}
    int myfunction(int x) {
        require("not too large", x < 3);
        ...
    }
@endcode

Example output of failed preconditions:

    myfile.c, line 12: myfunction's precondition "not too large" (x < 3) violated
    myfile.c, line 12: myfunction's precondition "sorted" (forall(int i = 0, i < n-1, i++, a[i] <= a[i+1])) violated

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
#define require_x(description, condition, ...)
#else
/**
Checks the given precondition. If the condition is true, does nothing. If the condition is false, reports the location of the precondition and stops the program. A precondition is a special type of assertion that has to be valid at the beginning of a function.

Example use of a precondition:
@code{.c}
    int myfunction(int x) {
        require_x("not too large", x < 3, "x == %d", x);
        ...
    }
@endcode

Example output of failed preconditions:

    myfile.c, line 12: myfunction's precondition "not too large" violated: x == 3
    myfile.c, line 12: myfunction's precondition "sorted" violated: a[2] == 5 && a[3] == 4

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

#if 0
// print stack trace using gcc, requires
// #include <execinfo.h>

#include <execinfo.h>
#define require_x(description, condition, ...) \
if (!(condition)) {\
    fprintf(stderr, "%s, line %d: %s's precondition \"%s\" violated: ", __FILE__, __LINE__, __func__, description);\
    fprintf(stderr, __VA_ARGS__);\
    fprintf(stderr, "\n");\
    void *buffer[10];\
    int size = backtrace(buffer, 10);\
    char** strings = backtrace_symbols(buffer, size);\
    if (strings != NULL) {\
        for (int i = 1; i < size - 1; i++) {\
            String s = strings[i] + 59;\
            String t = strchr(s, ' ');\
            int n = t ? t - s : strlen(s);\
            printf ("    is called from %.*s\n", n, s);\
        }\
    }\
    exit(EXIT_FAILURE);\
}
#endif

#ifdef NO_REQUIRE
#define require_not_null(argument)
#else
/**
Checks that the given argument is not NULL. If so, does nothing. Otherwise reports the location of the precondition and stops the program. A precondition is a special type of assertion that has to be valid at the beginning of a function.

Example use of a precondition:
@code{.c}
    int myfunction(String s) {
        require_not_null(s);
        ...
    }
@endcode

Example output of failed preconditions:

    myfile.c, line 12: myfunction's precondition "not null" (s) violated

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
Checks the given postcondition. If the condition is true, does nothing. If the condition is false, reports the location of the postcondition and stops the program. A postcondition is a special type of assertion that has to be valid before returning from a function.

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

    myfile.c, line 12: myfunction's postcondition "not negative" (result >= 0) violated
    myfile.c, line 12: myfunction's postcondition "sorted" (forall(int i = 0, i < n-1, i++, a[i] <= a[i+1])) violated

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
#define ensure_x(description, condition, ...)
#else
/**
Checks the given postcondition. If the condition is true, does nothing. If the condition is false, reports the location of the postcondition and stops the program. A postcondition is a special type of assertion that has to be valid before returning from a function.

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

    myfile.c, line 12: myfunction's postcondition "not negative" (result >= 0) violated
    myfile.c, line 12: myfunction's postcondition "sorted" violated: a[2] == 5 && a[3] == 4


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
Allows writing code that is meant for use in a postcondition. The code is removed if NO_ENSURE is defined.

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

#ifdef NO_ENSURE
#define ensure_not_null(pointer)
#else
/**
Checks that the given pointer is not NULL. If so, does nothing. Otherwise reports the location of the postcondition and stops the program. A postcondition is a special type of assertion that has to be valid before returning from a function.

Example use of a postcondition:
@code{.c}
    ...
    ensure_not_null(s);
    ...
@endcode

Example output of failed postcondition:

    myfile.c, line 12: myfunction's postcondition "not null" (s) violated

@param[in] pointer a pointer that must not be null
*/
#define ensure_not_null(pointer) \
if (pointer == NULL) {\
    fprintf(stderr, "%s, line %d: %s's postcondition \"not null\" (" #pointer ") violated\n", __FILE__, __LINE__, __func__);\
    exit(EXIT_FAILURE);\
}
#endif

/**
Checks whether the given condition is true for all steps of an iteration. Primarily for use in assertions, preconditions, and postconditions.
@param[in] i name of iteration variable
@param[in] length number of iteration steps, i in [0, length)
@param[in] condition boolean expression to check for each iteration step
@return true if the condition is true for all steps of the iteration, false otherwise

Example: Checking whether an array is sorted:
@code{.c}
    bool is_sorted = forall(k, arr_length - 1, arr[k] <= arr[k+1]);
@endcode
 */
#define forall(i, length, condition) ({\
   bool _forall_result = true;\
   for (int i = 0; i < length; i++) { if (!(condition)) { _forall_result = false; break; } }\
   _forall_result;\
})

/**
Checks whether the given condition is true for all steps of an iteration. Primarily for use in assertions, preconditions, and postconditions.
@param[in] init initialization expression
@param[in] has_more_steps boolean expression for continuing the iteration
@param[in] do_step advance the iteration state
@param[in] condition boolean expression to check for each iteration step
@return true if the condition is true for all steps of the iteration, false otherwise

Example: Checking whether an array is sorted:
@code{.c}
    bool is_sorted = forall_x(int i = 0, i < arr_length - 1, i++, arr[i] <= arr[i+1]);
@endcode
 */
#define forall_x(init, has_more_steps, do_step, condition) ({\
   bool _forall_result = true;\
   for (init; has_more_steps; do_step) { if (!(condition)) { _forall_result = false; break; } }\
   _forall_result;\
})

/**
Checks whether the given condition is true for at least one step of an iteration. Primarily for use in assertions, preconditions, and postconditions.
@param[in] i name of iteration variable
@param[in] length number of iteration steps, i in [0, length)
@param[in] condition boolean expression to check for each iteration step
@return true if the condition is true for at least one step of the iteration, false otherwise

Example: Checking whether an array contains negative elements:
@code{.c}
    bool has_negative_elements = exists(k, arr_length, arr[k] < 0);
@endcode
 */
#define exists(i, length, condition) ({\
   bool _exists_result = false;\
   for (int i = 0; i < length; i++) { if (condition) { _exists_result = true; break; } }\
   _exists_result;\
})

/**
Checks whether the given condition is true for at least one step of an iteration. Primarily for use in assertions, preconditions, and postconditions.
@param[in] init initialization expression
@param[in] has_more_steps boolean expression for continuing the iteration
@param[in] do_step advance the iteration state
@param[in] condition boolean expression to check for each iteration step
@return true if the condition is true for at least one step of the iteration, false otherwise

Example: Checking whether an array contains negative elements:
@code{.c}
    bool has_negative_elements = exists_x(int i = 0, i < arr_length, i++, arr[i] < 0);
@endcode
 */
#define exists_x(init, has_more_steps, do_step, condition) ({\
   bool _exists_result = false;\
   for (init; has_more_steps; do_step) { if (condition) { _exists_result = true; break; } }\
   _exists_result;\
})

/**
Counts the number of iteration steps for which the given condition is true. Primarily for use in assertions, preconditions, and postconditions.
@param[in] init initialization expression
@param[in] has_more_steps boolean expression for continuing the iteration
@param[in] do_step advance the iteration state
@param[in] condition boolean expression
@return the number of iteration steps for which the condition is true

Example: Count the number of non-zero array elements:
@code{.c}
    int n_non_zero = count_if(int i = 0, i < arr_length, i++, arr[i] != 0);
@endcode
 */
#define count_if(init, has_more_steps, do_step, condition) ({\
   int _count_if_result = 0;\
   for (init; has_more_steps; do_step) { if (condition) { _count_if_result++; } }\
   _count_if_result;\
})

////////////////////////////////////////////////////////////////////////////
// Exit on Failure

/**
Exits the program if the failure condition is true. The further arguments
contain the reason for exiting the program, using a format string.
*/
#define exit_if(condition, ...) \
if (condition) {\
    flockfile(stderr);\
    fprintf(stderr, __VA_ARGS__);\
    fprintf(stderr, "\n");\
    funlockfile(stderr);\
    exit(EXIT_FAILURE);\
}

/**
Exits the program. The arguments contain the reason for the panic, using a
format string. Reports the line number and the reason for the failure.
*/
#define panic(...) {\
    flockfile(stderr);\
    fprintf(stderr, "%s:%d, %s: ", __FILE__, __LINE__, __func__);\
    fprintf(stderr, __VA_ARGS__);\
    fprintf(stderr, "\n");\
    funlockfile(stderr);\
    exit(EXIT_FAILURE);\
}

/**
Exits the program if the failure condition is true. The further arguments
contain the reason for the panic, using a format string. Reports the line
number and the reason for the failure.
*/
#define panic_if(condition, ...) \
if (condition) {\
    flockfile(stderr);\
    fprintf(stderr, "%s:%d, %s: ", __FILE__, __LINE__, __func__);\
    fprintf(stderr, __VA_ARGS__);\
    fprintf(stderr, "\n");\
    funlockfile(stderr);\
    exit(EXIT_FAILURE);\
}

////////////////////////////////////////////////////////////////////////////
// Timing

/*
/// A structure for storing time values.
typedef struct timespec timespec;

/// Gets the current time.
timespec time_now(void);

/// Computes the time difference in milliseconds between now and the given time.
double time_ms_since(timespec t);
*/

////////////////////////////////////////////////////////////////////////////
// Debugging

#ifdef NO_DEBUG
    #define PL
    #define PLi(i)
    #define PLs(s)
    #define PLf(...)
#else
    /** Print line number. Example: PL; */
    #define PL printf("%s:%d\n", __func__, __LINE__)
    /** Print line number and integer. Example: PL(1); */
    #define PLi(i) printf("%s:%d: %d\n", __func__, __LINE__, i)
    /** Print line number and format string. Example: PLf("i = %d", 123); */
    #define PLf(...) {\
        fprintf(stderr, "%s:%d: ", __func__, __LINE__);\
        fprintf(stderr, __VA_ARGS__);\
        fprintf(stderr, "\n");\
    }
#endif

////////////////////////////////////////////////////////////////////////////
// Memory

// http://www.gnu.org/software/libc/manual/html_node/Malloc-Examples.html

/**
Allocates a block of size bytes using @c malloc. Exits with an error message on failure. The contents of the allocated memory block is not initialized (i.e., the memory block contains arbitrary values). Stores file name and line number for error reporting. For zero-initialized memory use @ref xcalloc.

@param[in] file file name of source code
@param[in] function function name of source code
@param[in] line line number in source code
@param[in] size number of bytes to allocate
@return pointer to the allocated memory block
@see xmalloc
@private
*/
Any base_malloc(const char *file, const char *function, int line, size_t size);

/**
Allocates a block of size bytes with xmalloc. Exits with an error message on failure. The contents of the allocated memory block is not initialized (i.e., the memory block contains arbitrary values).
@param[in] size number of bytes to allocate
@return pointer to the allocated memory block
@see xrealloc, xcalloc, free
*/
#define xmalloc(size) base_malloc(__FILE__, __func__, __LINE__, size)

/**
Reallocates a block of size bytes using @c realloc. Exits with error message on failure. The contents of the allocated memory block is not initialized (i.e., the memory block contains arbitrary values).
@param[in] file file name of source code
@param[in] function function name of source code
@param[in] line line number in source code
@param[in] ptr pointer to reallocate
@param[in] size number of bytes to allocate
@return pointer to the reallocated memory block
@see xrealloc
@private
*/
Any base_realloc(const char *file, const char *function, int line, Any ptr, size_t size);

/**
Reallocates a block of size bytes using @c realloc. Exits with error message on failure. The contents of the allocated memory block is not initialized (i.e., the memory block contains arbitrary values).
@param[in] ptr pointer to reallocate
@param[in] size number of bytes to allocate
@return pointer to the reallocated memory block
@see xcalloc, xmalloc, free
*/
#define xrealloc(ptr, size) base_realloc(__FILE__, __func__, __LINE__, ptr, size)


/**
Allocates a block of (num * size) bytes using @c calloc. Exits with error message on failure. The allocated bytes are set to zero (the initial @c c in @c calloc stands for <i>clear</i>).
Stores file name and line number for error reporting.
@param[in] file file name of source code
@param[in] function function name
@param[in] line line number in source code
@param[in] num number of elements
@param[in] size size (in bytes) of each element
@return pointer to the allocated memory block
@see xcalloc
@private
*/
Any base_calloc(const char *file, const char *function, int line, size_t num, size_t size);

/**
Allocates a block of (num * size) bytes with @c calloc. Exits with error message on failure. The allocated bytes are set to zero (the initial @c c in @c calloc stands for <i>clear</i>).
@param[in] num number of elements
@param[in] size size (in bytes) of each element
@return pointer to the allocated memory block
@see xrealloc, xmalloc, free
*/
#define xcalloc(num, size) base_calloc(__FILE__, __func__, __LINE__, num, size)

/**
Our own version of free. Keeps track of allocated blocks for error reporting.
@param[in] p pointer to memory block to free
@private
*/
void base_free(Any p);

/**
Frees memory blocks allocated with @ref xmalloc or @ref xcalloc.
@param[in] p pointer to memory block to free
*/
#define free base_free

/**
Our own version of exit. Remembers the exit status before calling the ``real'' exit function.
@param[in] status exit status of the process
@private
*/
void __attribute__((noreturn)) base_exit(int status);

/**
Exits the process and returns the given status to the operating system.
@param[in] status (int) exit status of the process
*/
#define exit base_exit

////////////////////////////////////////////////////////////////////////////
// Strings

/**
Creates a copy of the given string.
Memory of copy is dynamically allocated.
@param[in] s input string
@return copy of input string
*/
String s_copy(String s);

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
Returns the length of the string (number of characters).
@param[in] s input string
@return number of characters
*/
int s_length(String s);

/**
Returns true iff @c s and @c t are equal.
@param[in] s input string
@param[in] t input string
@return true iff @c s and @c t are equal
*/
bool s_equals(String s, String t);

/**
Returns true iff @c s contains part.
@param[in] s input string
@param[in] part input string
@return true iff s contains part
*/
bool s_contains(String s, String part);

////////////////////////////////////////////////////////////////////////////
// Conversion

/** Converts a String to an integer. */
int i_of_s(String s);

/** Converts a String to a double. */
double d_of_s(String s);

////////////////////////////////////////////////////////////////////////////
// Output

/** Prints an integer. */
void printi(int i);

/** Prints an integer followed by a line break. */
void printiln(int i);

/** Prints a double. */
void printd(double d);

/** Prints a double followed by a line break. */
void printdln(double d);

/** Prints a character. */
void printc(char c);

/** Prints a character followed by a line break. */
void printcln(char c);

/** Prints a String. */
void prints(String s);

/** Prints a String followed by a line break. */
void printsln(String s);

/** Prints a boolean value. */
void printb(bool b);

/** Prints a boolean value followed by a line break. */
void printbln(bool b);

/** Prints a line break. */
void println();

/** Prints a C-array of n integers. */
void printia(int *a, int n);

/** Prints a C-array of n integers followed by a line break. */
void printialn(int *a, int n);

/** Prints a C-array of n doubles. */
void printda(double *a, int n);

/** Prints a C-array of n doubles followed by a line break. */
void printdaln(double *a, int n);

/** Prints a C-array of n Strings. */
void printsa(String *a, int n); 

/** Prints a C-array of n Strings followed by a line break. */
void printsaln(String *a, int n); 

/** Prints a C-array of n characters. */
void printca(char *a, int n);

/** Prints a C-array of n characters followed by a line break. */
void printcaln(char *a, int n);

/** Prints a C-array of n Bytes. */
void printba(Byte *a, int n);

/** Prints a C-array of n Bytes followed by a line break. */
void printbaln(Byte *a, int n);

/** Prints a C-array of n booleans. */
void printboa(bool *a, int n);

/** Prints a C-array of n booleans followed by a line break. */
void printboaln(bool *a, int n);



////////////////////////////////////////////////////////////////////////////
// Input

/**
Reads at most n-1 characters into an existing buffer. Stops reading if newline or end-of-file have been reached. Does not return the newline character.
@param[in,out] line buffer to write into (size of buffer at least n bytes), whill be a zero-terminated String
@param[in] n maximum number of bytes to read, has to be at least 8
@pre "not too small", n >= 8
@see s_input
*/
void get_line(char *line, int n);

/**
Reads at most n-1 characters into a newly allocated string. Stops reading if newline or end-of-file is reached. Does not return the newline character.
@param[in] n maximum number of bytes to read, has to be at least 8
@return newly allocated String with bytes read
@see get_line
*/
String s_input(int n);

/**
Reads an integer from standard input. The input has to be terminated by a line break.
*/
int i_input(void);

/**
Reads a double from standard input. The input has to be terminated by a line break.
*/
double d_input(void);


////////////////////////////////////////////////////////////////////////////
// Files

/**
Reads the contents of a file into a String. The function fails if the file does not exist or cannot be read.
@param[in] name file name (including path)
@return newly allocated String with data read from file
@see s_write_file, write_file_data
*/
String s_read_file(String name);

/**
Writes a String to a file. An existing file of the same name will be overwritten.
@param[in] name file name (including path)
@param[in] data the data to write to the file
@see s_read_file, write_file_data
*/
void s_write_file(String name, String data);

/**
Writes a memory block to a file. An existing file of the same name will be overwritten.
@param[in] name file name (including path)
@param[in] data the data to write to the file
@param[in] n the number of bytes to write to the file
@pre "non-negative length", n >= 0
@see s_read_file, s_write_file
*/
void write_file_data(String name, Byte *data, int n);



////////////////////////////////////////////////////////////////////////////
// Random numbers

/**
Returns a random int between in the interval [0,i).
0 is inclusive, i is exclusive.
@param[in] i upper boundary (exclusive)
@return an integer value between 0 (inclusive) and i (exclusive)
@pre "positive range", i > 0
*/
int i_rnd(int i);

/**
Returns a random double between in the interval [0,i).
0 is inclusive, i is exclusive.
@param[in] i upper boundary (exclusive)
@return a double value between 0 (inclusive) and i (exclusive)
@pre "positive range", i > 0
*/
double d_rnd(double i);

/**
Returns a random Boolean between (true or false) with 50% probability.
@return true or false
*/
bool b_rnd(void);



////////////////////////////////////////////////////////////////////////////
// Testing

/** A very small positive value.*/
#define EPSILON 0.00000001

/** Checks whether the actual value @c a is equal to the expected value @c e. */
bool base_test_equal_b(const char *file, int line, bool a, bool e);

/** Checks whether the actual value @c a is equal to the expected value @c e. */
bool base_test_equal_i(const char *file, int line, int a, int e);

/** Checks whether the actual value @c a is within +/-epsilon of the expected value @c e. */
bool base_test_within_d(const char *file, int line, double a, double e, double epsilon);

/** Checks whether the actual value @c a is within +/-epsilon of the expected value @c e. */
bool base_test_within_i(const char *file, int line, int a, int e, int epsilon);

/** Checks whether the actual value @c a is equal to the expected value @c e. */
bool base_test_equal_c(const char *file, int line, char a, char e);

/** Checks whether the actual value @c a is equal to the expected value @c e. */
bool base_test_equal_s(const char *file, int line, String a, String e);

/** Checks whether the members of struct @c a are equal to the members of struct @c e. Uses the given @c predicate function to perform the comparison. */
bool base_test_equal_struct(const char *file, int line, 
        Any a, Any e, AnyFn predicate);



/** Checks whether the actual boolean (first argument) is equal to the expected boolean (second argument). */
#define test_equal_b(a, e) base_test_equal_b(__FILE__, __LINE__, a, e)

/** Checks whether the actual int (first argument) is equal to the expected int (second argument). */
#define test_equal_i(a, e) base_test_equal_i(__FILE__, __LINE__, a, e)

/** Checks whether the actual double (first argument) is within +/-epsilon (third argument) of the expected double (second argument). */
#define test_within_d(a, e, epsilon) base_test_within_d(__FILE__, __LINE__, a, e, epsilon)

/** Checks whether the actual int (first argument) is within +/-epsilon (third argument) of the expected int (second argument). */
#define test_within_i(a, e, epsilon) base_test_within_i(__FILE__, __LINE__, a, e, epsilon)

/** Checks whether the actual character (first argument) is equal to the expected character (second argument). */
#define test_equal_c(a, e) base_test_equal_c(__FILE__, __LINE__, a, e)

/** Checks whether the actual String (first argument) is equal to the expected String (second argument). */
#define test_equal_s(a, e) base_test_equal_s(__FILE__, __LINE__, a, e)



/** Checks whether the actual array (first argument) is equal to the expected array (second argument). */
#define test_equal_ca(a, e, ne) base_test_equal_ca(__FILE__, __LINE__, a, e, ne)

/** Checks whether the actual array (first argument) is equal to the expected array (second argument). */
#define test_equal_boa(a, e, ne) base_test_equal_ba(__FILE__, __LINE__, a, e, ne)

/** Checks whether the actual struct (first argument) is equal to the expected struct (second argument). */
#define test_equal_struct(a, e, p) base_test_equal_struct(__FILE__, __LINE__, a, e, p)



/**
Called from within test_* to count the number of tests.
@private
*/
void base_count_check(void);

/**
Called from within test_* to count the number of successful tests.
@private
*/
void base_count_success(void);

#endif
