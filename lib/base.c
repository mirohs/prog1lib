/*
@author Michael Rohs
@date 15.10.2015
@copyright Apache License, Version 2.0
*/

#include "base.h"
#include "string.h"
#include "list.h"
#undef free // use the 'real' free here
//#undef xmalloc
//#undef xcalloc



////////////////////////////////////////////////////////////////////////////
// Memory allocation

// http://www.gnu.org/software/libc/manual/html_node/Hooks-for-Malloc.html#Hooks-for-Malloc
// this GNU C solution does not work on Mac OS X
// Mac OS X solution does not work on other platforms
// so simply use preprocessor, does not catch things like strdup (or use macro for that as well)

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
        printf("base_free: trying to free unknown pointer %p\n", p);
    }

    free(p);
}

static bool base_atexit_registered = false;
void base_atexit(void);

static bool do_memory_check = false;

void base_set_memory_check(bool do_check) {
    do_memory_check = do_check;
}

void base_init(void) {
    if (!base_atexit_registered) {
        atexit(base_atexit);
        base_atexit_registered = true;
    }
}

Any base_malloc(const char *file, const char *function, int line, size_t size) {
    Any p = malloc(size);
    if (p == NULL) {
        printf("%s, line %d: malloc(%lu) returned NULL!\n", file, line, (unsigned long)size);
        exit(EXIT_FAILURE);
    }
    // printf("%s, line %d: malloc(%lu) returned %lx\n", file, line, (unsigned long)size, (unsigned long)p);

    BaseAllocInfo *ai = malloc(sizeof(BaseAllocInfo));
    if (ai == NULL) {
        printf("%s, line %d: malloc(%lu) returned NULL!\n", file, line, (unsigned long)size);
        exit(EXIT_FAILURE);
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

Any base_calloc(const char *file, const char *function, int line, size_t num, size_t size) {
    // printf("%s, line %d: xcalloc(%lu, %lu)\n", file, line, (unsigned long)num, (unsigned long)size);
    Any p = calloc(num, size);
    if (p == NULL) {
        printf("%s, line %d: xcalloc(%lu, %lu) returned NULL!\n", file, line, (unsigned long)num, (unsigned long)size);
        exit(EXIT_FAILURE);
    }
    // printf("%s, line %d: xcalloc(%lu, %lu) returned %lx\n", file, line, (unsigned long)num, (unsigned long)size, (unsigned long)p);

    BaseAllocInfo *ai = malloc(sizeof(BaseAllocInfo));
    if (ai == NULL) {
        printf("%s, line %d: malloc(%lu) returned NULL!\n", file, line, (unsigned long)num * (unsigned long)size);
        exit(EXIT_FAILURE);
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
    int n = 0;
    size_t s = 0;

    // @todo: group leaks by file, order by increasing line number, report each line number only once.
    for (BaseAllocInfo *ai = base_alloc_info; ai != NULL; ai = ai->next) {
        if (n < 5) { // only show the first ones explicitly
            printf("%5lu bytes allocated in %s (%s at line %d) not freed\n", (unsigned long)ai->size, ai->function, ai->file, ai->line);
        }
        n++;
        s += ai->size;
    }

    if (n > 0) {
        printf("%d memory leak%s, %lu bytes total\n", n, n == 1 ? "" : "s", (unsigned long)s);
    } else {
        // printf("No memory leaks.\n");
    }
}



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



////////////////////////////////////////////////////////////////////////////
// Conversion

int i_of_s(String s) {
    return atoi(s);
}

double d_of_s(String s) {
    return atof(s);
}

double d_of_s_sub(String s, int start, int end) {
    int n = s_length(s);
    if (n <= 0 || end <= 0 || start >= n || start >= end) return 0.0;
    String t = s_sub(s, start, end);
    double d = atof(t);
    s_free(t);
    return d;
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
    printf("%s", s);
}

void printsln(String s) {
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
    printia(a, n);
    println();
}

void printda(double *a, int n) {
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
    printda(a, n);
    println();
}

void printsa(String *a, int n) {
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
    printsa(a, n);
    println();
}

void printca(char *a, int n) {
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
    printca(a, n);
    println();
}

void printba(Byte *a, int n) {
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
    printba(a, n);
    println();
}

void printboa(bool *a, int n) {
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
    printboa(a, n);
    println();
}



////////////////////////////////////////////////////////////////////////////
// Input

void get_line(char *line, int n) {
    if (line == NULL) {
        printf("%s: line cannot be NULL\n", (String)__func__);
        exit(EXIT_FAILURE);
    }
    if (n < 8) {
        printf("%s: n = %d (has to be 8 or greater)\n", (String)__func__, n);
        exit(EXIT_FAILURE);
    }
    fgets(line, n, stdin);
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
    fgets(line, n, stdin);
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
    s_free(s);
    return i;
}

double d_input(void) {
    String s = s_input(100);
    double d = d_of_s(s);
    s_free(s);
    return d;
}



////////////////////////////////////////////////////////////////////////////
// Files

String s_read_file(String name) {
    FILE *f = fopen(name, "r");
    if (f == NULL) {
        fprintf(stderr, "%s: Cannot open %s\n", (String)__func__, name); 
        exit(EXIT_FAILURE);
    }
    fseek (f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);
    
    char *s = base_malloc(__FILE__, __func__, __LINE__, size + 1);
    if (s == NULL) {
        fprintf(stderr, "%s: Cannot allocate memory.\n", (String)__func__); 
        exit(EXIT_FAILURE);
    }
    long sizeRead = fread(s, 1, size, f);
    if (sizeRead != size) {
        fprintf(stderr, "%s: Only read %ld of %ld bytes.\n", (String)__func__, sizeRead, size); 
        exit(EXIT_FAILURE);
    }
    s[size] = '\0';
    
    fclose(f);
    return s;
}

void s_write_file(String name, String data) {
    FILE *f = fopen(name, "w");
    if (f == NULL) {
        fprintf(stderr, "%s: Cannot open %s\n", (String)__func__, name); 
        exit(EXIT_FAILURE);
    }
    
    fwrite (data , 1, strlen(data), f);
    
    fclose(f);
}

void write_file_data(String name, Byte *data, int n) {
    FILE *f = fopen(name, "w");
    if (f == NULL) {
        fprintf(stderr, "%s: Cannot open %s\n", (String)__func__, name); 
        exit(EXIT_FAILURE);
    }
    
    fwrite (data , 1, n, f);
    
    fclose(f);
}



////////////////////////////////////////////////////////////////////////////
// Random numbers

static bool srand_called = false;

int i_rnd(int i) {
    if (!srand_called) {
        srand(time(NULL) << 10);
        srand_called = true;
    }
    return rand() % i;
}

double d_rnd(double i) {
    if (!srand_called) {
        srand(time(NULL) << 10);
        srand_called = true;
    }
    double r = (double) rand() / (double) RAND_MAX;
    return i * r;   
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
    if (base_check_count > 0) { // @todo: improve error message on failure exit
        int fail_count = base_check_count - base_check_success_count;
        if (fail_count <= 0) {
            if (base_check_count == 1) {
                printf("The test passed!\n");
            } else if (base_check_count == 2) {
                printf("Both tests passed!\n");
            } else if (base_check_count >= 3) {
                printf("All %d tests passed!\n", base_check_count);
            }
        } else {
            if (base_check_count == 1) {
                printf("The test failed.\n");
            } else {
                if (base_check_success_count == 0) {
                    printf("0 of %d tests passed.\n", base_check_count);
                } else {
                    printf("%d of %d tests failed.\n", fail_count, base_check_count);
                }
            }
        }
    }
    
    if (do_memory_check) {
        base_check_memory();
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
        printf("%s, line %d: check passed\n", file, line);
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
        printf("%s, line %d: check passed\n", file, line);
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
        printf("%s, line %d: check passed\n", file, line);
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
        printf("%s, line %d: check passed\n", file, line);
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
        printf("%s, line %d: check passed\n", file, line);
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
    if (strcmp(a, e) == 0) {
        printf("%s, line %d: check passed\n", file, line);
        base_check_success_count++;
        return true;
    } else {
        printf("%s, line %d: Actual value \"%s\" differs from expected value \"%s\".\n", file, line, a, e);
        return false;
    }
}

bool base_test_equal_ca(const char *file, int line, Array a, char *e, int ne) {
    base_init();
    base_check_count++;
    if (a->n != ne) {
        printf("%s, line %d: Actual length %d "
            "differs from expected length %d\n", file, line, a->n, ne);
        return false;
    }
    if (a->s != sizeof(char)) {
        printf("%s, line %d: Actual element size %d "
            "differs from expected element size %lu\n", file, line, a->s, (unsigned long)sizeof(char));
        return false;
    }
    if (a->n < 0) {
        printf("%s, line %d: Invalid lengths %d\n", file, line, a->n);
        return false;       
    }
    if (ne < 0) {
        printf("%s, line %d: Invalid lengths %d\n", file, line, ne);
        return false;       
    }
    if (a->n > 0 && a->a == NULL) {
        printf("%s, line %d: Actual value array is NULL\n", file, line);
        return false;       
    }
    if (ne > 0 && e == NULL) {
        printf("%s, line %d: Expected value array is NULL\n", file, line);
        return false;       
    }
    char *ca = a->a;
    for (int i = 0; i < a->n; i++) {
        if (ca[i] != e[i]) {
            printf("%s, line %d: Actual value ", file, line);
            printca(ca, a->n);
            prints(" differs from expected value ");
            printca(e, ne);
            printf(" at index %d.\n", i);
            return false;
        }
    }
    printf("%s, line %d: check passed\n", file, line);
    base_check_success_count++;
    return true;
}

bool base_test_equal_boa(const char *file, int line, Array a, bool *e, int ne) {
    base_init();
    base_check_count++;
    if (a->n != ne) {
        printf("%s, line %d: Actual length %d "
            "differs from expected length %d\n", file, line, a->n, ne);
        return false;
    }
    if (a->s != sizeof(bool)) {
        printf("%s, line %d: Actual element size %d "
            "differs from expected element size %lu\n", file, line, a->s, (unsigned long)sizeof(bool));
        return false;
    }
    if (a->n < 0) {
        printf("%s, line %d: Invalid lengths %d\n", file, line, a->n);
        return false;       
    }
    if (ne < 0) {
        printf("%s, line %d: Invalid lengths %d\n", file, line, ne);
        return false;       
    }
    if (a->n > 0 && a->a == NULL) {
        printf("%s, line %d: Actual value array is NULL\n", file, line);
        return false;       
    }
    if (ne > 0 && e == NULL) {
        printf("%s, line %d: Expected value array is NULL\n", file, line);
        return false;       
    }
    bool *ba = a->a;
    for (int i = 0; i < a->n; i++) {
        if (ba[i] != e[i]) {
            printf("%s, line %d: Actual value ", file, line);
            printboa(ba, a->n);
            prints(" differs from expected value ");
            printboa(e, ne);
            printf(" at index %d.\n", i);
            return false;
        }
    }
    printf("%s, line %d: check passed\n", file, line);
    base_check_success_count++;
    return true;
}

bool base_test_equal_struct(const char *file, int line, 
    Any actual, Any expected, Any predicate) 
{
    bool (*pred)(Any, Any) = predicate;
    base_init();
    base_check_count++;
    if (pred(actual, expected)) {
        printf("%s, line %d: check passed\n", file, line);
        base_check_success_count++;
        return true;
    } else {
        printf("%s, line %d: Actual value differs from expected value.\n", file, line);
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
