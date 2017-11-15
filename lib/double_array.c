/*
@author Michael Rohs
@date 15.10.2015
@copyright Apache License, Version 2.0
*/

#include <stdarg.h>
#include "array.h"
#include "double_array.h"

static void da_create_test(void) {
    printsln((String)__func__);
    Array array;

    array = da_create(3, 0);
    double a1[] = { 0, 0, 0 };
    da_test_within2(array, a1, 3);
    a_free(array);

    array = da_create(5, -1);
    double a2[] = { -1, -1, -1, -1, -1 };
    da_test_within2(array, a2, 5);
    a_free(array);

    array = da_create(1, 2);
    double a3[] = { 2 };
    da_test_within2(array, a3, 1);
    a_free(array);

    array = da_create(0, 2);
    double a4[] = { };
    da_test_within2(array, a4, 0);
    a_free(array);
}

Array da_create(int n, double init) {
    if (n < 0) {
        printf("%s: n cannot be negative (is %d)\n", __func__, n);
        exit(EXIT_FAILURE);
    }
    double *a = xmalloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        a[i] = init;
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(double);
    result->a = a;
    return result;
}

static void da_range_test(void) {
    printsln((String)__func__);
    Array array;
    array = da_range(0, 3, 1);
    double a1[] = { 0, 1, 2 };
    da_test_within2(array, a1, 3);
    a_free(array);
    array = da_range(3, 2, 1);
    double a2[] = { 3 };
    da_test_within2(array, a2, 1);
    a_free(array);
    array = da_range(2, 3, 1);
    double a3[] = { 2 };
    da_test_within2(array, a3, 1);
    a_free(array);
    array = da_range(3, 3, 1);
    double a4[] = { };
    da_test_within2(array, a4, 0);
    a_free(array);
    array = da_range(-3, -3, 1);
    double a5[] = { };
    da_test_within2(array, a5, 0);
    a_free(array);
    array = da_range(1, 4, 1);
    double a6[] = { 1, 2, 3 };
    da_test_within2(array, a6, 3);
    a_free(array);
    array = da_range(4, 1, 1);
    double a7[] = { 4, 3, 2 };
    da_test_within2(array, a7, 3);
    a_free(array);
    array = da_range(0.0, 3.0, 0.5);
    double a8[] = { 0.0, 0.5, 1.0, 1.5, 2.0, 2.5 };
    da_test_within2(array, a8, 6);
    a_free(array);
    array = da_range(3.0, 0.0, -0.5);
    double a9[] = { 3.0, 2.5, 2.0, 1.5, 1.0, 0.5 };
    da_test_within2(array, a9, 6);
    a_free(array);
}

Array da_range(double a, double b, double step) {
    if (fabs(step) < 0.000001) {
        printf("da_range: step too small (is %g)\n", step);
        exit(EXIT_FAILURE);
    }
    int n = 0; 
    if (a <= b) {
        if (step < 0) {
            step = -step;
        }
        for (double x = a; x < b; x += step) {
            n++;
        }
    } else /* a > b */ {
        if (step > 0) {
            step = -step;
        }
        for (double x = a; x > b; x += step) {
            n++;
        }
    }
    double *arr = xmalloc(n * sizeof(double));
    double x = a;
    for (int i = 0; i < n; i++) {
        arr[i] = x;
        x += step;
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(double);
    result->a = arr;
    
    return result;
}

static void da_of_string_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = da_of_string("1, 2, 3, 4, 5, 6");
    ex = da_range(1, 7, 1);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = da_of_string("   1, 2, 3, 4, 5, 6   ");
    ex = da_range(1, 7, 1);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = da_of_string("1xx2asdfs3");
    ex = da_range(1, 4, 1);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = da_of_string("y1xx2asdfs3x");
    ex = da_range(1, 4, 1);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = da_of_string("-3, -2, -1, 0, 1");
    ex = da_range(-3, 2, 1);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = da_of_string(" -3, -2, -1 ");
    ex = da_range(-3, 0, 1);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = da_of_string(".1 -20. -3.5 1e5 -1.2e4 -1.2e-4");
    ex = da_create(6, 0);
    da_set(ex, 0,  0.1);
    da_set(ex, 1, -20.0);
    da_set(ex, 2, -3.5);
    da_set(ex, 3, 1e5);
    da_set(ex, 4, -1.2e4);
    da_set(ex, 5, -1.2e-4);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = da_of_string("-.1.-2.-.3.-");
    ex = da_create(3, 0);
    da_set(ex, 0, -0.1);
    da_set(ex, 1, -2.0);
    da_set(ex, 2, -0.3);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = da_of_string("---.1.----2.----.3.----");
    ex = da_create(3, 0);
    da_set(ex, 0, -0.1);
    da_set(ex, 1, -2.0);
    da_set(ex, 2, -0.3);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = da_of_string("");
    ex = da_create(0, 0);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
}

Array da_of_string(String s) {
    assert_argument_not_null(s);
    // count number of ints in s
    int n = 0; // array length
    char *t = s;
    char *endp;
    while (*t != '\0') {
        if (isdigit(*t)) {
            n++; // one more int found
            if (t > s && *(t - 1) == '.') t--; // check for '.'
            if (t > s && *(t - 1) == '-') t--; // check for '-'
            strtod(t, &endp); // convert digit string to int
            t = endp;
        } else {
            // assert: *t is not a digit, *t is not '\0'
            t++; // not a digit, skip
        }
    }

    // n ints found
    double *a = xmalloc(n * sizeof(double));
    t = s;
    int i = 0;
    while (*t != '\0') {
        if (isdigit(*t)) {
            if (t > s && *(t - 1) == '.') t--; // check for '.'
            if (t > s && *(t - 1) == '-') t--; // check for '-'
            a[i++] = strtod(t, &endp); // convert digit string to int
            t = endp;
        } else {
            // assert: *t is not a digit, *t is not '\0'
            t++; // not a digit, skip
        }
    }

    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(double);
    result->a = a;
    return result;
}

static double two_i_plus_x(int i, double x) {
    return 2.0 * i + x;
}

static void da_fn_test(void) {
    printsln((String)__func__);
    Array array;
    array = da_fn(3, two_i_plus_x, 1);
    double a1[] = { 1, 3, 5 };
    da_test_within2(array, a1, 3);
    a_free(array);
    array = da_fn(4, two_i_plus_x, 1);
    double a2[] = { 1, 3, 5, 7 };
    da_test_within2(array, a2, 4);
    a_free(array);
    array = da_fn(1, two_i_plus_x, 1);
    double a3[] = { 1 };
    da_test_within2(array, a3, 1);
    a_free(array);
    array = da_fn(0, two_i_plus_x, 1);
    double a4[] = { };
    da_test_within2(array, a4, 0);
    a_free(array);
}

Array da_fn(int n, IntDoubleToDouble init, double x) {
    assert_function_not_null(init);
    if (n < 0) {
        printf("da_fn: n cannot be negative (is %d)\n", n);
        exit(EXIT_FAILURE);
    }
    assert_function_not_null(init);
    double *a = xmalloc(n * sizeof(double));
    for (int i = 0; i < n; i++) {
        a[i] = init(i, x);
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(double);
    result->a = a;
    return result;
}

static void da_of_ia_test(void) {
    printsln((String)__func__);
    Array ia;
    Array ex, ac;
    ia = ia_of_string("-1 2 -3 44");
    ex = da_of_string("-1 2 -3 44");
    ac = da_of_ia(ia);
    da_test_within(ac, ex);
    a_free(ia);
    a_free(ex);
    a_free(ac);
}

Array da_of_ia(Array array) {
    assert_argument_not_null(array);
    ia_assert_element_size(array);
    int n = a_length(array);
    Array result = da_create(n, 0.0);
    int *src = array->a;
    double *dst = result->a;
    for (int i = 0; i < n; i++) {
        dst[i] = src[i];
    }
    return result;
}

#ifdef A_GET_SET
double da_get(Array array, int i) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    if (i < 0 || i >= array->n) {
        printf("da_get: index %d is out of range "
            "(array length: %d, allowed indices: 0..%d)\n", 
        i, array->n, array->n - 1);
        exit(EXIT_FAILURE);
    }
    double *a = array->a;
    return a[i];
}
#endif

#ifdef A_GET_SET
void da_set(Array array, int i, double v) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    if (i < 0 || i >= array->n) {
        printf("da_set: index %d is out of range "
            "(array length: %d, allowed indices: 0..%d)\n", 
        i, array->n, array->n - 1);
        exit(EXIT_FAILURE);
    }
    double *a = array->a;
    a[i] = v;
}
#endif

#ifdef A_GET_SET
void da_inc(Array array, int i, double v) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    if (i < 0 || i >= array->n) {
        printf("da_inc: index %d is out of range "
            "(array length: %d, allowed indices: 0..%d)\n", 
        i, array->n, array->n - 1);
        exit(EXIT_FAILURE);
    }
    double *a = array->a;
    a[i] += v;
}
#endif

void da_print(Array array) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    double *a = array->a;
    printf("[");
    if (array->n > 0) {
        printf("%g", a[0]);
    }
    for (int i = 1; i < array->n; i++) {
        printf(" %g", a[i]);
    }
    printf("]");
}

void da_println(Array array) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    da_print(array);
    printf("\n");
}

/////////////////////////////////////////////////////////////////////////////

static void da_contains_test(void) {
    printsln((String)__func__);
    Array array = da_of_string("10, 20, 30");
    test_equal_b(da_contains(array, 10, EPSILON), true);
    test_equal_b(da_contains(array, 11, EPSILON), false);
    test_equal_b(da_contains(array, 30, EPSILON), true);
    a_free(array);
}

bool da_contains(Array array, double value, double delta) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    double *a = array->a;
    for (int i = 0; i < array->n; i++) {
        if (fabs(a[i] - value) < delta) {
            return true;
        }
    }
    return false;
}

static void da_fill_test(void) {
    printsln((String)__func__);
    Array array;

    array = da_create(3, 0);
    da_fill(array, -1);
    double a1[] = { -1, -1, -1 };
    da_test_within2(array, a1, 3);
    a_free(array);

    array = da_create(2, 0);
    da_fill(array, 2);
    double a2[] = { 2, 2 };
    da_test_within2(array, a2, 2);
    a_free(array);

    array = da_create(0, 0);
    da_fill(array, 2);
    double a3[] = { };
    da_test_within2(array, a3, 0);
    a_free(array);
}

void da_fill(Array array, double value) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    double *a = array->a;
    for (int i = 0; i < array->n; i++) {
        a[i] = value;
    }
}

static void da_fill_from_to_test(void) {
    printsln((String)__func__);
    Array array;

    array = da_create(3, 0);
    da_fill_from_to(array, -1, 0, array->n);
    double a1[] = { -1, -1, -1 };
    da_test_within2(array, a1, 3);
    a_free(array);

    array = da_create(3, 0);
    da_fill_from_to(array, -1, -1, array->n + 1);
    double a2[] = { -1, -1, -1 };
    da_test_within2(array, a2, 3);
    a_free(array);

    array = da_create(3, 0);
    da_fill_from_to(array, -1, 0, 0);
    double a3[] = { 0, 0, 0 };
    da_test_within2(array, a3, 3);
    a_free(array);

    array = da_create(3, 0);
    da_fill_from_to(array, -1, 0, 1);
    double a4[] = { -1, 0, 0 };
    da_test_within2(array, a4, 3);
    a_free(array);

    array = da_create(3, 0);
    da_fill_from_to(array, -1, 2, 2);
    double a5[] = { 0, 0, 0 };
    da_test_within2(array, a5, 3);
    a_free(array);

    array = da_create(3, 0);
    da_fill_from_to(array, -1, 2, 3);
    double a6[] = { 0, 0, -1 };
    da_test_within2(array, a6, 3);
    a_free(array);

    array = da_create(3, 0);
    da_fill_from_to(array, -1, 2, 1);
    double a7[] = { 0, 0, 0 };
    da_test_within2(array, a7, 3);
    a_free(array);

    array = da_create(3, 0);
    da_fill_from_to(array, -1, 1, 3);
    double a8[] = { 0, -1, -1 };
    da_test_within2(array, a8, 3);
    a_free(array);
}

void da_fill_from_to(Array array, double value, int from, int to) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    double *a = array->a;
    if (from < 0) from = 0;
    if (to > array->n) to = array->n;
    for (int i = from; i < to; i++) {
        a[i] = value;
    }
}

static void da_index_test(void) {
    printsln((String)__func__);
    Array array;

    array = da_of_string("1 2");
    int i = da_index(array, 0, EPSILON);
#if 0
    if (i < 0) {
        printsln("value not found");
    } else {
        printf("value found at index %d\n", i);
    }
#endif
    test_equal_i(i, -1);

    i = da_index(array, 1, EPSILON);
    test_equal_i(i, 0);
        
    i = da_index(array, 2, EPSILON);
    test_equal_i(i, 1);
        
    a_free(array);
    
}

int da_index(Array array, double value, double delta) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    double *a = array->a;
    for (int i = 0; i < array->n; i++) {
        if (fabs(a[i] - value) < delta) {
            return i;
        }
    }
    return -1;
}

static void da_index_from_test(void) {
    printsln((String)__func__);
    Array a = da_of_string("10 20 30 40 50");
    test_equal_i(da_index_from(a, 20, 0, EPSILON), 1);
    test_equal_i(da_index_from(a, 20, 1, EPSILON), 1);
    test_equal_i(da_index_from(a, 20, 2, EPSILON), -1);
    test_equal_i(da_index_from(a, 30, -1, EPSILON), 2);
    test_equal_i(da_index_from(a, 60, 0, EPSILON), -1);
    a_free(a);
    
}

int da_index_from(Array array, double value, int from, double delta) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    double *a = array->a;
    if (from < 0) from = 0;
    for (int i = from; i < array->n; i++) {
        if (fabs(a[i] - value) < delta) {
            return i;
        }
    }
    return -1;
}

static void da_index_fn_test(void) {
    printsln((String)__func__);
    Array a = da_of_string("1 2 3 -4 5");
    test_equal_i(da_index_fn(a, da_gt, 0), 0);
    test_equal_i(da_index_fn(a, da_lt, 0), 3);
    a_free(a);
}

int da_index_fn(Array array, DoubleIntDoubleToBool predicate, double x) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    da_assert_element_size(array);
    double *a = array->a;
    for (int i = 0; i < array->n; i++) {
        if (predicate(a[i], i, x)) {
            return i;
        }
    }
    return -1;
}

static void da_last_index_test(void) {
    printsln((String)__func__);
    Array a = da_of_string("10 20 30 10 20 30");
    test_equal_i(da_last_index(a, 20, EPSILON), 4);
    test_equal_i(da_last_index(a, 30, EPSILON), 5);
    test_equal_i(da_last_index(a, 60, EPSILON), -1);
    a_free(a);
}

int da_last_index(Array array, double value, double delta) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    double *a = array->a;
    for (int i = array->n - 1; i >= 0; i--) {
        if (fabs(a[i] - value) < delta) {
            return i;
        }
    }
    return -1;
}

static void da_last_index_from_test(void) {
    printsln((String)__func__);
    Array a = da_of_string("10 20 30 10 20 30");
    test_equal_i(da_last_index_from(a, 20, 5, EPSILON), 4);
    test_equal_i(da_last_index_from(a, 20, 4, EPSILON), 4);
    test_equal_i(da_last_index_from(a, 20, 3, EPSILON), 1);
    test_equal_i(da_last_index_from(a, 30, 2, EPSILON), 2);
    test_equal_i(da_last_index_from(a, 30, 1, EPSILON), -1);
    a_free(a);    
}

int da_last_index_from(Array array, double value, int from, double delta) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    double *a = array->a;
    if (from >= array->n) from = array->n - 1;
    for (int i = from; i >= 0; i--) {
        if (fabs(a[i] - value) < delta) {
            return i;
        }
    }
    return -1;
}

static void da_last_index_fn_test(void) {
    printsln((String)__func__);
    Array a = da_of_string("1 2 2 3 -3 5");
    test_equal_i(da_last_index_fn(a, da_gt, 0), 5);
    test_equal_i(da_last_index_fn(a, da_lt, 0), 4);
    a_free(a);
}

int da_last_index_fn(Array array, DoubleIntDoubleToBool predicate, double x) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    da_assert_element_size(array);
    double *a = array->a;
    for (int i = array->n - 1; i >= 0; i--) {
        if (predicate(a[i], i, x)) {
            return i;
        }
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////

static void da_sort_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = da_of_string("1, 2, 3, 4, 5, 6, 7, 8, 9, 10");
    ex = da_of_string("1, 2, 3, 4, 5, 6, 7, 8, 9, 10");
    da_sort(ac);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = da_of_string("4.5, 4.5, 4.5, 4.51, 2, 3, 4, 5, 6, 7, 8, 9, 10");
    ex = da_of_string("2 3 4 4.5 4.5 4.5 4.51 5 6 7 8 9 10");
    da_sort(ac);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = da_of_string("");
    ex = da_of_string("");
    da_sort(ac);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = da_of_string("3, 2, 1, 0.9, -0.1");
    ex = da_of_string("-0.1 0.9 1 2 3");
    da_sort(ac);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
}

static CmpResult double_compare(ConstAny a, ConstAny b) {
    assert_argument_not_null(a);
    assert_argument_not_null(b);
    double x = *(double*)a;
    double y = *(double*)b;
    return (x == y) ? EQ : (x < y ? LT : GT);
}

void da_sort(Array array) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    qsort(array->a, array->n, sizeof(double), double_compare);
}

static void da_sort_dec_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = da_of_string("1, 2, 3, 4, 5, 6, 7, 8, 9, 10");
    ex = da_of_string("10, 9, 8, 7, 6, 5, 4, 3, 2, 1");
    da_sort_dec(ac);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = da_of_string("4.5, 4.5, 4.51, 2, 3, 9");
    ex = da_of_string("9 4.51 4.5 4.5 3 2");
    da_sort_dec(ac);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = da_of_string("");
    ex = da_of_string("");
    da_sort_dec(ac);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = da_of_string("3, 2, 1, 0.9, -0.1");
    ex = da_of_string("3, 2, 1, 0.9, -0.1");
    da_sort_dec(ac);
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
}

static CmpResult double_compare_dec(ConstAny a, ConstAny b) {
    assert_argument_not_null(a);
    assert_argument_not_null(b);
    double x = *(double*)b;
    double y = *(double*)a;
    return (x == y) ? EQ : (x < y ? LT : GT);
}

void da_sort_dec(Array array) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    qsort(array->a, array->n, sizeof(double), double_compare_dec);
}

#if 0
void da_insert(Array array, int i, double v);

static void da_insert_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = da_of_string("1, 2, 3, 4, 5, 6");
    da_insert(ac, 0, 9);
    ex = da_of_string("9, 1, 2, 3, 4, 5");
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = da_of_string("1, 2, 3, 4, 5, 6");
    da_insert(ac, 5, 9);
    ex = da_of_string("1, 2, 3, 4, 5, 9");
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = da_of_string("1, 2, 3, 4, 5, 6");
    da_insert(ac, 3, 9);
    ex = da_of_string("1, 2, 3, 9, 4, 5");
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = da_of_string("1");
    da_insert(ac, -1, 9);
    ex = da_of_string("1");
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = da_of_string("1");
    da_insert(ac, 1, 9);
    ex = da_of_string("1");
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = da_of_string("");
    da_insert(ac, 0, 9);
    ex = da_of_string("");
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
}

void da_insert(Array array, int i, double v) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    double *a = array->a;
    if (i < 0 || i >= array->n) return;
    // make space at i
    for (int j = array->n - 1; j > i; j--) {
        a[j] = a[j - 1];
    }
    // set value at i to v
    a[i] = v;
}

void da_remove(Array array, int i, double v);

static void da_remove_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = da_of_string("1, 2, 3, 4, 5, 6");
    da_remove(ac, 0, 9);
    ex = da_of_string("2, 3, 4, 5, 6, 9");
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = da_of_string("1, 2, 3, 4, 5, 6");
    da_remove(ac, 5, 9);
    ex = da_of_string("1, 2, 3, 4, 5, 9");
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = da_of_string("1, 2, 3, 4, 5, 6");
    da_remove(ac, 3, 9);
    ex = da_of_string("1, 2, 3, 5, 6, 9");
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = da_of_string("1");
    da_remove(ac, -1, 9);
    ex = da_of_string("1");
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = da_of_string("1");
    da_remove(ac, 1, 9);
    ex = da_of_string("1");
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = da_of_string("1");
    da_remove(ac, 0, 9);
    ex = da_of_string("9");
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = da_of_string("");
    da_remove(ac, 0, 9);
    ex = da_of_string("");
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
}

void da_remove(Array array, int i, double v) {
    assert_argument_not_null(array);
    da_assert_element_size(array);
    double *a = array->a;
    if (i < 0 || i >= array->n) return;
    // shift down, starting from i
    for (int j = i; j < array->n - 1; j++) {
        a[j] = a[j + 1];
    }
    // set value at n-1 to v
    a[array->n - 1] = v;
}
#endif

///////////////////////////////////////////////////////////////////////////////

bool da_index_even(double value, int index) {
    return (index & 1) == 0;
}

bool da_index_odd(double value, int index) {
    return (index & 1) == 1;
}

bool da_gt(double value, int index, double x) {
    return value > x;
}

bool da_ge(double value, int index, double x) {
    return value >= x;
}

bool da_lt(double value, int index, double x) {
    return value < x;
}

bool da_le(double value, int index, double x) {
    return value <= x;
}

///////////////////////////////////////////////////////////////////////////////

double da_square(double value, int index) {
    return value * value; // index not used
}

double da_rnd(int index, double maximum) {
    return d_rnd(maximum); // index not used
}

double da_times(double value, int index, double x) {
    return value * x;
}

double double_plus(double x, double y, int index) {
    return x + y;
}

double double_minus(double x, double y, int index) {
    return x - y;
}

double double_mult(double x, double y, int index) {
    return x * y;
}

double double_div(double x, double y, int index) {
    return x / y;
}

static void da_each_test(void) {
    printsln((String)__func__);
    Array a, ex;
    
    a = da_of_string("1, 2, 3, 4");
    da_each(a, da_times, 2);
    ex = da_of_string("2, 4, 6, 8");
    da_test_within(a, ex);
    a_free(a);
    a_free(ex);
    
    a = da_of_string("");
    da_each(a, da_times, 3);
    ex = da_of_string("");
    da_test_within(a, ex);
    a_free(a);
    a_free(ex);
}

void da_each(Array array, DoubleIntDoubleToDouble f, double x) {
    assert_argument_not_null(array);
    assert_function_not_null(f);
    da_assert_element_size(array);
    double *a = array->a;
    for (int i = 0; i < array->n; i++) {
        a[i] = f(a[i], i, x);
    }
}


// @todo: add tests
void da_each_state(Array array, DoubleIntDoubleAnyToDouble f, double x, Any state) {
    assert_argument_not_null(array);
    assert_function_not_null(f);
    da_assert_element_size(array);
    double *a = array->a;
    for (int i = 0; i < array->n; i++) {
        a[i] = f(a[i], i, x, state);
    }
}

// @todo: add tests

Array da_map(Array array, DoubleIntDoubleToDouble f, double x) {
    assert_argument_not_null(array);
    assert_function_not_null(f);
    da_assert_element_size(array);
    double *a = array->a;
    double *b = xmalloc(array->n * sizeof(double));
    for (int i = 0; i < array->n; i++) {
        b[i] = f(a[i], i, x);
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = array->n;
    result->s = sizeof(double);
    result->a = b;
    return result;
}

// @todo: add tests

Array da_map_state(Array array, DoubleIntDoubleAnyToDouble f, double x, Any state) {
    assert_argument_not_null(array);
    assert_function_not_null(f);
    da_assert_element_size(array);
    double *a = array->a;
    double *b = xmalloc(array->n * sizeof(double));
    for (int i = 0; i < array->n; i++) {
        b[i] = f(a[i], i, x, state);
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = array->n;
    result->s = sizeof(double);
    result->a = b;
    return result;
}

static void da_foldl_test(void) {
    printsln((String)__func__);
    Array a = da_of_string("");    
    test_within_d(da_foldl(a, double_plus, 100), 100, EPSILON);
    test_within_d(da_foldl(a, double_minus, 100), 100, EPSILON);
    a_free(a);
    
    a = da_of_string("1, 2, 3, 4");    
    test_within_d(da_foldl(a, double_plus, 0),    (((0 + 1) + 2) + 3) + 4, EPSILON);
    test_within_d(da_foldl(a, double_minus, 100), (((100 - 1) - 2) - 3) - 4, EPSILON);
    a_free(a);
}

double da_foldl(Array array, DoubleDoubleIntToDouble f, double init) {
    assert_argument_not_null(array);
    assert_function_not_null(f);
    da_assert_element_size(array);
    double *a = array->a;
    for (int i = 0; i < array->n; i++) {
        init = f(init, a[i], i);
    }
    return init;
}

static void da_foldr_test(void) {
    printsln((String)__func__);
    Array a = da_of_string("");    
    test_within_d(da_foldr(a, double_plus, 100), 100, EPSILON);
    test_within_d(da_foldr(a, double_minus, 100), 100, EPSILON);
    a_free(a);
    
    a = da_of_string("1, 2, 3, 4");    
    test_within_d(da_foldr(a, double_plus, 100), 1 + (2 + (3 + (4 + 100))), EPSILON);
    test_within_d(da_foldr(a, double_minus, 0), 1 - (2 - (3 - (4 - 0))), EPSILON);
    a_free(a);
}

double da_foldr(Array array, DoubleDoubleIntToDouble f, double init) {
    assert_argument_not_null(array);
    assert_function_not_null(f);
    da_assert_element_size(array);
    double *a = array->a;
    for (int i = array->n - 1; i >= 0; i--) {
        init = f(a[i], init, i);
    }
    return init;
}

static void da_filter_test(void) {
    printsln((String)__func__);
    Array a, ac, ex;

    a = da_of_string("1, 2, 3, 4, 5, 6");
    ac = da_filter(a, da_gt, 3);
    ex = da_of_string("4, 5, 6");
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = da_filter(a, da_lt, 4);
    ex = da_of_string("1, 2, 3");
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = da_filter(a, da_le, 3);
    ex = da_of_string("1, 2, 3");
    da_test_within(ac, ex);
    a_free(ac);
    a_free(ex);
    a_free(a);
}

Array da_filter(Array array, DoubleIntDoubleToBool predicate, double x) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    da_assert_element_size(array);
    bool *ps = xmalloc(array->n * sizeof(bool));
    int n = 0;
    double *a = array->a;
    for (int i = 0; i < array->n; i++) {
        ps[i] = predicate(a[i], i, x);
        if (ps[i]) n++;
    }
    double *b = xmalloc(n * sizeof(double));
    for (int i = 0, j = 0; i < array->n; i++) {
        if (ps[i]) {
            b[j++] = a[i];
        }
    }
    free(ps);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(double);
    result->a = b;    
    return result;
}

// @todo: add tests
Array da_filter_state(Array array, DoubleIntDoubleAnyToBool predicate, double x, Any state) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    da_assert_element_size(array);
    bool *ps = xmalloc(array->n * sizeof(bool));
    int n = 0;
    double *a = array->a;
    for (int i = 0; i < array->n; i++) {
        ps[i] = predicate(a[i], i, x, state);
        if (ps[i]) n++;
    }
    double *b = xmalloc(n * sizeof(double));
    for (int i = 0, j = 0; i < array->n; i++) {
        if (ps[i]) {
            b[j++] = a[i];
        }
    }
    free(ps);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(double);
    result->a = b;    
    return result;
}

static void da_exists_test(void) {
    printsln((String)__func__);
    Array a = da_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(da_exists(a, da_gt, 3), true);
    test_equal_b(da_exists(a, da_gt, 9), false);
    test_equal_b(da_exists(a, da_lt, -1), false);
    a_free(a);
}

bool da_exists(Array array, DoubleIntDoubleToBool predicate, double x) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    da_assert_element_size(array);
    double *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (predicate(a[i], i, x)) {
            return true;
        }
    }
    return false;
}

bool da_exists_state(Array array, DoubleIntDoubleAnyToBool predicate, double x, Any state) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    da_assert_element_size(array);
    double *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (predicate(a[i], i, x, state)) {
            return true;
        }
    }
    return false;
}

static void da_forall_test(void) {
    printsln((String)__func__);
    Array a = da_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(da_forall(a, da_gt, 0), true);
    test_equal_b(da_forall(a, da_gt, 1), false);
    test_equal_b(da_forall(a, da_lt, 7), true);
    a_free(a);
}
    
bool da_forall(Array array, DoubleIntDoubleToBool predicate, double x) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    da_assert_element_size(array);
    double *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (!predicate(a[i], i, x)) {
            return false;
        }
    }
    return true;
}

bool da_forall_state(Array array, DoubleIntDoubleAnyToBool predicate, double x, Any state) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    da_assert_element_size(array);
    double *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (!predicate(a[i], i, x, state)) {
            return false;
        }
    }
    return true;
}


///////////////////////////////////////////////////////////////////////////////
// Tests

bool da_test_within_file_line(const char *file, const char *function, int line, Array a, double *e, int ne, double epsilon) {
    base_init();
    base_count_check();
    if (a->n != ne) {
        printf("%s, line %d: Actual length %d "
            "differs from expected length %d\n", file, line, a->n, ne);
        return false;
    }
    if (a->s != sizeof(double)) {
        printf("%s, line %d: Actual element size %d "
            "differs from expected element size %lu\n", file, line, a->s, (unsigned long)sizeof(double));
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
    double *da = a->a;
    for (int i = 0; i < a->n; i++) {
        if (fabs(da[i] - e[i]) > epsilon) {
            printf("%s, line %d: Actual value ", file, line);
            printda(da, a->n);
            prints(" differs from expected value ");
            printda(e, ne);
            printf(" at index %d.\n", i);
            return false;
        }
    }
    printf("%s, line %d: check passed\n", file, line);
    base_count_success();
    return true;
}

///////////////////////////////////////////////////////////////////////////////

static void da_index_option_test(void) {
    printsln((String)__func__);
    Array array;

    array = da_of_string("1, 2");
    DoubleOption op = da_index_option(array, 0, EPSILON);
#if 0
    if (op.none) {
        printsln("value not found");
    } else {
        printf("value found at index %f\n", op.some);
    }
#endif
    test_equal_b(op.none, true);
        
    op = da_index_option(array, 1, EPSILON);
    test_equal_b(op.none, false);
    test_equal_i(op.some, 0);
        
    op = da_index_option(array, 2, EPSILON);
    test_equal_b(op.none, false);
    test_equal_i(op.some, 1);
        
    a_free(array);
}
    
DoubleOption da_index_option(Array array, double value, double delta) {
    da_assert_element_size(array);
    double *a = array->a;
    for (int i = 0; i < array->n; i++) {
        if (fabs(a[i] - value) < delta) {
            return make_double_some(i);
        }
    }
    return make_double_none();
}



///////////////////////////////////////////////////////////////////////////////



void da_test_all(void) {
    da_create_test();
    da_range_test();
    da_of_string_test();
    da_fn_test();
    da_of_ia_test();
    da_contains_test();
    da_fill_test();
    da_fill_from_to_test();
    da_index_test();
    da_index_from_test();
    da_index_fn_test();
    da_last_index_test();
    da_last_index_from_test();
    da_last_index_fn_test();
    da_sort_test();
    da_sort_dec_test();
//    da_insert_test();
//    da_remove_test();
    da_each_test();
    da_foldl_test();
    da_foldr_test();
    da_filter_test();
    da_exists_test();
    da_forall_test();
    da_index_option_test();
}

#if 0
int main(void) {
    da_test_all();
    return EXIT_SUCCESS;
}
#endif
