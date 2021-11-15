/*
@author Michael Rohs
@date 15.10.2015
@copyright Apache License, Version 2.0
*/

#include "array.h"
#include "int_array.h"
#include "list.h"
#include "string.h"



// Address is used for testing.
typedef struct Address {
    String firstname;
    String lastname;
    String city;
} Address;

static Address make_address(String f, String l, String c) {
    Address a = { f, l, c };
    return a;
}

static void a_create_test(void) {
    printsln((String)__func__);
    Array a;
    Address addr;

    // would violate first precondition:
    // a = a_create(-1, sizeof(Address));
    // would violate second precondition:
    // a = a_create(1, 0);
    a = a_create(3, sizeof(Address));

    addr = make_address("Fred", "Oyster", "Hannover");
    a_set(a, 0, &addr);
    addr = make_address("Frida", "Qwirin", "Hannover");
    a_set(a, 1, &addr);
    addr = make_address("James", "Bond", "London");
    a_set(a, 2, &addr);
    
    Address *pa = a_get(a, 0);
    // printsln(pa->firstname);
    test_equal_s(pa->firstname, "Fred");
    pa = a_get(a, 1);
    // printsln(pa->firstname);
    test_equal_s(pa->city, "Hannover");
    pa = a_get(a, 2);
    // printsln(pa->firstname);
    test_equal_s(pa->lastname, "Bond");
    
    a_free(a);
}

Array a_create(int n, int s) {
    require("non-negative length", n >= 0);
    require("positive size", s > 0);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = s;
    result->a = xcalloc(n, s);
    return result;
}

static void a_of_buffer_test(void) {
    printsln((String)__func__);
    Address addr[] = {
        make_address("Fred", "Oyster", "Hannover"), 
        make_address("Frida", "Qwirin", "Berlin"), 
        make_address("James", "Bond", "London"), 
    };
    Array a = a_of_buffer(addr, 3, sizeof(Address));

    // would violate precondition:
    // a = a_of_buffer(NULL, 3, sizeof(Address));
    // would violate precondition:
    // a = a_of_buffer(&addr, -1, sizeof(Address));
    // would violate precondition:
    // a = a_of_buffer(&addr, 3, 0);

    Address *pa = a_get(a, 0);
    printsln(pa->firstname);
    test_equal_s(pa->firstname, "Fred");
    pa = a_get(a, 1);
    printsln(pa->city);
    test_equal_s(pa->city, "Berlin");
    pa = a_get(a, 2);
    printsln(pa->lastname);
    test_equal_s(pa->lastname, "Bond");
    
    a_free(a);
    
}

Array a_of_buffer(Any buffer, int n, int s) {
    require_not_null(buffer);
    require("non-negative length", n >= 0);
    require("positive size", s > 0);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = s;
    result->a = xmalloc(n * s);
    memcpy(result->a, buffer, n * s);
    return result;
}

static void x_and_xsquare(Any element, int index, Any state) {
    IntPair *ip = element;
    ip->i = index;
    ip->j = index * index;
}

static void a_fn_test(void) {
    printsln((String)__func__);
    Array ac, ex;
    IntPair ip;

    ac = a_fn(3, sizeof(IntPair), x_and_xsquare, NULL);

    // would violate precondition:
    // ac = a_fn(-1, sizeof(IntPair), x_and_xsquare, NULL);
    // would violate precondition:
    // ac = a_fn(3, 0, x_and_xsquare, NULL);
    // would violate precondition:
    // ac = a_fn(3, sizeof(IntPair), NULL, NULL);

    ex = a_create(3, sizeof(IntPair));
    ip = make_int_pair(0, 0);
    a_set(ex, 0, &ip);
    ip = make_int_pair(1, 1);
    a_set(ex, 1, &ip);
    ip = make_int_pair(2, 4);
    a_set(ex, 2, &ip);
    
    a_test_equal(ac, ex);
    
    a_free(ac);
    a_free(ex);
}

Array a_fn(int n, int s, AnyFn init, Any state) {
    require("non-negative length", n >= 0);
    require("positive size", s > 0);
    require_not_null(init);
    AnyIntAnyToVoid f = init;
    Byte *a = xcalloc(n, s);
    for (int i = 0; i < n; i++) {
        Any element = a + i * s;
        f(element, i, state);
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = s;
    result->a = a;
    return result;
}

///////////////////////////////////////////////////////////////////////////////

static void a_copy_test(void) {
    printsln((String)__func__);
    Array array, copy;
    Address a;
    
    array = a_create(3, sizeof(Address));
    a = make_address("Fred", "Oyster", "Hannover");
    a_set(array, 0, &a);
    a = make_address("Frida", "Qwirin", "Hannover");
    a_set(array, 1, &a);
    a = make_address("James", "Bond", "London");
    a_set(array, 2, &a);

    copy = a_copy(array);
    a_test_equal(copy, array);

    // would violate precondition:
    // copy = a_copy(NULL);
    
    a_free(array);
    a_free(copy);

    array = ia_of_string("-1, -2, -3");
    copy = a_copy(array);
    ia_test_equal(copy, array);
    a_free(array);
    a_free(copy);

    array = ia_of_string("10");
    copy = a_copy(array);
    ia_test_equal(copy, array);
    a_free(array);
    a_free(copy);

    array = ia_of_string("100, -200");
    copy = a_copy(array);
    ia_test_equal(copy, array);
    a_free(array);
    a_free(copy);

    array = ia_of_string("");
    copy = a_copy(array);
    ia_test_equal(copy, array);
    a_free(array);
    a_free(copy);

    array = da_of_string("-1, -2, -3");
    copy = a_copy(array);
    da_test_within(array, copy);
    a_free(array);
    a_free(copy);
    array = da_of_string("10");
    copy = a_copy(array);
    da_test_within(array, copy);
    a_free(array);
    a_free(copy);
    array = da_of_string(" 100, -200 ");
    copy = a_copy(array);
    da_test_within(array, copy);
    a_free(array);
    a_free(copy);
    array = da_of_string("");
    copy = a_copy(array);
    da_test_within(array, copy);
    a_free(array);
    a_free(copy);
}

Array a_copy(Array array) {
    require_not_null(array);
    int n = array->n * array->s;
    Any a = xmalloc(n);
    memcpy(a, array->a, n);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = array->n;
    result->s = array->s;
    result->a = a;
    return result;
}

static void print_int_pair(IntPair *element) {
    printf("(%d, %d)", element->i, element->j);
}

static void a_sub_test(void) {
    printsln((String)__func__);
    Array a, ac, ex;
    IntPair ip;

    a = a_create(3, sizeof(IntPair));
    ip = make_int_pair(1, 10);
    a_set(a, 0, &ip);
    ip = make_int_pair(2, 20);
    a_set(a, 1, &ip);
    ip = make_int_pair(3, 30);
    a_set(a, 2, &ip);
    a_println(a, print_int_pair);

    ex = a_create(2, sizeof(IntPair));
    ip = make_int_pair(1, 10);
    a_set(ex, 0, &ip);
    ip = make_int_pair(2, 20);
    a_set(ex, 1, &ip);

    // would violate precondition:
    // ac = a_sub(NULL, 1, 2);
    
    ac = a_sub(a, 0, 2);
    a_test_equal(ac, ex);
    
    ip = make_int_pair(2, 20);
    a_set(ex, 0, &ip);
    ip = make_int_pair(3, 30);
    a_set(ex, 1, &ip);
    
    a_free(ac);
    ac = a_sub(a, 1, 3);
    a_test_equal(ac, ex);
    
    a_free(a);
    a_free(ac);
    a_free(ex);

    Array array, array2, sub;
    array = ia_of_string("1 2 3 4");

    sub = a_sub(array, 0, a_length(array));
    ia_test_equal(array, sub);
    a_free(sub);

    sub = a_sub(array, -1, a_length(array) + 1);
    ia_test_equal(array, sub);
    a_free(sub);

    array2 = ia_of_string("2 3 4");
    sub = a_sub(array, 1, a_length(array));
    ia_test_equal(sub, array2);
    a_free(array2);
    a_free(sub);

    array2 = ia_of_string("2 3");
    sub = a_sub(array, 1, a_length(array) - 1);
    ia_test_equal(sub, array2);
    a_free(array2);
    a_free(sub);

    array2 = ia_of_string("");
    sub = a_sub(array, 1, 1);
    ia_test_equal(sub, array2);
    a_free(sub);

    sub = a_sub(array, 2, 1);
    ia_test_equal(sub, array2);
    a_free(array2);
    a_free(sub);
    a_free(array);
    
    array = da_range(1, 10, 1);
    
    sub = a_sub(array, 0, array->n);
    da_test_within(sub, array);
    a_free(sub);

    sub = a_sub(array, -10, array->n + 10);
    da_test_within(sub, array);
    a_free(sub);

    array2 = da_range(2, 10, 1);
    sub = a_sub(array, 1, array->n);
    da_test_within(sub, array2);
    a_free(array2);
    a_free(sub);

    array2 = da_range(2, 9, 1);
    sub = a_sub(array, 1, array->n - 1);
    da_test_within(sub, array2);
    a_free(array2);
    a_free(sub);

    array2 = da_range(0, 0, 1);
    sub = a_sub(array, 1, 1);
    da_test_within(sub, array2);
    a_free(sub);

    sub = a_sub(array, 2, 1);
    da_test_within(sub, array2);
    a_free(array2);
    a_free(sub);

    array2 = da_range(3, 6, 1);
    sub = a_sub(array, 2, 5);
    da_test_within(sub, array2);
    a_free(array2);
    a_free(sub);
    a_free(array);
}

Array a_sub(Array array, int i, int j) {
    require_not_null(array);
    if (i >= j || i >= array->n || j <= 0) {
        Array result = xmalloc(sizeof(ArrayHead));
        result->n = 0;
        result->s = array->s;
        result->a = NULL;
        return result;
    }
    // assert: i < j && i < array->n && j > 0
    if (i < 0) i = 0;
    if (j > array->n) j = array->n;
    int n = j - i;
    Any a = xmalloc(n * array->s);
    Byte *p = array->a;
    memcpy(a, p + i * array->s, n * array->s);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = array->s;
    result->a = a;
    return result;
}

Array a_of_l(List list) {
    require_not_null(list);
    int n = l_length(list);
    int i = 0;
    Array result = a_create(n, list->s);
    for (ListNode *node = list->first; node != NULL; node = node->next, i++) {
        Any element = node + 1;
        a_set(result, i, element);
    }
    return result;
}

static void a_blit_test(void) {
    printsln((String)__func__);
    Array s, d, e;

    s = ia_range(1, 10);
    d = ia_create(5, 0);
    a_blit(s, 0, d, 0, 5);
    e = ia_of_string("1 2 3 4 5");
    ia_test_equal(d, e);

    // would violate precondition:
    // a_blit(NULL, 0, d, 0, 5);
    // would violate precondition:
    // a_blit(s, -1, d, 0, 5);
    // would violate precondition:
    // a_blit(s, 9, d, 0, 5);
    // would violate precondition:
    // a_blit(s, 0, d, -1, 5);
    // would violate precondition:
    // a_blit(s, 0, d, 100, 5);
    // would violate precondition:
    // a_blit(s, 0, d, 0, 10); // src too small
    // would violate precondition:
    // a_blit(s, 0, d, 0, 9); // dst too small
    // would violate precondition:
    // s->s = 24;
    // a_blit(s, 0, d, 0, 5);

    a_free(s);
    a_free(d);
    a_free(e);

    s = ia_range(1, 10);
    d = ia_create(5, 0);
    a_blit(s, 0, d, 1, 4);
    e = ia_of_string("0 1 2 3 4");
    ia_test_equal(d, e);
    a_free(s);
    a_free(d);
    a_free(e);

    s = ia_range(1, 10);
    d = ia_create(5, 0);
    a_blit(s, 0, d, 1, 3);
    e = ia_of_string("0 1 2 3 0");
    ia_test_equal(d, e);
    a_free(s);
    a_free(d);
    a_free(e);

    s = ia_range(1, 10);
    d = ia_create(5, 0);
    a_blit(s, 0, d, 0, 0);
    e = ia_of_string("0 0 0 0 0");
    ia_test_equal(d, e);
    a_free(s);
    a_free(d);
    a_free(e);

    s = ia_range(1, 10);
    d = ia_create(5, 0);
    a_blit(s, 1, d, 0, 3);
    e = ia_of_string("2 3 4 0 0");
    ia_test_equal(d, e);
    a_free(s);
    a_free(d);
    a_free(e);

    s = da_range(1, 10, 1);
    d = da_create(5, 0);
    a_blit(s, 0, d, 0, 5);
    e = da_of_string("1 2 3 4 5");
    da_test_within(d, e);
    a_free(s);
    a_free(d);
    a_free(e);

    s = da_range(1, 10, 1);
    d = da_create(5, 0);
    a_blit(s, 0, d, 1, 4);
    e = da_of_string("0 1 2 3 4");
    da_test_within(d, e);
    a_free(s);
    a_free(d);
    a_free(e);

    s = da_range(1, 10, 1);
    d = da_create(5, 0);
    a_blit(s, 0, d, 1, 3);
    e = da_of_string("0 1 2 3 0");
    da_test_within(d, e);
    a_free(s);
    a_free(d);
    a_free(e);

    s = da_range(1, 10, 1);
    d = da_create(5, 0);
    a_blit(s, 0, d, 0, 0);
    e = da_of_string("0 0 0 0 0");
    da_test_within(d, e);
    a_free(s);
    a_free(d);
    a_free(e);

    s = da_range(1, 10, 1);
    d = da_create(5, 0);
    a_blit(s, 1, d, 0, 3);
    e = da_of_string("2 3 4 0 0");
    da_test_within(d, e);
    a_free(s);
    a_free(d);
    a_free(e);

    // overlapping arrays
    s = ia_range(1, 6);
    a_blit(s, 0, s, 1, 4);
    e = ia_of_string("1 1 2 3 4");
    ia_test_equal(s, e);
    a_free(s);
    a_free(e);

    s = ia_range(1, 6);
    a_blit(s, 1, s, 0, 4);
    e = ia_of_string("2 3 4 5 5");
    ia_test_equal(s, e);
    a_free(s);
    a_free(e);
}

void a_blit(Array source, int source_index, Array destination, int destination_index, int count) {
    require_not_null(source);
    require_not_null(destination);
    if (count <= 0) return;
    require_x("equal element sizes", source->s == destination->s, "source->s == %d, destination->s == %d", source->s, destination->s);
    require_x("source_index in range", source_index >= 0 && source_index < source->n, 
            "source_index == %d, source->n == %d", source_index, source->n);
    require_x("destination_index in range", destination_index >= 0 && destination_index < destination->n, 
            "destination_index == %d, destination->n == %d", destination_index, destination->n);
    require_x("(source_index + count) in range", source_index + count <= source->n, 
            "source_index + count == %d, source->n == %d", source_index + count, source->n);
    require_x("(destination_index + count) in range", destination_index + count <= destination->n, 
            "destination_index + count == %d, destination->n == %d", destination_index + count, destination->n);
    Byte *src = (Byte*)source->a + source_index * source->s;
    Byte *dst = (Byte*)destination->a + destination_index * destination->s;
    memcpy(dst, src, count * source->s);
}

///////////////////////////////////////////////////////////////////////////////

void a_free(Array array) {
    if (array != NULL) {
        array->n = 0;
        array->s = 0;
        if (array->a != NULL) {
            free(array->a);
            array->a = NULL;
        }
        free(array);
    }
}

Any a_get(Array array, int index) {
    require_not_null(array);
    require_x("index in range", index >= 0 && index < array->n, "index == %d, length == %d", index, array->n);
    return (Byte*)array->a + index * array->s;
}

void a_set(Array array, int index, Any value) {
    require_not_null(array);
    require_x("index in range", index >= 0 && index < array->n, "index == %d, length == %d", index, array->n);
    memcpy((Byte*)array->a + index * array->s, value, array->s);
}

int a_length(Array array) {
    require_not_null(array);
    return array->n;
}

int a_element_size(Array array) {
    require_not_null(array);
    return array->s;
}

void a_print(Array array, AnyFn print_element) {
    require_not_null(array);
    require_not_null(print_element);
    AnyToVoid f = print_element;
    printf("[");
    if (array->n > 0) {
        f(array->a);
    }
    for (int i = 1; i < array->n; i++) {
        printf(", ");
        f((Byte*)array->a + i * array->s);
    }
    printf("]");
}

void a_println(Array array, AnyFn print_element) {
    require_not_null(array);
    require_not_null(print_element);
    a_print(array, print_element);
    printf("\n");
}

bool a_equals(Array a, Array b) {
    require_not_null(a);
    require_not_null(b);
    if (a->n != b->n || a->s != b->s) return false;
    return memcmp(a->a, b->a, a->n * a->s) == 0;
}

///////////////////////////////////////////////////////////////////////////////

static void a_concat_test(void) {
    printsln((String)__func__);
    Array a1, a2, ac, ex;
    IntPair ip;

    a1 = a_create(3, sizeof(IntPair));
    ip = make_int_pair(1, 10);
    a_set(a1, 0, &ip);
    ip = make_int_pair(2, 20);
    a_set(a1, 1, &ip);
    ip = make_int_pair(3, 30);
    a_set(a1, 2, &ip);

    // would violate precondition:
    // a_set(NULL, 0, &ip);
    // would violate precondition:
    // a_set(a1, -1, &ip);
    // would violate precondition:
    // a_set(a1, 3, &ip);
    // would violate precondition:
    // a_get(NULL, 0);
    // would violate precondition:
    // a_get(a1, -1);
    // would violate precondition:
    // a_get(a1, 3);
    
    a2 = a_create(2, sizeof(IntPair));
    ip = make_int_pair(4, 40);
    a_set(a2, 0, &ip);
    ip = make_int_pair(5, 50);
    a_set(a2, 1, &ip);
    
    ac = a_concat(a1, a2);
    
    ex = a_create(5, sizeof(IntPair));
    ip = make_int_pair(1, 10);
    a_set(ex, 0, &ip);
    ip = make_int_pair(2, 20);
    a_set(ex, 1, &ip);
    ip = make_int_pair(3, 30);
    a_set(ex, 2, &ip);
    ip = make_int_pair(4, 40);
    a_set(ex, 3, &ip);
    ip = make_int_pair(5, 50);
    a_set(ex, 4, &ip);
    
    a_test_equal(ac, ex);
    
    a_free(a1);
    a_free(a2);
    a_free(ac);
    a_free(ex);
    
    a1 = ia_of_string("10, 20, 30");
    a2 = ia_of_string("1, 2, 3");
    ac = a_concat(a1, a2);
    ex = ia_of_string("10, 20, 30, 1, 2, 3");
    ia_test_equal(ac, ex);
    a_free(a1);
    a_free(a2);
    a_free(ac);
    a_free(ex);

    a1 = ia_range(-9, 0);
    a2 = ia_range(0, 10);
    ac = a_concat(a1, a2);
    ex = ia_range(-9, 10);
    ia_test_equal(ac, ex);
    a_free(a1);
    a_free(a2);
    a_free(ac);
    a_free(ex);

    a1 = ia_range(-9, 0);
    a2 = ia_range(0, 0);
    ac = a_concat(a1, a2);
    ex = ia_range(-9, 0);
    ia_test_equal(ac, ex);
    a_free(a1);
    a_free(a2);
    a_free(ac);
    a_free(ex);

    a1 = ia_range(0, 0);
    a2 = ia_range(0, 10);
    ac = a_concat(a1, a2);
    ex = ia_range(0, 10);
    ia_test_equal(ac, ex);
    a_free(a1);
    a_free(a2);
    a_free(ac);
    a_free(ex);

    a1 = ia_range(0, 0);
    a2 = ia_range(0, 0);
    ac = a_concat(a1, a2);
    ex = ia_range(0, 0);
    ia_test_equal(ac, ex);
    a_free(a1);
    a_free(a2);
    a_free(ac);
    a_free(ex);
    
    a1 = da_of_string("10, 20, 30");
    a2 = da_of_string("1, 2, 3");
    ac = a_concat(a1, a2);
    ex = da_of_string("10, 20, 30, 1, 2, 3");
    da_test_within(ac, ex);
    a_free(a1);
    a_free(a2);
    a_free(ac);
    a_free(ex);

    a1 = da_range(-9, 0, 1);
    a2 = da_range(0, 10, 1);
    ac = a_concat(a1, a2);
    ex = da_range(-9, 10, 1);
    da_test_within(ac, ex);
    a_free(a1);
    a_free(a2);
    a_free(ac);
    a_free(ex);

    a1 = da_range(-9, 0, 1);
    a2 = da_range(0, 0, 1);
    ac = a_concat(a1, a2);
    ex = da_range(-9, 0, 1);
    da_test_within(ac, ex);
    a_free(a1);
    a_free(a2);
    a_free(ac);
    a_free(ex);

    a1 = da_range(0, 0, 1);
    a2 = da_range(0, 10, 1);
    ac = a_concat(a1, a2);
    ex = da_range(0, 10, 1);
    da_test_within(ac, ex);
    a_free(a1);
    a_free(a2);
    a_free(ac);
    a_free(ex);

    a1 = da_range(0, 0, 1);
    a2 = da_range(0, 0, 1);

    // would violate precondition:
    // a1->s=2;
    
    ac = a_concat(a1, a2);
    ex = da_range(0, 0, 1);
    da_test_within(ac, ex);
    a_free(a1);
    a_free(a2);
    a_free(ac);
    a_free(ex);
}

Array a_concat(Array x, Array y) {
    require_not_null(x);
    require_not_null(y);
    require_x("equal element sizes", x->s == y->s, "x->s == %d, y->s == %d", x->s, y->s);
    int n = x->n + y->n;
    Byte *a = xmalloc(n * x->s);
    memcpy(a,               x->a, x->n * x->s);
    memcpy(a + x->n * x->s, y->a, y->n * y->s);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = x->s;
    result->a = a;
    return result;
}



///////////////////////////////////////////////////////////////////////////////



static bool j_is_gt(IntPair *element, int index, int *state) {
    return element->j > *state;
}

static void a_index_fn_test(void) {
    printsln((String)__func__);
    Array ac;
    IntPair ip;

    ac = a_create(3, sizeof(IntPair));
    ip = make_int_pair(1, 10);
    a_set(ac, 0, &ip);
    ip = make_int_pair(2, 20);
    a_set(ac, 1, &ip);
    ip = make_int_pair(3, 30);
    a_set(ac, 2, &ip);
    // a_println(ac, print_int_pair);
    
    int state = 15;
    test_equal_i(a_index_fn(ac, j_is_gt, &state), 1);

    a_free(ac);
}

int a_index_fn(Array array, AnyFn predicate, Any state) {
    require_not_null(array);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    for (int i = 0; i < array->n; i++) {
        if (f((Byte*)array->a + i * array->s, i, state)) {
            return i;
        }
    }
    return -1;
}

static void a_last_index_fn_test(void) {
    printsln((String)__func__);
    Array ac;
    IntPair ip;

    ac = a_create(3, sizeof(IntPair));
    ip = make_int_pair(1, 10);
    a_set(ac, 0, &ip);
    ip = make_int_pair(2, 20);
    a_set(ac, 1, &ip);
    ip = make_int_pair(3, 30);
    a_set(ac, 2, &ip);
    // a_println(ac, print_int_pair);
    
    int state = 15;
    test_equal_i(a_last_index_fn(ac, j_is_gt, &state), 2);

    a_free(ac);
}

int a_last_index_fn(Array array, AnyFn predicate, Any state) {
    require_not_null(array);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    for (int i = array->n - 1; i >= 0; i--) {
        if (f((Byte*)array->a + i * array->s, i, state)) {
            return i;
        }
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////

static void a_reverse_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = ia_of_string("1, 2, 3");
    ex = ia_of_string("3, 2, 1");
    a_reverse(ac);
    a_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("1, 2, 3, 4");
    ex = ia_of_string("4, 3, 2, 1");
    a_reverse(ac);
    a_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("1");
    ex = ia_of_string("1");
    a_reverse(ac);
    a_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = a_create(0, 1);
    ex = a_create(0, 1);
    a_reverse(ac);
    a_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
}

void a_reverse(Array array) {
    require_not_null(array);
    if (array->n <= 1) return;
    Byte *tmp = xmalloc(array->s);
    for (int i = 0, j = array->n - 1; i < j; i++, j--) {
        memcpy(tmp, (Byte*)array->a + i * array->s, array->s);
        memcpy((Byte*)array->a + i * array->s, (Byte*)array->a + j * array->s, array->s);
        memcpy((Byte*)array->a + j * array->s, tmp, array->s);
    }
    free(tmp);
}

static void a_shuffle_test(void) {
    printsln((String)__func__);
    Array a = ia_of_string("1, 2, 3, 4, 5, 6, 7, 8, 9, 10");
    ia_println(a);
    a_shuffle(a);
    ia_println(a);
    a_free(a);
}

void a_shuffle(Array array) {
    require_not_null(array);
    Byte *tmp = xmalloc(array->s);
    for (int i = array->n - 1; i > 0; i--) {
        int r = i_rnd(i + 1); // random number between [0,i]
        // swap indices i and r
        memcpy(tmp, (Byte*)array->a + r * array->s, array->s);
        memcpy((Byte*)array->a + r * array->s, (Byte*)array->a + i * array->s, array->s);
        memcpy((Byte*)array->a + i * array->s, tmp, array->s);
    }
    free(tmp);
}

static CmpResult a_compare_i(ConstAny a, ConstAny b) {
    require_not_null(a);
    require_not_null(b);
    int x = *(int*)a;
    int y = *(int*)b;
    return (x < y) ? LT : ((x > y) ? GT : EQ);
}

static void a_sort_test(void) {
    printsln((String)__func__);
    Array ac, ex;

    ac = ia_of_string("1, 2, 3, 4, 5");
    ex = ia_of_string("1, 2, 3, 4, 5");
    a_shuffle(ac);
    a_sort(ac, a_compare_i);
    a_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("1, 2, 1, 3, 2");
    ex = ia_of_string("1, 1, 2, 2, 3");
    a_shuffle(ac);
    a_sort(ac, a_compare_i);
    a_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("");
    ex = ia_of_string("");
    a_shuffle(ac);
    a_sort(ac, a_compare_i);
    a_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);

    ac = ia_of_string("-1, -2, -3, -1");
    ex = ia_of_string("-3, -2, -1, -1");
    a_shuffle(ac);
    a_sort(ac, a_compare_i);
    a_test_equal(ac, ex);
    a_free(ac);
    a_free(ex);
}

void a_sort(Array array, Comparator c) {
    require_not_null(array);
    require_not_null(c);
    qsort(array->a, array->n, array->s, c);
}



///////////////////////////////////////////////////////////////////////////////


static void a_inc(IntPair *element, int index, Any state, IntPair *mapped_element) {
    mapped_element->i = element->i + 1;
    mapped_element->j = element->j + 1;
}

static void a_int_pair_i(IntPair *element, int index, Any state, int *mapped_element) {
    *mapped_element = element->i;
}

static void a_map_test(void) {
    printsln((String)__func__);
    Array ac, acm, ex;
    IntPair ip;

    ac = a_create(3, sizeof(IntPair));
    ip = make_int_pair(1, 10);
    a_set(ac, 0, &ip);
    ip = make_int_pair(2, 20);
    a_set(ac, 1, &ip);
    ip = make_int_pair(3, 30);
    a_set(ac, 2, &ip);
    // a_println(ac, print_int_pair);
    
    acm = a_map(ac, a_inc, sizeof(IntPair), NULL);
    // a_println(acm, print_int_pair);

    ex = a_create(3, sizeof(IntPair));
    ip = make_int_pair(2, 11);
    a_set(ex, 0, &ip);
    ip = make_int_pair(3, 21);
    a_set(ex, 1, &ip);
    ip = make_int_pair(4, 31);
    a_set(ex, 2, &ip);
    
    a_test_equal(acm, ex);
    a_free(acm);
    a_free(ex);

    ex = a_create(3, sizeof(int));
    int i = 1;
    a_set(ex, 0, &i);
    i = 2;
    a_set(ex, 1, &i);
    i = 3;
    a_set(ex, 2, &i);

    acm = a_map(ac, a_int_pair_i, sizeof(int), NULL);

    a_test_equal(acm, ex);
    
    ia_println(acm);

    a_free(ex);
    a_free(acm);
    a_free(ac);
}

Array a_map(Array array, AnyFn f, int mapped_element_size, Any state) {
    require_not_null(array);
    require_not_null(f);
    require("positive size", mapped_element_size > 0);
    AnyIntAnyAnyToVoid ff = f;
    Byte *a = xcalloc(array->n, mapped_element_size);
    for (int i = 0; i < array->n; i++) {
        ff((Byte*)array->a + i * array->s, i, state, a + i * mapped_element_size);
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = array->n;
    result->s = mapped_element_size;
    result->a = a;
    return result;
    
    return result;
}

#if 0
Array a_map2(Array a1, Array a2, AnyFn f, int mapped_element_size, Any state);

static void pair_i_j(int *element1, int *element2, int index, Any state, IntPair *mapped_element) {
    mapped_element->i = *element1;
    mapped_element->j = *element2;
}

static void a_map2_test(void) {
    printsln((String)__func__);
    Array a1, a2, ac, ex;
    IntPair ip;
    
    a1 = ia_of_string("1, 2, 3");
    a2 = ia_of_string("10, 20, 30");
    
    ac = a_map2(a1, a2, pair_i_j, sizeof(IntPair), NULL);
    a_free(a1);
    a_free(a2);

    ex = a_create(3, sizeof(IntPair));
    ip = make_int_pair(1, 10);
    a_set(ex, 0, &ip);
    ip = make_int_pair(2, 20);
    a_set(ex, 1, &ip);
    ip = make_int_pair(3, 30);
    a_set(ex, 2, &ip);

    a_test_equal(ac, ex);
    
    a_free(ac);
    a_free(ex);
}

Array a_map2(Array a1, Array a2, AnyFn f, int mapped_element_size, Any state) {
    require_not_null(a1);
    require_not_null(a2);
    require_not_null(f);
    AnyAnyIntAnyAnyToVoid ff = f;
    int n = (a1->n < a2->n) ? a1->n : a2->n;
    Byte *a = xcalloc(n, mapped_element_size);
    for (int i = 0; i < n; i++) {
        ff((Byte*)a1->a + i * a1->s, 
          (Byte*)a2->a + i * a2->s, 
          i, state, a + i * mapped_element_size);
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = mapped_element_size;
    result->a = a;
    return result;
}

Array a_map3(Array a1, Array a2, Array a3, AnyFn f, int mapped_element_size, Any state);

static void triple_i_j_k(int *element1, int *element2, int *element3, int index, Any state, IntTriple *mapped_element) {
    mapped_element->i = *element1;
    mapped_element->j = *element2;
    mapped_element->k = *element3;
}

static void triple_k(IntTriple *element, int index, Any state, int *mapped_element) {
    *mapped_element = element->k;
}

static void a_map3_test(void) {
    printsln((String)__func__);
    Array a1, a2, a3, ac, ex;
    IntTriple it;
    
    a1 = ia_of_string("1, 2, 3");
    a2 = ia_of_string("10, 20, 30");
    a3 = ia_of_string("100, 200, 300");
    
    ac = a_map3(a1, a2, a3, triple_i_j_k, sizeof(IntTriple), NULL);
    a_free(a1);
    a_free(a2);
    a_free(a3);

    ex = a_create(3, sizeof(IntTriple));
    it = make_int_triple(1, 10, 100);
    a_set(ex, 0, &it);
    it = make_int_triple(2, 20, 200);
    a_set(ex, 1, &it);
    it = make_int_triple(3, 30, 300);
    a_set(ex, 2, &it);

    a_test_equal(ac, ex);

    // map triple to third element
    Array ak = a_map(ac, triple_k, sizeof(int), NULL);
    ia_println(ak);
    a_free(ak);
    
    a_free(ac);
    a_free(ex);
}

// @todo: add tests

Array a_map3(Array a1, Array a2, Array a3, AnyFn f, int mapped_element_size, Any state) {
    require_not_null(a1);
    require_not_null(a2);
    require_not_null(a3);
    require_not_null(f);
    AnyAnyAnyIntAnyAnyToVoid ff = f;
    int n = (a1->n < a2->n && a1->n < a3->n) ? a1->n : ((a2->n < a1->n && a2->n < a3->n) ? a2->n : a3->n);
    Byte *a = xcalloc(n, mapped_element_size);
    for (int i = 0; i < n; i++) {
        ff((Byte*)a1->a + i * a1->s, 
          (Byte*)a2->a + i * a2->s, 
          (Byte*)a3->a + i * a3->s, 
          i, state, a + i * mapped_element_size);
    }
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = mapped_element_size;
    result->a = a;
    return result;
}
#endif

static void a_inc_in_place(IntPair *element, int index, Any state) {
    element->i += 1;
    element->j += 1;
}

static void a_each_test(void) {
    printsln((String)__func__);
    Array ac, ex;
    IntPair ip;

    ac = a_create(3, sizeof(IntPair));
    ip = make_int_pair(1, 10);
    a_set(ac, 0, &ip);
    ip = make_int_pair(2, 20);
    a_set(ac, 1, &ip);
    ip = make_int_pair(3, 30);
    a_set(ac, 2, &ip);
    // a_println(ac, print_int_pair);
    
    a_each(ac, a_inc_in_place, NULL);
    // a_println(acm, print_int_pair);

    ex = a_create(3, sizeof(IntPair));
    ip = make_int_pair(2, 11);
    a_set(ex, 0, &ip);
    ip = make_int_pair(3, 21);
    a_set(ex, 1, &ip);
    ip = make_int_pair(4, 31);
    a_set(ex, 2, &ip);
    
    a_test_equal(ac, ex);
    
    a_free(ac);
    a_free(ex);
}

void a_each(Array array, AnyFn f, Any state) {
    require_not_null(array);
    require_not_null(f);
    AnyIntAnyToVoid ff = f;
    for (int i = 0; i < array->n; i++) {
        ff((Byte*)array->a + i * array->s, i, state);
    }
}

#if 0
// @todo: add tests

void a_each2(Array a1, Array a2, AnyFn f, Any state) {
    require_not_null(a1);
    require_not_null(a2);
    require_not_null(f);
    AnyAnyIntAnyToVoid ff = f;
    int n = (a1->n < a2->n) ? a1->n : a2->n;
    for (int i = 0; i < n; i++) {
        ff((Byte*)a1->a + i * a1->s, (Byte*)a2->a + i * a2->s, i, state);
    }
}

// @todo: add tests

void a_each3(Array a1, Array a2, Array a3, AnyFn f, Any state) {
    require_not_null(a1);
    require_not_null(a2);
    require_not_null(a3);
    require_not_null(f);
    AnyAnyAnyIntAnyToVoid ff = f;
    int n = (a1->n < a2->n && a1->n < a3->n) ? a1->n : ((a2->n < a1->n && a2->n < a3->n) ? a2->n : a3->n);
    for (int i = 0; i < n; i++) {
        ff((Byte*)a1->a + i * a1->s, (Byte*)a2->a + i * a2->s, (Byte*)a3->a + i * a3->s, i, state);
    }
}
#endif

static void a_sum_j(int *state, IntPair *element, int index) {
    *state += element->j;
}

static void a_foldl_test(void) {
    printsln((String)__func__);
    Array ac;
    IntPair ip;

    ac = a_create(3, sizeof(IntPair));
    ip = make_int_pair(1, 10);
    a_set(ac, 0, &ip);
    ip = make_int_pair(2, 20);
    a_set(ac, 1, &ip);
    ip = make_int_pair(3, 30);
    a_set(ac, 2, &ip);
    
    int n = 0;
    a_foldl(ac, a_sum_j, &n);
    
    test_equal_i(n, 10 + 20 + 30);
    
    a_free(ac);
}

void a_foldl(Array array, AnyFn f, Any state) {
    require_not_null(array);
    require_not_null(f);
    AnyAnyIntToVoid ff = f;
    for (int i = 0; i < array->n; i++) {
        ff(state, (Byte*)array->a + i * array->s, i);
    }
}

#if 0
// @todo: add tests

void a_foldl2(Array a1, Array a2, AnyFn f, Any state) {
    require_not_null(a1);
    require_not_null(a2);
    require_not_null(f);
    AnyAnyAnyIntToVoid ff = f;
    int n = (a1->n < a2->n) ? a1->n : a2->n;
    for (int i = 0; i < n; i++) {
        ff(state, (Byte*)a1->a + i * a1->s, (Byte*)a2->a + i * a2->s, i);
    }
}

// @todo: add tests

void a_foldl3(Array a1, Array a2, Array a3, AnyFn f, Any state) {
    require_not_null(a1);
    require_not_null(a2);
    require_not_null(a3);
    require_not_null(f);
    AnyAnyAnyAnyIntToVoid ff = f;
    int n = (a1->n < a2->n && a1->n < a3->n) ? a1->n : ((a2->n < a1->n && a2->n < a3->n) ? a2->n : a3->n);
    for (int i = 0; i < n; i++) {
        ff(state, (Byte*)a1->a + i * a1->s, (Byte*)a2->a + i * a2->s, (Byte*)a3->a + i * a3->s, i);
    }
}
#endif

static void a_minus_j(IntPair *element, int *state, int index) {
    *state = element->j - *state;
}

static void a_foldr_test(void) {
    printsln((String)__func__);
    Array ac;
    IntPair ip;

    ac = a_create(3, sizeof(IntPair));
    ip = make_int_pair(1, 10);
    a_set(ac, 0, &ip);
    ip = make_int_pair(2, 20);
    a_set(ac, 1, &ip);
    ip = make_int_pair(3, 30);
    a_set(ac, 2, &ip);
    
    int n = 0;
    a_foldr(ac, a_minus_j, &n);
    
    test_equal_i(n, 10 - (20 - (30 - 0)));
    
    a_free(ac);
}

void a_foldr(Array array, AnyFn f, Any state) {
    require_not_null(array);
    require_not_null(f);
    AnyAnyIntToVoid ff = f;
    for (int i = array->n - 1; i >= 0; i--) {
        ff((Byte*)array->a + i * array->s, state, i);
    }
}

static bool a_eq2(IntPair *element, int index, Any state) {
    return element->i == 2;
}

static void a_filter_test(void) {
    printsln((String)__func__);
    Array ac, ac2, ex;
    IntPair ip;

    ac = a_create(3, sizeof(IntPair));
    ip = make_int_pair(1, 10);
    a_set(ac, 0, &ip);
    ip = make_int_pair(2, 20);
    a_set(ac, 1, &ip);
    ip = make_int_pair(3, 30);
    a_set(ac, 2, &ip);
    
    ac2 = a_filter(ac, a_eq2, NULL);
    
    ex = a_create(1, sizeof(IntPair));
    ip = make_int_pair(2, 20);
    a_set(ex, 0, &ip);
    
    a_test_equal(ac2, ex);
    
    a_free(ac);
    a_free(ac2);
    a_free(ex);
}

Array a_filter(Array array, AnyFn predicate, Any state) {
    require_not_null(array);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    bool *ps = xmalloc(array->n * sizeof(bool));
    int n = 0;
    for (int i = 0; i < array->n; i++) {
        ps[i] = f((Byte*)array->a + i * array->s, i, state);
        if (ps[i]) n++;
    }
    Byte *a = xcalloc(n, array->s);
    for (int i = 0, j = 0; i < array->n; i++) {
        if (ps[i]) {
            memcpy(a + j * array->s, (Byte*)array->a + i * array->s, array->s);
            j++;
        }
    }
    free(ps);
    Array result = xmalloc(sizeof(ArrayHead));
    result->n = n;
    result->s = array->s;
    result->a = a;
    return result;
}

static void a_exists_test(void) {
    printsln((String)__func__);
    Array ac;
    IntPair ip;

    ac = a_create(3, sizeof(IntPair));
    ip = make_int_pair(1, 10);
    a_set(ac, 0, &ip);
    ip = make_int_pair(2, 20);
    a_set(ac, 1, &ip);
    ip = make_int_pair(3, 30);
    a_set(ac, 2, &ip);
    // a_println(ac, print_int_pair);
    
    int state = 29;
    test_equal_b(a_exists(ac, j_is_gt, &state), true);
    state = 30;
    test_equal_b(a_exists(ac, j_is_gt, &state), false);

    a_free(ac);
}

bool a_exists(Array array, AnyFn predicate, Any state) {
    require_not_null(array);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    for (int i = 0; i < array->n; i++) {
        if (f((Byte*)array->a + i * array->s, i, state)) {
            return true;
        }
    }
    return false;
}

static void a_forall_test(void) {
    printsln((String)__func__);
    Array ac;
    IntPair ip;

    ac = a_create(3, sizeof(IntPair));
    ip = make_int_pair(1, 10);
    a_set(ac, 0, &ip);
    ip = make_int_pair(2, 20);
    a_set(ac, 1, &ip);
    ip = make_int_pair(3, 30);
    a_set(ac, 2, &ip);
    // a_println(ac, print_int_pair);
    
    int state = 9;
    test_equal_b(a_forall(ac, j_is_gt, &state), true);
    state = 10;
    test_equal_b(a_forall(ac, j_is_gt, &state), false);

    a_free(ac);
}

bool a_forall(Array array, AnyFn predicate, Any state) {
    require_not_null(array);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    for (int i = 0; i < array->n; i++) {
        if (!f((Byte*)array->a + i * array->s, i, state)) {
            return false;
        }
    }
    return true;
}



///////////////////////////////////////////////////////////////////////////////

bool a_test_equal_file_line(const char *file, const char *function, int line, Array a, Array e) {
    base_init();
    base_count_check();
    if (a->n != e->n) {
        printf("%s, line %d: Actual length %d "
            "differs from expected length %d\n", file, line, a->n, e->n);
        return false;
    }
    if (a->s != e->s) {
        printf("%s, line %d: Actual element size %d "
            "differs from expected element size %d\n", file, line, a->s, e->s);
        return false;
    }
    if (a->n < 0) {
        printf("%s, line %d: Invalid actual length %d\n", file, line, a->n);
        return false;        
    }
    if (e->n < 0) {
        printf("%s, line %d: Invalid expected length %d\n", file, line, e->n);
        return false;        
    }
    if (a->n > 0 && a->a == NULL) {
        printf("%s, line %d: Actual value array is NULL\n", file, line);
        return false;        
    }
    if (e->n > 0 && e->a == NULL) {
        printf("%s, line %d: Expected value array is NULL\n", file, line);
        return false;        
    }
    char *ap = a->a;
    char *ep = e->a;
    for (int i = 0; i < a->n; i++) {
        if (memcmp(ap + i * e->s, ep + i * e->s, e->s) != 0) {
            printf("%s, line %d: Actual value differs from expected value at index %d.\n", file, line, i);
            return false;
        }
    }
    printf("%s, line %d: Test passed.\n", file, line);
    base_count_success();
    return true;
}

void a_test_all(void) {
    a_create_test();
    a_of_buffer_test();
    a_fn_test();
    a_copy_test();
    a_sub_test();
    a_blit_test();
    a_concat_test();
    a_index_fn_test();
    a_last_index_fn_test();
    a_reverse_test();
    a_shuffle_test();
    a_sort_test();
    a_map_test();
//    a_map2_test();
//    a_map3_test();
    a_each_test();
    a_foldl_test();
    a_foldr_test();
    a_filter_test();
    a_exists_test();
    a_forall_test();
}

#if 0
int main(void) {
    base_init();
    a_test_all();
    return 0;
}
#endif
