/*
@author Michael Rohs
@date 15.10.2015
@copyright Apache License, Version 2.0
*/

#include "array.h"
#include "byte_array.h"



static void ba_create_test(void) {
    printsln((String)__func__);
    Array array;
    
    array = ba_create(3, 0);
    Byte a1[] = { 0, 0, 0 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a1, 3);
    a_free(array);
    
    array = ba_create(5, 1);
    Byte a2[] = { 1, 1, 1, 1, 1 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a2, 5);
    a_free(array);
    
    array = ba_create(1, 2);
    Byte a3[] = { 2 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a3, 1);
    a_free(array);
    
    array = ba_create(0, 2);
    Byte a4[] = { };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a4, 0);
    a_free(array);
}

Array ba_create(int n, Byte value) {
    if (n < 0) {
        printf("%s: length cannot be negative (is %d)\n", __func__, n);
        exit(EXIT_FAILURE);
    }
    Byte *a = xmalloc(n * sizeof(Byte));
    for (int i = 0; i < n; i++) {
        a[i] = value;
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(Byte);
    result->a = a;
    return result;
}

static void ba_range_test(void) {
    printsln((String)__func__);
    Array array;

    array = ba_range(0, 3);
    Byte a1[] = { 0, 1, 2 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a1, 3);
    a_free(array);

    array = ba_range(3, 2);
    Byte a2[] = { 3 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a2, 1);
    a_free(array);

    array = ba_range(2, 3);
    Byte a3[] = { 2 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a3, 1);
    a_free(array);

    array = ba_range(3, 3);
    Byte a4[] = { };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a4, 0);
    a_free(array);

    array = ba_range(-3, -3);
    Byte a5[] = { };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a5, 0);
    a_free(array);

    array = ba_range(1, 4);
    Byte a6[] = { 1, 2, 3 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a6, 3);
    a_free(array);

    array = ba_range(4, 1);
    Byte a7[] = { 4, 3, 2 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a7, 3);
    a_free(array);
}

Array ba_range(Byte a, Byte b) {
    if (a <= b) {
        int n = b - a;
        Byte *arr = xmalloc(n * sizeof(Byte));
        for (int i = 0; i < n; i++) {
            arr[i] = a + i;
        }
        Array result = xmalloc(sizeof(ArrayHead));
        result->n = n;
        result->s = sizeof(Byte);
        result->a = arr;
        return result;
    } else /* a > b */ {
        int n = a - b;
        Byte *arr = xmalloc(n * sizeof(Byte));
        for (int i = 0; i < n; i++) {
            arr[i] = a - i;
        }
        Array result = xmalloc(sizeof(ArrayHead));
        result->n = n;
        result->s = sizeof(Byte);
        result->a = arr;
        return result;
    }
}

static void ba_of_string_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = ba_of_string("1, 2, 3, 4, 5, 6");
    ex = ba_range(1, 7);
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ba_of_string("   1, 2, 3, 4, 5, 6   ");
    ex = ba_range(1, 7);
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ba_of_string("1xx2asdfs3");
    ex = ba_range(1, 4);
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ba_of_string("y1xx2asdfs3x");
    ex = ba_range(1, 4);
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ba_of_string("3, 2, 1");
    ex = ba_range(3, 0);
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ba_of_string(".1.2..3.");
    ex = ba_create(3, 0);
    ba_set(ex, 0, 1);
    ba_set(ex, 1, 2);
    ba_set(ex, 2, 3);
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ba_of_string("-x--+ +.+++a-b c-");
    ex = ba_create(0, 0);
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ba_of_string("");
    ex = ba_create(0, 0);
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
}

Array ba_of_string(String s) {
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
    Byte *a = xmalloc(n * sizeof(Byte));
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
    result->s = sizeof(Byte);
    result->a = a;
    return result;
}


static Byte two_i_plus_1(int index, Byte x) {
    return 2 * index + 1;
}

static void ba_fn_test(void) {
    printsln((String)__func__);
    Array array;

    array = ba_fn(3, two_i_plus_1, 0);
    Byte a1[] = { 1, 3, 5 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a1, 3);
    a_free(array);

    array = ba_fn(4, two_i_plus_1, 0);
    Byte a2[] = { 1, 3, 5, 7 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a2, 4);
    a_free(array);

    array = ba_fn(1, two_i_plus_1, 0);
    Byte a3[] = { 1 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a3, 1);
    a_free(array);

    array = ba_fn(0, two_i_plus_1, 0);
    Byte a4[] = { };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a4, 0);
    a_free(array);
}

Array ba_fn(int n, IntByteToByte init, Byte x) {
    assert_function_not_null(init);
    if (n < 0) {
        printf("ba_fn: length cannot be negative (is %d)\n", n);
        exit(EXIT_FAILURE);
    }
    Byte *a = xmalloc(n * sizeof(Byte));
    for (int i = 0; i < n; i++) {
        a[i] = init(i, x);
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(Byte);
    result->a = a;
    return result;
}

#ifdef A_GET_SET
Byte ba_get(Array array, int index) {
    ba_assert_element_size(array);
    if (index < 0 || index >= array->n) {
        printf("%s: index %d is out of range "
            "(array length: %d, allowed indices: 0..%d)\n", 
        __func__, index, array->n, array->n - 1);
        exit(EXIT_FAILURE);
    }
    Byte *a = array->a;
    return a[index];
}
#endif

#ifdef A_GET_SET
void ba_set(Array array, int index, Byte value) {
    ba_assert_element_size(array);
    if (index < 0 || index >= array->n) {
        printf("%s: index %d is out of range "
            "(array length: %d, allowed indices: 0..%d)\n", 
        __func__, index, array->n, array->n - 1);
        exit(EXIT_FAILURE);
    }
    Byte *a = array->a;
    a[index] = value;
}
#endif

void ba_print(Array array) {
    assert_argument_not_null(array);
    ba_assert_element_size(array);
    Byte *a = array->a;
    printf("[");
    if (array->n > 0) {
        printf("%d", a[0]);
    }
    for (int i = 1; i < array->n; i++) {
        printf(" %d", a[i]);
    }
    printf("]");
}

void ba_println(Array array) {
    assert_argument_not_null(array);
    ba_assert_element_size(array);
    ba_print(array);
    printf("\n");
}

static void ba_contains_test(void) {
    printsln((String)__func__);
    Array array;
    array = ba_of_string("10, 20, 30");
    test_equal_b(ba_contains(array, 10), true);
    test_equal_b(ba_contains(array, 11), false);
    test_equal_b(ba_contains(array, 30), true);
    a_free(array);
}

bool ba_contains(Array array, Byte value) {
    assert_argument_not_null(array);
    ba_assert_element_size(array);
    Byte *a = array->a;
    for (int i = 0; i < array->n; i++) {
        if (a[i] == value) {
            return true;
        }
    }
    return false;
}

static void ba_fill_test(void) {
    printsln((String)__func__);
    Array array;

    array = ba_create(3, 0);
    ba_fill(array, -1);
    Byte a1[] = { -1, -1, -1 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a1, 3);
    a_free(array);

    array = ba_create(2, 0);
    ba_fill(array, 2);
    Byte a2[] = { 2, 2 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a2, 2);
    a_free(array);

    array = ba_create(0, 0);
    ba_fill(array, 2);
    Byte a3[] = { };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a3, 0);
    a_free(array);
}

void ba_fill(Array array, Byte value) {
    assert_argument_not_null(array);
    ba_assert_element_size(array);
    Byte *a = array->a;
    for (int i = 0; i < array->n; i++) {
        a[i] = value;
    }
}

static void ba_fill_from_to_test(void) {
    printsln((String)__func__);
    Array array;

    array = ba_create(3, 0);
    ba_fill_from_to(array, -1, 0, array->n);
    Byte a1[] = { -1, -1, -1 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a1, 3);
    a_free(array);

    array = ba_create(3, 0);
    ba_fill_from_to(array, -1, -1, array->n + 1);
    Byte a2[] = { -1, -1, -1 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a2, 3);
    a_free(array);

    array = ba_create(3, 0);
    ba_fill_from_to(array, -1, 0, 0);
    Byte a3[] = { 0, 0, 0 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a3, 3);
    a_free(array);

    array = ba_create(3, 0);
    ba_fill_from_to(array, -1, 0, 1);
    Byte a4[] = { -1, 0, 0 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a4, 3);
    a_free(array);

    array = ba_create(3, 0);
    ba_fill_from_to(array, -1, 2, 2);
    Byte a5[] = { 0, 0, 0 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a5, 3);
    a_free(array);

    array = ba_create(3, 0);
    ba_fill_from_to(array, -1, 2, 3);
    Byte a6[] = { 0, 0, -1 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a6, 3);
    a_free(array);

    array = ba_create(3, 0);
    ba_fill_from_to(array, -1, 2, 1);
    Byte a7[] = { 0, 0, 0 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a7, 3);
    a_free(array);

    array = ba_create(3, 0);
    ba_fill_from_to(array, -1, 1, 3);
    Byte a8[] = { 0, -1, -1 };
    ba_test_equal_file_line(__FILE__, __func__, __LINE__, array, a8, 3);
    a_free(array);
}

void ba_fill_from_to(Array array, Byte value, int from, int to) {
    assert_argument_not_null(array);
    ba_assert_element_size(array);
    if (from < 0) from = 0;
    if (to > array->n) to = array->n;
    Byte *a = array->a;
    for (int i = from; i < to; i++) {
        a[i] = value;
    }
}

static void ba_index_test(void) {
    printsln((String)__func__);
    Array array;

    array = ba_range(1, 3);
    int i = ba_index(array, 0);
#if 0
    if (i < 0) {
        printsln("value not found");
    } else {
        printf("value found at index %d\n", i);
    }
#endif
    test_equal_i(i, -1);

    i = ba_index(array, 1);
    test_equal_i(i, 0);
        
    i = ba_index(array, 2);
    test_equal_i(i, 1);
        
    a_free(array);
    
}

int ba_index(Array array, Byte value) {
    assert_argument_not_null(array);
    ba_assert_element_size(array);
    Byte *a = array->a;
    for (int i = 0; i < array->n; i++) {
        if (a[i] == value) {
            return i;
        }
    }
    return -1;
}

static void ba_index_from_test(void) {
    printsln((String)__func__);
    Array a = ba_of_string("10 20 30 40 50");
    test_equal_i(ba_index_from(a, 20, 0), 1);
    test_equal_i(ba_index_from(a, 20, 1), 1);
    test_equal_i(ba_index_from(a, 20, 2), -1);
    test_equal_i(ba_index_from(a, 30, -1), 2);
    test_equal_i(ba_index_from(a, 60, 0), -1);
    a_free(a);
    
}

int ba_index_from(Array array, Byte value, int from) {
    assert_argument_not_null(array);
    ba_assert_element_size(array);
    if (from < 0) from = 0;
    Byte *a = array->a;
    for (int i = from; i < array->n; i++) {
        if (a[i] == value) {
            return i;
        }
    }
    return -1;
}

static void ba_index_fn_test(void) {
    printsln((String)__func__);
    Array a = ba_of_string("1 2 3 4 5");
    test_equal_i(ba_index_fn(a, ba_even, 0), 1);
    test_equal_i(ba_index_fn(a, ba_lt, 0), -1);
    a_free(a);
}

int ba_index_fn(Array array, ByteIntByteToBool predicate, Byte x) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    ba_assert_element_size(array);
    Byte *a = array->a;
    for (int i = 0; i < array->n; i++) {
        if (predicate(a[i], i, x)) {
            return i;
        }
    }
    return -1;
}

static void ba_last_index_test(void) {
    printsln((String)__func__);
    Array a = ba_of_string("10 20 30 10 20 30");
    test_equal_i(ba_last_index(a, 20), 4);
    test_equal_i(ba_last_index(a, 30), 5);
    test_equal_i(ba_last_index(a, 60), -1);
    a_free(a);
}

int ba_last_index(Array array, Byte value) {
    assert_argument_not_null(array);
    ba_assert_element_size(array);
    Byte *a = array->a;
    for (int i = array->n - 1; i >= 0; i--) {
        if (a[i] == value) {
            return i;
        }
    }
    return -1;
}

static void ba_last_index_from_test(void) {
    printsln((String)__func__);
    Array a = ba_of_string("10 20 30 10 20 30");
    test_equal_i(ba_last_index_from(a, 20, 5), 4);
    test_equal_i(ba_last_index_from(a, 20, 4), 4);
    test_equal_i(ba_last_index_from(a, 20, 3), 1);
    test_equal_i(ba_last_index_from(a, 30, 2), 2);
    test_equal_i(ba_last_index_from(a, 30, 1), -1);
    a_free(a);    
}

int ba_last_index_from(Array array, Byte value, int from) {
    assert_argument_not_null(array);
    ba_assert_element_size(array);
    if (from >= array->n) from = array->n - 1;
    Byte *a = array->a;
    for (int i = from; i >= 0; i--) {
        if (a[i] == value) {
            return i;
        }
    }
    return -1;
}

static void ba_last_index_fn_test(void) {
    printsln((String)__func__);
    Array a = ba_of_string("1 2 2 3 0 5");
    test_equal_i(ba_last_index_fn(a, ba_even, 0), 4);
    test_equal_i(ba_last_index_fn(a, ba_lt, 1), 4);
    a_free(a);
}

int ba_last_index_fn(Array array, ByteIntByteToBool predicate, Byte x) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    ba_assert_element_size(array);
    Byte *a = array->a;
    for (int i = array->n - 1; i >= 0; i--) {
        if (predicate(a[i], i, x)) {
            return i;
        }
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////

static CmpResult byte_compare(ConstAny a, ConstAny b) {
    assert_argument_not_null(a);
    assert_argument_not_null(b);
    Byte x = *(Byte*)a;
    Byte y = *(Byte*)b;
    return (x == y) ? 0 : (x < y ? -1 : 1);
}

static void ba_sort_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = ba_of_string("1, 2, 3, 4, 5");
    ex = ba_of_string("1, 2, 3, 4, 5");
    a_shuffle(ac);
    ba_sort(ac);
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ba_of_string("1, 2, 1, 3, 2");
    ex = ba_of_string("1, 1, 2, 2, 3");
    ba_sort(ac);
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ba_of_string("");
    ex = ba_of_string("");
    ba_sort(ac);
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ba_of_string("-1, -2, -3, -1");
    ex = ba_of_string("-3, -2, -1, -1");
    ba_sort(ac);
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
}

void ba_sort(Array array) {
    assert_argument_not_null(array);
    ba_assert_element_size(array);
    qsort(array->a, array->n, array->s, byte_compare);
}

static CmpResult byte_compare_dec(ConstAny a, ConstAny b) {
    assert_argument_not_null(a);
    assert_argument_not_null(b);
    Byte x = *(Byte*)b;
    Byte y = *(Byte*)a;
    return (x == y) ? 0 : (x < y ? -1 : 1);
}

void ba_sort_dec(Array array);

static void ba_sort_dec_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = ba_of_string("1, 2, 3, 4, 5");
    ex = ba_of_string("5, 4, 3, 2, 1");
    a_shuffle(ac);
    ba_sort_dec(ac);
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ba_of_string("1, 2, 1, 3, 2");
    ex = ba_of_string("3, 2, 2, 1, 1");
    ba_sort_dec(ac);
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ba_of_string("");
    ex = ba_of_string("");
    ba_sort_dec(ac);
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ba_of_string("-3, -2, -1");
    ex = ba_of_string("-1, -2, -3");
    ba_sort_dec(ac);
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
}

void ba_sort_dec(Array array) {
    assert_argument_not_null(array);
    ba_assert_element_size(array);
    qsort(array->a, array->n, array->s, byte_compare_dec);
}

static void ba_insert_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = ba_of_string("1, 2, 3, 4, 5, 6");
    ba_insert(ac, 0, 9);
    ex = ba_of_string("9, 1, 2, 3, 4, 5");
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ba_of_string("1, 2, 3, 4, 5, 6");
    ba_insert(ac, 5, 9);
    ex = ba_of_string("1, 2, 3, 4, 5, 9");
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ba_of_string("1, 2, 3, 4, 5, 6");
    ba_insert(ac, 3, 9);
    ex = ba_of_string("1, 2, 3, 9, 4, 5");
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ba_of_string("1");
    ba_insert(ac, -1, 9);
    ex = ba_of_string("1");
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ba_of_string("1");
    ba_insert(ac, 1, 9);
    ex = ba_of_string("1");
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ba_of_string("");
    ba_insert(ac, 0, 9);
    ex = ba_of_string("");
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
}

void ba_insert(Array array, int i, Byte v) {
    assert_argument_not_null(array);
    ba_assert_element_size(array);
    if (i < 0 || i >= array->n) return;
    // make space at i
    Byte *a = array->a;
    for (int j = array->n - 1; j > i; j--) {
        a[j] = a[j - 1];
    }
    // set value at i to v
    a[i] = v;
}

static void ba_remove_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = ba_of_string("1, 2, 3, 4, 5, 6");
    ba_remove(ac, 0, 9);
    ex = ba_of_string("2, 3, 4, 5, 6, 9");
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ba_of_string("1, 2, 3, 4, 5, 6");
    ba_remove(ac, 5, 9);
    ex = ba_of_string("1, 2, 3, 4, 5, 9");
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ba_of_string("1, 2, 3, 4, 5, 6");
    ba_remove(ac, 3, 9);
    ex = ba_of_string("1, 2, 3, 5, 6, 9");
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ba_of_string("1");
    ba_remove(ac, -1, 9);
    ex = ba_of_string("1");
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ba_of_string("1");
    ba_remove(ac, 1, 9);
    ex = ba_of_string("1");
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ba_of_string("1");
    ba_remove(ac, 0, 9);
    ex = ba_of_string("9");
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    
    ac = ba_of_string("");
    ba_remove(ac, 0, 9);
    ex = ba_of_string("");
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
}

void ba_remove(Array array, int i, Byte v) {
    assert_argument_not_null(array);
    ba_assert_element_size(array);
    if (i < 0 || i >= array->n) return;
    // shift down, starting from i
    Byte *a = array->a;
    for (int j = i; j < array->n - 1; j++) {
        a[j] = a[j + 1];
    }
    // set value at n-1 to v
    a[array->n - 1] = v;
}

///////////////////////////////////////////////////////////////////////////////

bool ba_even(Byte value, int index, Byte x) {
    return (value & 1) == 0;
}

bool ba_odd(Byte value, int index, Byte x) {
    return (value & 1) == 1;
}

bool ba_index_even(Byte value, int index) {
    return (index & 1) == 0;
}

bool ba_index_odd(Byte value, int index) {
    return (index & 1) == 1;
}

bool ba_pos(Byte value, int index) {
    return value > 0;
}

bool ba_neg(Byte value, int index) {
    return value < 0;
}

bool ba_ge0(Byte value, int index) {
    return value >= 0;
}

bool ba_gt(Byte value, int index, Byte x) {
    return value > x;
}

bool ba_ge(Byte value, int index, Byte x) {
    return value >= x;
}

bool ba_lt(Byte value, int index, Byte x) {
    return value < x;
}

bool ba_le(Byte value, int index, Byte x) {
    return value <= x;
}

///////////////////////////////////////////////////////////////////////////////

Byte ba_square(Byte value, int index) {
    return value * value; // index not used
}

Byte ba_rnd(int index, Byte maximum) {
    return i_rnd(maximum); // index not used
}

Byte ba_times(Byte value, int index, Byte x) {
    return value * x;
}

Byte ba_plus(Byte x, Byte y, int index) {
    return x + y;
}

Byte ba_minus(Byte x, Byte y, int index) {
    return x - y;
}

Byte ba_mult(Byte x, Byte y, int index) {
    return x * y;
}

Byte ba_div(Byte x, Byte y, int index) {
    return x / y;
}

static void ba_each_test(void) {
    printsln((String)__func__);
    Array a, ex;
    
    a = ba_of_string("1, 2, 3, 4");
    ba_each(a, ba_times, 2);
    ex = ba_of_string("2, 4, 6, 8");
    ba_test_equal(a, ex);
    a_free(a);
    a_free(ex);
    
    a = ba_of_string("");
    ba_each(a, ba_times, 3);
    ex = ba_of_string("");
    ba_test_equal(a, ex);
    a_free(a);
    a_free(ex);
}

void ba_each(Array array, ByteIntByteToByte f, Byte x) {
    assert_argument_not_null(array);
    assert_function_not_null(f);
    ba_assert_element_size(array);
    Byte *a = array->a;
    for (int i = 0; i < array->n; i++) {
        a[i] = f(a[i], i, x);
    }
}

static void ba_each_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

void ba_each_state(Array array, ByteIntByteAnyToByte f, Byte x, Any state) {
    assert_argument_not_null(array);
    assert_function_not_null(f);
    assert_argument_not_null(array);
    ba_assert_element_size(array);
    Byte *a = array->a;
    for (int i = 0; i < array->n; i++) {
        a[i] = f(a[i], i, x, state);
    }
}

// @todo: add tests
Array ba_map(Array array, ByteIntByteToByte f, Byte x) {
    assert_argument_not_null(array);
    assert_function_not_null(f);
    ba_assert_element_size(array);
    Byte *a = array->a;
    Byte *b = xmalloc(array->n * sizeof(Byte));
    for (int i = 0; i < array->n; i++) {
        b[i] = f(a[i], i, x);
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = array->n;
    result->s = sizeof(Byte);
    result->a = b;
    return result;
}

// @todo: add tests
Array ba_map_state(Array array, ByteIntByteAnyToByte f, Byte x, Any state) {
    assert_argument_not_null(array);
    assert_function_not_null(f);
    ba_assert_element_size(array);
    Byte *a = array->a;
    Byte *b = xmalloc(array->n * sizeof(Byte));
    for (int i = 0; i < array->n; i++) {
        b[i] = f(a[i], i, x, state);
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = array->n;
    result->s = sizeof(Byte);
    result->a = b;
    return result;
}

static void ba_foldl_test(void) {
    printsln((String)__func__);
    Array a = ba_of_string("");    
    test_equal_b(ba_foldl(a, ba_plus, 100), 100);
    test_equal_b(ba_foldl(a, ba_minus, 100), 100);
    a_free(a);
    
    a = ba_of_string("1, 2, 3, 4");    
    test_equal_b(ba_foldl(a, ba_plus, 0),    (((0 + 1) + 2) + 3) + 4);
    test_equal_b(ba_foldl(a, ba_minus, 100), (((100 - 1) - 2) - 3) - 4);
    a_free(a);
}

int ba_foldl(Array array, ByteByteIntToByte f, Byte init) {
    assert_argument_not_null(array);
    assert_function_not_null(f);
    ba_assert_element_size(array);
    Byte *a = array->a;
    for (int i = 0; i < array->n; i++) {
        init = f(init, a[i], i);
    }
    return init;
}

static void ba_foldr_test(void) {
    printsln((String)__func__);
    Array a = ba_of_string("");    
    test_equal_b(ba_foldr(a, ba_plus, 100), 100);
    test_equal_b(ba_foldr(a, ba_minus, 100), 100);
    a_free(a);
    
    a = ba_of_string("1, 2, 3, 4");    
    test_equal_b(ba_foldr(a, ba_plus, 100), 1 + (2 + (3 + (4 + 100))));
    test_equal_b(ba_foldr(a, ba_minus, 0), 1 - (2 - (3 - (4 - 0))));
    a_free(a);
}

int ba_foldr(Array array, ByteByteIntToByte f, Byte init) {
    assert_argument_not_null(array);
    assert_function_not_null(f);
    ba_assert_element_size(array);
    Byte *a = array->a;
    for (int i = array->n - 1; i >= 0; i--) {
        init = f(a[i], init, i);
    }
    return init;
}

static void ba_filter_test(void) {
    printsln((String)__func__);
    Array a, ac, ex;

    a = ba_of_string("1, 2, 3, 4, 5, 6");
    ac = ba_filter(a, ba_gt, 3);
    ex = ba_of_string("4, 5, 6");
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ba_filter(a, ba_lt, 4);
    ex = ba_of_string("1, 2, 3");
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ba_filter(a, ba_le, 3);
    ex = ba_of_string("1, 2, 3");
    ba_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
    a_free(a);
}

Array ba_filter(Array array, ByteIntByteToBool predicate, Byte x) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    ba_assert_element_size(array);
    Byte *a = array->a;
    bool *ps = xmalloc(array->n * sizeof(bool));
    int n = 0;
    for (int i = 0; i < array->n; i++) {
        ps[i] = predicate(a[i], i, x);
        if (ps[i]) n++;
    }
    Byte *b = xmalloc(n * sizeof(Byte));
    for (int i = 0, j = 0; i < array->n; i++) {
        if (ps[i]) {
            b[j++] = a[i];
        }
    }
    free(ps);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(Byte);
    result->a = b;
    return result;
}

// @todo: add tests

Array ba_filter_state(Array array, ByteIntByteAnyToBool predicate, Byte x, Any state) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    ba_assert_element_size(array);
    Byte *a = array->a;
    bool *ps = xmalloc(array->n * sizeof(bool));
    int n = 0;
    for (int i = 0; i < array->n; i++) {
        ps[i] = predicate(a[i], i, x, state);
        if (ps[i]) n++;
    }
    Byte *b = xmalloc(n * sizeof(Byte));
    for (int i = 0, j = 0; i < array->n; i++) {
        if (ps[i]) {
            b[j++] = a[i];
        }
    }
    free(ps);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(Byte);
    result->a = b;
    return result;
}

static ByteOption evens_times_x(Byte element, int index, Byte x) {
    if ((element % 2) == 0) {
        return make_byte_some(x * element);
    }
    return make_byte_none();
}

static void ba_choose_test(void) {
    printsln((String)__func__);
    Array a, ac, ex;

    a = ba_of_string("1, 2, 3, 4, 5, 6");
    ac = ba_choose(a, evens_times_x, 3);
    ex = ba_of_string("6, 12, 18");
    ba_test_equal(ac, ex);

    a_free(ac);
    a_free(ex);
    a_free(a);
}

Array ba_choose(Array array, ByteIntByteToByteOption f, Byte x) {
    assert_argument_not_null(array);
    assert_function_not_null(f);
    ba_assert_element_size(array);
    Byte *a = array->a;
    Byte *b = xmalloc(array->n * sizeof(Byte));
    int n = 0;
    for (int i = 0; i < array->n; i++) {
        ByteOption op = f(a[i], i, x);
        if (!op.none) {
            b[n++] = op.some;
        }
    }
    Byte *c = xmalloc(n * sizeof(Byte));
    memcpy(c, b, n * sizeof(Byte));
    free(b);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(Byte);
    result->a = c;
    return result;
}

// @todo: add tests

Array ba_choose_state(Array array, ByteIntByteAnyToByteOption f, Byte x, Any state) {
    assert_argument_not_null(array);
    assert_function_not_null(f);
    ba_assert_element_size(array);
    Byte *a = array->a;
    Byte *b = xmalloc(array->n * sizeof(Byte));
    int n = 0;
    for (int i = 0; i < array->n; i++) {
        ByteOption op = f(a[i], i, x, state);
        if (!op.none) {
            b[n++] = op.some;
        }
    }
    Byte *c = xmalloc(n * sizeof(Byte));
    memcpy(c, b, n * sizeof(Byte));
    free(b);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = sizeof(Byte);
    result->a = c;
    return result;
}

static void ba_exists_test(void) {
    printsln((String)__func__);
    Array a = ba_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(ba_exists(a, ba_gt, 3), true);
    test_equal_b(ba_exists(a, ba_gt, 9), false);
    test_equal_b(ba_exists(a, ba_lt, 1), false);
    a_free(a);
}

bool ba_exists(Array array, ByteIntByteToBool predicate, Byte x) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    ba_assert_element_size(array);
    Byte *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (predicate(a[i], i, x)) {
            return true;
        }
    }
    return false;
}

// @todo: add tests

bool ba_exists_state(Array array, ByteIntByteAnyToBool predicate, Byte x, Any state) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    ba_assert_element_size(array);
    Byte *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (predicate(a[i], i, x, state)) {
            return true;
        }
    }
    return false;
}

static void ba_forall_test(void) {
    printsln((String)__func__);
    Array a = ba_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(ba_forall(a, ba_gt, 0), true);
    test_equal_b(ba_forall(a, ba_gt, 1), false);
    test_equal_b(ba_forall(a, ba_lt, 7), true);
    a_free(a);
}

bool ba_forall(Array array, ByteIntByteToBool predicate, Byte x) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    ba_assert_element_size(array);
    Byte *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (!predicate(a[i], i, x)) {
            return false;
        }
    }
    return true;
}

bool ba_forall_state(Array array, ByteIntByteAnyToBool predicate, Byte x, Any state) {
    assert_argument_not_null(array);
    assert_function_not_null(predicate);
    ba_assert_element_size(array);
    Byte *a = array->a;
    for (int i = 0; i < a_length(array); i++) {
        if (!predicate(a[i], i, x, state)) {
            return false;
        }
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Testing

bool ba_test_equal_file_line(const char *file, const char *function, int line, Array a, Byte *e, int ne) {
    base_init();
    base_count_check();
    if (a->n != ne) {
        printf("%s, line %d: Actual length %d "
            "differs from expected length %d\n", file, line, a->n, ne);
        return false;
    }
    if (a->s != sizeof(Byte)) {
        printf("%s, line %d: Actual element size %d "
            "differs from expected element size %lu\n", file, line, a->s, (unsigned long)sizeof(Byte));
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
    Byte *ia = a->a;
    for (int i = 0; i < a->n; i++) {
        if (ia[i] != e[i]) {
            printf("%s, line %d: Actual value ", file, line);
            printba(ia, a->n);
            prints(" differs from expected value ");
            printba(e, ne);
            printf(" at index %d.\n", i);
            return false;
        }
    }
    printf("%s, line %d: check passed\n", file, line);
    base_count_success();
    return true;
}



///////////////////////////////////////////////////////////////////////////////

void ba_test_all(void) {
    ba_create_test();
    ba_range_test();
    ba_of_string_test();
    ba_fn_test();
    ba_contains_test();
    ba_fill_test();
    ba_fill_from_to_test();
    ba_index_test();
    ba_index_from_test();
    ba_index_fn_test();
    ba_last_index_test();
    ba_last_index_from_test();
    ba_last_index_fn_test();
    ba_sort_test();
    ba_sort_dec_test();
    ba_insert_test();
    ba_remove_test();
    ba_each_test();
    ba_each_state_test();
    ba_foldl_test();
    ba_foldr_test();
    ba_filter_test();
    // ba_filter_state_test();
    ba_choose_test();
    ba_exists_test();
    ba_forall_test();
}

#if 0
int main(void) {
    ba_test_all();
    return 0;
}
#endif
