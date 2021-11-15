/*
@author Michael Rohs
@date 15.10.2015, 6.10.2020
@copyright Apache License, Version 2.0
*/

#include "array.h"
#include "string.h"
#include "string_array.h"



Array sa_create(int n, String value);

static void sa_create_test(void) {
    printsln((String)__func__);
    Array array;
    
    array = sa_create(3, "");
    String a1[] = { "", "", "" };
    sa_test_equal_file_line(__FILE__, __func__, __LINE__, array, a1, 3);
    a_free(array);
    
    array = sa_create(5, "x");
    String a2[] = { "x", "x", "x", "x", "x" };
    sa_test_equal_file_line(__FILE__, __func__, __LINE__, array, a2, 5);
    a_free(array);
    
    array = sa_create(1, "y");
    String a3[] = { "y" };
    sa_test_equal_file_line(__FILE__, __func__, __LINE__, array, a3, 1);
    a_free(array);
    
    array = sa_create(0, "z");
    String a4[] = { };
    sa_test_equal_file_line(__FILE__, __func__, __LINE__, array, a4, 0);
    a_free(array);
}

Array sa_create(int n, String value) {
    require("non-negative length", n >= 0);
    String *a = xmalloc(n * sizeof(String));
    for (int i = 0; i < n; i++) {
        a[i] = value;
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(String);
    result->a = a;
    return result;
}

Array sa_of_string(String s);

static void sa_of_string_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = sa_of_string("1, 2, 3");
    ex = sa_create(3, "");
    sa_set(ex, 0, "1");
    sa_set(ex, 1, "2");
    sa_set(ex, 2, "3");
    sa_test_equal(ac, ex);
    sa_free(ac);

    ac = sa_of_string("1    ,      2,         3  ");
    sa_set(ex, 0, "1    ");
    sa_set(ex, 1, "2");
    sa_set(ex, 2, "3  ");
    sa_test_equal(ac, ex);
    sa_free(ac); 
    a_free(ex);

    ac = sa_of_string("1    ,      2");
    ex = sa_create(2, "");
    sa_set(ex, 0, "1    ");
    sa_set(ex, 1, "2");
    sa_test_equal(ac, ex);
    sa_free(ac);
    a_free(ex);

    ac = sa_of_string("");
    ex = sa_create(1, "");
    sa_test_equal(ac, ex);
    sa_free(ac);
    a_free(ex);

    ac = sa_of_string("123");
    ex = sa_create(1, "123");
    sa_test_equal(ac, ex);
    sa_free(ac);
    a_free(ex);
}

Array sa_of_string(String s) {
    // count number of commas in s
    int n = 0; // number of commas
    char *t = s;
    while (*t != '\0') {
        if (*t == ',') n++;
        t++;
    }
    
    n++; // n commas, n + 1 array elements
    String *a = xmalloc(n * sizeof(String));
    t = s;
    int i = 0;
    char *start = s;
    while (*t != '\0') {
        while (*t != ',' && *t != '\0') t++;
        if (*t == '\0') break;
        // assert: *t is comma
        a[i++] = s_sub(start, 0, t - start);
        t++; // skip comma
        while (*t == ' ' || *t == '\t' || *t == '\n' || *t == '\r') t++; // skip whitespace
        start = t;
    }
    a[i++] = s_sub(start, 0, t - start);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(String);
    result->a = a;
    return result;
}

Array sa_split(String s, char separator);

// @todo: add test

Array sa_split(String s, char separator) {
    require_not_null(s);
    // count number of separators in s
    int n = 0; // number of separators
    char *t = s;
    while (*t != '\0') {
        if (*t == separator) n++;
        t++;
    }
    
    n++; // n separators, n + 1 array elements
    String *a = xmalloc(n * sizeof(String));
    t = s;
    int i = 0;
    char *start = s;
    while (*t != '\0') {
        // find next separator
        while (*t != separator && *t != '\0') t++;
        if (*t == '\0') break;
        // assert: *t is separator
        a[i++] = s_sub(start, 0, t - start);
        t++; // skip separator
        start = t;
    }
    a[i++] = s_sub(start, 0, t - start);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(String);
    result->a = a;
    return result;
}

static void a_copy_test(void) {
    printsln((String)__func__);
    Array array, copy;

    array = sa_of_string("-1, -2, -3");
    copy = a_copy(array);
    sa_test_equal(copy, array);
    sa_free(array);
    a_free(copy);

    array = sa_of_string("10");
    copy = a_copy(array);
    sa_test_equal(copy, array);
    sa_free(array);
    a_free(copy);

    array = sa_of_string("100, -200");
    copy = a_copy(array);
    sa_test_equal(copy, array);
    sa_free(array);
    a_free(copy);

    array = sa_of_string("");
    copy = a_copy(array);
    sa_test_equal(copy, array);
    sa_free(array);
    a_free(copy);
}

static void a_sub_test(void) {
    printsln((String)__func__);
    Array array, array2, sub;
    array = sa_of_string("1, 2, 3, 4");

    sub = a_sub(array, 0, a_length(array));
    sa_test_equal(array, sub);
    a_free(sub);

    sub = a_sub(array, -1, a_length(array) + 1);
    sa_test_equal(array, sub);
    a_free(sub);

    array2 = sa_of_string("2, 3, 4");
    // sa_println(array2);
    // printiln(a_length(array2));
    sub = a_sub(array, 1, a_length(array));
    sa_test_equal(array2, sub);
    sa_free(array2);
    a_free(sub);

    array2 = sa_of_string("2, 3");
    sub = a_sub(array, 1, a_length(array) - 1);
    sa_test_equal(array2, sub);
    sa_free(array2);
    a_free(sub);

    array2 = sa_create(0, "");
    sub = a_sub(array, 1, 1);
    sa_test_equal(array2, sub);
    a_free(sub);

    sub = a_sub(array, 2, 1);
    sa_test_equal(array2, sub);
    sa_free(array2);
    a_free(sub);

    sa_free(array);
}

void sa_free(Array array) {
    if (array != NULL) {
        require_element_size_string(array);
        if (array->a != NULL) {
            for (int i = 0; i < array->n; i++) {
                String s = sa_get(array, i);
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
String sa_get(Array array, int index) {
    require_not_null(array);
    require_element_size_string(array);
    require_x("index in range", index >= 0 && index < array->n, 
            "index == %d, length == %d", index, array->n);
    String *a = array->a;
    return a[index];
}
#endif

#ifndef NO_GET_SET
void sa_set(Array array, int index, String value) {
    require_not_null(array);
    require_element_size_string(array);
    require_x("index in range", index >= 0 && index < array->n, 
            "index == %d, length == %d", index, array->n);
    String *a = array->a;
    a[index] = value;
}
#endif

void sa_print(Array array) {
    require_not_null(array);
    require_element_size_string(array);
    String *a = array->a;
    printf("[");
    if (array->n > 0) {
        printf("\"%s\"", a[0]);
    }
    for (int i = 1; i < array->n; i++) {
        printf(", \"%s\"", a[i]);
    }
    printf("]");
}

void sa_println(Array array) {
    require_not_null(array);
    require_element_size_string(array);
    sa_print(array);
    printf("\n");
}

static void a_concat_test(void) {
    printsln((String)__func__);

    Array a1, a2, ac, ex;

    a1 = sa_of_string("10, 20, 30");
    a2 = sa_of_string("1, 2, 3");
    ac = a_concat(a1, a2);
    ex = sa_of_string("10, 20, 30, 1, 2, 3");
    sa_test_equal(ac, ex);
    sa_free(a1);
    sa_free(a2);
    a_free(ac);
    sa_free(ex);

    a1 = sa_of_string("10, 20, 30");
    a2 = sa_create(0, "");
    ac = a_concat(a1, a2);
    ex = sa_of_string("10, 20, 30");
    sa_test_equal(ac, ex);
    sa_free(a1);
    a_free(a2);
    a_free(ac);
    sa_free(ex);

    a1 = sa_create(0, "");
    a2 = sa_of_string("10, 20, 30");
    ac = a_concat(a1, a2);
    ex = sa_of_string("10, 20, 30");
    sa_test_equal(ac, ex);
    a_free(a1);
    sa_free(a2);
    a_free(ac);
    sa_free(ex);

    a1 = sa_create(0, "");
    a2 = sa_create(0, "");
    ac = a_concat(a1, a2);
    ex = sa_create(0, "");
    sa_test_equal(ac, ex);
    a_free(a1);
    a_free(a2);
    a_free(ac);
    a_free(ex);
}

bool sa_contains(Array array, String value);

static void sa_contains_test(void) {
    printsln((String)__func__);
    Array array;
    array = sa_of_string("10, 20, 30");
    test_equal_b(sa_contains(array, "10"), true);
    test_equal_b(sa_contains(array, "11"), false);
    test_equal_b(sa_contains(array, "30"), true);
    sa_free(array);
}

bool sa_contains(Array array, String value) {
    require_not_null(array);
    require_element_size_string(array);
    String *a = array->a;
    for (int i = 0; i < array->n; i++) {
        if (s_equals(a[i], value)) {
            return true;
        }
    }
    return false;
}

int sa_index(Array array, String value);

static void sa_index_test(void) {
    printsln((String)__func__);
    Array array;

    array = sa_of_string("1, 2, 3");
    int i = sa_index(array, "0");
#if 1
    if (i < 0) {
        printsln("value not found");
    } else {
        printf("value found at index %d\n", i);
    }
#endif
    test_equal_i(i, -1);
    test_equal_i(sa_index(array, "1"), 0);
    test_equal_i(sa_index(array, "2"), 1);
        
    sa_free(array);
}

int sa_index(Array array, String value) {
    require_not_null(array);
    require_element_size_string(array);
    String *a = array->a;
    for (int i = 0; i < array->n; i++) {
        if (s_equals(a[i], value)) {
            return i;
        }
    }
    return -1;
}

int sa_index_from(Array array, String value, int from);

static void sa_index_from_test(void) {
    printsln((String)__func__);
    Array a = sa_of_string("10, 20, 30, 40, 50");
    test_equal_i(sa_index_from(a, "20", 0), 1);
    test_equal_i(sa_index_from(a, "20", 1), 1);
    test_equal_i(sa_index_from(a, "20", 2), -1);
    test_equal_i(sa_index_from(a, "30", -1), 2);
    test_equal_i(sa_index_from(a, "60", 0), -1);
    sa_free(a);
}

int sa_index_from(Array array, String value, int from) {
    require_not_null(array);
    require_element_size_string(array);
    String *a = array->a;
    if (from < 0) from = 0;
    for (int i = from; i < array->n; i++) {
        if (s_equals(a[i], value)) {
            return i;
        }
    }
    return -1;
}

int sa_index_fn(Array array, StringIntStringToBool predicate, String x);

static bool is_equal_to(String s, int index, String x) {
    return s_equals(s, x);
}

static void sa_index_fn_test(void) {
    printsln((String)__func__);
    Array a = sa_of_string("test, dog, a, cat");
    test_equal_i(sa_index_fn(a, is_equal_to, "dog"), 1);
    sa_free(a);
}

int sa_index_fn(Array array, StringIntStringToBool predicate, String x) {
    require_not_null(array);
    require_element_size_string(array);
    require_not_null(predicate);
    String *a = array->a;
    for (int i = 0; i < array->n; i++) {
        if (predicate(a[i], i, x)) {
            return i;
        }
    }
    return -1;
}

int sa_last_index(Array array, String value);

static void sa_last_index_test(void) {
    printsln((String)__func__);
    Array a = sa_of_string("10, 20, 30, 10, 20, 30");
    test_equal_i(sa_last_index(a, "20"), 4);
    test_equal_i(sa_last_index(a, "30"), 5);
    test_equal_i(sa_last_index(a, "60"), -1);
    sa_free(a);
}

int sa_last_index(Array array, String value) {
    require_not_null(array);
    require_element_size_string(array);
    String *a = array->a;
    for (int i = array->n - 1; i >= 0; i--) {
        if (s_equals(a[i], value)) {
            return i;
        }
    }
    return -1;
}

int sa_last_index_from(Array array, String value, int from);

static void sa_last_index_from_test(void) {
    printsln((String)__func__);
    Array a = sa_of_string("10, 20, 30, 10, 20, 30");
    test_equal_i(sa_last_index_from(a, "20", 5), 4);
    test_equal_i(sa_last_index_from(a, "20", 4), 4);
    test_equal_i(sa_last_index_from(a, "20", 3), 1);
    test_equal_i(sa_last_index_from(a, "30", 2), 2);
    test_equal_i(sa_last_index_from(a, "30", 1), -1);
    sa_free(a);    
}

int sa_last_index_from(Array array, String value, int from) {
    require_not_null(array);
    require_element_size_string(array);
    if (from >= array->n) from = array->n - 1;
    String *a = array->a;
    for (int i = from; i >= 0; i--) {
        if (s_equals(a[i], value)) {
            return i;
        }
    }
    return -1;
}

int sa_last_index_fn(Array array, StringIntStringToBool predicate, String x);

static void sa_last_index_fn_test(void) {
    printsln((String)__func__);
    Array a = sa_of_string("no, dog, test, dog, a, cat");
    test_equal_i(sa_last_index_fn(a, is_equal_to, "dog"), 3);
    sa_free(a);
}

int sa_last_index_fn(Array array, StringIntStringToBool predicate, String x) {
    require_not_null(array);
    require_not_null(predicate);
    require_element_size_string(array);
    String *a = array->a;
    for (int i = array->n - 1; i >= 0; i--) {
        if (predicate(a[i], i, x)) {
            return i;
        }
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////

static void sa_reverse_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = sa_of_string("1, 2, 3");
    ex = sa_of_string("3, 2, 1");
    a_reverse(ac);
    sa_test_equal(ac, ex);
    sa_free(ac);
    sa_free(ex);

    ac = sa_of_string("1, 2, 3, 4");
    ex = sa_of_string("4, 3, 2, 1");
    a_reverse(ac);
    sa_test_equal(ac, ex);
    sa_free(ac);
    sa_free(ex);

    ac = sa_create(0, "");
    ex = sa_create(0, "");
    a_reverse(ac);
    sa_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = sa_create(0, "x");
    ex = sa_create(0, "x");
    a_reverse(ac);
    sa_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
}

static void sa_shuffle_test(void) {
    printsln((String)__func__);
    Array a = sa_of_string("1, 2, 3, 4, 5, 6, 7, 8, 9, 10");
    sa_println(a);
    a_shuffle(a);
    sa_println(a);
    sa_free(a);
}

static CmpResult string_compare(ConstAny a, ConstAny b) {
    require_not_null(a);
    require_not_null(b);
    String x = *(String*)a;
    String y = *(String*)b;
    return s_compare(x, y);
}

void sa_sort(Array array);

static void sa_sort_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = sa_of_string("1, 2, 3, 4, 5");
    ex = sa_of_string("1, 2, 3, 4, 5");
    a_shuffle(ac);
    sa_sort(ac);
    sa_test_equal(ac, ex);
    sa_free(ac);
    sa_free(ex);

    ac = sa_of_string("1, 2, 1, 3, 2");
    ex = sa_of_string("1, 1, 2, 2, 3");
    sa_sort(ac);
    sa_test_equal(ac, ex);
    sa_free(ac);
    sa_free(ex);

    ac = sa_of_string("");
    ex = sa_of_string("");
    sa_sort(ac);
    sa_test_equal(ac, ex);
    sa_free(ac);
    sa_free(ex);

    ac = sa_of_string("-1, -2, -3, -1");
    ex = sa_of_string("-1, -1, -2, -3"); // alphabetic, not numeric sort
    sa_sort(ac);
    sa_test_equal(ac, ex);
    sa_free(ac);
    sa_free(ex);
}

void sa_sort(Array array) {
    require_not_null(array);
    require_element_size_string(array);
    qsort(array->a, array->n, sizeof(String), string_compare);
}

static CmpResult string_compare_ignore_case(ConstAny a, ConstAny b) {
    require_not_null(a);
    require_not_null(b);
    String x = *(String*)a;
    String y = *(String*)b;
    return s_compare_ignore_case(x, y);
}

void sa_sort_ignore_case(Array array);

static void sa_sort_ignore_case_test(void) {
    printsln((String)__func__);
#if 0
    Array ac, ex;
    ac = sa_of_string("a, b, c, A, B");
    ex = sa_of_string("a, A, b, B, c");
    sa_sort_ignore_case(ac);
    sa_test_equal(ac, ex);
    sa_free(ac);
    sa_free(ex);
#endif
}

void sa_sort_ignore_case(Array array) {
    require_not_null(array);
    require_element_size_string(array);
    qsort(array->a, array->n, sizeof(String), string_compare_ignore_case);
}

static CmpResult string_compare_dec(ConstAny a, ConstAny b) {
    String x = *(String*)a;
    String y = *(String*)b;
    return s_compare(y, x);
}

void sa_sort_dec(Array array);

static void sa_sort_dec_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = sa_of_string("1, 2, 3, 4, 5");
    ex = sa_of_string("5, 4, 3, 2, 1");
    a_shuffle(ac);
    sa_sort_dec(ac);
    sa_test_equal(ac, ex);
    sa_free(ac);
    sa_free(ex);

    ac = sa_of_string("1, 2, 1, 3, 2");
    ex = sa_of_string("3, 2, 2, 1, 1");
    sa_sort_dec(ac);
    sa_test_equal(ac, ex);
    sa_free(ac);
    sa_free(ex);

    ac = sa_of_string("");
    ex = sa_of_string("");
    sa_sort_dec(ac);
    sa_test_equal(ac, ex);
    sa_free(ac);
    sa_free(ex);

    ac = sa_of_string("-3, -2, -1");
    ex = sa_of_string("-3, -2, -1"); // decreasing order alphabetically, not numerically!
    sa_sort_dec(ac);
    sa_test_equal(ac, ex);
    sa_free(ac);
    sa_free(ex);
}

void sa_sort_dec(Array array) {
    require_not_null(array);
    require_element_size_string(array);
    qsort(array->a, array->n, sizeof(String), string_compare_dec);
}

static CmpResult string_compare_dec_ignore_case(ConstAny a, ConstAny b) {
    require_not_null(a);
    require_not_null(b);
    String x = *(String*)a;
    String y = *(String*)b;
    return s_compare_ignore_case(y, x);
}

void sa_sort_dec_ignore_case(Array array);

static void sa_sort_dec_ignore_case_test(void) {
    printsln((String)__func__);
#if 0
    Array ac, ex;
    ac = sa_of_string("a, b, c, A, B");
    ex = sa_of_string("c, b, B, a, A");
    sa_sort_dec_ignore_case(ac);
    sa_test_equal(ac, ex);
    sa_free(ac);
    sa_free(ex);
#endif
}

void sa_sort_dec_ignore_case(Array array) {
    require_not_null(array);
    require_element_size_string(array);
    qsort(array->a, array->n, sizeof(String), string_compare_dec_ignore_case);
}

#if 0
void sa_insert(Array array, int i, int v);
// unclear what to do with element that falls off

void sa_remove(Array array, int i, int v);
// unclear what to do with removed element
#endif

///////////////////////////////////////////////////////////////////////////////

// @todo: sa_each_test

void sa_each(Array array, StringIntStringToString f, String x) {
    require_not_null(array);
    require_not_null(f);
    require_element_size_string(array);
    String *a = array->a;
    for (int i = 0; i < array->n; i++) {
        a[i] = f(a[i], i, x);
    }
}

// @todo: sa_map_test

Array sa_map(Array array, StringIntStringToString f, String x) {
    require_not_null(array);
    require_not_null(f);
    require_element_size_string(array);
    int n = array->n;
    String *a = array->a;
    String *b = xmalloc(n * sizeof(String));
    for (int i = 0; i < n; i++) {
        b[i] = f(a[i], i, x);
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(String);
    result->a = b;
    return result;
}

static String sa_concat(String state, String element, int index) {
    String c = s_concat(state, element);
    s_free(state);
    return c;
}

static void sa_foldl_test(void) {
    printsln((String)__func__);
    Array a;
    String s;
    
    a = sa_of_string("a, b, c");    
    s = sa_foldl(a, sa_concat, s_copy("init"));
    test_equal_s(s, "initabc");
    s_free(s);
    sa_free(a);

    a = sa_of_string("");    
    s = sa_foldl(a, sa_concat, s_copy("init"));
    test_equal_s(s, "init");
    s_free(s);
    sa_free(a);
}

String sa_foldl(Array array, StringStringIntToString f, String state) {
    require_not_null(array);
    require_not_null(f);
    require_element_size_string(array);
    String *a = array->a;
    for (int i = 0; i < array->n; i++) {
        state = f(state, a[i], i);
    }
    return state;
}

#if 0
void sa_foldl_state(Array array, AnyStringIntToVoid f, Any state);

static void f_length(Any p, String element, int index) {
    int *n = (int*)p;
    *n += s_length(element);
}


static void sa_foldl_state_test(void) {
    printsln((String)__func__);
    Array a;
    
    // compute total length of all strings in array
    a = sa_of_string("11, 22, 33, 44");
    int length = 0;
    sa_foldl_state(a, f_length, &length);
    test_equal_i(length, 8);
    sa_free(a);

    a = sa_of_string("11, 22, 33, ");
    length = 0;
    sa_foldl_state(a, f_length, &length);
    test_equal_i(length, 6);
    sa_free(a);

    a = sa_of_string(",   22,   33,   ");
    length = 0;
    sa_foldl_state(a, f_length, &length);
    test_equal_i(length, 4);
    sa_free(a);

    a = sa_of_string(",   ,   ,   ");
    length = 0;
    sa_foldl_state(a, f_length, &length);
    test_equal_i(length, 0);
    sa_free(a);

    a = sa_of_string("");
    length = 0;
    sa_foldl_state(a, f_length, &length);
    test_equal_i(length, 0);
    sa_free(a);
}

void sa_foldl_state(Array array, AnyStringIntToVoid f, Any state) {
    require_not_null(array);
    require_not_null(f);
    require_element_size_string(array);
    String *a = array->a;
    for (int i = 0; i < array->n; i++) {
        f(state, a[i], i);
    }
}
#endif

String sa_foldr(Array array, StringStringIntToString f, String state);

static String sa_concat_r(String element, String state, int index) {
    String c = s_concat(element, state);
    s_free(state);
    return c;
}

static void sa_foldr_test(void) {
    printsln((String)__func__);
    
    Array a;
    String s;
    
    a = sa_of_string("a, b, c");    
    s = sa_foldr(a, sa_concat_r, s_copy("X"));
    test_equal_s(s, "abcX");
    s_free(s);
    sa_free(a);

    a = sa_of_string("");    
    s = sa_foldr(a, sa_concat_r, s_copy("X"));
    test_equal_s(s, "X");
    s_free(s);
    sa_free(a);
}

String sa_foldr(Array array, StringStringIntToString f, String state) {
    require_not_null(array);
    require_not_null(f);
    require_element_size_string(array);
    String *a = array->a;
    for (int i = array->n - 1; i >= 0; i--) {
        state = f(a[i], state, i);
    }
    return state;
}

///////////////////////////////////////////////////////////////////////////////

bool sa_index_even(String value, int index) {
    return (index & 1) == 0;
}

bool sa_index_odd(String value, int index) {
    return (index & 1) == 1;
}

bool sa_element_empty(String value, int index) {
    return s_length(value) <= 0;
}

Array sa_filter(Array array, StringIntStringToBool predicate, String x);

static bool f_ends_width(String element, int index, String start) {
    return s_ends_with(element, start);
}

static void sa_filter_test(void) {
    printsln((String)__func__);
    Array a, ac, ex;

    a = sa_of_string("Kürbis, Brombeere, Heidelbeere, Himbeere, Apfel");
    ac = sa_filter(a, f_ends_width, "beere");
    ex = sa_of_string("Brombeere, Heidelbeere, Himbeere");
    sa_test_equal(ac, ex);
    sa_free(a);
    a_free(ac);
    sa_free(ex);
}

Array sa_filter(Array array, StringIntStringToBool predicate, String x) {
    require_not_null(array);
    require_not_null(predicate);
    require_element_size_string(array);
    bool *ps = xmalloc(array->n * sizeof(bool));
    int n = 0;
    String *a = array->a;
    for (int i = 0; i < array->n; i++) {
        ps[i] = predicate(a[i], i, x);
        if (ps[i]) n++;
    }
    String *b = xmalloc(n * sizeof(String));
    for (int i = 0, j = 0; i < array->n; i++) {
        if (ps[i]) {
            b[j++] = a[i];
        }
    }
    free(ps);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(String);
    result->a = b;
    return result;
}

// @todo: add tests

Array sa_filter_state(Array array, StringIntStringAnyToBool predicate, String x, Any state) {
    require_not_null(array);
    require_not_null(predicate);
    require_element_size_string(array);
    bool *ps = xmalloc(array->n * sizeof(bool));
    int n = 0;
    String *a = array->a;
    for (int i = 0; i < array->n; i++) {
        ps[i] = predicate(a[i], i, x, state);
        if (ps[i]) n++;
    }
    String *b = xmalloc(n * sizeof(String));
    for (int i = 0, j = 0; i < array->n; i++) {
        if (ps[i]) {
            b[j++] = a[i];
        }
    }
    free(ps);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(String);
    result->a = b;
    return result;
}

static StringOption starts_with_toupper(String element, int index, String x) {
    if (s_starts_with(element, x)) {
        String u = s_upper_case(element);
        return make_string_some(u);
    }
    return make_string_none(); // exclude this element
}

static void sa_choose_test(void) {
    printsln((String)__func__);
    Array a, ac, ex;

    a = sa_of_string("a1, a2, b3, b4, a5, b6");
    ac = sa_choose(a, starts_with_toupper, "a");
    ex = sa_of_string("A1, A2, A5");
    sa_test_equal(ac, ex);

    sa_free(ac);
    sa_free(ex);
    sa_free(a);
}

Array sa_choose(Array array, StringIntStringToStringOption f, String x) {
    require_not_null(array);
    require_not_null(f);
    require_element_size_string(array);
    String *a = array->a;
    String *b = xmalloc(array->n * sizeof(String));
    int n = 0;
    for (int i = 0; i < array->n; i++) {
        StringOption op = f(a[i], i, x);
        if (!op.none) {
            b[n++] = op.some;
        }
    }
    int *c = xmalloc(n * sizeof(String));
    memcpy(c, b, n * sizeof(String));
    free(b);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(String);
    result->a = c;
    return result;
}

Array sa_choose_state(Array array, StringIntStringAnyToStringOption f, String x, Any state) {
    require_not_null(array);
    require_not_null(f);
    require_element_size_string(array);
    String *a = array->a;
    String *b = xmalloc(array->n * sizeof(String));
    int n = 0;
    for (int i = 0; i < array->n; i++) {
        StringOption op = f(a[i], i, x, state);
        if (!op.none) {
            b[n++] = op.some;
        }
    }
    int *c = xmalloc(n * sizeof(String));
    memcpy(c, b, n * sizeof(String));
    free(b);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(String);
    result->a = c;
    return result;
}

bool sa_exists(Array array, StringIntStringToBool predicate, String x);

static bool sa_element_eq(String element, int index, String x) {
    return s_equals(element, x);
}

static bool sa_element_ne(String element, int index, String x) {
    return !s_equals(element, x);
}

static void sa_exists_test(void) {
    printsln((String)__func__);
    Array a = sa_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(sa_exists(a, sa_element_eq, "3"), true);
    test_equal_b(sa_exists(a, sa_element_eq, "9"), false);
    test_equal_b(sa_exists(a, sa_element_ne, "9"), true);
    sa_free(a);
}

bool sa_exists(Array array, StringIntStringToBool predicate, String x) {
    require_not_null(array);
    require_not_null(predicate);
    require_element_size_string(array);
    String *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (predicate(a[i], i, x)) {
            return true;
        }
    }
    return false;
}

bool sa_exists_state(Array array, StringIntStringAnyToBool predicate, String x, Any state) {
    require_not_null(array);
    require_not_null(predicate);
    require_element_size_string(array);
    String *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (predicate(a[i], i, x, state)) {
            return true;
        }
    }
    return false;
}

bool sa_forall(Array array, StringIntStringToBool predicate, String x);

static void sa_forall_test(void) {
    printsln((String)__func__);
    Array a = sa_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(sa_forall(a, sa_element_eq, "3"), false);
    test_equal_b(sa_forall(a, sa_element_ne, "9"), true);
    sa_free(a);
}

bool sa_forall(Array array, StringIntStringToBool predicate, String x) {
    require_not_null(array);
    require_not_null(predicate);
    require_element_size_string(array);
    String *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (!predicate(a[i], i, x)) {
            return false;
        }
    }
    return true;
}

// @todo: add tests

bool sa_forall_state(Array array, StringIntStringAnyToBool predicate, String x, Any state) {
    require_not_null(array);
    require_not_null(predicate);
    require_element_size_string(array);
    String *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (!predicate(a[i], i, x, state)) {
            return false;
        }
    }
    return true;
}


///////////////////////////////////////////////////////////////////////////////
// Tests

bool sa_test_equal_file_line(const char *file, const char *function, int line, Array a, String *e, int ne) {
    base_init();
    base_count_check();
    if (a->n != ne) {
        printf("%s, line %d: Actual length %d "
            "differs from expected length %d\n", file, line, a->n, ne);
        return false;
    }
    if (a->s != sizeof(String)) {
        printf("%s, line %d: Actual element size %d "
            "differs from expected element size %lu\n", file, line, a->s, (unsigned long)sizeof(String));
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
    String *sa = a->a;
    for (int i = 0; i < a->n; i++) {
        if (!s_equals(sa[i], e[i])) {
            printf("%s, line %d: Actual value ", file, line);
            printsa(sa, a->n);
            prints(" differs from expected value ");
            printsa(e, ne);
            printf(" at index %d.\n", i);
            return false;
        }
    }
    printf("%s, line %d: Test passed.\n", file, line);
    base_count_success();
    return true;
}

///////////////////////////////////////////////////////////////////////////////

void sa_test_all(void) {
    sa_create_test();
    sa_of_string_test();
    a_copy_test();
    a_sub_test();
    a_concat_test();
    sa_contains_test();
//    sa_fill_test();
//    sa_fill_from_to_test();
    sa_index_test();
    sa_index_from_test();
    sa_index_fn_test();
    sa_last_index_test();
    sa_last_index_from_test();
    sa_last_index_fn_test();
    sa_reverse_test();
    sa_shuffle_test();
    sa_sort_test();
    sa_sort_ignore_case_test();
    sa_sort_dec_test();
    sa_sort_dec_ignore_case_test();
//    sa_insert_test();
//    sa_remove_test();
//    sa_each_test();
//    sa_map_test();
    sa_choose_test();
    sa_foldl_test();
    sa_foldr_test();
    sa_filter_test();
    sa_exists_test();
    sa_forall_test();
}

#if 0
int main(void) {
    sa_test_all();
    return 0;
}
#endif
