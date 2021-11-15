/*
@author Michael Rohs
@date 15.10.2015, 6.10.2020
@copyright Apache License, Version 2.0
*/

#include "array.h"
#include "pointer_array.h"
#include "string_array.h"
#include "string.h"



static void pa_create_test(void) {
    printsln((Any)__func__);
    Array array;
    
    array = pa_create(3, "");
    Any a1[] = { "", "", "" };
    pa_test_equal_file_line(__FILE__, __func__, __LINE__, array, a1, 3);
    a_free(array);
    
    array = pa_create(5, "x");
    Any a2[] = { "x", "x", "x", "x", "x" };
    pa_test_equal_file_line(__FILE__, __func__, __LINE__, array, a2, 5);
    a_free(array);
    
    array = pa_create(1, "y");
    Any a3[] = { "y" };
    pa_test_equal_file_line(__FILE__, __func__, __LINE__, array, a3, 1);
    a_free(array);
    
    array = pa_create(0, "z");
    Any a4[] = { };
    pa_test_equal_file_line(__FILE__, __func__, __LINE__, array, a4, 0);
    a_free(array);
}

Array pa_create(int n, Any value) {
    require("non-negative length", n >= 0);
    Any *a = xmalloc(n * sizeof(Any));
    for (int i = 0; i < n; i++) {
        a[i] = value;
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(Any);
    result->a = a;
    return result;
}

static void a_copy_test(void) {
    printsln((Any)__func__);
    Array array, copy;

    array = sa_of_string("-1, -2, -3");
    copy = a_copy(array);
    pa_test_equal(copy, array);
    pa_free(array);
    a_free(copy);

    array = sa_of_string("10");
    copy = a_copy(array);
    pa_test_equal(copy, array);
    pa_free(array);
    a_free(copy);

    array = sa_of_string("100, -200");
    copy = a_copy(array);
    pa_test_equal(copy, array);
    pa_free(array);
    a_free(copy);

    array = sa_of_string("");
    copy = a_copy(array);
    pa_test_equal(copy, array);
    pa_free(array);
    a_free(copy);
}

static void a_sub_test(void) {
    printsln((Any)__func__);
    Array array, array2, sub;
    array = sa_of_string("1, 2, 3, 4");

    sub = a_sub(array, 0, a_length(array));
    pa_test_equal(array, sub);
    a_free(sub);

    sub = a_sub(array, -1, a_length(array) + 1);
    pa_test_equal(array, sub);
    a_free(sub);

    array2 = sa_of_string("2, 3, 4");
    // pa_println(array2);
    // printiln(a_length(array2));
    sub = a_sub(array, 1, a_length(array));
    sa_test_equal(array2, sub);
    pa_free(array2);
    a_free(sub);

    array2 = sa_of_string("2, 3");
    sub = a_sub(array, 1, a_length(array) - 1);
    sa_test_equal(array2, sub);
    pa_free(array2);
    a_free(sub);

    array2 = pa_create(0, "");
    sub = a_sub(array, 1, 1);
    pa_test_equal(array2, sub);
    a_free(sub);

    sub = a_sub(array, 2, 1);
    pa_test_equal(array2, sub);
    pa_free(array2);
    a_free(sub);

    pa_free(array);
}

void pa_free(Array array) {
    if (array != NULL) {
        require_element_size_pointer(array);
        if (array->a != NULL) {
            for (int i = 0; i < array->n; i++) {
                Any s = pa_get(array, i);
                s_free(s);
            }
            free(array->a);
            array->a = NULL;
        }
        array->n = 0;
        array->s = 0;
        a_free(array);
    }
}

#ifndef NO_GET_SET
Any pa_get(Array array, int index) {
    require_not_null(array);
    require_element_size_pointer(array);
    require_x("index in range", index >= 0 && index < array->n, 
            "index == %d, length == %d", index, array->n);
    Any *a = array->a;
    return a[index];
}
#endif

#ifndef NO_GET_SET
void pa_set(Array array, int index, Any value) {
    require_not_null(array);
    require_element_size_pointer(array);
    require_x("index in range", index >= 0 && index < array->n, 
            "index == %d, length == %d", index, array->n);
    Any *a = array->a;
    a[index] = value;
}
#endif

#if 0
void pa_print(Array array) {
    require_not_null(array);
    require_element_size_pointer(array);
    Any *a = array->a;
    printf("[");
    if (array->n > 0) {
        printf("\"%s\"", a[0]);
    }
    for (int i = 1; i < array->n; i++) {
        printf(", \"%s\"", a[i]);
    }
    printf("]");
}

void pa_println(Array array) {
    require_not_null(array);
    require_element_size_pointer(array);
    pa_print(array);
    printf("\n");
}
#endif

static void a_concat_test(void) {
    printsln((Any)__func__);

    Array a1, a2, ac, ex;

    a1 = sa_of_string("10, 20, 30");
    a2 = sa_of_string("1, 2, 3");
    ac = a_concat(a1, a2);
    ex = sa_of_string("10, 20, 30, 1, 2, 3");
    sa_test_equal(ac, ex);
    pa_free(a1);
    pa_free(a2);
    a_free(ac);
    pa_free(ex);

    a1 = sa_of_string("10, 20, 30");
    a2 = pa_create(0, "");
    ac = a_concat(a1, a2);
    ex = sa_of_string("10, 20, 30");
    sa_test_equal(ac, ex);
    pa_free(a1);
    a_free(a2);
    a_free(ac);
    pa_free(ex);

    a1 = pa_create(0, "");
    a2 = sa_of_string("10, 20, 30");
    ac = a_concat(a1, a2);
    ex = sa_of_string("10, 20, 30");
    sa_test_equal(ac, ex);
    a_free(a1);
    pa_free(a2);
    a_free(ac);
    pa_free(ex);

    a1 = pa_create(0, "");
    a2 = pa_create(0, "");
    ac = a_concat(a1, a2);
    ex = pa_create(0, "");
    sa_test_equal(ac, ex);
    a_free(a1);
    a_free(a2);
    a_free(ac);
    a_free(ex);
}

static void pa_contains_test(void) {
    printsln((Any)__func__);
    Array array;
    array = sa_of_string("10, 20, 30");
    test_equal_b(pa_contains(array, pa_get(array, 0)), true);
    test_equal_b(pa_contains(array, pa_get(array, 1)), true);
    test_equal_b(pa_contains(array, pa_get(array, 2)), true);
    pa_free(array);
}

bool pa_contains(Array array, Any value) {
    require_not_null(array);
    require_element_size_pointer(array);
    Any *a = array->a;
    for (int i = 0; i < array->n; i++) {
        if (a[i] == value) {
            return true;
        }
    }
    return false;
}

static void pa_index_test(void) {
    printsln((Any)__func__);
    Array array;

    array = sa_of_string("1, 2, 3");
    int i = pa_index(array, "0");
#if 1
    if (i < 0) {
        printsln("value not found");
    } else {
        printf("value found at index %d\n", i);
    }
#endif
    test_equal_i(i, -1);
    test_equal_i(pa_index(array, pa_get(array, 0)), 0);
    test_equal_i(pa_index(array, pa_get(array, 1)), 1);
        
    pa_free(array);
}

int pa_index(Array array, Any value) {
    require_not_null(array);
    require_element_size_pointer(array);
    Any *a = array->a;
    for (int i = 0; i < array->n; i++) {
        if (a[i] == value) {
            return i;
        }
    }
    return -1;
}

static void pa_index_from_test(void) {
    printsln((Any)__func__);
    Array a = sa_of_string("10, 20, 30, 40, 50");
    test_equal_i(pa_index_from(a, pa_get(a, 1), 0), 1);
    test_equal_i(pa_index_from(a, pa_get(a, 1), 1), 1);
    test_equal_i(pa_index_from(a, pa_get(a, 1), 2), -1);
    test_equal_i(pa_index_from(a, pa_get(a, 2), -1), 2);
    test_equal_i(pa_index_from(a, "60", 0), -1);
    pa_free(a);
}

int pa_index_from(Array array, Any value, int from) {
    require_not_null(array);
    require_element_size_pointer(array);
    Any *a = array->a;
    if (from < 0) from = 0;
    for (int i = from; i < array->n; i++) {
        if (a[i] == value) {
            return i;
        }
    }
    return -1;
}

static bool is_equal_to(String s, int index, String x) {
    return s_equals(s, x);
}

static void pa_index_fn_test(void) {
    printsln((Any)__func__);
    Array a = sa_of_string("test, dog, a, cat");
    test_equal_i(pa_index_fn(a, is_equal_to, "dog"), 1);
    pa_free(a);
}

int pa_index_fn(Array array, AnyFn predicate, Any x) {
    require_not_null(array);
    require_element_size_pointer(array);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    Any *a = array->a;
    for (int i = 0; i < array->n; i++) {
        if (f(a[i], i, x)) {
            return i;
        }
    }
    return -1;
}

static void pa_last_index_test(void) {
    printsln((Any)__func__);
    Array a = sa_of_string("10, 20, 30, 10, 20, 30");
    test_equal_i(pa_last_index(a, pa_get(a, 4)), 4);
    test_equal_i(pa_last_index(a, pa_get(a, 5)), 5);
    test_equal_i(pa_last_index(a, "60"), -1);
    pa_free(a);
}

int pa_last_index(Array array, Any value) {
    require_not_null(array);
    require_element_size_pointer(array);
    Any *a = array->a;
    for (int i = array->n - 1; i >= 0; i--) {
        if (a[i] == value) {
            return i;
        }
    }
    return -1;
}

static void pa_last_index_from_test(void) {
    printsln((Any)__func__);
    Array a = sa_of_string("10, 20, 30, 10, 20, 30");
    test_equal_i(pa_last_index_from(a, pa_get(a, 4), 5), 4);
    test_equal_i(pa_last_index_from(a, pa_get(a, 4), 4), 4);
    test_equal_i(pa_last_index_from(a, pa_get(a, 1), 3), 1);
    test_equal_i(pa_last_index_from(a, pa_get(a, 2), 2), 2);
    test_equal_i(pa_last_index_from(a, pa_get(a, 2), 1), -1);
    pa_free(a);    
}

int pa_last_index_from(Array array, Any value, int from) {
    require_not_null(array);
    require_element_size_pointer(array);
    Any *a = array->a;
    if (from >= array->n) from = array->n - 1;
    for (int i = from; i >= 0; i--) {
        if (a[i] == value) {
            return i;
        }
    }
    return -1;
}

static void pa_last_index_fn_test(void) {
    printsln((Any)__func__);
    Array a = sa_of_string("no, dog, test, dog, a, cat");
    test_equal_i(pa_last_index_fn(a, is_equal_to, "dog"), 3);
    pa_free(a);
}

int pa_last_index_fn(Array array, AnyFn predicate, Any x) {
    require_not_null(array);
    require_element_size_pointer(array);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    Any *a = array->a;
    for (int i = array->n - 1; i >= 0; i--) {
        if (f(a[i], i, x)) {
            return i;
        }
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////

static void pa_reverse_test(void) {
    printsln((Any)__func__);
    Array ac, ex;

    ac = sa_of_string("1, 2, 3");
    ex = sa_of_string("3, 2, 1");
    a_reverse(ac);
    sa_test_equal(ac, ex);
    pa_free(ac);
    pa_free(ex);

    ac = sa_of_string("1, 2, 3, 4");
    ex = sa_of_string("4, 3, 2, 1");
    a_reverse(ac);
    sa_test_equal(ac, ex);
    pa_free(ac);
    pa_free(ex);

    ac = pa_create(0, "");
    ex = pa_create(0, "");
    a_reverse(ac);
    pa_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = pa_create(0, "x");
    ex = pa_create(0, "x");
    a_reverse(ac);
    pa_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
}

static void pa_shuffle_test(void) {
    printsln((Any)__func__);
    Array a = sa_of_string("1, 2, 3, 4, 5, 6, 7, 8, 9, 10");
    sa_println(a);
    a_shuffle(a);
    sa_println(a);
    pa_free(a);
}

#if 0
static CmpResult Any_compare(ConstAny a, ConstAny b) {
    Any x = *(Any*)a;
    Any y = *(Any*)b;
    return s_compare(x, y);
}

void pa_sort(Array array);

static void pa_sort_test(void) {
    printsln((Any)__func__);
    Array ac, ex;

    ac = sa_of_string("1, 2, 3, 4, 5");
    ex = sa_of_string("1, 2, 3, 4, 5");
    a_shuffle(ac);
    pa_sort(ac);
    pa_test_equal(ac, ex);
    pa_free(ac);
    pa_free(ex);

    ac = sa_of_string("1, 2, 1, 3, 2");
    ex = sa_of_string("1, 1, 2, 2, 3");
    pa_sort(ac);
    pa_test_equal(ac, ex);
    pa_free(ac);
    pa_free(ex);

    ac = sa_of_string("");
    ex = sa_of_string("");
    pa_sort(ac);
    pa_test_equal(ac, ex);
    pa_free(ac);
    pa_free(ex);

    ac = sa_of_string("-1, -2, -3, -1");
    ex = sa_of_string("-1, -1, -2, -3"); // alphabetic, not numeric sort
    pa_sort(ac);
    pa_test_equal(ac, ex);
    pa_free(ac);
    pa_free(ex);
}

void pa_sort(Array array) {
    require_element_size_pointer(array);
    qsort(array->a, array->n, sizeof(Any), Any_compare);
}

static CmpResult Any_compare_ignore_case(ConstAny a, ConstAny b) {
    Any x = *(Any*)a;
    Any y = *(Any*)b;
    return s_compare_ignore_case(x, y);
}

void pa_sort_ignore_case(Array array);

static void pa_sort_ignore_case_test(void) {
    printsln((Any)__func__);
    Array ac, ex;

    ac = sa_of_string("a, b, c, A, B");
    ex = sa_of_string("a, A, b, B, c");
    pa_sort_ignore_case(ac);
    pa_test_equal(ac, ex);
    pa_free(ac);
    pa_free(ex);
}

void pa_sort_ignore_case(Array array) {
    require_element_size_pointer(array);
    qsort(array->a, array->n, sizeof(Any), Any_compare_ignore_case);
}

static CmpResult Any_compare_dec(ConstAny a, ConstAny b) {
    Any x = *(Any*)a;
    Any y = *(Any*)b;
    return s_compare(y, x);
}

void pa_sort_dec(Array array);

static void pa_sort_dec_test(void) {
    printsln((Any)__func__);
    Array ac, ex;

    ac = sa_of_string("1, 2, 3, 4, 5");
    ex = sa_of_string("5, 4, 3, 2, 1");
    a_shuffle(ac);
    pa_sort_dec(ac);
    pa_test_equal(ac, ex);
    pa_free(ac);
    pa_free(ex);

    ac = sa_of_string("1, 2, 1, 3, 2");
    ex = sa_of_string("3, 2, 2, 1, 1");
    pa_sort_dec(ac);
    pa_test_equal(ac, ex);
    pa_free(ac);
    pa_free(ex);

    ac = sa_of_string("");
    ex = sa_of_string("");
    pa_sort_dec(ac);
    pa_test_equal(ac, ex);
    pa_free(ac);
    pa_free(ex);

    ac = sa_of_string("-3, -2, -1");
    ex = sa_of_string("-3, -2, -1"); // decreasing order alphabetically, not numerically!
    pa_sort_dec(ac);
    pa_test_equal(ac, ex);
    pa_free(ac);
    pa_free(ex);
}

void pa_sort_dec(Array array) {
    require_element_size_pointer(array);
    qsort(array->a, array->n, sizeof(Any), Any_compare_dec);
}

static CmpResult Any_compare_dec_ignore_case(ConstAny a, ConstAny b) {
    Any x = *(Any*)a;
    Any y = *(Any*)b;
    return s_compare_ignore_case(y, x);
}

void pa_sort_dec_ignore_case(Array array);

static void pa_sort_dec_ignore_case_test(void) {
    printsln((Any)__func__);
    Array ac, ex;

    ac = sa_of_string("a, b, c, A, B");
    ex = sa_of_string("c, b, B, a, A");
    pa_sort_dec_ignore_case(ac);
    pa_test_equal(ac, ex);
    pa_free(ac);
    pa_free(ex);
}

void pa_sort_dec_ignore_case(Array array) {
    require_element_size_pointer(array);
    qsort(array->a, array->n, sizeof(Any), Any_compare_dec_ignore_case);
}
#endif

///////////////////////////////////////////////////////////////////////////////

// @todo: pa_each_test

void pa_each(Array array, AnyFn f, Any x) {
    require_not_null(array);
    require_element_size_pointer(array);
    require_not_null(f);
    AnyIntAnyToAny ff = f;
    Any *a = array->a;
    for (int i = 0; i < array->n; i++) {
        a[i] = ff(a[i], i, x);
    }
}

#if 0
Array pa_map(Array array, AnyIntToAny f);

Any pa_upper_case(Any s, int index) {
    return s_upper_case(s);
}

static void pa_map_test(void) {
    printsln((Any)__func__);
    Array a, m, ex;
    
    a = sa_of_string("a, b, c, d");
    m = pa_map(a, pa_upper_case);
    ex = sa_of_string("A, B, C, D");
    pa_test_equal(m, ex);
    pa_free(a);
    pa_free(m);
    pa_free(ex);
}

Array pa_map(Array array, AnyIntToAny f) {
    require_not_null(f);
    require_element_size_pointer(array);
    int n = array->n;
    Any *a = array->a;
    Any *b = xmalloc(n * sizeof(Any));
    for (int i = 0; i < n; i++) {
        b[i] = f(a[i], i);
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(Any);
    result->a = b;
    return result;

}
#endif

// @todo: pa_map_test

Array pa_map(Array array, AnyFn f, Any x) {
    require_not_null(array);
    require_element_size_pointer(array);
    require_not_null(f);
    AnyIntAnyToAny ff = f;
    int n = array->n;
    Any *a = array->a;
    Any *b = xmalloc(n * sizeof(Any));
    for (int i = 0; i < n; i++) {
        b[i] = ff(a[i], i, x);
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(Any);
    result->a = b;
    return result;
}

static Any pa_concat(Any state, Any element, int index) {
    Any c = s_concat(state, element);
    s_free(state);
    return c;
}

static void pa_foldl_test(void) {
    printsln((Any)__func__);
    Array a;
    Any s;
    
    a = sa_of_string("a, b, c");    
    s = pa_foldl(a, pa_concat, s_copy("init"));
    test_equal_s(s, "initabc");
    s_free(s);
    pa_free(a);

    a = sa_of_string("");    
    s = pa_foldl(a, pa_concat, s_copy("init"));
    test_equal_s(s, "init");
    s_free(s);
    pa_free(a);
}

Any pa_foldl(Array array, AnyFn f, Any state) {
    require_not_null(array);
    require_element_size_pointer(array);
    require_not_null(f);
    AnyAnyIntToAny ff = f;
    Any *a = array->a;
    for (int i = 0; i < array->n; i++) {
        state = ff(state, a[i], i);
    }
    return state;
}

#if 0
void pa_foldl_state(Array array, AnyAnyIntToVoid f, Any state);

static void f_length(Any p, Any element, int index) {
    int *n = (int*)p;
    *n += s_length(element);
}


static void pa_foldl_state_test(void) {
    printsln((Any)__func__);
    Array a;
    
    // compute total length of all Anys in array
    a = sa_of_string("11, 22, 33, 44");
    int length = 0;
    pa_foldl_state(a, f_length, &length);
    test_equal_i(length, 8);
    pa_free(a);

    a = sa_of_string("11, 22, 33, ");
    length = 0;
    pa_foldl_state(a, f_length, &length);
    test_equal_i(length, 6);
    pa_free(a);

    a = sa_of_string(",   22,   33,   ");
    length = 0;
    pa_foldl_state(a, f_length, &length);
    test_equal_i(length, 4);
    pa_free(a);

    a = sa_of_string(",   ,   ,   ");
    length = 0;
    pa_foldl_state(a, f_length, &length);
    test_equal_i(length, 0);
    pa_free(a);

    a = sa_of_string("");
    length = 0;
    pa_foldl_state(a, f_length, &length);
    test_equal_i(length, 0);
    pa_free(a);
}

void pa_foldl_state(Array array, AnyAnyIntToVoid f, Any state) {
    require_not_null(f);
    require_element_size_pointer(array);
    Any *a = array->a;
    for (int i = 0; i < array->n; i++) {
        f(state, a[i], i);
    }
}
#endif

static String pa_concat_r(String element, String state, int index) {
    String c = s_concat(element, state);
    s_free(state);
    return c;
}

static void pa_foldr_test(void) {
    printsln((Any)__func__);
    
    Array a;
    Any s;
    
    a = sa_of_string("a, b, c");    
    s = pa_foldr(a, pa_concat_r, s_copy("X"));
    test_equal_s(s, "abcX");
    s_free(s);
    pa_free(a);

    a = sa_of_string("");    
    s = pa_foldr(a, pa_concat_r, s_copy("X"));
    test_equal_s(s, "X");
    s_free(s);
    pa_free(a);
}

Any pa_foldr(Array array, AnyFn f, Any state) {
    require_not_null(array);
    require_element_size_pointer(array);
    require_not_null(f);
    AnyAnyIntToAny ff = f;
    Any *a = array->a;
    for (int i = array->n - 1; i >= 0; i--) {
        state = ff(a[i], state, i);
    }
    return state;
}

///////////////////////////////////////////////////////////////////////////////

bool pa_index_even(Any value, int index) {
    return (index & 1) == 0;
}

bool pa_index_odd(Any value, int index) {
    return (index & 1) == 1;
}

bool pa_element_empty(Any value, int index) {
    return s_length(value) <= 0;
}

static bool f_ends_width(String element, int index, String start) {
    return s_ends_with(element, start);
}

static void pa_filter_test(void) {
    printsln((Any)__func__);
    Array a, ac, ex;

    a = sa_of_string("Kürbis, Brombeere, Heidelbeere, Himbeere, Apfel");
    ac = pa_filter(a, f_ends_width, "beere");
    ex = sa_of_string("Brombeere, Heidelbeere, Himbeere");
    sa_test_equal(ac, ex);
    pa_free(a);
    a_free(ac);
    pa_free(ex);
}

Array pa_filter(Array array, AnyFn predicate, Any x) {
    require_not_null(array);
    require_element_size_pointer(array);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    bool *ps = xmalloc(array->n * sizeof(bool));
    int n = 0;
    Any *a = array->a;
    for (int i = 0; i < array->n; i++) {
        ps[i] = f(a[i], i, x);
        if (ps[i]) n++;
    }
    Any *b = xmalloc(n * sizeof(Any));
    for (int i = 0, j = 0; i < array->n; i++) {
        if (ps[i]) {
            b[j++] = a[i];
        }
    }
    free(ps);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(Any);
    result->a = b;
    return result;
}

// @todo: add tests

Array pa_filter_state(Array array, AnyFn predicate, Any x, Any state) {
    require_not_null(array);
    require_element_size_pointer(array);
    require_not_null(predicate);
    AnyIntAnyAnyToBool f = predicate;
    bool *ps = xmalloc(array->n * sizeof(bool));
    int n = 0;
    Any *a = array->a;
    for (int i = 0; i < array->n; i++) {
        ps[i] = f(a[i], i, x, state);
        if (ps[i]) n++;
    }
    Any *b = xmalloc(n * sizeof(Any));
    for (int i = 0, j = 0; i < array->n; i++) {
        if (ps[i]) {
            b[j++] = a[i];
        }
    }
    free(ps);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(Any);
    result->a = b;
    return result;
}

static String starts_with_toupper(String element, int index, String x) {
    if (s_starts_with(element, x)) {
        return s_upper_case(element);
    }
    return NULL; // exclude this element
}

static void pa_choose_test(void) {
    printsln((Any)__func__);
    Array a, ac, ex;

    a = sa_of_string("a1, a2, b3, b4, a5, b6");
    ac = pa_choose(a, starts_with_toupper, "a");
    ex = sa_of_string("A1, A2, A5");
    sa_test_equal(ac, ex);

    pa_free(ac);
    pa_free(ex);
    pa_free(a);
}

Array pa_choose(Array array, AnyFn f, Any x) {
    require_not_null(array);
    require_element_size_pointer(array);
    require_not_null(f);
    AnyIntAnyToAny ff = f;
    Any *a = array->a;
    Any *b = xmalloc(array->n * sizeof(Any));
    int n = 0;
    for (int i = 0; i < array->n; i++) {
        Any op = ff(a[i], i, x);
        if (op != NULL) {
            b[n++] = op;
        }
    }
    int *c = xmalloc(n * sizeof(Any));
    memcpy(c, b, n * sizeof(Any));
    free(b);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(Any);
    result->a = c;
    return result;
}

Array pa_choose_state(Array array, AnyFn f, Any x, Any state) {
    require_not_null(array);
    require_element_size_pointer(array);
    require_not_null(f);
    AnyIntAnyAnyToAny ff = f;
    Any *a = array->a;
    Any *b = xmalloc(array->n * sizeof(Any));
    int n = 0;
    for (int i = 0; i < array->n; i++) {
        Any op = ff(a[i], i, x, state);
        if (op != NULL) {
            b[n++] = op;
        }
    }
    int *c = xmalloc(n * sizeof(Any));
    memcpy(c, b, n * sizeof(Any));
    free(b);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(Any);
    result->a = c;
    return result;
}

static bool pa_element_eq(String element, int index, String x) {
    return s_equals(element, x);
}

static bool pa_element_ne(String element, int index, String x) {
    return !s_equals(element, x);
}

static void pa_exists_test(void) {
    printsln((Any)__func__);
    Array a = sa_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(pa_exists(a, pa_element_eq, "3"), true);
    test_equal_b(pa_exists(a, pa_element_eq, "9"), false);
    test_equal_b(pa_exists(a, pa_element_ne, "9"), true);
    pa_free(a);
}

bool pa_exists(Array array, AnyFn predicate, Any x) {
    require_not_null(array);
    require_element_size_pointer(array);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    Any *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (f(a[i], i, x)) {
            return true;
        }
    }
    return false;
}

bool pa_exists_state(Array array, AnyFn predicate, Any x, Any state) {
    require_not_null(array);
    require_element_size_pointer(array);
    require_not_null(predicate);
    AnyIntAnyAnyToBool f = predicate;
    Any *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (f(a[i], i, x, state)) {
            return true;
        }
    }
    return false;
}

static void pa_forall_test(void) {
    printsln((Any)__func__);
    Array a = sa_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(pa_forall(a, pa_element_eq, "3"), false);
    test_equal_b(pa_forall(a, pa_element_ne, "9"), true);
    pa_free(a);
}

bool pa_forall(Array array, AnyFn predicate, Any x) {
    require_not_null(array);
    require_element_size_pointer(array);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    Any *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (!f(a[i], i, x)) {
            return false;
        }
    }
    return true;
}

// @todo: add tests

bool pa_forall_state(Array array, AnyFn predicate, Any x, Any state) {
    require_not_null(array);
    require_element_size_pointer(array);
    require_not_null(predicate);
    AnyIntAnyAnyToBool f = predicate;
    Any *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (!f(a[i], i, x, state)) {
            return false;
        }
    }
    return true;
}


///////////////////////////////////////////////////////////////////////////////
// Tests

bool pa_test_equal_file_line(const char *file, const char *function, int line, Array a, Any *e, int ne) {
    base_init();
    base_count_check();
    if (a->n != ne) {
        printf("%s, line %d: Actual length %d "
            "differs from expected length %d\n", file, line, a->n, ne);
        return false;
    }
    if (a->s != sizeof(Any)) {
        printf("%s, line %d: Actual element size %d "
            "differs from expected element size %lu\n", file, line, a->s, (unsigned long)sizeof(Any));
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
    Any *aa = a->a;
    for (int i = 0; i < a->n; i++) {
        if (aa[i] != e[i]) {
            printf("%s, line %d: Actual value ", file, line);
            printf("%p differs from expected value %p ", aa[i], e[i]);
            printf("at index %d.\n", i);
            return false;
        }
    }
    printf("%s, line %d: Test passed.\n", file, line);
    base_count_success();
    return true;
}

///////////////////////////////////////////////////////////////////////////////

void pa_test_all(void) {
    pa_create_test();
    a_copy_test();
    a_sub_test();
    a_concat_test();
    pa_contains_test();
//    pa_fill_test();
//    pa_fill_from_to_test();
    pa_index_test();
    pa_index_from_test();
    pa_index_fn_test();
    pa_last_index_test();
    pa_last_index_from_test();
    pa_last_index_fn_test();
    pa_reverse_test();
    pa_shuffle_test();
//    pa_sort_test();
//    pa_sort_ignore_case_test();
//    pa_sort_dec_test();
//    pa_sort_dec_ignore_case_test();
//    pa_insert_test();
//    pa_remove_test();
//    pa_each_test();
//    pa_map_test();
    pa_choose_test();
    pa_foldl_test();
    pa_foldr_test();
    pa_filter_test();
    pa_exists_test();
    pa_forall_test();
}

#if 0
int main(void) {
    pa_test_all();
    return 0;
}
#endif
