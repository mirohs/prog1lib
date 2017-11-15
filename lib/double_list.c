/*
@author Michael Rohs
@date 15.10.2015
@copyright Apache License, Version 2.0
*/

#include "list.h"
#include "int_list.h"
#include "double_list.h"
#include "array.h"

/*
 * Access the content of a DoubleListNode
 */
#define VALUE(node) (*(double*)(((ListNode*)(node)) + 1))

List dl_create() {
    ListHead *lh = xcalloc(1, sizeof(ListHead));
    lh->s = sizeof(double); // content size
    return lh;
}

List dl_of_buffer(Any buffer, int n) {
    return l_of_buffer(buffer, n, sizeof(double));
}

#if 0
static void dl_repeat_test(void) {
    printsln((String)__func__);
    List list;
    
    list = dl_repeat(3, 0);
    test_within_d(dl_get(list, 0), 0, EPSILON);
    test_within_d(dl_get(list, 1), 0, EPSILON);
    test_within_d(dl_get(list, 2), 0, EPSILON);
    l_free(list);
    
    list = dl_repeat(2, -1);
    test_within_d(dl_get(list, 0), -1, EPSILON);
    test_within_d(dl_get(list, 1), -1, EPSILON);
    l_free(list);

    list = dl_repeat(0, 2);
    test_equal_i(l_length(list), 0);
    l_free(list);
}

List dl_repeat(int n, double value) {
    if (n < 0) {
        printf("%s: length cannot be negative (is %d)\n", __func__, n);
        exit(EXIT_FAILURE);
    }
    ListHead *lh = xcalloc(1, sizeof(ListHead));
    lh->s = sizeof(value); // content size
    for (int i = 0; i < n; i++) {
        dl_append(lh, value);
    }
    return lh;
}
#endif

static void dl_range_test(void) {
    printsln((String)__func__);
    List list;

    list = dl_range(1, -2, 1);
    dl_println(list);
    l_free(list);

    list = dl_range(1, -1, 0.5);
    dl_println(list);
    l_free(list);

    list = dl_range(1, 0, 0.1);
    dl_println(list);
    l_free(list);

    list = dl_range(1, 1, 1);
    dl_println(list);
    l_free(list);

    list = dl_range(1, 2, 0.3);
    dl_println(list);
    l_free(list);

    list = dl_range(1, 3, 0.3);
    dl_println(list);
    l_free(list);
}

List dl_range(double a, double b, double step) {
    if (fabs(step) < 0.000001) {
        printf("%s: step too small (is %g)\n", __func__, step);
        exit(EXIT_FAILURE);
    }
    ListHead *list = xcalloc(1, sizeof(ListHead));
    list->s = sizeof(double); // content size
    if (a <= b) {
        if (step < 0) {
            step = -step;
        }
        for (double x = a; x < b; x += step) {
            dl_append(list, x);
        }
    } else /* a > b */ {
        if (step > 0) {
            step = -step;
        }
        for (double x = a; x > b; x += step) {
            dl_append(list, x);
        }
    }
    return list;
}

static void dl_of_string_test(void) {
    printsln((String)__func__);
    List ac, ex;

    ac = dl_of_string("1, 2, 3, 4, 5, 6");
    ex = dl_range(1, 7, 1);
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string("   1, 2, 3, 4, 5, 6   ");
    ex = dl_range(1, 7, 1);
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string("1xx2asdfs3");
    ex = dl_range(1, 4, 1);
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string("y1xx2asdfs3x");
    ex = dl_range(1, 4, 1);
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string("-3, -2, -1, 0, 1");
    ex = dl_range(-3, 2, 1);
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string(" -3, -2, -1 ");
    ex = dl_range(-3, 0, 1);
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string(".1 -20. -3.5 1e5 -1.2e4 -1.2e-4");
    ex = dl_create();
    dl_append(ex, 0.1);
    dl_append(ex, -20.0);
    dl_append(ex, -3.5);
    dl_append(ex, 1e5);
    dl_append(ex, -1.2e4);
    dl_append(ex, -1.2e-4);
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string("-.1.-2.-.3.-");
    ex = dl_create();
    dl_append(ex, -0.1);
    dl_append(ex, -2.0);
    dl_append(ex, -0.3);
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string("---.1.----2.----.3.----");
    ex = dl_create();
    dl_append(ex, -0.1);
    dl_append(ex, -2.0);
    dl_append(ex, -0.3);
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = dl_of_string("");
    ex = dl_create();
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
}

List dl_of_string(String s) {
    assert_argument_not_null(s);
    ListHead *list = xcalloc(1, sizeof(ListHead));
    list->s = sizeof(double); // content size
    char *t = s;
    char *endp;
    while (*t != '\0') {
        if (isdigit(*t)) {
            if (t > s && *(t - 1) == '.') t--; // check for '.'
            if (t > s && *(t - 1) == '-') t--; // check for '-'
            dl_append(list, strtod(t, &endp)); // convert digit string to int
            t = endp;
        } else {
            // assert: *t is not a digit, *t is not '\0'
            t++; // not a digit, skip
        }
    }
    return list;
}

static double two_d_plus_1(int index, double x) {
    return 2 * index + 1;
}

static void dl_fn_test(void) {
    printsln((String)__func__);
    List ac, ex;

    ac = dl_fn(3, two_d_plus_1, 0);
    ex = dl_of_string("1, 3, 5");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_fn(4, two_d_plus_1, 0);
    ex = dl_of_string("1, 3, 5, 7");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_fn(1, two_d_plus_1, 0);
    ex = dl_of_string("1");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_fn(0, two_d_plus_1, 0);
    ex = dl_create();
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
}

List dl_fn(int n, IntDoubleToDouble init, double x) {
    assert_function_not_null(init);
    if (n < 0) {
        printf("%s: length cannot be negative (is %d)\n", __func__, n);
        exit(EXIT_FAILURE);
    }
    List result = dl_create();
    for (int i = 0; i < n; i++) {
        dl_append(result, init(i, x));
    }
    return result;
}

static void dl_of_il_test(void) {
    printsln((String)__func__);
    List il = il_of_string("-20, -1, 0, 1, 20");
    List ex = dl_of_string("-20, -1, 0, 1, 20");
    List ac = dl_of_il(il);
    dl_test_within(ac, ex);
    l_free(il);
    l_free(ex);
    l_free(ac);
}

List dl_of_il(List list) {
    assert_argument_not_null(list);
    il_assert_element_size(list);
    List result = dl_create();
    for (IntListNode *node = list->first; node != NULL; node = node->next) {
        dl_append(result, node->value);
    }
    return result;
}

double dl_get(List list, int index) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
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

void dl_set(List list, int index, double value) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
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

void dl_inc(List list, int index, double value) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
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

static void dl_iterator_test(void) {
    printsln((String)__func__);
    List ac;

    ac = dl_of_string("1, 2, 3, 4, 5");
    
    // various ways of iterating a list:
    
    ListIterator iter = l_iterator(ac);
    while (l_has_next(iter)) {
        double i = dl_next(&iter);
        printiln(i);
    }
    
    // DoubleListNode *first = (DoubleListNode*)ac->first;
    for (DoubleListNode *node = ac->first; node != NULL; node = node->next) {
        double d = node->value;
        printiln(d);
    }
    
#if 0
    while (iter = dl_next(iter)) {
        double d = dl_current(iter);
        printiln(d);
    }
        
    for (DoubleListIterator iter = dl_iterator(ac); dl_has_current(iter); iter = dl_next(iter)) {
        double d = dl_current(iter);
        printiln(d);
    }
#endif
    
    l_free(ac);
}

double dl_next(ListIterator *iter) {
    if (*iter == NULL) {
        printf("%s: iterator does not have a next value\n", __func__);
        exit(EXIT_FAILURE);
    }
    double value = VALUE(*iter);
    *iter = (*iter)->next;
    return value;
}

///////////////////////////////////////////////////////////////////////////////

static void dl_prepend_append_test(void) {
    printsln((String)__func__);
    List ac, ex;
    ac = dl_create();
    dl_append(ac, 1);
    dl_append(ac, 2);
    dl_append(ac, 3);
    ex = dl_create();
    dl_prepend(ex, 3);
    dl_prepend(ex, 2);
    dl_prepend(ex, 1);
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
}

void dl_append(List list, double value) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    // allocate memory for next-pointer and content
    DoubleListNode *node = xcalloc(1, sizeof(ListNode*) + sizeof(value));
    // copy content, leave next-pointer NULL
    node->value = value;
    // if this is the first element of the list set first pointer
    if (list->first == NULL) {
        list->first = node;
    }
    // if there is a last element, update its next-pointer
    if (list->last != NULL) {
        DoubleListNode *list_last = list->last;
        list_last->next = node;
    }
    // the new node is the last node of the list
    list->last = node;
}

void dl_prepend(List list, double value) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    // allocate memory for next-pointer and content
    DoubleListNode *node = xcalloc(1, sizeof(ListNode*) + sizeof(value));
    // copy content, leave next-pointer NULL
    node->value = value;
    node->next = list->first;
    list->first = node;
    if (list->last == NULL) {
        list->last = node;
    }
}

void dl_print(List list) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    ListNode *node = list->first;
    printf("[");
    if (node != NULL) {
        printf("%g", VALUE(node));
        node = node->next;
    }
    for (; node != NULL; node = node->next) {
        printf(", %g", VALUE(node));
    }
    printf("]");
}

void dl_println(List list) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    dl_print(list);
    printf("\n");
}

static void dl_contains_test(void) {
    printsln((String)__func__);
    List list = dl_of_string("10, 20, 30");
    test_equal_b(dl_contains(list, 10, EPSILON), true);
    test_equal_b(dl_contains(list, 11, EPSILON), false);
    test_equal_b(dl_contains(list, 30, EPSILON), true);
    l_free(list);
}

bool dl_contains(List list, double value, double epsilon) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    for (DoubleListNode *node = list->first; node != NULL; node = node->next) {
        if (fabs(node->value - value) < epsilon) {
            return true;
        }
    }
    return false;
}

static void dl_fill_test(void) {
    printsln((String)__func__);
    List ex, ac;

    ac = dl_of_string("1, 1, 1");
    dl_fill(ac, -1);
    ex = dl_of_string("-1, -1, -1");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string("1, 1");
    dl_fill(ac, 2);
    ex = dl_of_string("2, 2");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string("");
    dl_fill(ac, 2);
    ex = dl_create();
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
}

void dl_fill(List list, double value) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    for (DoubleListNode *node = list->first; node != NULL; node = node->next) {
        node->value = value;
    }
}

static void dl_fill_from_to_test(void) {
    printsln((String)__func__);
    List ac, ex;

    ac = dl_of_string("0, 0, 0");
    dl_fill_from_to(ac, -1, 0, 3);
    ex = dl_of_string("-1, -1, -1");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string("0, 0, 0");
    dl_fill_from_to(ac, -1, -1, 4);
    ex = dl_of_string("-1, -1, -1");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string("0, 0, 0");
    dl_fill_from_to(ac, -1, 0, 0);
    ex = dl_of_string("0, 0, 0");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string("0, 0, 0");
    dl_fill_from_to(ac, -1, 0, 1);
    ex = dl_of_string("-1, 0, 0");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string("0, 0, 0");
    dl_fill_from_to(ac, -1, 2, 2);
    ex = dl_of_string("0, 0, 0");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string("0, 0, 0");
    dl_fill_from_to(ac, -1, 2, 3);
    ex = dl_of_string("0, 0, -1");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string("0, 0, 0");
    dl_fill_from_to(ac, -1, 2, 1);
    ex = dl_of_string("0, 0, 0");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_of_string("0, 0, 0");
    dl_fill_from_to(ac, -1, 1, 3);
    ex = dl_of_string("0, -1, -1");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
}

void dl_fill_from_to(List list, double value, int from, int to) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    if (from < 0) from = 0;
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL && i < to; node = node->next, i++) {
        if (i >= from) {
            node->value = value;
        }
    }
}

static void dl_index_test(void) {
    printsln((String)__func__);
    List list;

    list = dl_range(1, 3, 1);
    int i = dl_index(list, 0, EPSILON);
#if 0
    if (i < 0) {
        printsln("value not found");
    } else {
        printf("value found at index %d\n", i);
    }
#endif
    test_equal_i(i, -1);

    i = dl_index(list, 1, EPSILON);
    test_equal_i(i, 0);
        
    i = dl_index(list, 2, EPSILON);
    test_equal_i(i, 1);
        
    l_free(list);
    
}

int dl_index(List list, double value, double epsilon) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (fabs(node->value - value) < epsilon) {
            return i;
        }
    }
    return -1;
}

static void dl_index_from_test(void) {
    printsln((String)__func__);
    List a = dl_of_string("10 20 30 40 50");
    test_equal_i(dl_index_from(a, 20, 0, EPSILON), 1);
    test_equal_i(dl_index_from(a, 20, 1, EPSILON), 1);
    test_equal_i(dl_index_from(a, 20, 2, EPSILON), -1);
    test_equal_i(dl_index_from(a, 30, -1, EPSILON), 2);
    test_equal_i(dl_index_from(a, 60, 0, EPSILON), -1);
    l_free(a);
    
}

int dl_index_from(List list, double value, int from, double epsilon) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    if (from < 0) from = 0;
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (i >= from && fabs(node->value - value) < epsilon) {
            return i;
        }
    }
    return -1;
}

static void dl_index_fn_test(void) {
    printsln((String)__func__);
    List a = dl_of_string("1 2 3 -4 5");
    test_equal_i(dl_index_fn(a, dl_gt, 0.0), 0);
    test_equal_i(dl_index_fn(a, dl_lt, 0.0), 3);
    l_free(a);
}

int dl_index_fn(List list, DoubleIntDoubleToBool predicate, double x) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    assert_function_not_null(predicate);
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (predicate(node->value, i, x)) {
            return i;
        }
    }
    return -1;
}

///////////////////////////////////////////////////////////////////////////////

static CmpResult double_compare(ConstAny a, ConstAny b) {
    double x = *(double*)a;
    double y = *(double*)b;
    return (x == y) ? EQ : (x < y ? LT : GT);
}

static void dl_sort_test(void) {
    printsln((String)__func__);
    List ac, ex, as;

    ac = dl_of_string("5, 4, 3, 2, 1");
    ex = dl_of_string("1, 2, 3, 4, 5");
    as = dl_sort(ac);
    dl_test_within(as, ex);
    l_free(ac);
    l_free(ex);
    l_free(as);

    ac = dl_of_string("1, 2, 1, 3, 2");
    ex = dl_of_string("1, 1, 2, 2, 3");
    as = dl_sort(ac);
    dl_test_within(as, ex);
    l_free(ac);
    l_free(ex);
    l_free(as);

    ac = dl_of_string("");
    ex = dl_of_string("");
    as = dl_sort(ac);
    dl_test_within(as, ex);
    l_free(ac);
    l_free(ex);
    l_free(as);

    ac = dl_of_string("-1, -2, -3, -1");
    ex = dl_of_string("-3, -2, -1, -1");
    as = dl_sort(ac);
    dl_test_within(as, ex);
    l_free(ac);
    l_free(ex);
    l_free(as);
}

List dl_sort(List list) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    return l_sort(list, double_compare);
}

static CmpResult double_compare_dec(ConstAny a, ConstAny b) {
    double x = *(double*)b;
    double y = *(double*)a;
    return (x == y) ? EQ : (x < y ? LT : GT);
}

static void dl_sort_dec_test(void) {
    printsln((String)__func__);
    List ac, ex, as;

    ac = dl_of_string("1, 2, 3, 4, 5");
    ex = dl_of_string("5, 4, 3, 2, 1");
    as = dl_sort_dec(ac);
    dl_test_within(as, ex);
    l_free(ac);
    l_free(ex);
    l_free(as);

    ac = dl_of_string("1, 2, 1, 3, 2");
    ex = dl_of_string("3, 2, 2, 1, 1");
    as = dl_sort_dec(ac);
    dl_test_within(as, ex);
    l_free(ac);
    l_free(ex);
    l_free(as);

    ac = dl_of_string("");
    ex = dl_of_string("");
    as = dl_sort_dec(ac);
    dl_test_within(as, ex);
    l_free(ac);
    l_free(ex);
    l_free(as);

    ac = dl_of_string("-3, -2, -1");
    ex = dl_of_string("-1, -2, -3");
    as = dl_sort_dec(ac);
    dl_test_within(as, ex);
    l_free(ac);
    l_free(ex);
    l_free(as);
}

List dl_sort_dec(List list) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    return l_sort(list, double_compare_dec);
}

static void dl_insert_test(void) {
    printsln((String)__func__);
    List ac, ex;

    ac = dl_of_string("1, 2, 3, 4, 5");
    dl_insert(ac, 0, 9);
    ex = dl_of_string("9, 1, 2, 3, 4, 5");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = dl_of_string("1, 2, 3, 4, 5");
    dl_insert(ac, 5, 9);
    ex = dl_of_string("1, 2, 3, 4, 5, 9");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = dl_of_string("1, 2, 3, 4, 5");
    dl_insert(ac, 3, 9);
    ex = dl_of_string("1, 2, 3, 9, 4, 5");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = dl_of_string("1");
    dl_insert(ac, -1, 9);
    ex = dl_of_string("1");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = dl_of_string("1");
    dl_insert(ac, 1, 9);
    ex = dl_of_string("1, 9");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = dl_of_string("");
    dl_insert(ac, 0, 9);
    ex = dl_of_string("9");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
}

void dl_insert(List list, int index, double value) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    l_insert(list, index, &value);
}

static void dl_remove_test(void) {
    printsln((String)__func__);
    List ac, ex;

    ac = dl_of_string("1, 2, 3, 4, 5, 6");
    dl_remove(ac, 0);
    ex = dl_of_string("2, 3, 4, 5, 6");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = dl_of_string("1, 2, 3, 4, 5, 6");
    dl_remove(ac, 5);
    ex = dl_of_string("1, 2, 3, 4, 5");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = dl_of_string("1, 2, 3, 4, 5, 6");
    dl_remove(ac, 3);
    ex = dl_of_string("1, 2, 3, 5, 6");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = dl_of_string("1");
    dl_remove(ac, -1);
    ex = dl_of_string("1");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = dl_of_string("1");
    dl_remove(ac, 1);
    ex = dl_of_string("1");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = dl_of_string("1");
    dl_remove(ac, 0);
    ex = dl_of_string("");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = dl_of_string("");
    dl_remove(ac, 0);
    ex = dl_of_string("");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
}

void dl_remove(List list, int index) {
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    l_remove(list, index);
}

///////////////////////////////////////////////////////////////////////////////

bool dl_index_even(double value, int index) {
    return (index & 1) == 0;
}

bool dl_index_odd(double value, int index) {
    return (index & 1) == 1;
}

bool dl_pos(double value, int index) {
    return value > 0;
}

bool dl_neg(double value, int index) {
    return value < 0;
}

bool dl_ge0(double value, int index) {
    return value >= 0;
}

bool dl_gt(double value, int index, double x) {
    return value > x;
}

bool dl_ge(double value, int index, double x) {
    return value >= x;
}

bool dl_lt(double value, int index, double x) {
    return value < x;
}

bool dl_le(double value, int index, double x) {
    return value <= x;
}

///////////////////////////////////////////////////////////////////////////////

/*
 * Functions for initialization, map, and fold
 */
double dl_square(double value, int index) {
    return value * value; // index not used
}

double dl_rnd(int index, double maximum) {
    return d_rnd(maximum); // index not used
}

double dl_times(double value, int index, double x) {
    return value * x;
}

static void dl_each_test(void) {
    printsln((String)__func__);
    List ac, ex;
    
    ac = dl_of_string("1, 2, 3, 4");
    dl_each(ac, dl_times, 2);
    ex = dl_of_string("2, 4, 6, 8");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
    
    ac = dl_of_string("");
    dl_each(ac, dl_times, 3);
    ex = dl_of_string("");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
}

void dl_each(List list, DoubleIntDoubleToDouble f, double x) {
    assert_function_not_null(f);
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
        node->value = f(node->value, i, x);
    }
}

static void dl_each_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

void dl_each_state(List list, DoubleIntDoubleAnyToDouble f, double x, Any state) {
    assert_function_not_null(f);
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
        node->value = f(node->value, i, x, state);
    }
}

static void dl_map_test(void) {
    printsln((String)__func__);
    List l, ac, ex;
    
    l = dl_of_string("1, 2, 3, 4");
    ac = dl_map(l, dl_times, 2);
    ex = dl_of_string("2, 4, 6, 8");
    dl_test_within(ac, ex);
    l_free(l);
    l_free(ac);
    l_free(ex);
    
    l = dl_of_string("");
    ac = dl_map(l, dl_times, 3);
    ex = dl_of_string("");
    dl_test_within(ac, ex);
    l_free(l);
    l_free(ac);
    l_free(ex);
}

List dl_map(List list, DoubleIntDoubleToDouble f, double x) {
    assert_function_not_null(f);
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    List result = dl_create();
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
        dl_append(result, f(node->value, i, x));
    }
    return result;
}

static void dl_map_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

List dl_map_state(List list, DoubleIntDoubleAnyToDouble f, double x, Any state) {
    assert_function_not_null(f);
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    List result = dl_create();
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
        dl_append(result, f(node->value, i, x, state));
    }
    return result;
}

static void dl_foldl_test(void) {
    printsln((String)__func__);
    List a = dl_of_string("");    
    test_within_d(dl_foldl(a, double_plus, 100), 100, EPSILON);
    test_within_d(dl_foldl(a, double_minus, 100), 100, EPSILON);
    l_free(a);
    
    a = dl_of_string("1, 2, 3, 4");    
    test_within_d(dl_foldl(a, double_plus, 0),    (((0 + 1) + 2) + 3) + 4, EPSILON);
    test_within_d(dl_foldl(a, double_minus, 100), (((100 - 1) - 2) - 3) - 4, EPSILON);
    l_free(a);
}

double dl_foldl(List list, DoubleDoubleIntToDouble f, double init) {
    assert_function_not_null(f);
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
        init = f(init, node->value, i);
    }
    return init;
}

static void dl_foldr_test(void) {
    printsln((String)__func__);
    List a = dl_of_string("");    
    test_within_d(dl_foldr(a, double_plus, 100), 100, EPSILON);
    test_within_d(dl_foldr(a, double_minus, 100), 100, EPSILON);
    l_free(a);
    
    a = dl_of_string("1, 2, 3, 4");    
    test_within_d(dl_foldr(a, double_plus, 100), 1 + (2 + (3 + (4 + 100))), EPSILON);
    test_within_d(dl_foldr(a, double_minus, 0), 1 - (2 - (3 - (4 - 0))), EPSILON);
    l_free(a);
}

double dl_foldr(List list, DoubleDoubleIntToDouble f, double init) {
    assert_function_not_null(f);
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    List rev = l_reverse(list);
    int i = l_length(list) - 1;
    for (DoubleListNode *node = rev->first; node != NULL; node = node->next, i--) {
        init = f(node->value, init, i);
    }
    l_free(rev);
    return init;
}

static void dl_filter_test(void) {
    printsln((String)__func__);
    List a, ac, ex;

    a = dl_of_string("1, 2, 3, 4, 5, 6");
    ac = dl_filter(a, dl_gt, 3);
    ex = dl_of_string("4, 5, 6");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_filter(a, dl_lt, 4);
    ex = dl_of_string("1, 2, 3");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = dl_filter(a, dl_le, 3);
    ex = dl_of_string("1, 2, 3");
    dl_test_within(ac, ex);
    l_free(ac);
    l_free(ex);
    l_free(a);
}

List dl_filter(List list, DoubleIntDoubleToBool predicate, double x) {
    assert_function_not_null(predicate);
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    List result = dl_create();
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (predicate(node->value, i, x)) {
            dl_append(result, node->value);
        }
    }
    return result;
}

static void dl_filter_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

List dl_filter_state(List list, DoubleIntDoubleAnyToBool predicate, double x, Any state) {
    assert_function_not_null(predicate);
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    List result = dl_create();
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (predicate(node->value, i, x, state)) {
            dl_append(result, node->value);
        }
    }
    return result;
}

DoubleOption gt3_times_x(double element, int index, double x) {
    if (element > 3) {
        return make_double_some(x * element);
    }
    return make_double_none();
}

static void dl_choose_test(void) {
    printsln((String)__func__);
    List a, ac, ex;

    a = dl_of_string("1, 2, 3, 4, 5, 6");
    ac = dl_choose(a, gt3_times_x, 4);
    ex = dl_of_string("16, 20, 24");
    dl_test_within(ac, ex);

    l_free(ac);
    l_free(ex);
    l_free(a);
}

List dl_choose(List list, DoubleIntDoubleToDoubleOption f, double x) {
    assert_function_not_null(f);
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    List result = dl_create();
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
        DoubleOption op = f(node->value, i, x);
        if (!op.none) {
            dl_append(result, op.some);
        }
    }
    return result;
}

DoubleOption gt_x_times_y(double element, int index, double x, Any state) {
    double y = *(double*)state;
    if (element > x) {
        return make_double_some(y * element);
    }
    return make_double_none();
}

static void dl_choose_state_test(void) {
    printsln((String)__func__);
    List a, ac, ex;

    a = dl_of_string("1, 2, 3, 4, 5, 6");
    double y = 4;
    ac = dl_choose_state(a, gt_x_times_y, 3, &y);
    ex = dl_of_string("16, 20, 24");
    dl_test_within(ac, ex);

    l_free(ac);
    l_free(ex);
    l_free(a);
}

List dl_choose_state(List list, DoubleIntDoubleAnyToDoubleOption f, double x, Any state) {
    assert_function_not_null(f);
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    List result = dl_create();
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
        DoubleOption op = f(node->value, i, x, state);
        if (!op.none) {
            dl_append(result, op.some);
        }
    }
    return result;
}

static void dl_exists_test(void) {
    printsln((String)__func__);
    List a = dl_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(dl_exists(a, dl_gt, 3), true);
    test_equal_b(dl_exists(a, dl_gt, 9), false);
    test_equal_b(dl_exists(a, dl_lt, -1), false);
    l_free(a);
}

bool dl_exists(List list, DoubleIntDoubleToBool predicate, double x) {
    assert_function_not_null(predicate);
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (predicate(node->value, i, x)) {
            return true;
        }
    }
    return false;
}

static void dl_exists_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

bool dl_exists_state(List list, DoubleIntDoubleAnyToBool predicate, double x, Any state) {
    assert_function_not_null(predicate);
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (predicate(node->value, i, x, state)) {
            return true;
        }
    }
    return false;
}

static void dl_forall_test(void) {
    printsln((String)__func__);
    List a = dl_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(dl_forall(a, dl_gt, 0), true);
    test_equal_b(dl_forall(a, dl_gt, 1), false);
    test_equal_b(dl_forall(a, dl_lt, 7), true);
    l_free(a);
}

bool dl_forall(List list, DoubleIntDoubleToBool predicate, double x) {
    assert_function_not_null(predicate);
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (!predicate(node->value, i, x)) {
            return false;
        }
    }
    return true;
}

static void dl_forall_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

bool dl_forall_state(List list, DoubleIntDoubleAnyToBool predicate, double x, Any state) {
    assert_function_not_null(predicate);
    assert_argument_not_null(list);
    dl_assert_element_size(list);
    int i = 0;
    for (DoubleListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (!predicate(node->value, i, x, state)) {
            return false;
        }
    }
    return true;
}



///////////////////////////////////////////////////////////////////////////////
// Tests

bool dl_test_within_file_line(const char *file, const char *function, int line, List a, List e, double epsilon) {
    base_count_check();

    if (a == NULL) {
        printf("%s, line %d: Actual list is NULL\n", file, line);
        return false;        
    }
    if (e == NULL) {
        printf("%s, line %d: Expected list is NULL\n", file, line);
        return false;        
    }
    if (a->s != sizeof(double)) {
        printf("%s, line %d: Actual list is not a double list (element size %d)\n", 
                file, line, a->s);
        return false;
    }
    if (e->s != sizeof(double)) {
        printf("%s, line %d: Expected list is not a double list (element size %d)\n", 
                file, line, e->s);
        return false;
    }
    ListNode *an = a->first;
    ListNode *en = e->first;
    int i = 0;
    for (; an != NULL && en != NULL; an = an->next, en = en->next, i++) {
        double av = *(double*)(an + 1);
        double ev = *(double*)(en + 1);
        if (fabs(av - ev) > epsilon) {
            printf("%s, line %d: Actual value %g differs from expected value %g at index %d.\n", 
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

void dl_test_all(void) {
//    dl_repeat_test();
    dl_range_test();
    dl_of_string_test();
    dl_fn_test();
    dl_of_il_test();
    dl_prepend_append_test();
    dl_iterator_test();
    dl_contains_test();
    dl_fill_test();
    dl_fill_from_to_test();
    dl_index_test();
    dl_index_from_test();
    dl_index_fn_test();
    dl_sort_test();
    dl_sort_dec_test();
    dl_insert_test();
    dl_remove_test();
    dl_each_test();
    dl_each_state_test();
    dl_map_test();
    dl_map_state_test();
    dl_foldl_test();
    dl_foldr_test();
    dl_filter_test();
    dl_filter_state_test();
    dl_choose_test();
    dl_choose_state_test();
    dl_exists_test();
    dl_exists_state_test();
    dl_forall_test();
    dl_forall_state_test();
}

#if 0
int main(void) {
    dl_test_all();
    return 0;
}
#endif
