/*
@author Michael Rohs
@date 15.10.2015
@copyright Apache License, Version 2.0
*/

#include "list.h"
#include "double_list.h"
#include "int_list.h"
#include "array.h"

#define VALUE(node) (*(int*)(((ListNode*)(node)) + 1))

List il_create() {
    ListHead *lh = xcalloc(1, sizeof(ListHead));
    lh->s = sizeof(int); // content size
    return lh;
}

List il_of_buffer(Any buffer, int n) {
    return l_of_buffer(buffer, n, sizeof(int));
}

List il_repeat(int n, int init);

static void il_repeat_test(void) {
    printsln((String)__func__);
    List list;
    
    list = il_repeat(3, 0);
    test_equal_i(il_get(list, 0), 0);
    test_equal_i(il_get(list, 1), 0);
    test_equal_i(il_get(list, 2), 0);
    l_free(list);
    
    list = il_repeat(2, -1);
    test_equal_i(il_get(list, 0), -1);
    test_equal_i(il_get(list, 1), -1);
    l_free(list);

    list = il_repeat(0, 2);
    test_equal_i(l_length(list), 0);
    l_free(list);
}

List il_repeat(int n, int value) {
    if (n < 0) {
        printf("%s: length cannot be negative (is %d)\n", __func__, n);
        exit(EXIT_FAILURE);
    }
    ListHead *lh = xcalloc(1, sizeof(ListHead));
    lh->s = sizeof(value); // content size
    for (int i = 0; i < n; i++) {
        il_append(lh, value);
    }
    return lh;
}

List il_range(int a, int b);

static void il_range_test(void) {
    printsln((String)__func__);
    List list;

    list = il_range(1, -2);
    il_println(list);
    l_free(list);

    list = il_range(1, -1);
    il_println(list);
    l_free(list);

    list = il_range(1, 0);
    il_println(list);
    l_free(list);

    list = il_range(1, 1);
    il_println(list);
    l_free(list);

    list = il_range(1, 2);
    il_println(list);
    l_free(list);

    list = il_range(1, 3);
    il_println(list);
    l_free(list);
}

List il_range(int a, int b) {
    ListHead *list = xcalloc(1, sizeof(ListHead));
    list->s = sizeof(int); // content size
    if (a <= b) {
        int n = b - a;
        for (int i = 0; i < n; i++) {
            il_append(list, a + i);
        }
    } else /* a > b */ {
        int n = a - b;
        for (int i = 0; i < n; i++) {
            il_append(list, a - i);
        }
    }
    return list;
}

List il_of_string(String s);

static void il_of_string_test(void) {
    printsln((String)__func__);
    List ac, ex;

    ac = il_of_string("1,2, 3, 4, 5, 6");
    ex = il_range(1, 7);
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_of_string("   1, 2, 3, 4, 5, 6   ");
    ex = il_range(1, 7);
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_of_string("1xx2asdfs3");
    ex = il_range(1, 4);
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_of_string("y1xx2asdfs3x");
    ex = il_range(1, 4);
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_of_string("-3, -2, -1, 0, 1");
    ex = il_range(-3, 2);
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_of_string(" -3 -2 -1 ");
    ex = il_range(-3, 0);
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_of_string(".1.-2.-.3.");
    ex = il_repeat(3, 0);
    il_set(ex, 0, 1);
    il_set(ex, 1, -2);
    il_set(ex, 2, 3);
    il_test_equal(ac, ex);
    l_free(ac);

    ac = il_of_string("-.1.-2.-.3.-");
    il_test_equal(ac, ex);
    l_free(ac);

    ac = il_of_string("---.1.----2.----.3.----");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_of_string("-x--+ +.+++a-b c-");
    ex = il_repeat(0, 0);
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_of_string("");
    ex = il_repeat(0, 0);
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
}

List il_of_string(String s) {
    assert_argument_not_null(s);
    ListHead *list = xcalloc(1, sizeof(ListHead));
    list->s = sizeof(int); // content size

    char *t = s;
    while (*t != '\0') {
        if (isdigit(*t)) {
            if ((t > s) && (*(t - 1) == '-')) t--; // check for minus sign, no whitespace between '-' and digit
            il_append(list, atoi(t)); // convert digit string to int
            t++; // assert: *t is a digit or '-', skip
            while (isdigit(*t)) t++; // skip int
            // assert: *t is not a digit, *t may be '\0'
        } else {
            // assert: *t is not a digit, *t is not '\0'
            t++; // not a digit, skip
        }
    }

    return list;
}

List il_fn(int n, IntIntToInt init, int x);

static int two_i_plus_1(int index, int x) {
    return 2 * index + 1;
}

static void il_fn_test(void) {
    printsln((String)__func__);
    List ac, ex;

    ac = il_fn(3, two_i_plus_1, 0);
    ex = il_of_string("1, 3, 5");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_fn(4, two_i_plus_1, 0);
    ex = il_of_string("1, 3, 5, 7");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_fn(1, two_i_plus_1, 0);
    ex = il_of_string("1");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_fn(0, two_i_plus_1, 0);
    ex = il_create();
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
}

List il_fn(int n, IntIntToInt init, int x) {
    assert_function_not_null(init);
    if (n < 0) {
        printf("il_fn: length cannot be negative (is %d)\n", n);
        exit(EXIT_FAILURE);
    }
    List result = il_create();
    for (int i = 0; i < n; i++) {
        il_append(result, init(i, x));
    }
    return result;
}

List il_of_dl(List list);

static void il_of_dl_test(void) {
    printsln((String)__func__);
    List dl = dl_of_string("0.0 0.49999 0.5 1.0 -0.49999 -0.50001 -1.0");
    List ex = il_of_string(" 0   0       1   1    0       -1       -1 ");
    List ac = il_of_dl(dl);
    il_test_equal(ac, ex);
    l_free(dl);
    l_free(ex);
    l_free(ac);
}

List il_of_dl(List list) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    List result = il_create();
    for (DoubleListNode *node = list->first; node != NULL; node = node->next) {
        il_append(result, round(node->value));
    }
    return result;
}

int il_get(List list, int index) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (i == index) {
            return node->value;
        }
    }
    printf("%s: index %d is out of range "
        "(current list length: %d, allowed indices: 0..%d)\n", 
        __func__, index, i, i - 1);
    exit(EXIT_FAILURE);
    return 0;
}

void il_set(List list, int index, int value) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (i == index) {
            node->value = value;
            return;
        }
    }
    printf("%s: index %d is out of range "
        "(current list length: %d, allowed indices: 0..%d)\n", 
        __func__, index, i, i - 1);
    exit(EXIT_FAILURE);
}

void il_inc(List list, int index, int value) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (i == index) {
            node->value += value;
            return;
        }
    }
    printf("%s: index %d is out of range "
        "(current list length: %d, allowed indices: 0..%d)\n", 
        __func__, index, i, i - 1);
    exit(EXIT_FAILURE);
}

///////////////////////////////////////////////////////////////////////////////

static void il_iterator_test(void) {
    printsln((String)__func__);
    List ac;

    ac = il_of_string("1, 2, 3, 4, 5");
    
    // various ways of iterating a list:
    
    ListIterator iter = l_iterator(ac);
    while (l_has_next(iter)) {
        int i = il_next(&iter);
        printiln(i);
    }
    
    // IntListNode *first = (IntListNode*)ac->first;
    for (IntListNode *node = ac->first; node != NULL; node = node->next) {
        int i = node->value;
        printiln(i);
    }
    
#if 0
    while (iter = il_next(iter)) {
        int i = il_current(iter);
        printiln(i);
    }
        
    for (IntListIterator iter = il_iterator(ac); il_has_current(iter); iter = il_next(iter)) {
        int i = il_current(iter);
        printiln(i);
    }
#endif
    
    l_free(ac);
}

int il_next(ListIterator *iter) {
    if (*iter == NULL) {
        printf("%s: iterator does not have a next value\n", __func__);
        exit(EXIT_FAILURE);
    }
    int value = VALUE(*iter);
    *iter = (*iter)->next;
    return value;
}

///////////////////////////////////////////////////////////////////////////////

static void il_prepend_append_test(void) {
    printsln((String)__func__);
    List ac, ex;
    ac = il_create();
    il_append(ac, 1);
    il_append(ac, 2);
    il_append(ac, 3);
    ex = il_create();
    il_prepend(ex, 3);
    il_prepend(ex, 2);
    il_prepend(ex, 1);
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
}

void il_append(List list, int value) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    // allocate memory for next-pointer and content
    IntListNode *node = xcalloc(1, sizeof(ListNode*) + sizeof(value));
    // copy content, leave next-pointer NULL
    node->value = value;
    // if this is the first element of the list set first pointer
    if (list->first == NULL) {
        list->first = node;
    }
    // if there is a last element, update its next-pointer
    if (list->last != NULL) {
        IntListNode *list_last = list->last;
        list_last->next = node;
    }
    // the new node is the last node of the list
    list->last = node;
}

void il_prepend(List list, int value) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    // allocate memory for next-pointer and content
    IntListNode *node = xcalloc(1, sizeof(ListNode*) + sizeof(value));
    // copy content, leave next-pointer NULL
    node->value = value;
    node->next = list->first;
    list->first = node;
    if (list->last == NULL) {
        list->last = node;
    }
}

void il_print(List list) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    ListNode *node = list->first;
    printf("[");
    if (node != NULL) {
        printf("%d", *(int*)(node + 1));
        node = node->next;
    }
    for (; node != NULL; node = node->next) {
        printf(", %d", *(int*)(node + 1));
    }
    printf("]");
}

void il_println(List list) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    il_print(list);
    printf("\n");
}

bool il_contains(List list, int value);

static void il_contains_test(void) {
    printsln((String)__func__);
    List list;
    list = il_of_string("10, 20, 30");
    test_equal_b(il_contains(list, 10), true);
    test_equal_b(il_contains(list, 11), false);
    test_equal_b(il_contains(list, 30), true);
    l_free(list);
}

bool il_contains(List list, int value) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    for (IntListNode *node = list->first; node != NULL; node = node->next) {
        if (node->value == value) {
            return true;
        }
    }
    return false;
}

void il_fill(List list, int value);

static void il_fill_test(void) {
    printsln((String)__func__);
    List ex, ac;

    ac = il_repeat(3, 0);
    il_fill(ac, -1);
    ex = il_of_string("-1, -1, -1");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_repeat(2, 0);
    il_fill(ac, 2);
    ex = il_of_string("2, 2");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_repeat(0, 0);
    il_fill(ac, 2);
    ex = il_create();
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
}

void il_fill(List list, int value) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    for (IntListNode *node = list->first; node != NULL; node = node->next) {
        node->value = value;
    }
}

void il_fill_from_to(List list, int value, int from, int to);

static void il_fill_from_to_test(void) {
    printsln((String)__func__);
    List ac, ex;

    ac = il_repeat(3, 0);
    il_fill_from_to(ac, -1, 0, 3);
    ex = il_of_string("-1, -1, -1");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_repeat(3, 0);
    il_fill_from_to(ac, -1, -1, 4);
    ex = il_of_string("-1, -1, -1");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_repeat(3, 0);
    il_fill_from_to(ac, -1, 0, 0);
    ex = il_of_string("0, 0, 0");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_repeat(3, 0);
    il_fill_from_to(ac, -1, 0, 1);
    ex = il_of_string("-1, 0, 0");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_repeat(3, 0);
    il_fill_from_to(ac, -1, 2, 2);
    ex = il_of_string("0, 0, 0");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_repeat(3, 0);
    il_fill_from_to(ac, -1, 2, 3);
    ex = il_of_string("0, 0, -1");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_repeat(3, 0);
    il_fill_from_to(ac, -1, 2, 1);
    ex = il_of_string("0, 0, 0");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_repeat(3, 0);
    il_fill_from_to(ac, -1, 1, 3);
    ex = il_of_string("0, -1, -1");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
}

void il_fill_from_to(List list, int value, int from, int to) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    if (from < 0) from = 0;
    int i = 0;
    for (IntListNode *node = list->first; node != NULL && i < to; node = node->next, i++) {
        if (i >= from) {
            node->value = value;
        }
    }
}

int il_index(List list, int value);

static void il_index_test(void) {
    printsln((String)__func__);
    List list;

    list = il_range(1, 3);
    int i = il_index(list, 0);
#if 0
    if (i < 0) {
        printsln("value not found");
    } else {
        printf("value found at index %d\n", i);
    }
#endif
    test_equal_i(i, -1);

    i = il_index(list, 1);
    test_equal_i(i, 0);
        
    i = il_index(list, 2);
    test_equal_i(i, 1);
        
    l_free(list);
    
}

int il_index(List list, int value) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (node->value == value) {
            return i;
        }
    }
    return -1;
}

int il_index_from(List list, int value, int from);

static void il_index_from_test(void) {
    printsln((String)__func__);
    List a = il_of_string("10 20 30 40 50");
    test_equal_i(il_index_from(a, 20, 0), 1);
    test_equal_i(il_index_from(a, 20, 1), 1);
    test_equal_i(il_index_from(a, 20, 2), -1);
    test_equal_i(il_index_from(a, 30, -1), 2);
    test_equal_i(il_index_from(a, 60, 0), -1);
    l_free(a);
    
}

int il_index_from(List list, int value, int from) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    if (from < 0) from = 0;
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (i >= from && node->value == value) {
            return i;
        }
    }
    return -1;
}

int il_index_fn(List list, IntIntIntToBool predicate, int x);

static void il_index_fn_test(void) {
    printsln((String)__func__);
    List a = il_of_string("1 2 3 -4 5");
    test_equal_i(il_index_fn(a, il_gt, 1), 1);
    test_equal_i(il_index_fn(a, il_lt, 0), 3);
    l_free(a);
}

int il_index_fn(List list, IntIntIntToBool predicate, int x) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    assert_function_not_null(predicate);
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (predicate(node->value, i, x)) {
            return i;
        }
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////

static CmpResult int_compare(ConstAny a, ConstAny b) {
    int x = *(int*)a;
    int y = *(int*)b;
    return (x == y) ? 0 : (x < y ? -1 : 1);
}

List il_sort(List list);

static void il_sort_test(void) {
    printsln((String)__func__);
    List ac, ex, as;

    ac = il_of_string("5, 4, 3, 2, 1");
    ex = il_of_string("1, 2, 3, 4, 5");
    as = il_sort(ac);
    il_test_equal(as, ex);
    l_free(ac);
    l_free(ex);
    l_free(as);

    ac = il_of_string("1, 2, 1, 3, 2");
    ex = il_of_string("1, 1, 2, 2, 3");
    as = il_sort(ac);
    il_test_equal(as, ex);
    l_free(ac);
    l_free(ex);
    l_free(as);

    ac = il_of_string("");
    ex = il_of_string("");
    as = il_sort(ac);
    il_test_equal(as, ex);
    l_free(ac);
    l_free(ex);
    l_free(as);

    ac = il_of_string("-1, -2, -3, -1");
    ex = il_of_string("-3, -2, -1, -1");
    as = il_sort(ac);
    il_test_equal(as, ex);
    l_free(ac);
    l_free(ex);
    l_free(as);
}

List il_sort(List list) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    return l_sort(list, int_compare);
}

static CmpResult int_compare_dec(ConstAny a, ConstAny b) {
    int x = *(int*)b;
    int y = *(int*)a;
    return (x == y) ? 0 : (x < y ? -1 : 1);
}

List il_sort_dec(List list);

static void il_sort_dec_test(void) {
    printsln((String)__func__);
    List ac, ex, as;

    ac = il_of_string("1, 2, 3, 4, 5");
    ex = il_of_string("5, 4, 3, 2, 1");
    as = il_sort_dec(ac);
    il_test_equal(as, ex);
    l_free(ac);
    l_free(ex);
    l_free(as);

    ac = il_of_string("1, 2, 1, 3, 2");
    ex = il_of_string("3, 2, 2, 1, 1");
    as = il_sort_dec(ac);
    il_test_equal(as, ex);
    l_free(ac);
    l_free(ex);
    l_free(as);

    ac = il_of_string("");
    ex = il_of_string("");
    as = il_sort_dec(ac);
    il_test_equal(as, ex);
    l_free(ac);
    l_free(ex);
    l_free(as);

    ac = il_of_string("-3, -2, -1");
    ex = il_of_string("-1, -2, -3");
    as = il_sort_dec(ac);
    il_test_equal(as, ex);
    l_free(ac);
    l_free(ex);
    l_free(as);
}

List il_sort_dec(List list) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    return l_sort(list, int_compare_dec);
}

void il_insert(List list, int index, int value);

static void il_insert_test(void) {
    printsln((String)__func__);
    List ac, ex;

    ac = il_of_string("1, 2, 3, 4, 5");
    il_insert(ac, 0, 9);
    ex = il_of_string("9, 1, 2, 3, 4, 5");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = il_of_string("1, 2, 3, 4, 5");
    il_insert(ac, 5, 9);
    ex = il_of_string("1, 2, 3, 4, 5, 9");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = il_of_string("1, 2, 3, 4, 5");
    il_insert(ac, 3, 9);
    ex = il_of_string("1, 2, 3, 9, 4, 5");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = il_of_string("1");
    il_insert(ac, -1, 9);
    ex = il_of_string("1");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = il_of_string("1");
    il_insert(ac, 1, 9);
    ex = il_of_string("1, 9");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = il_of_string("");
    il_insert(ac, 0, 9);
    ex = il_of_string("9");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
}

void il_insert(List list, int index, int value) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    l_insert(list, index, &value);
}

void il_remove(List list, int index);

static void il_remove_test(void) {
    printsln((String)__func__);
    List ac, ex;

    ac = il_of_string("1, 2, 3, 4, 5, 6");
    il_remove(ac, 0);
    ex = il_of_string("2, 3, 4, 5, 6");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = il_of_string("1, 2, 3, 4, 5, 6");
    il_remove(ac, 5);
    ex = il_of_string("1, 2, 3, 4, 5");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = il_of_string("1, 2, 3, 4, 5, 6");
    il_remove(ac, 3);
    ex = il_of_string("1, 2, 3, 5, 6");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = il_of_string("1");
    il_remove(ac, -1);
    ex = il_of_string("1");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = il_of_string("1");
    il_remove(ac, 1);
    ex = il_of_string("1");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = il_of_string("1");
    il_remove(ac, 0);
    ex = il_of_string("");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = il_of_string("");
    il_remove(ac, 0);
    ex = il_of_string("");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
}

void il_remove(List list, int index) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    l_remove(list, index);
}

///////////////////////////////////////////////////////////////////////////////

/* 
 * Predicates
 */

bool il_even(int value, int index) {
    return (value & 1) == 0;
}

bool il_index_even(int value, int index) {
    return (index & 1) == 0;
}

bool il_index_odd(int value, int index) {
    return (index & 1) == 1;
}

bool il_pos(int value, int index) {
    return value > 0;
}

bool il_neg(int value, int index) {
    return value < 0;
}

bool il_ge0(int value, int index) {
    return value >= 0;
}

bool il_gt(int value, int index, int x) {
    return value > x;
}

bool il_ge(int value, int index, int x) {
    return value >= x;
}

bool il_lt(int value, int index, int x) {
    return value < x;
}

bool il_le(int value, int index, int x) {
    return value <= x;
}

///////////////////////////////////////////////////////////////////////////////

/*
 * Functions for initialization, map, and fold
 */
int il_square(int value, int index) {
    return value * value; // index not used
}

int il_rnd(int index, int maximum) {
    return i_rnd(maximum); // index not used
}

int il_times(int value, int index, int x) {
    return value * x;
}

void il_each(List list, IntIntIntToInt f, int x);

static void il_each_test(void) {
    printsln((String)__func__);
    List ac, ex;
    
    ac = il_of_string("1, 2, 3, 4");
    il_each(ac, il_times, 2);
    ex = il_of_string("2, 4, 6, 8");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = il_of_string("");
    il_each(ac, il_times, 3);
    ex = il_of_string("");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
}

void il_each(List list, IntIntIntToInt f, int x) {
    assert_function_not_null(f);
    assert_argument_not_null(list);
    il_assert_element_size(list);
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        node->value = f(node->value, i, x);
    }
}

void il_each_state(List list, IntIntIntAnyToInt f, int x, Any state);

static void il_each_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

void il_each_state(List list, IntIntIntAnyToInt f, int x, Any state) {
    assert_function_not_null(f);
    assert_argument_not_null(list);
    il_assert_element_size(list);
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        node->value = f(node->value, i, x, state);
    }
}

// @todo: add tests
List il_map(List list, IntIntIntToInt f, int x) {
    assert_function_not_null(f);
    assert_argument_not_null(list);
    il_assert_element_size(list);
    List result = il_create();
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        il_append(result, f(node->value, i, x));
    }
    return result;
}

// @todo: add tests
List il_map_state(List list, IntIntIntAnyToInt f, int x, Any state) {
    assert_function_not_null(f);
    assert_argument_not_null(list);
    il_assert_element_size(list);
    List result = il_create();
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        il_append(result, f(node->value, i, x, state));
    }
    return result;
}

int il_foldl(List list, IntIntIntToInt f, int init);

static void il_foldl_test(void) {
    printsln((String)__func__);
    List a = il_of_string("");    
    test_equal_i(il_foldl(a, int_plus, 100), 100);
    test_equal_i(il_foldl(a, int_minus, 100), 100);
    l_free(a);
    
    a = il_of_string("1, 2, 3, 4");    
    test_equal_i(il_foldl(a, int_plus, 0),    (((0 + 1) + 2) + 3) + 4);
    test_equal_i(il_foldl(a, int_minus, 100), (((100 - 1) - 2) - 3) - 4);
    l_free(a);
}

int il_foldl(List list, IntIntIntToInt f, int init) {
    assert_function_not_null(f);
    assert_argument_not_null(list);
    il_assert_element_size(list);
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        init = f(init, node->value, i);
    }
    return init;
}

int il_foldr(List list, IntIntIntToInt f, int init);

static void il_foldr_test(void) {
    printsln((String)__func__);
    List a = il_of_string("");    
    test_equal_i(il_foldr(a, int_plus, 100), 100);
    test_equal_i(il_foldr(a, int_minus, 100), 100);
    l_free(a);
    
    a = il_of_string("1, 2, 3, 4");    
    test_equal_i(il_foldr(a, int_plus, 100), 1 + (2 + (3 + (4 + 100))));
    test_equal_i(il_foldr(a, int_minus, 0), 1 - (2 - (3 - (4 - 0))));
    l_free(a);
}

int il_foldr(List list, IntIntIntToInt f, int init) {
    assert_function_not_null(f);
    assert_argument_not_null(list);
    il_assert_element_size(list);
    List rev = l_reverse(list);
    int i = l_length(list) - 1;
    for (IntListNode *node = rev->first; node != NULL; node = node->next, i--) {
        init = f(node->value, init, i);
    }
    l_free(rev);
    return init;
}

List il_filter(List list, IntIntIntToBool predicate, int x);

static void il_filter_test(void) {
    printsln((String)__func__);
    List a, ac, ex;

    a = il_of_string("1, 2, 3, 4, 5, 6");
    ac = il_filter(a, il_gt, 3);
    ex = il_of_string("4, 5, 6");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_filter(a, il_lt, 4);
    ex = il_of_string("1, 2, 3");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_filter(a, il_le, 3);
    ex = il_of_string("1, 2, 3");
    il_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
    l_free(a);
}

List il_filter(List list, IntIntIntToBool predicate, int x) {
    assert_function_not_null(predicate);
    assert_argument_not_null(list);
    il_assert_element_size(list);
    List result = il_create();
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (predicate(node->value, i, x)) {
            il_append(result, node->value);
        }
    }
    return result;
}

List il_filter_state(List list, IntIntIntAnyToBool predicate, int x, Any state);

static void il_filter_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

List il_filter_state(List list, IntIntIntAnyToBool predicate, int x, Any state) {
    assert_function_not_null(predicate);
    assert_argument_not_null(list);
    il_assert_element_size(list);
    List result = il_create();
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (predicate(node->value, i, x, state)) {
            il_append(result, node->value);
        }
    }
    return result;
}

List il_choose(List list, IntIntIntToIntOption f, int x);

static IntOption evens_times_x(int element, int index, int x) {
    if ((element % 2) == 0) {
        return make_int_some(x * element);
    }
    return make_int_none();
}

static void il_choose_test(void) {
    printsln((String)__func__);
    List a, ac, ex;

    a = il_of_string("1, 2, 3, 4, 5, 6");
    ac = il_choose(a, evens_times_x, 3);
    ex = il_of_string("6, 12, 18");
    il_test_equal(ac, ex);

    l_free(ac);
    l_free(ex);
    l_free(a);
}

List il_choose(List list, IntIntIntToIntOption f, int x) {
    assert_function_not_null(f);
    assert_argument_not_null(list);
    il_assert_element_size(list);
    List result = il_create();
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        IntOption io = f(node->value, i, x);
        if (!io.none) {
            il_append(result, io.some);
        }
    }
    return result;
}

List il_choose_state(List list, IntIntIntAnyToIntOption f, int x, Any state);

static IntOption evens_times_x_state(int element, int index, int x, Any state) {
    if (state == NULL && (element % 2) == 0) {
        return make_int_some(x * element);
    }
    return make_int_none();
}

static void il_choose_state_test(void) {
    printsln((String)__func__);
    List a, ac, ex;

    a = il_of_string("1, 2, 3, 4, 5, 6");
    ac = il_choose_state(a, evens_times_x_state, 3, NULL);
    ex = il_of_string("6, 12, 18");
    il_test_equal(ac, ex);

    l_free(ac);
    l_free(ex);
    l_free(a);
}

List il_choose_state(List list, IntIntIntAnyToIntOption f, int x, Any state) {
    assert_function_not_null(f);
    assert_argument_not_null(list);
    il_assert_element_size(list);
    List result = il_create();
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        IntOption io = f(node->value, i, x, state);
        if (!io.none) {
            il_append(result, io.some);
        }
    }
    return result;
}

bool il_exists(List list, IntIntIntToBool predicate, int x);

static void il_exists_test(void) {
    printsln((String)__func__);
    List a = il_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(il_exists(a, il_gt, 3), true);
    test_equal_b(il_exists(a, il_gt, 9), false);
    test_equal_b(il_exists(a, il_lt, -1), false);
    l_free(a);
}


bool il_exists(List list, IntIntIntToBool predicate, int x) {
    assert_function_not_null(predicate);
    assert_argument_not_null(list);
    il_assert_element_size(list);
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (predicate(node->value, i, x)) {
            return true;
        }
    }
    return false;
}

bool il_exists_state(List list, IntIntIntAnyToBool predicate, int x, Any state);

static void il_exists_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

bool il_exists_state(List list, IntIntIntAnyToBool predicate, int x, Any state) {
    assert_function_not_null(predicate);
    assert_argument_not_null(list);
    il_assert_element_size(list);
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (predicate(node->value, i, x, state)) {
            return true;
        }
    }
    return false;
}

bool il_forall(List list, IntIntIntToBool predicate, int x);

static void il_forall_test(void) {
    printsln((String)__func__);
    List a = il_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(il_forall(a, il_gt, 0), true);
    test_equal_b(il_forall(a, il_gt, 1), false);
    test_equal_b(il_forall(a, il_lt, 7), true);
    l_free(a);
}

bool il_forall(List list, IntIntIntToBool predicate, int x) {
    assert_function_not_null(predicate);
    assert_argument_not_null(list);
    il_assert_element_size(list);
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (!predicate(node->value, i, x)) {
            return false;
        }
    }
    return true;
}

bool il_forall_state(List list, IntIntIntAnyToBool predicate, int x, Any state);

static void il_forall_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

bool il_forall_state(List list, IntIntIntAnyToBool predicate, int x, Any state) {
    assert_function_not_null(predicate);
    assert_argument_not_null(list);
    il_assert_element_size(list);
    int i = 0;
    for (IntListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (!predicate(node->value, i, x, state)) {
            return false;
        }
    }
    return true;
}



///////////////////////////////////////////////////////////////////////////////

bool il_test_equal_file_line(const char *file, const char *function, int line, List a, List e) {
    base_count_check();

    if (a == NULL) {
        printf("%s, line %d: Actual list is NULL\n", file, line);
        return false;        
    }
    if (e == NULL) {
        printf("%s, line %d: Expected list is NULL\n", file, line);
        return false;        
    }
    if (a->s != sizeof(int)) {
        printf("%s, line %d: Actual list is not an int list (element size %d)\n", 
                file, line, a->s);
        return false;
    }
    if (e->s != sizeof(int)) {
        printf("%s, line %d: Expected list is not an int list (element size %d)\n", 
                file, line, e->s);
        return false;
    }
    ListNode *an = a->first;
    ListNode *en = e->first;
    int i = 0;
    for (; an != NULL && en != NULL; an = an->next, en = en->next, i++) {
        int av = *(int*)(an + 1);
        int ev = *(int*)(en + 1);
        if (av != ev) {
            printf("%s, line %d: Actual value %d differs from expected value %d at index %d.\n", 
                    file, line, av, ev, i);
            return false;
        }
    }
    if (an != NULL || en != NULL) {
        printf("%s, line %d: Actual and expected lengths differ\n", file, line);
        return false;
    }
    printf("%s, line %d: check passed\n", file, line);
    base_count_success();
    return true;
}



///////////////////////////////////////////////////////////////////////////////

void il_test_all(void) {
    il_repeat_test();
    il_range_test();
    il_of_string_test();
    il_fn_test();
    il_of_dl_test();
    il_prepend_append_test();
    il_iterator_test();
    il_contains_test();
    il_fill_test();
    il_fill_from_to_test();
    il_index_test();
    il_index_from_test();
    il_index_fn_test();
    il_sort_test();
    il_sort_dec_test();
    il_insert_test();
    il_remove_test();
    il_each_test();
    il_each_state_test();
    il_foldl_test();
    il_foldr_test();
    il_filter_test();
    il_filter_state_test();
    il_choose_test();
    il_choose_state_test();
    il_exists_test();
    il_exists_state_test();
    il_forall_test();
    il_forall_state_test();
}

#if 0
int main(void) {
    il_test_all();
    return 0;
}
#endif
