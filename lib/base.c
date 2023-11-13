/*
@author Michael Rohs
@date 15.10.2015. 26.09.2020, 25.09.2022
@copyright Apache License, Version 2.0
*/

#include "base.h"
#undef free // use the 'real' free here
#undef exit // use the 'real' exit here
//#undef xmalloc
//#undef xcalloc



////////////////////////////////////////////////////////////////////////////
// Memory allocation

// http://www.gnu.org/software/libc/manual/html_node/Hooks-for-Malloc.html#Hooks-for-Malloc
// this GNU C solution does not work on Mac OS X
// Mac OS X solution does not work on other platforms
// so simply use preprocessor, does not catch things like strdup (stderr, or use macro for that as well)

typedef struct BaseAllocInfo {
    Any p;
    size_t size;
    const char *file;
    const char *function;
    int line;
    struct BaseAllocInfo *next;
} BaseAllocInfo;

BaseAllocInfo *base_alloc_info = NULL;

void base_free(Any p) {
#if 0
    // debug output
    printf("base_free: Calling free on %p\n", p);
    for (BaseAllocInfo *dp = base_alloc_info; dp != NULL; dp = dp->next) {
        printf("%p\n", dp->p);
    }
#endif
    bool removed = false;
    BaseAllocInfo *ai = base_alloc_info;
    if (ai != NULL) {
        if (ai->p == p) { // remove first
            BaseAllocInfo *del = ai;
            base_alloc_info = ai->next;
            free(del);
            removed = true;
        } else { // remove other than first
            for (; ai != NULL; ai = ai->next) {
                if (ai->next != NULL && ai->next->p == p) {
                    BaseAllocInfo *del = ai->next;
                    ai->next = ai->next->next;
                    free(del);
                    removed = true;
                    break;
                }
            }
        }
    }
    if (!removed) {
        fprintf(stderr, "base_free: trying to free unknown pointer %p\n", p);
    }

    free(p);
}

static int exit_status = EXIT_SUCCESS;

void base_exit(int status) {
    // printsln("base_exit called");
    exit_status = status;
    exit(status);
}

static bool base_atexit_registered = false;
void base_atexit(void);

static bool do_memory_check = false;

void base_init(void) {
    if (!base_atexit_registered) {
        atexit(base_atexit);
        base_atexit_registered = true;
    }
}

void report_memory_leaks(bool do_check) {
    base_init();
    do_memory_check = do_check;
}

Any base_malloc(const char *file, const char *function, int line, size_t size) {
    // allocate four bytes more than requested and fill with garbage, 
    // such that non-terminated strings will produce an unexpected result
    Any p = malloc(size + 4);
    if (p == NULL) {
        fprintf(stderr, "%s, line %d: malloc(%lu) called in base_malloc returned NULL!\n", 
                file, line, (unsigned long)size);
        base_exit(EXIT_FAILURE);
    }
    // printf("%s, line %d: malloc(%lu) returned %lx\n", file, line, (unsigned long)size, (unsigned long)p);

    // fill with garbage
    memset(p, '?', size + 3);
    ((char*)p)[size + 3] = '\0';

    BaseAllocInfo *ai = malloc(sizeof(BaseAllocInfo));
    if (ai == NULL) {
        fprintf(stderr, "%s, line %d: malloc(sizeof(BaseAllocInfo)) called in base_malloc returned NULL!\n", 
                file, line);
        base_exit(EXIT_FAILURE);
    }
    ai->p = p;
    ai->size = size;
    ai->file = file;
    ai->function = function;
    ai->line = line;
    ai->next = base_alloc_info;
    base_alloc_info = ai;

    return p;
}

Any base_realloc(const char *file, const char *function, int line, Any ptr, size_t size) {
    Any p = realloc(ptr, size);
    if (p == NULL) {
        fprintf(stderr, "%s, line %d: malloc(%lu) called in base_realloc returned NULL!\n",
                file, line, (unsigned long)size);
        base_exit(EXIT_FAILURE);
    }
    BaseAllocInfo *ai = base_alloc_info;
    while(ai && ai->p != ptr) ai = ai->next;
    if (ai == NULL) {
        ai = malloc(sizeof(BaseAllocInfo));
        ai->next = base_alloc_info;
        base_alloc_info = ai;
    }
    ai->p = p;
    ai->size = size;
    ai->file = file;
    ai->function = function;
    ai->line = line;
    return p;
}

Any base_calloc(const char *file, const char *function, int line, size_t num, size_t size) {
    // printf("%s, line %d: xcalloc(%lu, %lu)\n", file, line, (unsigned long)num, (unsigned long)size);
    Any p = calloc(num, size);
    if (p == NULL) {
        fprintf(stderr, "%s, line %d: calloc(%lu, %lu) called in base_calloc returned NULL!\n", 
                file, line, (unsigned long)num, (unsigned long)size);
        base_exit(EXIT_FAILURE);
    }
    // printf("%s, line %d: xcalloc(%lu, %lu) returned %lx\n", file, line, (unsigned long)num, (unsigned long)size, (unsigned long)p);

    BaseAllocInfo *ai = malloc(sizeof(BaseAllocInfo));
    if (ai == NULL) {
        fprintf(stderr, "%s, line %d: malloc(sizeof(BaseAllocInfo)) called in base_calloc returned NULL!\n", 
                file, line);
        base_exit(EXIT_FAILURE);
    }
    ai->p = p;
    ai->size = num * size;
    ai->file = file;
    ai->function = function;
    ai->line = line;
    ai->next = base_alloc_info;
    base_alloc_info = ai;
    // printf("base_calloc entered %p\n", base_alloc_info->p);

    return p;   
}

static void base_check_memory(void) {
    // printsln("Checking for memory leaks:");
    int n = 0; // number of memory leaks
    size_t s = 0; // total number of leaked bytes

    for (BaseAllocInfo *ai = base_alloc_info; ai != NULL; ai = ai->next) {
        if (n < 5) { // only show the first ones explicitly
            fprintf(stderr, "%5lu bytes allocated in %s (%s, line %d) not freed\n", 
                    (unsigned long)ai->size, ai->function, ai->file, ai->line);
        }
        n++;
        s += ai->size;
    }

    if (n > 0) {
        fprintf(stderr, "%d memory leak%s, %lu bytes total\n", n, n == 1 ? "" : "s", (unsigned long)s);
    } else {
        // fprintf(stderr, "No memory leaks.\n");
    }
}


////////////////////////////////////////////////////////////////////////////
// Strings

String s_copy(String s) {
    require_not_null(s);
    int n = (strlen(s) + 1) * sizeof(char); // + 1 for '\0' termination
    char *a = xmalloc(n);
    memcpy(a, s, n);
    return a;
}

char s_get(String s, int i) {
    require_not_null(s);
    int n = strlen(s);
    require_x("index in range", i >= 0 && i < n, "index == %d, length == %d", i, n);
    return s[i];
}

void s_set(String s, int i, char v) {
    require_not_null(s);
    int n = strlen(s);
    require_x("index in range", i >= 0 && i < n, "index == %d, length == %d", i, n);
    s[i] = v;
}

int s_length(String s) {
    require_not_null(s);
    return strlen(s);
}

bool s_equals(String s, String t) {
    require_not_null(s);
    require_not_null(t);
    return strcmp(s, t) == 0;
}

bool s_contains(String s, String part) {
    require_not_null(s);
    require_not_null(part);
    return strstr(s, part) != NULL;
}

////////////////////////////////////////////////////////////////////////////
// Conversion

int i_of_s(String s) {
    require_not_null(s);
    return atoi(s);
}

double d_of_s(String s) {
    require_not_null(s);
    return atof(s);
}

////////////////////////////////////////////////////////////////////////////
// Output

void printi(int i) {
    printf("%d", i);
}

void printiln(int i) {
    printf("%d\n", i);
}

void printd(double d) {
    printf("%g", d);
}

void printdln(double d) {
    printf("%g\n", d);
}

void printc(char c) {
    printf("%c", c);
}

void printcln(char c) {
    printf("%c\n", c);
}


void prints(String s) {
    require_not_null(s);
    printf("%s", s);
}

void printsln(String s) {
    require_not_null(s);
    printf("%s\n", s);
}

void printb(bool b) {
    printf("%s", b ? "true" : "false");
}

void printbln(bool b) {
    printf("%s\n", b ? "true" : "false");
}

void println() {
    printf("\n");
}

void printia(int *a, int n) {
    require_not_null(a);
    require("non-negative length", n >= 0);
    putchar('[');
    if (n > 0) {
        printf("%d", a[0]);
    }
    for (int i = 1; i < n; i++) {
        printf(" %d", a[i]);
    }
    putchar(']');
}

void printialn(int *a, int n) {
    require_not_null(a);
    require("non-negative length", n >= 0);
    printia(a, n);
    println();
}

void printda(double *a, int n) {
    require_not_null(a);
    require("non-negative length", n >= 0);
    putchar('[');
    if (n > 0) {
        printf("%g", a[0]);
    }
    for (int i = 1; i < n; i++) {
        printf(" %g", a[i]);
    }
    putchar(']');
}

void printdaln(double *a, int n) {
    require_not_null(a);
    require("non-negative length", n >= 0);
    printda(a, n);
    println();
}

void printsa(String *a, int n) {
    require_not_null(a);
    require("non-negative length", n >= 0);
    putchar('[');
    if (n > 0) {
        printf("\"%s\"", a[0]);
    }
    for (int i = 1; i < n; i++) {
        printf(" \"%s\"", a[i]);
    }
    putchar(']');
}

void printsaln(String *a, int n) {
    require_not_null(a);
    require("non-negative length", n >= 0);
    printsa(a, n);
    println();
}

void printca(char *a, int n) {
    require_not_null(a);
    require("non-negative length", n >= 0);
    putchar('[');
    if (n > 0) {
        printf("'%c'", a[0]);
    }
    for (int i = 1; i < n; i++) {
        printf(" '%c'", a[i]);
    }
    putchar(']');
}

void printcaln(char *a, int n) {
    require_not_null(a);
    require("non-negative length", n >= 0);
    printca(a, n);
    println();
}

void printba(Byte *a, int n) {
    require_not_null(a);
    require("non-negative length", n >= 0);
    putchar('[');
    if (n > 0) {
        printf("%d", a[0]);
    }
    for (int i = 1; i < n; i++) {
        printf(" %d", a[i]);
    }
    putchar(']');
}

void printbaln(Byte *a, int n) {
    require_not_null(a);
    require("non-negative length", n >= 0);
    printba(a, n);
    println();
}

void printboa(bool *a, int n) {
    require_not_null(a);
    require("non-negative length", n >= 0);
    putchar('[');
    if (n > 0) {
        printf("%s", a[0] ? "true" : "false");
    }
    for (int i = 1; i < n; i++) {
        printf(" %s", a[i] ? "true" : "false");
    }
    putchar(']');
}

void printboaln(bool *a, int n) {
    require_not_null(a);
    require("non-negative length", n >= 0);
    printboa(a, n);
    println();
}



////////////////////////////////////////////////////////////////////////////
// Input

void get_line(char *line, int n) {
    require_not_null(line);
    require("not too small", n >= 8);

    if(fgets(line, n, stdin) == NULL) {
        fprintf(stderr, "get_line has failed: fgets() returned NULL!\n");
        exit(EXIT_FAILURE);
    }

    n = strlen(line);
    if (n >= 1 && (line[n-1] == '\n' || line[n-1] == '\r')) line[n-1] = '\0'; 
    if (n >= 2 && (line[n-2] == '\n' || line[n-2] == '\r')) line[n-2] = '\0'; 
    // printf("%d %d %d\n", line[n-2], line[n-1], line[n]);
    // printf("'%s'\n", line);
}

String s_input(int n) {
    if (n < 8) n = 8;
    char *line = base_malloc(__FILE__, __func__, __LINE__, n);
    *line = '\0';

    if(fgets(line, n, stdin) == NULL) {
        fprintf(stderr, "s_input has failed: fgets() returned NULL!\n");
        exit(EXIT_FAILURE);
    }

    n = strlen(line);
    if (n >= 1 && (line[n-1] == '\n' || line[n-1] == '\r')) line[n-1] = '\0'; 
    if (n >= 2 && (line[n-2] == '\n' || line[n-2] == '\r')) line[n-2] = '\0'; 
    n = strlen(line);
    String s = base_malloc(__FILE__, __func__, __LINE__, n + 1);
    strcpy(s, line);
    base_free(line);
    return s;
}

int i_input(void) {
    String s = s_input(100);
    int i = i_of_s(s);
    free(s);
    return i;
}

double d_input(void) {
    String s = s_input(100);
    double d = d_of_s(s);
    free(s);
    return d;
}



////////////////////////////////////////////////////////////////////////////
// Files

String s_read_file(String name) {
    require_not_null(name);
    
    FILE *f = fopen(name, "r"); // removes \r from read content, only leaves \n
    if (f == NULL) {
        fprintf(stderr, "%s: Cannot open %s\n", (String)__func__, name); 
        base_exit(EXIT_FAILURE);
    }
    fseek (f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    
    char *s = base_malloc(__FILE__, __func__, __LINE__, size + 1);
    if (s == NULL) {
        fprintf(stderr, "%s: Cannot allocate memory.\n", (String)__func__); 
        base_exit(EXIT_FAILURE);
    }
    long sizeRead = fread(s, 1, size, f);
    // assert: size >= sizeRead (> if file contains \r characters)
    // printf("size = %lu, sizeRead = %lu, feof = %d\n", size, sizeRead, feof(f));
    if (sizeRead < size && feof(f) == 0) {
        fprintf(stderr, "%s: Cannot read file %s to end.\n", (String)__func__, name); 
        base_exit(EXIT_FAILURE);
    }
    s[sizeRead] = '\0';
    
    fclose(f);
    return s;
}

void s_write_file(String name, String data) {
    require_not_null(name);
    require_not_null(data);
    
    FILE *f = fopen(name, "w");
    if (f == NULL) {
        fprintf(stderr, "%s: Cannot open %s\n", (String)__func__, name); 
        base_exit(EXIT_FAILURE);
    }

    size_t n_data = strlen(data);
    size_t n_written = fwrite(data , 1, n_data, f);
    if (n_written != n_data) {
        fprintf(stderr, "%s: Cannot write data to file %s.\n", (String)__func__, name); 
        base_exit(EXIT_FAILURE);
    }
    
    fclose(f);
}

void write_file_data(String name, Byte *data, int n_data) {
    require_not_null(name);
    require_not_null(data);
    require("non-negative length", n_data >= 0);

    FILE *f = fopen(name, "w");
    if (f == NULL) {
        fprintf(stderr, "%s: Cannot open %s\n", (String)__func__, name); 
        base_exit(EXIT_FAILURE);
    }
    
    size_t n_written = fwrite(data , 1, n_data, f);
    if (n_written != n_data) {
        fprintf(stderr, "%s: Cannot write data to file %s.\n", (String)__func__, name); 
        base_exit(EXIT_FAILURE);
    }
    
    fclose(f);
}

///////////////////////////////////////////////////////////////////////////////
// Time taking
// https://en.cppreference.com/w/c/chrono/clock
/*
timespec time_now(void) {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    return now;
}

double time_ms_since(timespec t) {
    timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    double duration =  1000.0 * now.tv_sec + 1e-6 * now.tv_nsec
                     - (1000.0 * t.tv_sec + 1e-6*t.tv_nsec);
    return duration;
}
*/

////////////////////////////////////////////////////////////////////////////
// Random numbers

static bool srand_called = false;

int i_rnd(int i) {
    require("positive range", i > 0);
    if (!srand_called) {
        srand(time(NULL) << 10);
        srand_called = true;
    }
    int result = 0;
    if (RAND_MAX == 32767) {
        result = ((rand() << 16) | rand()) % i;
    } else {
        result = rand() % i;
    }
    ensure("random number in range", 0 <= result && result < i);
    return result;
}

double d_rnd(double i) {
    require("positive range", i > 0);
    if (!srand_called) {
        srand(time(NULL) << 10);
        srand_called = true;
    }
    double r = (double) rand() / (double) RAND_MAX;
    double result = i * r;
    ensure("random number in range", 0 <= result && result < i);
    return result;
}

bool b_rnd(void) {
    return i_rnd(2) == 0;
}



////////////////////////////////////////////////////////////////////////////
// Testing

int base_check_count = 0;
int base_check_success_count = 0;

// http://www.gnu.org/software/libc/manual/html_node/Cleanups-on-Exit.html#Cleanups-on-Exit
void base_atexit(void) {
    // if not a successful exit, supress further output
    if (exit_status == EXIT_SUCCESS) {
        // summary information about tests (if any)
        if (base_check_count > 0) {
            int fail_count = base_check_count - base_check_success_count;
            if (fail_count <= 0) {
                if (base_check_count == 1) {
                    fprintf(stderr, "The test passed!\n");
                } else if (base_check_count == 2) {
                    fprintf(stderr, "Both tests passed!\n");
                } else if (base_check_count >= 3) {
                    fprintf(stderr, "All %d tests passed!\n", base_check_count);
                }
            } else {
                if (base_check_count == 1) {
                    fprintf(stderr, "The test failed.\n");
                } else {
                    if (base_check_success_count == 0) {
                        fprintf(stderr, "0 of %d tests passed.\n", base_check_count);
                    } else {
                        fprintf(stderr, "%d of %d tests failed.\n", fail_count, base_check_count);
                    }
                }
            }
        }
        // information about memory leaks (if any)
        if (do_memory_check) {
            base_check_memory();
        }
    }
}

#if 0
void base_atexit_test(void) {
    base_check_count = 0;
    base_check_success_count = 0;
    base_atexit();
    
    base_check_count = 1;
    base_check_success_count = 0;
    base_atexit();
    
    base_check_count = 1;
    base_check_success_count = 1;
    base_atexit();

    base_check_count = 2;
    base_check_success_count = 0;
    base_atexit();
    
    base_check_count = 2;
    base_check_success_count = 1;
    base_atexit();
    
    base_check_count = 2;
    base_check_success_count = 2;
    base_atexit();
    
    base_check_count = 3;
    base_check_success_count = 0;
    base_atexit();
    
    base_check_count = 3;
    base_check_success_count = 1;
    base_atexit();
    
    base_check_count = 3;
    base_check_success_count = 2;
    base_atexit();
    
    base_check_count = 3;
    base_check_success_count = 3;
    base_atexit();
    
}
#endif

bool base_test_equal_b(const char *file, int line, bool a, bool e) {
    base_init();
    base_check_count++;
    if (a == e) {
        printf("%s, line %d: Test passed.\n", file, line);
        base_check_success_count++;
        return true;
    } else {
        printf("%s, line %d: Actual value ", file, line);
        printb(a);
        prints(" differs from expected value ");
        printb(e);
        printsln(".");
        return false;
    }
}

bool base_test_equal_i(const char *file, int line, int a, int e) {
    base_init();
    base_check_count++;
    if (a == e) {
        printf("%s, line %d: Test passed.\n", file, line);
        base_check_success_count++;
        return true;
    } else {
        printf("%s, line %d: Actual value %d differs from expected value %d.\n", file, line, a, e);
        return false;
    }
}

bool base_test_within_d(const char *file, int line, double a, double e, double epsilon) {
    base_init();
    base_check_count++;
    if (fabs(a - e) <= epsilon) {
        printf("%s, line %d: Test passed.\n", file, line);
        base_check_success_count++;
        return true;
    } else {
        printf("%s, line %d: Actual value %g is not within %g of expected value %g.\n", file, line, a, epsilon, e);
        return false;
    }
}

bool base_test_within_i(const char *file, int line, int a, int e, int epsilon) {
    base_init();
    base_check_count++;
    if (abs(a - e) <= epsilon) {
        printf("%s, line %d: Test passed.\n", file, line);
        base_check_success_count++;
        return true;
    } else {
        printf("%s, line %d: Actual value %d is not within %d of expected value %d.\n", file, line, a, epsilon, e);
        return false;
    }
}

bool base_test_equal_c(const char *file, int line, char a, char e) {
    base_init();
    base_check_count++;
    if (a == e) {
        printf("%s, line %d: Test passed.\n", file, line);
        base_check_success_count++;
        return true;
    } else {
        printf("%s, line %d: Actual value '%c' differs from expected value '%c'.\n", file, line, a, e);
        return false;
    }
}

bool base_test_equal_s(const char *file, int line, String a, String e) {
    base_init();
    base_check_count++;
    if ((a != NULL && e != NULL && strcmp(a, e) == 0) || (a == NULL && e == NULL)) {
        printf("%s, line %d: Test passed.\n", file, line);
        base_check_success_count++;
        return true;
    } else {
        printf("%s, line %d: Actual value \"%s\" differs from expected value \"%s\".\n", file, line, a, e);
        return false;
    }
}

bool base_test_equal_struct(const char *file, int line, 
    Any actual, Any expected, Any predicate) 
{
    bool (*pred)(Any, Any) = predicate;
    base_init();
    base_check_count++;
    if (pred(actual, expected)) {
        printf("%s, line %d: Test passed.\n", file, line);
        base_check_success_count++;
        return true;
    } else {
        printf("%s, line %d: Test failed. Actual value differs from expected value.\n", file, line);
        return false;
    }
}

void base_count_check(void) {
    base_init();
    base_check_count++;
}

void base_count_success(void) {
    base_init();
    base_check_success_count++;
}



////////////////////////////////////////////////////////////////////////////
// Testing base itself

int baseTest(void) {
/*
    printiln(123);
    printi(123);
    println();
    printdln(123456.123456);
    printd(123456.123456);
    println();
    printcln('a');
    printc('x');
    printc('y');
    printcln('z');
    printc('a');
    printc('\t');
    printc('b');
    println();
    printsln("hello");
    prints("hello");
    println();
    
    int as[] = { 1, 2, 3, 123, 234, 345, 1234, -2345 };
    printia(as, 8);
    println();
    printialn(as, 8);
    printialn(as, 7);
    printialn(as, 9);
    
    double ds[] = { 1.1, 2.2, 3.3, 123.123, 234.234, 345.345, 1234.1234, -2345.2345 };
    printda(ds, 8);
    println();
    printdaln(ds, 8);
    
    String ss[] = { "a", "b", "c", "ab", "hello", "world" };
    printsa(ss, 6);
    println();
    printsaln(ss, 6);
    
*/
    return 0;
}
