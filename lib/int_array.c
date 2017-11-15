/*
@author Michael Rohs
@date 15.10.2015
@copyright Apache License, Version 2.0
*/

#include "array.h"
#include "int_array.h"

Array ia_create(int n, int init);

static void ia_create_test(void) {
    printsln((String)__func__);
    Array array;
    
    array = ia_create(3, 0);
    int a1[] = { 0, 0, 0 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a1, 3);
    a_free(array);
    
    array = ia_create(5, -1);
    int a2[] = { -1, -1, -1, -1, -1 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a2, 5);
    a_free(array);
    
    array = ia_create(1, 2);
    int a3[] = { 2 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a3, 1);
    a_free(array);
    
    array = ia_create(0, 2);
    int a4[] = { };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a4, 0);
    a_free(array);
}

Array ia_create(int n, int value) {
    if (n < 0) {
        printf("%s: length cannot be negative (is %d)\n", __func__, n);
        exit(EXIT_FAILURE);
    }
    int *a = xmalloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        a[i] = value;
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(int);
    result->a = a;
    return result;
}

Array ia_range(int a, int b);

static void ia_range_test(void) {
    printsln((String)__func__);
    Array array;

    array = ia_range(0, 3);
    int a1[] = { 0, 1, 2 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a1, 3);
    a_free(array);

    array = ia_range(3, 2);
    int a2[] = { 3 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a2, 1);
    a_free(array);

    array = ia_range(2, 3);
    int a3[] = { 2 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a3, 1);
    a_free(array);

    array = ia_range(3, 3);
    int a4[] = { };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a4, 0);
    a_free(array);

    array = ia_range(-3, -3);
    int a5[] = { };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a5, 0);
    a_free(array);

    array = ia_range(1, 4);
    int a6[] = { 1, 2, 3 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a6, 3);
    a_free(array);

    array = ia_range(4, 1);
    int a7[] = { 4, 3, 2 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a7, 3);
    a_free(array);
}

Array ia_range(int a, int b) {
    if (a <= b) {
        int n = b - a;
        int *arr = xmalloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            arr[i] = a + i;
        }
        Array result = xmalloc(sizeof(ArrayHead));
        result->n = n;
        result->s = sizeof(int);
        result->a = arr;
        return result;
    } else /* a > b */ {
        int n = a - b;
        int *arr = xmalloc(n * sizeof(int));
        for (int i = 0; i < n; i++) {
            arr[i] = a - i;
        }
        Array result = xmalloc(sizeof(ArrayHead));
        result->n = n;
        result->s = sizeof(int);
        result->a = arr;
        return result;
    }
}

Array ia_of_string(String s);

static void ia_of_string_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = ia_of_string("1, 2, 3, 4, 5, 6");
    ex = ia_range(1, 7);
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("   1, 2, 3, 4, 5, 6   ");
    ex = ia_range(1, 7);
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("1xx2asdfs3");
    ex = ia_range(1, 4);
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("y1xx2asdfs3x");
    ex = ia_range(1, 4);
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("-3, -2, -1, 0, 1");
    ex = ia_range(-3, 2);
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string(" -3 -2 -1 ");
    ex = ia_range(-3, 0);
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string(".1.-2.-.3.");
    ex = ia_create(3, 0);
    ia_set(ex, 0, 1);
    ia_set(ex, 1, -2);
    ia_set(ex, 2, 3);
    ia_test_equal(ac, ex);
    a_free(ac);

    ac = ia_of_string("-.1.-2.-.3.-");
    ia_test_equal(ac, ex);
    a_free(ac);

    ac = ia_of_string("---.1.----2.----.3.----");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("-x--+ +.+++a-b c-");
    ex = ia_create(0, 0);
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("");
    ex = ia_create(0, 0);
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
}

Array ia_of_string(String s) {
    assert_argument_not_null(s);
    // count number of ints in s
    int n = 0; // array length
    char *t = s;
    while (*t != '\0') {
        if (isdigit(*t)) { // found start of int
            n++; // one more int found
            t++; // assert: *t is a digit, skip it
            while (isdigit(*t)) t++; // skip int
            // assert: *t is not a digit, *t may be '\0'
        } else {
            // assert: *t is not a digit, *t is not '\0'
            t++; // not a digit, skip
        }
    }

    // n ints found
    int *a = xmalloc(n * sizeof(int));
    t = s;
    int i = 0;
    while (*t != '\0') {
        if (isdigit(*t)) {
            if ((t > s) && (*(t - 1) == '-')) t--; // check for minus sign, no whitespace between '-' and digit
            a[i++] = atoi(t); // convert digit string to int
            t++; // assert: *t is a digit or '-', skip
            while (isdigit(*t)) t++; // skip int
            // assert: *t is not a digit, *t may be '\0'
        } else {
            // assert: *t is not a digit, *t is not '\0'
            t++; // not a digit, skip
        }
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(int);
    result->a = a;
    return result;
}


Array ia_fn(int n, IntIntToInt init, int x);

static int two_i_plus_1(int index, int x) {
    return 2 * index + 1;
}

static void ia_fn_test(void) {
    printsln((String)__func__);
    Array array;

    array = ia_fn(3, two_i_plus_1, 0);
    int a1[] = { 1, 3, 5 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a1, 3);
    a_free(array);

    array = ia_fn(4, two_i_plus_1, 0);
    int a2[] = { 1, 3, 5, 7 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a2, 4);
    a_free(array);

    array = ia_fn(1, two_i_plus_1, 0);
    int a3[] = { 1 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a3, 1);
    a_free(array);

    array = ia_fn(0, two_i_plus_1, 0);
    int a4[] = { };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a4, 0);
    a_free(array);
}

Array ia_fn(int n, IntIntToInt init, int x) {
    assert_function_not_null(init);
    if (n < 0) {
        printf("ia_fn: length cannot be negative (is %d)\n", n);
        exit(EXIT_FAILURE);
    }
    int *a = xmalloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        a[i] = init(i, x);
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(int);
    result->a = a;
    return result;
}

Array ia_of_da(Array array);

static void ia_of_da_test(void) {
    printsln((String)__func__);
    Array da;
    Array ex, ac;
    da = da_of_string("0.0 0.49999 0.5 1.0 -0.49999 -0.50001 -1.0");
    ex = ia_of_string(" 0   0       1   1    0       -1       -1 ");
    ac = ia_of_da(da);
    ia_test_equal(ac, ex);
    a_free(da);
    a_free(ex);
    a_free(ac);
}

Array ia_of_da(Array array) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    int n = a_length(array);
    Array result = ia_create(n, 0);
    double *a = array->a;
    int *b = result->a;
    for (int i = 0; i < n; i++) {
        b[i] = round(a[i]);
    }
    return result;
}

#ifdef A_GET_SET
int ia_get(Array array, int index) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    if (index < 0 || index >= array->n) {
        printf("ia_get: index %d is out of range "
            "(array length: %d, allowed indices: 0..%d)\n", 
        index, array->n, array->n - 1);
        exit(EXIT_FAILURE);
    }
    int *a = array->a;
    return a[index];
}
#endif

#ifdef A_GET_SET
void ia_set(Array array, int index, int value) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    if (index < 0 || index >= array->n) {
        printf("ia_set: index %d is out of range "
            "(array length: %d, allowed indices: 0..%d)\n", 
        index, array->n, array->n - 1);
        exit(EXIT_FAILURE);
    }
    int *a = array->a;
    a[index] = value;
}
#endif

#ifdef A_GET_SET
int ia_inc(Array array, int index, int value) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    if (index < 0 || index >= array->n) {
        printf("ia_inc: index %d is out of range "
            "(array length: %d, allowed indices: 0..%d)\n", 
        index, array->n, array->n - 1);
        exit(EXIT_FAILURE);
    }
    int *a = array->a;
    value += a[index];
    a[index] = value;
    return value;
}
#endif

void ia_print(Array array) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    int *a = array->a;
    printf("[");
    if (array->n > 0) {
        printf("%d", a[0]);
    }
    for (int i = 1; i < array->n; i++) {
        printf(" %d", a[i]);
    }
    printf("]");
}

void ia_println(Array array) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    ia_print(array);
    printf("\n");
}

bool ia_contains(Array array, int value);

static void ia_contains_test(void) {
    printsln((String)__func__);
    Array array;
    array = ia_of_string("10, 20, 30");
    test_equal_b(ia_contains(array, 10), true);
    test_equal_b(ia_contains(array, 11), false);
    test_equal_b(ia_contains(array, 30), true);
    a_free(array);
}

bool ia_contains(Array array, int value) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    int *a = array->a;
    for (int i = 0; i < array->n; i++) {
        if (a[i] == value) {
            return true;
        }
    }
    return false;
}

void ia_fill(Array array, int value);

static void ia_fill_test(void) {
    printsln((String)__func__);
    Array array;

    array = ia_create(3, 0);
    ia_fill(array, -1);
    int a1[] = { -1, -1, -1 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a1, 3);
    a_free(array);

    array = ia_create(2, 0);
    ia_fill(array, 2);
    int a2[] = { 2, 2 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a2, 2);
    a_free(array);

    array = ia_create(0, 0);
    ia_fill(array, 2);
    int a3[] = { };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a3, 0);
    a_free(array);
}

void ia_fill(Array array, int value) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    int *a = array->a;
    for (int i = 0; i < array->n; i++) {
        a[i] = value;
    }
}

void ia_fill_from_to(Array array, int value, int from, int to);

static void ia_fill_from_to_test(void) {
    printsln((String)__func__);
    Array array;

    array = ia_create(3, 0);
    ia_fill_from_to(array, -1, 0, array->n);
    int a1[] = { -1, -1, -1 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a1, 3);
    a_free(array);

    array = ia_create(3, 0);
    ia_fill_from_to(array, -1, -1, array->n + 1);
    int a2[] = { -1, -1, -1 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a2, 3);
    a_free(array);

    array = ia_create(3, 0);
    ia_fill_from_to(array, -1, 0, 0);
    int a3[] = { 0, 0, 0 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a3, 3);
    a_free(array);

    array = ia_create(3, 0);
    ia_fill_from_to(array, -1, 0, 1);
    int a4[] = { -1, 0, 0 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a4, 3);
    a_free(array);

    array = ia_create(3, 0);
    ia_fill_from_to(array, -1, 2, 2);
    int a5[] = { 0, 0, 0 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a5, 3);
    a_free(array);

    array = ia_create(3, 0);
    ia_fill_from_to(array, -1, 2, 3);
    int a6[] = { 0, 0, -1 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a6, 3);
    a_free(array);

    array = ia_create(3, 0);
    ia_fill_from_to(array, -1, 2, 1);
    int a7[] = { 0, 0, 0 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a7, 3);
    a_free(array);

    array = ia_create(3, 0);
    ia_fill_from_to(array, -1, 1, 3);
    int a8[] = { 0, -1, -1 };
    ia_test_equal_file_line(__FILE__, __func__, __LINE__, array, a8, 3);
    a_free(array);
}

void ia_fill_from_to(Array array, int value, int from, int to) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    if (from < 0) from = 0;
    if (to > array->n) to = array->n;
    int *a = array->a;
    for (int i = from; i < to; i++) {
        a[i] = value;
    }
}

int ia_index(Array array, int value);

static void ia_index_test(void) {
    printsln((String)__func__);
    Array array;

    array = ia_range(1, 3);
    int i = ia_index(array, 0);
#if 0
    if (i < 0) {
        printsln("value not found");
    } else {
        printf("value found at index %d\n", i);
    }
#endif
    test_equal_i(i, -1);

    i = ia_index(array, 1);
    test_equal_i(i, 0);
        
    i = ia_index(array, 2);
    test_equal_i(i, 1);
        
    a_free(array);
    
}

int ia_index(Array array, int value) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    int *a = array->a;
    for (int i = 0; i < array->n; i++) {
        if (a[i] == value) {
            return i;
        }
    }
    return -1;
}

int ia_index_from(Array array, int value, int from);

static void ia_index_from_test(void) {
    printsln((String)__func__);
    Array a = ia_of_string("10 20 30 40 50");
    test_equal_i(ia_index_from(a, 20, 0), 1);
    test_equal_i(ia_index_from(a, 20, 1), 1);
    test_equal_i(ia_index_from(a, 20, 2), -1);
    test_equal_i(ia_index_from(a, 30, -1), 2);
    test_equal_i(ia_index_from(a, 60, 0), -1);
    a_free(a);
    
}

int ia_index_from(Array array, int value, int from) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    if (from < 0) from = 0;
    int *a = array->a;
    for (int i = from; i < array->n; i++) {
        if (a[i] == value) {
            return i;
        }
    }
    return -1;
}

int ia_index_fn(Array array, IntIntIntToBool predicate, int x);

static void ia_index_fn_test(void) {
    printsln((String)__func__);
    Array a = ia_of_string("1 2 3 -4 5");
    test_equal_i(ia_index_fn(a, ia_even, 0), 1);
    test_equal_i(ia_index_fn(a, ia_lt, 0), 3);
    a_free(a);
}

int ia_index_fn(Array array, IntIntIntToBool predicate, int x) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    ia_assert_element_size(array);
    int *a = array->a;
    for (int i = 0; i < array->n; i++) {
        if (predicate(a[i], i, x)) {
            return i;
        }
    }
    return -1;
}

int ia_last_index(Array array, int value);

static void ia_last_index_test(void) {
    printsln((String)__func__);
    Array a = ia_of_string("10 20 30 10 20 30");
    test_equal_i(ia_last_index(a, 20), 4);
    test_equal_i(ia_last_index(a, 30), 5);
    test_equal_i(ia_last_index(a, 60), -1);
    a_free(a);
}

int ia_last_index(Array array, int value) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    int *a = array->a;
    for (int i = array->n - 1; i >= 0; i--) {
        if (a[i] == value) {
            return i;
        }
    }
    return -1;
}

int ia_last_index_from(Array array, int value, int from);

static void ia_last_index_from_test(void) {
    printsln((String)__func__);
    Array a = ia_of_string("10 20 30 10 20 30");
    test_equal_i(ia_last_index_from(a, 20, 5), 4);
    test_equal_i(ia_last_index_from(a, 20, 4), 4);
    test_equal_i(ia_last_index_from(a, 20, 3), 1);
    test_equal_i(ia_last_index_from(a, 30, 2), 2);
    test_equal_i(ia_last_index_from(a, 30, 1), -1);
    a_free(a);    
}

int ia_last_index_from(Array array, int value, int from) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    if (from >= array->n) from = array->n - 1;
    int *a = array->a;
    for (int i = from; i >= 0; i--) {
        if (a[i] == value) {
            return i;
        }
    }
    return -1;
}

int ia_last_index_fn(Array array, IntIntIntToBool predicate, int x);

static void ia_last_index_fn_test(void) {
    printsln((String)__func__);
    Array a = ia_of_string("1 2 2 3 -3 5");
    test_equal_i(ia_last_index_fn(a, ia_even, 0), 2);
    test_equal_i(ia_last_index_fn(a, ia_lt, 0), 4);
    a_free(a);
}

int ia_last_index_fn(Array array, IntIntIntToBool predicate, int x) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    ia_assert_element_size(array);
    int *a = array->a;
    for (int i = array->n - 1; i >= 0; i--) {
        if (predicate(a[i], i, x)) {
            return i;
        }
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////

static CmpResult int_compare(ConstAny a, ConstAny b) {
    assert_argument_not_null(a);
    assert_argument_not_null(b);
    int x = *(int*)a;
    int y = *(int*)b;
    return (x == y) ? 0 : (x < y ? -1 : 1);
}

void ia_sort(Array array);

static void ia_sort_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = ia_of_string("1, 2, 3, 4, 5");
    ex = ia_of_string("1, 2, 3, 4, 5");
    a_shuffle(ac);
    ia_sort(ac);
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("1, 2, 1, 3, 2");
    ex = ia_of_string("1, 1, 2, 2, 3");
    ia_sort(ac);
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("");
    ex = ia_of_string("");
    ia_sort(ac);
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("-1, -2, -3, -1");
    ex = ia_of_string("-3, -2, -1, -1");
    ia_sort(ac);
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
}

void ia_sort(Array array) {
    ia_assert_element_size(array);
    qsort(array->a, array->n, array->s, int_compare);
}

static CmpResult int_compare_dec(ConstAny a, ConstAny b) {
    assert_argument_not_null(a);
    assert_argument_not_null(b);
    int x = *(int*)b;
    int y = *(int*)a;
    return (x == y) ? 0 : (x < y ? -1 : 1);
}

void ia_sort_dec(Array array);

static void ia_sort_dec_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = ia_of_string("1, 2, 3, 4, 5");
    ex = ia_of_string("5, 4, 3, 2, 1");
    a_shuffle(ac);
    ia_sort_dec(ac);
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("1, 2, 1, 3, 2");
    ex = ia_of_string("3, 2, 2, 1, 1");
    ia_sort_dec(ac);
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("");
    ex = ia_of_string("");
    ia_sort_dec(ac);
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("-3, -2, -1");
    ex = ia_of_string("-1, -2, -3");
    ia_sort_dec(ac);
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
}

void ia_sort_dec(Array array) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    qsort(array->a, array->n, array->s, int_compare_dec);
}

#if 0
void ia_insert(Array array, int i, int v);

static void ia_insert_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = ia_of_string("1, 2, 3, 4, 5, 6");
    ia_insert(ac, 0, 9);
    ex = ia_of_string("9, 1, 2, 3, 4, 5");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ia_of_string("1, 2, 3, 4, 5, 6");
    ia_insert(ac, 5, 9);
    ex = ia_of_string("1, 2, 3, 4, 5, 9");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ia_of_string("1, 2, 3, 4, 5, 6");
    ia_insert(ac, 3, 9);
    ex = ia_of_string("1, 2, 3, 9, 4, 5");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ia_of_string("1");
    ia_insert(ac, -1, 9);
    ex = ia_of_string("1");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ia_of_string("1");
    ia_insert(ac, 1, 9);
    ex = ia_of_string("1");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ia_of_string("");
    ia_insert(ac, 0, 9);
    ex = ia_of_string("");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
}

void ia_insert(Array array, int i, int v) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    if (i < 0 || i >= array->n) return;
    // make space at i
    int *a = array->a;
    for (int j = array->n - 1; j > i; j--) {
        a[j] = a[j - 1];
    }
    // set value at i to v
    a[i] = v;
}

void ia_remove(Array array, int i, int v);

static void ia_remove_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = ia_of_string("1, 2, 3, 4, 5, 6");
    ia_remove(ac, 0, 9);
    ex = ia_of_string("2, 3, 4, 5, 6, 9");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ia_of_string("1, 2, 3, 4, 5, 6");
    ia_remove(ac, 5, 9);
    ex = ia_of_string("1, 2, 3, 4, 5, 9");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ia_of_string("1, 2, 3, 4, 5, 6");
    ia_remove(ac, 3, 9);
    ex = ia_of_string("1, 2, 3, 5, 6, 9");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ia_of_string("1");
    ia_remove(ac, -1, 9);
    ex = ia_of_string("1");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ia_of_string("1");
    ia_remove(ac, 1, 9);
    ex = ia_of_string("1");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ia_of_string("1");
    ia_remove(ac, 0, 9);
    ex = ia_of_string("9");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ia_of_string("");
    ia_remove(ac, 0, 9);
    ex = ia_of_string("");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
}

void ia_remove(Array array, int i, int v) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    if (i < 0 || i >= array->n) return;
    // shift down, starting from i
    int *a = array->a;
    for (int j = i; j < array->n - 1; j++) {
        a[j] = a[j + 1];
    }
    // set value at n-1 to v
    a[array->n - 1] = v;
}
#endif

///////////////////////////////////////////////////////////////////////////////

bool ia_even(int value, int index, int x) {
    return (value & 1) == 0;
}

bool int_odd(int value, int index, int x) {
    return (value & 1) == 1;
}

bool ia_index_even(int value, int index) {
    return (index & 1) == 0;
}

bool ia_index_odd(int value, int index) {
    return (index & 1) == 1;
}

bool ia_pos(int value, int index) {
    return value > 0;
}

bool ia_neg(int value, int index) {
    return value < 0;
}

bool ia_ge0(int value, int index) {
    return value >= 0;
}

bool ia_gt(int value, int index, int x) {
    return value > x;
}

bool ia_ge(int value, int index, int x) {
    return value >= x;
}

bool ia_lt(int value, int index, int x) {
    return value < x;
}

bool ia_le(int value, int index, int x) {
    return value <= x;
}

///////////////////////////////////////////////////////////////////////////////

int ia_square(int value, int index) {
    return value * value; // index not used
}

int ia_rnd(int index, int maximum) {
    return i_rnd(maximum); // index not used
}

int ia_times(int value, int index, int x) {
    return value * x;
}

int int_plus(int x, int y, int index) {
    return x + y;
}

int int_minus(int x, int y, int index) {
    return x - y;
}

int int_mult(int x, int y, int index) {
    return x * y;
}

int int_div(int x, int y, int index) {
    return x / y;
}

void ia_each(Array array, IntIntIntToInt f, int x);

static void ia_each_test(void) {
    printsln((String)__func__);
    Array a, ex;
    
    a = ia_of_string("1, 2, 3, 4");
    ia_each(a, ia_times, 2);
    ex = ia_of_string("2, 4, 6, 8");
    ia_test_equal(a, ex);
    a_free(a);
    a_free(ex);
    
    a = ia_of_string("");
    ia_each(a, ia_times, 3);
    ex = ia_of_string("");
    ia_test_equal(a, ex);
    a_free(a);
    a_free(ex);
}

void ia_each(Array array, IntIntIntToInt f, int x) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    assert_function_not_null(f);
    int *a = array->a;
    for (int i = 0; i < array->n; i++) {
        a[i] = f(a[i], i, x);
    }
}

void ia_each_state(Array array, IntIntIntAnyToInt f, int x, Any state);

static void ia_each_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

void ia_each_state(Array array, IntIntIntAnyToInt f, int x, Any state) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    assert_function_not_null(f);
    int *a = array->a;
    for (int i = 0; i < array->n; i++) {
        a[i] = f(a[i], i, x, state);
    }
}

// @todo: add tests
Array ia_map(Array array, IntIntIntToInt f, int x) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    assert_function_not_null(f);
    int *a = array->a;
    int *b = xmalloc(array->n * sizeof(int));
    for (int i = 0; i < array->n; i++) {
        b[i] = f(a[i], i, x);
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = array->n;
    result->s = sizeof(int);
    result->a = b;
    return result;
}

// @todo: add tests
Array ia_map_state(Array array, IntIntIntAnyToInt f, int x, Any state) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    assert_function_not_null(f);
    int *a = array->a;
    int *b = xmalloc(array->n * sizeof(int));
    for (int i = 0; i < array->n; i++) {
        b[i] = f(a[i], i, x, state);
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = array->n;
    result->s = sizeof(int);
    result->a = b;
    return result;
}

int ia_foldl(Array array, IntIntIntToInt f, int init);

static void ia_foldl_test(void) {
    printsln((String)__func__);
    Array a = ia_of_string("");    
    test_equal_i(ia_foldl(a, int_plus, 100), 100);
    test_equal_i(ia_foldl(a, int_minus, 100), 100);
    a_free(a);
    
    a = ia_of_string("1, 2, 3, 4");    
    test_equal_i(ia_foldl(a, int_plus, 0),    (((0 + 1) + 2) + 3) + 4);
    test_equal_i(ia_foldl(a, int_minus, 100), (((100 - 1) - 2) - 3) - 4);
    a_free(a);
}

int ia_foldl(Array array, IntIntIntToInt f, int init) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    assert_function_not_null(f);
    int *a = array->a;
    for (int i = 0; i < array->n; i++) {
        init = f(init, a[i], i);
    }
    return init;
}

int ia_foldr(Array array, IntIntIntToInt f, int init);

static void ia_foldr_test(void) {
    printsln((String)__func__);
    Array a = ia_of_string("");    
    test_equal_i(ia_foldr(a, int_plus, 100), 100);
    test_equal_i(ia_foldr(a, int_minus, 100), 100);
    a_free(a);
    
    a = ia_of_string("1, 2, 3, 4");    
    test_equal_i(ia_foldr(a, int_plus, 100), 1 + (2 + (3 + (4 + 100))));
    test_equal_i(ia_foldr(a, int_minus, 0), 1 - (2 - (3 - (4 - 0))));
    a_free(a);
}

int ia_foldr(Array array, IntIntIntToInt f, int init) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    assert_function_not_null(f);
    int *a = array->a;
    for (int i = array->n - 1; i >= 0; i--) {
        init = f(a[i], init, i);
    }
    return init;
}

Array ia_filter(Array array, IntIntIntToBool predicate, int x);

static void ia_filter_test(void) {
    printsln((String)__func__);
    Array a, ac, ex;

    a = ia_of_string("1, 2, 3, 4, 5, 6");
    ac = ia_filter(a, ia_gt, 3);
    ex = ia_of_string("4, 5, 6");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_filter(a, ia_lt, 4);
    ex = ia_of_string("1, 2, 3");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_filter(a, ia_le, 3);
    ex = ia_of_string("1, 2, 3");
    ia_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    a_free(a);
}

Array ia_filter(Array array, IntIntIntToBool predicate, int x) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    assert_function_not_null(predicate);
    int *a = array->a;
    bool *ps = xmalloc(array->n * sizeof(bool));
    int n = 0;
    for (int i = 0; i < array->n; i++) {
        ps[i] = predicate(a[i], i, x);
        if (ps[i]) n++;
    }
    int *b = xmalloc(n * sizeof(int));
    for (int i = 0, j = 0; i < array->n; i++) {
        if (ps[i]) {
            b[j++] = a[i];
        }
    }
    free(ps);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(int);
    result->a = b;
    return result;
}

// @todo: add tests
Array ia_filter_state(Array array, IntIntIntAnyToBool predicate, int x, Any state) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    assert_function_not_null(predicate);
    int *a = array->a;
    bool *ps = xmalloc(array->n * sizeof(bool));
    int n = 0;
    for (int i = 0; i < array->n; i++) {
        ps[i] = predicate(a[i], i, x, state);
        if (ps[i]) n++;
    }
    int *b = xmalloc(n * sizeof(int));
    for (int i = 0, j = 0; i < array->n; i++) {
        if (ps[i]) {
            b[j++] = a[i];
        }
    }
    free(ps);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(int);
    result->a = b;
    return result;
}

static IntOption evens_times_x(int element, int index, int x) {
    if ((element % 2) == 0) {
        return make_int_some(x * element);
    }
    return make_int_none();
}

static void ia_choose_test(void) {
    printsln((String)__func__);
    Array a, ac, ex;

    a = ia_of_string("1, 2, 3, 4, 5, 6");
    ac = ia_choose(a, evens_times_x, 3);
    ex = ia_of_string("6, 12, 18");
    ia_test_equal(ac, ex);

    a_free(ac);
    a_free(ex);
    a_free(a);
}

Array ia_choose(Array array, IntIntIntToIntOption f, int x) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    assert_function_not_null(f);
    int *a = array->a;
    int *b = xmalloc(array->n * sizeof(int));
    int n = 0;
    for (int i = 0; i < array->n; i++) {
        IntOption op = f(a[i], i, x);
        if (!op.none) {
            b[n++] = op.some;
        }
    }
    int *c = xmalloc(n * sizeof(int));
    memcpy(c, b, n * sizeof(int));
    free(b);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(int);
    result->a = c;
    return result;
}

// @todo: add tests
Array ia_choose_state(Array array, IntIntIntAnyToIntOption f, int x, Any state) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    assert_function_not_null(f);
    int *a = array->a;
    int *b = xmalloc(array->n * sizeof(int));
    int n = 0;
    for (int i = 0; i < array->n; i++) {
        IntOption op = f(a[i], i, x, state);
        if (!op.none) {
            b[n++] = op.some;
        }
    }
    int *c = xmalloc(n * sizeof(int));
    memcpy(c, b, n * sizeof(int));
    free(b);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(int);
    result->a = c;
    return result;
}

bool ia_exists(Array array, IntIntIntToBool predicate, int x);

static void ia_exists_test(void) {
    printsln((String)__func__);
    Array a = ia_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(ia_exists(a, ia_gt, 3), true);
    test_equal_b(ia_exists(a, ia_gt, 9), false);
    test_equal_b(ia_exists(a, ia_lt, -1), false);
    a_free(a);
}

bool ia_exists(Array array, IntIntIntToBool predicate, int x) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    assert_function_not_null(predicate);
    int *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (predicate(a[i], i, x)) {
            return true;
        }
    }
    return false;
}

// @todo: add tests
bool ia_exists_state(Array array, IntIntIntAnyToBool predicate, int x, Any state) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    assert_function_not_null(predicate);
    int *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (predicate(a[i], i, x, state)) {
            return true;
        }
    }
    return false;
}

bool ia_forall(Array array, IntIntIntToBool predicate, int x);

static void ia_forall_test(void) {
    printsln((String)__func__);
    Array a = ia_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(ia_forall(a, ia_gt, 0), true);
    test_equal_b(ia_forall(a, ia_gt, 1), false);
    test_equal_b(ia_forall(a, ia_lt, 7), true);
    a_free(a);
}

bool ia_forall(Array array, IntIntIntToBool predicate, int x) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    assert_function_not_null(predicate);
    int *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (!predicate(a[i], i, x)) {
            return false;
        }
    }
    return true;
}

bool ia_forall_state(Array array, IntIntIntAnyToBool predicate, int x, Any state) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    assert_function_not_null(predicate);
    int *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (!predicate(a[i], i, x, state)) {
            return false;
        }
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Testing

bool ia_test_equal_file_line(const char *file, const char *function, int line, Array a, int *e, int ne) {
    base_init();
    base_count_check();
    if (a->n != ne) {
        printf("%s, line %d: Actual length %d "
            "differs from expected length %d\n", file, line, a->n, ne);
        return false;
    }
    if (a->s != sizeof(int)) {
        printf("%s, line %d: Actual element size %d "
            "differs from expected element size %lu\n", file, line, a->s, (unsigned long)sizeof(int));
        return false;
    }
    if (a->n < 0) {
        printf("%s, line %d: Invalid actual length %d\n", file, line, a->n);
        return false;        
    }
    if (ne < 0) {
        printf("%s, line %d: Invalid expected length %d\n", file, line, ne);
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
    int *ia = a->a;
    for (int i = 0; i < a->n; i++) {
        if (ia[i] != e[i]) {
            printf("%s, line %d: Actual value ", file, line);
            printia(ia, a->n);
            prints(" differs from expected value ");
            printia(e, ne);
            printf(" at index %d.\n", i);
            return false;
        }
    }
    printf("%s, line %d: check passed\n", file, line);
    base_count_success();
    return true;
}

///////////////////////////////////////////////////////////////////////////////

void ia_test_all(void) {
    ia_create_test();
    ia_range_test();
    ia_of_string_test();
    ia_fn_test();
    ia_of_da_test();
    ia_contains_test();
    ia_fill_test();
    ia_fill_from_to_test();
    ia_index_test();
    ia_index_from_test();
    ia_index_fn_test();
    ia_last_index_test();
    ia_last_index_from_test();
    ia_last_index_fn_test();
    ia_sort_test();
    ia_sort_dec_test();
//    ia_insert_test();
//    ia_remove_test();
    ia_each_test();
    ia_each_state_test();
    ia_foldl_test();
    ia_foldr_test();
    ia_filter_test();
    // ia_filter_state_test();
    ia_choose_test();
    ia_exists_test();
    ia_forall_test();
    
#if 0
    Array a = ia_fn(20, ia_rnd, 5);
    ia_println(a);

    // get number of different numbers in array
    ia_sort(a);
    ia_println(a);
    int n = a_length(a);
    int k = (n > 0) ? 1 : 0;
    for (int i = 1; i < n; i++) {
        if (ia_get(a, i-1) != ia_get(a, i)) {
            k++;
        }
    }
    printiln(k);
    a_free(a);
#endif

#if 0
    // timing
    int n = 2000000000;
    Array a = ia_create(n, 0);
    time_function(   ia_fill_fn(a, fn_id)   );
    a_free(a);
    a = ia_create(n, 0);
    time_function(   ia_id(a)   );
    a_free(a);
#endif
    
}

#if 0
int main(void) {
    ia_test_all();
    return 0;
}
#endif
