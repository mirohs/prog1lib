/*
@author Michael Rohs
@date 15.10.2015
@copyright Apache License, Version 2.0
*/

#include "list.h"
#include "array.h"
#include "int_array.h"
#include "string.h"



/*
 * Access to the content of a ListNode
 */
#define VALUE(node) ((Any)(((ListNode*)(node)) + 1))

/*
 * Appends a zero-initialized node to list.
 */
static ListNode *l_append_empty(List list) {
    require_not_null(list);
    // allocate memory for next-pointer and content
    Any a = xcalloc(1, sizeof(ListNode*) + list->s);
    // if this is the first element of the result set first pointer
    if (list->first == NULL) {
        list->first = a;
    }
    // if there is a last element, update its next-pointer
    if (list->last != NULL) {
        ListNode *list_last = list->last;
        list_last->next = a;
    }
    // the new node is the last node of the list
    list->last = a;
    return a;
}

List l_create(int s);

// Address is used within testing.
typedef struct Address {
    String firstname;
    String lastname;
    String city;
} Address;

static Address make_address(String f, String l, String c) {
    Address a = { f, l, c };
    return a;
}

static void l_create_test(void) {
    printsln((String)__func__);
    List list;
    Address addr;
    
    list = l_create(sizeof(Address));
    addr = make_address("Fred", "Oyster", "Hannover");
    l_append(list, &addr);
    addr = make_address("Frida", "Qwirin", "Berlin");
    l_append(list, &addr);
    addr = make_address("James", "Bond", "London");
    l_append(list, &addr);
    
    printiln(l_length(list));

    Address *pa = l_get(list, 0);
    printsln(pa->firstname);
    test_equal_s(pa->firstname, "Fred");
    pa = l_get(list, 1);
    printsln(pa->firstname);
    test_equal_s(pa->city, "Berlin");
    pa = l_get(list, 2);
    printsln(pa->firstname);
    test_equal_s(pa->lastname, "Bond");
    
    l_free(list);
}

List l_create(int s) {
    require("positive size", s > 0);
    ListHead *lh = xcalloc(1, sizeof(ListHead));
    lh->s = s; // content size
    return lh;
}

List l_of_buffer(Any buffer, int n, int s);

static void l_of_buffer_test(void) {
    printsln((String)__func__);
    Address addr[] = {
        make_address("Fred", "Oyster", "Hannover"), 
        make_address("Frida", "Qwirin", "Berlin"), 
        make_address("James", "Bond", "London"), 
    };
    List l = l_of_buffer(&addr, 3, sizeof(Address));

    Address *pa = l_get(l, 0);
    printsln(pa->firstname);
    test_equal_s(pa->firstname, "Fred");
    pa = l_get(l, 1);
    printsln(pa->firstname);
    test_equal_s(pa->city, "Berlin");
    pa = l_get(l, 2);
    printsln(pa->firstname);
    test_equal_s(pa->lastname, "Bond");
    
    l_free(l);
}

List l_of_buffer(Any buffer, int n, int s) {
    require_not_null(buffer);
    require("non-negative length", n >= 0);
    require("positive size", s > 0);
    List list = l_create(s);
    Byte *p = buffer;
    for (int i = 0; i < n; i++) {
        l_append(list, p);
        p += s;
    }
    return list;
}

List l_fn(int n, int s, AnyFn init, Any state);

static void x_and_xsquare(Any element, int index, Any state) {
    IntPair *ip = element;
    ip->i = index;
    ip->j = index * index;
}

static void l_fn_test(void) {
    printsln((String)__func__);
    List ac, ex;
    IntPair ip;

    ac = l_fn(3, sizeof(IntPair), x_and_xsquare, NULL);

    ex = l_create(sizeof(IntPair));
    ip = make_int_pair(0, 0);
    l_append(ex, &ip);
    ip = make_int_pair(1, 1);
    l_append(ex, &ip);
    ip = make_int_pair(2, 4);
    l_append(ex, &ip);
    
    l_test_equal(ac, ex);
    
    l_free(ac);
    l_free(ex);
}

List l_fn(int n, int s, AnyFn init, Any state) {
    require("non-negative length", n >= 0);
    require("positive size", s > 0);
    require_not_null(init);
    AnyIntAnyToVoid finit = init;
    List list = l_create(s);
    for (int i = 0; i < n; i++) {
        // allocate memory for next-pointer and content
        Any a = xcalloc(1, sizeof(ListNode*) + s);
        // if this is the first element of the list set first pointer
        if (list->first == NULL) {
            list->first = a;
        }
        // if there is a last element, update its next-pointer
        if (list->last != NULL) {
            ListNode *list_last = list->last;
            list_last->next = a;
        }
        // the new node is the last node of the list
        list->last = a;
//        Any element = a + sizeof(ListNode*); // arithmetic on void-pointers is a gcc extension!
        Any element = (ListNode*)a + 1;
        finit(element, i, state);
    }
    return list;
}

///////////////////////////////////////////////////////////////////////////////

List l_copy(List list);

static void l_copy_test(void) {
    printsln((String)__func__);
    List list, copy;
    Address a;
    
    list = l_create(sizeof(Address));
    a = make_address("Fred", "Oyster", "Hannover");
    l_append(list, &a);
    a = make_address("Frida", "Qwirin", "Hannover");
    l_append(list, &a);
    a = make_address("James", "Bond", "London");
    l_append(list, &a);
    
    copy = l_copy(list);
    l_test_equal(copy, list);
    
    l_free(list);
    l_free(copy);
#if 0
    list = il_of_string("-1, -2, -3");
    copy = l_copy(list);
    il_test_equal(copy, list);
    l_free(list);
    l_free(copy);

    list = il_of_string("10");
    copy = l_copy(list);
    il_test_equal(copy, list);
    l_free(list);
    l_free(copy);

    list = il_of_string("100, -200");
    copy = l_copy(list);
    il_test_equal(copy, list);
    l_free(list);
    l_free(copy);

    list = il_of_string("");
    copy = l_copy(list);
    il_test_equal(copy, list);
    l_free(list);
    l_free(copy);

    list = dl_of_string("-1, -2, -3");
    copy = l_copy(list);
    dl_test_within(list, copy);
    l_free(list);
    l_free(copy);
    list = dl_of_string("10");
    copy = l_copy(list);
    dl_test_within(list, copy);
    l_free(list);
    l_free(copy);
    list = dl_of_string(" 100, -200 ");
    copy = l_copy(list);
    dl_test_within(list, copy);
    l_free(list);
    l_free(copy);
    list = dl_of_string("");
    copy = l_copy(list);
    dl_test_within(list, copy);
    l_free(list);
    l_free(copy);
#endif
}

List l_copy(List list) {
    require_not_null(list);
    List result = l_create(list->s);
    for (ListNode *node = list->first; node != NULL; node = node->next) {
        l_append(result, node + 1);
    }
    return result;
}

List l_sub(List list, int i, int j);

static void print_int_pair(Any element) {
    IntPair *ip = element;
    printf("(%d, %d)", ip->i, ip->j);
}

static void l_sub_test(void) {
    printsln((String)__func__);
    List l, ac, ex;
    IntPair ip;

    l = l_create(sizeof(IntPair));
    ip = make_int_pair(1, 10);
    l_append(l, &ip);
    ip = make_int_pair(2, 20);
    l_append(l, &ip);
    ip = make_int_pair(3, 30);
    l_append(l, &ip);
    l_println(l, print_int_pair);

    ex = l_create(sizeof(IntPair));
    ip = make_int_pair(1, 10);
    l_append(ex, &ip);
    ip = make_int_pair(2, 20);
    l_append(ex, &ip);
    
    ac = l_sub(l, 0, 2);
    // l_println(ac, print_int_pair);
    l_test_equal(ac, ex);
    l_free(ex);
    
    ex = l_create(sizeof(IntPair));
    ip = make_int_pair(2, 20);
    l_append(ex, &ip);
    ip = make_int_pair(3, 30);
    l_append(ex, &ip);
    
    l_free(ac);
    ac = l_sub(l, 1, 3);
    // l_println(ac, print_int_pair);
    // l_println(ex, print_int_pair);
    l_test_equal(ac, ex);
    
    l_free(l);
    l_free(ac);
    l_free(ex);

#if 0
    List list, list2, sub;
    list = il_of_string("1 2 3 4");

    sub = l_sub(list, 0, l_length(list));
    il_test_equal(list, sub);
    l_free(sub);

    sub = l_sub(list, -1, l_length(list) + 1);
    il_test_equal(list, sub);
    l_free(sub);

    list2 = il_of_string("2 3 4");
    sub = l_sub(list, 1, l_length(list));
    il_test_equal(sub, list2);
    l_free(list2);
    l_free(sub);

    list2 = il_of_string("2 3");
    sub = l_sub(list, 1, l_length(list) - 1);
    il_test_equal(sub, list2);
    l_free(list2);
    l_free(sub);

    list2 = il_of_string("");
    sub = l_sub(list, 1, 1);
    il_test_equal(sub, list2);
    l_free(sub);

    sub = l_sub(list, 2, 1);
    il_test_equal(sub, list2);
    l_free(list2);
    l_free(sub);

    l_free(list);
    l_free(list2);
    l_free(sub);
    
    list = dl_range(1, 10, 1);
    
    sub = l_sub(list, 0, list.n);
    dl_test_within(sub, list);
    l_free(sub);

    sub = l_sub(list, -10, list.n + 10);
    dl_test_within(sub, list);
    l_free(sub);

    list2 = dl_range(2, 10, 1);
    sub = l_sub(list, 1, list.n);
    dl_test_within(sub, list2);
    l_free(list2);
    l_free(sub);

    list2 = dl_range(2, 9, 1);
    sub = l_sub(list, 1, list.n - 1);
    dl_test_within(sub, list2);
    l_free(list2);
    l_free(sub);

    list2 = dl_range(0, 0, 1);
    sub = l_sub(list, 1, 1);
    dl_test_within(sub, list2);
    l_free(sub);

    sub = l_sub(list, 2, 1);
    dl_test_within(sub, list2);
    l_free(list2);
    l_free(sub);

    list2 = dl_range(3, 6, 1);
    sub = l_sub(list, 2, 5);
    dl_test_within(sub, list2);
    l_free(list2);
    l_free(sub);

    l_free(list);
    l_free(list2);
    l_free(sub);
#endif
}

List l_sub(List list, int i, int j) {
    require_not_null(list);
    List result = l_create(list->s);
    if (i >= j || j <= 0) {
        return result;
    }
    // assert: i < j && j > 0
    if (i < 0) i = 0;
    
    int k = 0;
    for (ListNode *node = list->first; node != NULL && k < j; node = node->next, k++) {
        if (k >= i) {
            l_append(result, node + 1);
        }
    }
    return result;
}

List l_of_a(Array array);

static void l_of_a_test(void) {
    printsln((String)__func__);
    Array ar;
    List ac, ex;
    int i;
    
    ar = ia_of_string("1, 2, 3");
    ac = l_of_a(ar);

    ex = l_create(sizeof(int));
    i = 1; l_append(ex, &i);
    i = 2; l_append(ex, &i);
    i = 3; l_append(ex, &i);
    
    l_test_equal(ac, ex);
    
    a_free(ar);
    l_free(ac);
    l_free(ex);
}

List l_of_a(Array array) {
    require_not_null(array);
    List result = l_create(array->s);
    for (int i = 0; i < array->n; i++) {
        Any element = a_get(array, i);
        l_append(result, element);
    }
    return result;
}

///////////////////////////////////////////////////////////////////////////////

void l_free(List list) {
    if (list != NULL) {
        ListNode *next = NULL;
        for (ListNode *node = list->first; node != NULL; node = next) {
            next = node->next;
            free(node);
        }
        list->s = 0;
        list->first = NULL;
        list->last = NULL;
        free(list);
    }
}

Any l_get(List list, int index) {
    require_not_null(list);
    int i = 0;
    for (ListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (i == index) {
            return node + 1;
        }
    }
    require_x("index in range", false, "index == %d", index);
    return NULL;
}

void l_set(List list, int index, Any value) {
    require_not_null(list);
    int i = 0;
    for (ListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (i == index) {
            memcpy(node + 1, value, list->s);
            return;
        }
    }
    require_x("index in range", false, "index == %d", index);
}

static void l_iterator_test(void) {
    printsln((String)__func__);
    int i;

    List ac = l_create(sizeof(int));
    i = 1; l_append(ac, &i);
    i = 2; l_append(ac, &i);
    i = 3; l_append(ac, &i);
    
    // various ways of iterating a list:
    
    ListIterator iter = l_iterator(ac);
    while (l_has_next(iter)) {
        int *i = l_next(&iter);
        printiln(*i);
    }
    
    for (ListNode *node = ac->first; node != NULL; node = node->next) {
        int *a = VALUE(node);
        printiln(*a);
    }
    
    l_free(ac);
}

ListIterator l_iterator(List list) {
    require_not_null(list);
    return list->first;
}

bool l_has_next(ListIterator iter) {
    return iter != NULL;
}

Any l_next(ListIterator *iter) {
    require("iterator has more values", *iter);
    Any value = VALUE(*iter);
    *iter = (*iter)->next;
    return value;
}

void l_append(List list, Any value) {
    require_not_null(list);
    // allocate memory for next-pointer and content
    Any a = xcalloc(1, sizeof(ListNode*) + list->s);
    // copy content, leave next-pointer NULL
    memcpy((ListNode*)a + 1, value, list->s);
    // if this is the first element of the list set first pointer
    if (list->first == NULL) {
        list->first = a;
    }
    // if there is a last element, update its next-pointer
    if (list->last != NULL) {
        ListNode *list_last = list->last;
        list_last->next = a;
    }
    // the new node is the last node of the list
    list->last = a;
}

void l_prepend(List list, Any value) {
    require_not_null(list);
    // allocate memory for next-pointer and content
    Any a = xcalloc(1, sizeof(ListNode*) + list->s);
    // copy content, leave next-pointer NULL
    memcpy((ListNode*)a + 1, value, list->s);
    ListNode *node = a;
    node->next = list->first;
    list->first = node;
    if (list->last == NULL) {
        list->last = node;
    }
}

int l_length(List list) {
    require_not_null(list);
    int n = 0;
    for (ListNode *node = list->first; node != NULL; node = node->next) {
        n++;
    }
    return n;
}

int l_element_size(List list) {
    require_not_null(list);
    return list->s;
}

void l_print(List list, AnyFn print_element) {
    require_not_null(list);
    require_not_null(print_element);
    AnyToVoid fprint_element = print_element;
    printf("[");
    ListNode *node = list->first;
    if (node != NULL) {
        fprint_element(node + 1);
        node = node->next;
    }
    for (; node != NULL; node = node->next) {
        printf(", ");
        fprint_element(node + 1);
    }
    printf("]");
}

void l_println(List list, AnyFn print_element) {
    require_not_null(list);
    require_not_null(print_element);
    l_print(list, print_element);
    printf("\n");
}

bool l_equals(List a, List b) {
    require_not_null(a);
    require_not_null(b);
    if (a->s != b->s) return false;
    int s = a->s;
    ListNode* na = a->first;
    ListNode* nb = b->first;
    while (na != NULL && nb != NULL) {
        if (memcmp(VALUE(na), VALUE(nb), s) != 0) {
            return false;
        }
        na = na->next;
        nb = nb->next;
    }
    return na == NULL && nb == NULL;
}

///////////////////////////////////////////////////////////////////////////////

List l_concat(List x, List y);

static void l_concat_test(void) {
    printsln((String)__func__);
    List a1, a2, ac, ex;
    IntPair ip;

    a1 = l_create(sizeof(IntPair));
    ip = make_int_pair(1, 10);
    l_append(a1, &ip);
    ip = make_int_pair(2, 20);
    l_append(a1, &ip);
    ip = make_int_pair(3, 30);
    l_append(a1, &ip);
    
    a2 = l_create(sizeof(IntPair));
    ip = make_int_pair(4, 40);
    l_append(a2, &ip);
    ip = make_int_pair(5, 50);
    l_append(a2, &ip);
    
    ac = l_concat(a1, a2);
    
    ex = l_create(sizeof(IntPair));
    ip = make_int_pair(1, 10);
    l_append(ex, &ip);
    ip = make_int_pair(2, 20);
    l_append(ex, &ip);
    ip = make_int_pair(3, 30);
    l_append(ex, &ip);
    ip = make_int_pair(4, 40);
    l_append(ex, &ip);
    ip = make_int_pair(5, 50);
    l_append(ex, &ip);
    
    l_test_equal(ac, ex);
    
    l_free(a1);
    l_free(a2);
    l_free(ac);
    l_free(ex);
#if 0
    a1 = il_of_string("10, 20, 30");
    a2 = il_of_string("1, 2, 3");
    ac = l_concat(a1, a2);
    ex = il_of_string("10, 20, 30, 1, 2, 3");
    il_test_equal(ac, ex);
    l_free(a1);
    l_free(a2);
    l_free(ac);
    l_free(ex);

    a1 = il_range(-9, 0);
    a2 = il_range(0, 10);
    ac = l_concat(a1, a2);
    ex = il_range(-9, 10);
    il_test_equal(ac, ex);
    l_free(a1);
    l_free(a2);
    l_free(ac);
    l_free(ex);

    a1 = il_range(-9, 0);
    a2 = il_range(0, 0);
    ac = l_concat(a1, a2);
    ex = il_range(-9, 0);
    il_test_equal(ac, ex);
    l_free(a1);
    l_free(a2);
    l_free(ac);
    l_free(ex);

    a1 = il_range(0, 0);
    a2 = il_range(0, 10);
    ac = l_concat(a1, a2);
    ex = il_range(0, 10);
    il_test_equal(ac, ex);
    l_free(a1);
    l_free(a2);
    l_free(ac);
    l_free(ex);

    a1 = il_range(0, 0);
    a2 = il_range(0, 0);
    ac = l_concat(a1, a2);
    ex = il_range(0, 0);
    il_test_equal(ac, ex);
    l_free(a1);
    l_free(a2);
    l_free(ac);
    l_free(ex);
    
    a1 = dl_of_string("10, 20, 30");
    a2 = dl_of_string("1, 2, 3");
    ac = l_concat(a1, a2);
    ex = dl_of_string("10, 20, 30, 1, 2, 3");
    dl_test_within(ac, ex);
    l_free(a1);
    l_free(a2);
    l_free(ac);
    l_free(ex);

    a1 = dl_range(-9, 0, 1);
    a2 = dl_range(0, 10, 1);
    ac = l_concat(a1, a2);
    ex = dl_range(-9, 10, 1);
    dl_test_within(ac, ex);
    l_free(a1);
    l_free(a2);
    l_free(ac);
    l_free(ex);

    a1 = dl_range(-9, 0, 1);
    a2 = dl_range(0, 0, 1);
    ac = l_concat(a1, a2);
    ex = dl_range(-9, 0, 1);
    dl_test_within(ac, ex);
    l_free(a1);
    l_free(a2);
    l_free(ac);
    l_free(ex);

    a1 = dl_range(0, 0, 1);
    a2 = dl_range(0, 10, 1);
    ac = l_concat(a1, a2);
    ex = dl_range(0, 10, 1);
    dl_test_within(ac, ex);
    l_free(a1);
    l_free(a2);
    l_free(ac);
    l_free(ex);

    a1 = dl_range(0, 0, 1);
    a2 = dl_range(0, 0, 1);
    ac = l_concat(a1, a2);
    ex = dl_range(0, 0, 1);
    dl_test_within(ac, ex);
    l_free(a1);
    l_free(a2);
    l_free(ac);
    l_free(ex);
#endif
}

List l_concat(List x, List y) {
    require_not_null(x);
    require_not_null(y);
    require_x("equal element sizes", x->s == y->s, "x->s == %d, y->s == %d", x->s, y->s);
    List result = l_create(x->s);
    for (ListNode *node = x->first; node != NULL; node = node->next) {
        l_append(result, node + 1);
    }
    for (ListNode *node = y->first; node != NULL; node = node->next) {
        l_append(result, node + 1);
    }
    return result;
}



///////////////////////////////////////////////////////////////////////////////



int l_index_fn(List list, AnyFn predicate, Any state);

static bool j_is_gt(Any element, int index, Any state) {
    IntPair *ip = element;
    int *i = state;
    return ip->j > *i;
}

static void l_index_fn_test(void) {
    printsln((String)__func__);
    List ac;
    IntPair ip;

    ac = l_create(sizeof(IntPair));
    ip = make_int_pair(1, 10);
    l_append(ac, &ip);
    ip = make_int_pair(2, 20);
    l_append(ac, &ip);
    ip = make_int_pair(3, 30);
    l_append(ac, &ip);
    l_println(ac, print_int_pair);
    
    int state = 15;
    test_equal_i(l_index_fn(ac, j_is_gt, &state), 1);

    l_free(ac);
}

int l_index_fn(List list, AnyFn predicate, Any state) {
    require_not_null(list);
    require_not_null(predicate);
    AnyIntAnyToBool fpredicate = predicate;
    int i = 0;
    for (ListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (fpredicate(node + 1, i, state)) {
            return i;
        }
    }
    return -1;
}

Any l_find(List list, AnyFn predicate, Any state) {
    require_not_null(list);
    require_not_null(predicate);
    AnyIntAnyToBool fpredicate = predicate;
    int i = 0;
    for (ListNode *node = list->first; node != NULL; node = node->next, i++) {
        // #define VALUE(node) ((Any)(((ListNode*)(node)) + 1))
        if (fpredicate(VALUE(node), i, state)) {
            return VALUE(node);
        }
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////

List l_reverse(List list);

static void l_reverse_test(void) {
    printsln((String)__func__);
    List a, ac, ex;
    int i;
    a = l_create(sizeof(int));
    i = 3; l_append(a, &i);
    i = 2; l_append(a, &i);
    i = 1; l_append(a, &i);
    ex = l_create(sizeof(int));
    i = 1; l_append(ex, &i);
    i = 2; l_append(ex, &i);
    i = 3; l_append(ex, &i);
    ac = l_reverse(a);
    l_test_equal(ac, ex);
    l_free(a);
    l_free(ac);
    l_free(ex);
#if 0
    ac = il_of_string("1, 2, 3, 4");
    ex = il_of_string("4, 3, 2, 1");
    l_reverse(ac);
    l_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_of_string("1");
    ex = il_of_string("1");
    l_reverse(ac);
    l_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = l_create(0, 1);
    ex = l_create(0, 1);
    l_reverse(ac);
    l_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
#endif
}

List l_reverse(List list) {
    require_not_null(list);
    List result = l_create(list->s);
    for (ListNode *node = list->first; node != NULL; node = node->next) {
        l_prepend(result, node + 1);
    }
    return result;
}


List l_shuffle(List list);

void printpi(Any element) {
    int *i = element;
    printi(*i);
}

static void l_shuffle_test(void) {
    printsln((String)__func__);

    int i;
    List a = l_create(sizeof(int));
    i = 1; l_append(a, &i);
    i = 2; l_append(a, &i);
    i = 3; l_append(a, &i);
    i = 4; l_append(a, &i);
    i = 5; l_append(a, &i);

    l_println(a, printpi);
    List s = l_shuffle(a);
    l_println(s, printpi);
    l_free(a);
    l_free(s);
}

List l_shuffle(List list) {
    require_not_null(list);
    int n = l_length(list);
    Array indices = ia_range(0, n);
    a_shuffle(indices);
    List result = l_create(list->s);
    for (int i = 0; i < n; i++) {
        int j = ia_get(indices, i);
        l_append(result, l_get(list, j));
    }
    a_free(indices);
    return result;
}

static CmpResult l_compare_i(ConstAny a, ConstAny b) {
    require_not_null(a);
    require_not_null(b);
    int x = *(int*)a;
    int y = *(int*)b;
    return (x < y) ? LT : ((x > y) ? GT : EQ);
}

List l_sort(List list, Comparator c);

static void l_sort_test(void) {
    printsln((String)__func__);

    int i;
    List a = l_create(sizeof(int));
    i = 9; l_append(a, &i);
    i = 2; l_append(a, &i);
    i = 6; l_append(a, &i);
    i = 7; l_append(a, &i);
    i = 2; l_append(a, &i);
    i = 4; l_append(a, &i);
    i = 3; l_append(a, &i);

    l_println(a, printpi);
    List s= l_sort(a, l_compare_i);
    l_println(s, printpi);
    l_free(a);
    l_free(s);

#if 0
    ac = il_of_string("1, 2, 3, 4, 5");
    ex = il_of_string("1, 2, 3, 4, 5");
    l_shuffle(ac);
    l_sort(ac, l_compare_i);
    l_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_of_string("1, 2, 1, 3, 2");
    ex = il_of_string("1, 1, 2, 2, 3");
    l_shuffle(ac);
    l_sort(ac, l_compare_i);
    l_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_of_string("");
    ex = il_of_string("");
    l_shuffle(ac);
    l_sort(ac, l_compare_i);
    l_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = il_of_string("-1, -2, -3, -1");
    ex = il_of_string("-3, -2, -1, -1");
    l_shuffle(ac);
    l_sort(ac, l_compare_i);
    l_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
#endif
}

List l_sort(List list, Comparator c) {
    require_not_null(list);
    require_not_null(c);
    int n = l_length(list);
    Array a = a_create(n, list->s);
    int i = 0;
    for (ListNode *node = list->first; node != NULL; node = node->next, i++) {
        a_set(a, i, node + 1);
    }
    qsort(a->a, a->n, a->s, c);
    List result = l_create(list->s);
    for (i = 0; i < n; i++) {
        l_append(result, a_get(a, i));
    }
    a_free(a);
    return result;
}

void l_insert(List list, int index, Any value);

static void l_insert_test(void) {
    printsln((String)__func__);
    List ac, ex;
    int i;

    ac = l_create(sizeof(int));
    i = 1; l_append(ac, &i);
    i = 3; l_append(ac, &i);
    i = 5; l_append(ac, &i);

    ex = l_create(sizeof(int));
    i = 9; l_append(ex, &i);
    i = 1; l_append(ex, &i);
    i = 3; l_append(ex, &i);
    i = 5; l_append(ex, &i);
    
    i = 9; l_insert(ac, 0, &i);

    l_test_equal(ac, ex);
    
    l_free(ac);
    l_free(ex);

    ac = l_create(sizeof(int));
    i = 1; l_append(ac, &i);
    i = 3; l_append(ac, &i);
    i = 5; l_append(ac, &i);

    ex = l_create(sizeof(int));
    i = 1; l_append(ex, &i);
    i = 3; l_append(ex, &i);
    i = 5; l_append(ex, &i);
    i = 9; l_append(ex, &i);
    
    i = 9; l_insert(ac, 3, &i);

    l_test_equal(ac, ex);
    l_println(ac, printpi);
    
    l_free(ac);
    l_free(ex);
}

void l_insert(List list, int index, Any value) {
    require_not_null(list);
    if (index <= 0) {
        l_prepend(list, value);
        return;
    }
    // assert: index > 0
    ListNode *node = list->first;
    int k = 1;
    while (node != NULL && k < index) {
        node = node->next;
        k++;
    }
    if (node != NULL) {
        ListNode *new_node = xcalloc(1, sizeof(ListNode*) + list->s);
        memcpy(new_node + 1, value, list->s);
        new_node->next = node->next;
        node->next = new_node;
    }
}

void l_remove(List list, int index);

static void l_remove_test(void) {
    printsln((String)__func__);
    List ac, ex;
    int i;

    ac = l_create(sizeof(int));
    ex = l_create(sizeof(int));

    l_remove(ac, -1);
    l_test_equal(ac, ex);

    l_remove(ac, 0);
    l_test_equal(ac, ex);

    l_remove(ac, 1);
    l_test_equal(ac, ex);

    i = 1; l_append(ac, &i);
    l_remove(ac, 0);
    l_test_equal(ac, ex);


    i = 1; l_append(ac, &i);
    i = 3; l_append(ac, &i);
    i = 5; l_append(ac, &i);

    i = 3; l_append(ex, &i);
    i = 5; l_append(ex, &i);
    
    l_remove(ac, -1);
    l_test_equal(ac, ex);
    
    l_remove(ac, 3);
    l_test_equal(ac, ex);
    
    l_free(ac);
    l_free(ex);

    ac = l_create(sizeof(int));
    i = 1; l_append(ac, &i);
    i = 3; l_append(ac, &i);
    i = 5; l_append(ac, &i);

    ex = l_create(sizeof(int));
    i = 3; l_append(ex, &i);
    i = 5; l_append(ex, &i);
    
    l_remove(ac, 0);
    l_test_equal(ac, ex);
    
    l_free(ac);
    l_free(ex);

    ac = l_create(sizeof(int));
    i = 1; l_append(ac, &i);
    i = 3; l_append(ac, &i);
    i = 5; l_append(ac, &i);

    ex = l_create(sizeof(int));
    i = 1; l_append(ex, &i);
    i = 3; l_append(ex, &i);
    
    l_remove(ac, 2);
    l_test_equal(ac, ex);
    
    l_free(ac);
    l_free(ex);
}

void l_remove(List list, int index) {
    require_not_null(list);
    if (list->first == NULL) return;
    // assert: list->first != NULL
    if (index <= 0) {
        ListNode *del = list->first;
        list->first = del->next;
        free(del);
        if (list->first == NULL) {
            list->last = NULL;
        }
        return;
    }
    // assert: index > 0 && list->first != NULL
    ListNode *node = list->first;
    int k = 1;
    while (node != NULL && k < index) {
        node = node->next;
        k++;
    }
    if (node != NULL && node->next != NULL) {
        ListNode *del = node->next;
        node->next = del->next;
        free(del);
        if (node->next == NULL) {
            list->last = node;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////


List l_map(List list, AnyFn f, int mapped_element_size, Any state);

static void l_inc(IntPair *element, int index, Any state, IntPair *mapped_element) {
    mapped_element->i = element->i + 1;
    mapped_element->j = element->j + 1;
}

static void l_int_pair_i(IntPair *element, int index, Any state, int *mapped_element) {
    *mapped_element = element->i;
}

static void l_map_test(void) {
    printsln((String)__func__);
    List ac, acm, ex;
    IntPair ip;

    ac = l_create(sizeof(IntPair));
    ip = make_int_pair(1, 10);
    l_append(ac, &ip);
    ip = make_int_pair(2, 20);
    l_append(ac, &ip);
    ip = make_int_pair(3, 30);
    l_append(ac, &ip);
    // l_println(ac, print_int_pair);
    
    acm = l_map(ac, l_inc, sizeof(IntPair), NULL);
    // l_println(acm, print_int_pair);

    ex = l_create(sizeof(IntPair));
    ip = make_int_pair(2, 11);
    l_append(ex, &ip);
    ip = make_int_pair(3, 21);
    l_append(ex, &ip);
    ip = make_int_pair(4, 31);
    l_append(ex, &ip);
    
    l_test_equal(acm, ex);
    l_free(acm);
    l_free(ex);

    ex = l_create(sizeof(int));
    int i = 1;
    l_append(ex, &i);
    i = 2;
    l_append(ex, &i);
    i = 3;
    l_append(ex, &i);

    acm = l_map(ac, l_int_pair_i, sizeof(int), NULL);

    l_println(ac, print_int_pair);
    l_println(acm, printpi);
    l_test_equal(acm, ex);

    l_free(ex);
    l_free(acm);
    l_free(ac);
}

List l_map(List list, AnyFn f, int mapped_element_size, Any state) {
    require_not_null(list);
    require_not_null(f);
    require("positive", mapped_element_size > 0);
    AnyIntAnyAnyToVoid ff = f;
    List result = l_create(mapped_element_size);
    int i = 0;
    for (ListNode *node = list->first; node != NULL; node = node->next, i++) {
        ListNode *mapped_node = l_append_empty(result);
        ff(node + 1, i, state, mapped_node + 1);
    }
    return result;
}

List l_map2(AnyAnyIntAnyAnyToVoid f, int mapped_element_size, Any state, List a1, List a2);

static void pair_i_j(Any element1, Any element2, int index, Any state, Any mapped_element) {
    int *i = element1;
    int *j = element2;
    IntPair *ip = mapped_element;
    ip->i = *i;
    ip->j = *j;
}

static void l_map2_test(void) {
    printsln((String)__func__);
    List a1, a2, ac, ex;
    IntPair ip;
    int i;
    
    a1 = l_create(sizeof(int));
    i = 1; l_append(a1, &i);
    i = 2; l_append(a1, &i);
    i = 3; l_append(a1, &i);
    
    a2 = l_create(sizeof(int));
    i = 10; l_append(a2, &i);
    i = 20; l_append(a2, &i);
    i = 30; l_append(a2, &i);
    
    ac = l_map2(pair_i_j, sizeof(IntPair), NULL, a1, a2);
    l_free(a1);
    l_free(a2);

    ex = l_create(sizeof(IntPair));
    ip = make_int_pair(1, 10);
    l_append(ex, &ip);
    ip = make_int_pair(2, 20);
    l_append(ex, &ip);
    ip = make_int_pair(3, 30);
    l_append(ex, &ip);

    l_test_equal(ac, ex);
    
    l_free(ac);
    l_free(ex);
}

List l_map2(AnyAnyIntAnyAnyToVoid f, int mapped_element_size, Any state, List l1, List l2) {
    require_not_null(f);
    require_not_null(l1);
    require_not_null(l2);
    require("positive", mapped_element_size > 0);
    List result = l_create(mapped_element_size);
    int i = 0;
    ListNode *n1 = l1->first;
    ListNode *n2 = l2->first;
    for (; n1 != NULL && n2 != NULL; n1 = n1->next, n2 = n2->next, i++) {
        ListNode *mapped_node = l_append_empty(result);
        f(n1 + 1, n2 + 1, i, state, mapped_node + 1);
    }
    return result;
}

List l_map3(AnyAnyAnyIntAnyAnyToVoid f, int mapped_element_size, Any state, List a1, List a2, List a3);

static void triple_i_j_k(Any element1, Any element2, Any element3, int index, Any state, Any mapped_element) {
    int *i = element1;
    int *j = element2;
    int *k = element3;
    IntTriple *ip = mapped_element;
    ip->i = *i;
    ip->j = *j;
    ip->k = *k;
}

static void triple_k(Any element, int index, Any state, Any mapped_element) {
    IntTriple *t = element;
    int *k = mapped_element;
    *k = t->k;
}

static void l_map3_test(void) {
    printsln((String)__func__);
    List a1, a2, a3, ac, ex;
    IntTriple it;
    int i;
    
    a1 = l_create(sizeof(int));
    i = 1; l_append(a1, &i);
    i = 2; l_append(a1, &i);
    i = 3; l_append(a1, &i);
    
    a2 = l_create(sizeof(int));
    i = 10; l_append(a2, &i);
    i = 20; l_append(a2, &i);
    i = 30; l_append(a2, &i);
    
    a3 = l_create(sizeof(int));
    i = 100; l_append(a3, &i);
    i = 200; l_append(a3, &i);
    i = 300; l_append(a3, &i);
    
    ac = l_map3(triple_i_j_k, sizeof(IntTriple), NULL, a1, a2, a3);
    l_free(a1);
    l_free(a2);
    l_free(a3);

    ex = l_create(sizeof(IntTriple));
    it = make_int_triple(1, 10, 100);
    l_append(ex, &it);
    it = make_int_triple(2, 20, 200);
    l_append(ex, &it);
    it = make_int_triple(3, 30, 300);
    l_append(ex, &it);

    l_test_equal(ac, ex);

    // map triple to third element
    List ak = l_map(ac, triple_k, sizeof(int), NULL);
    l_println(ak, printpi);
    l_free(ak);
    
    l_free(ac);
    l_free(ex);
}

List l_map3(AnyAnyAnyIntAnyAnyToVoid f, int mapped_element_size,
             Any state, List l1, List l2, List l3) 
{
    require_not_null(f);
    require_not_null(l1);
    require_not_null(l2);
    require_not_null(l3);
    require("positive", mapped_element_size > 0);
    List result = l_create(mapped_element_size);
    int i = 0;
    ListNode *n1 = l1->first;
    ListNode *n2 = l2->first;
    ListNode *n3 = l3->first;
    for (; n1 != NULL && n2 != NULL && n3 != NULL; 
         n1 = n1->next, n2 = n2->next, n3 = n3->next, i++) 
    {
        ListNode *mapped_node = l_append_empty(result);
        f(n1 + 1, n2 + 1, n3 + 1, i, state, mapped_node + 1);
    }
    return result;
}

void l_each(List list, AnyFn f, Any state);

static void l_inc_in_place(IntPair *element, int index, Any state) {
    element->i += 1;
    element->j += 1;
}

static void l_each_test(void) {
    printsln((String)__func__);
    List ac, ex;
    IntPair ip;

    ac = l_create(sizeof(IntPair));
    ip = make_int_pair(1, 10);
    l_append(ac, &ip);
    ip = make_int_pair(2, 20);
    l_append(ac, &ip);
    ip = make_int_pair(3, 30);
    l_append(ac, &ip);
    // l_println(ac, print_int_pair);
    
    l_each(ac, l_inc_in_place, NULL);
    // l_println(acm, print_int_pair);

    ex = l_create(sizeof(IntPair));
    ip = make_int_pair(2, 11);
    l_append(ex, &ip);
    ip = make_int_pair(3, 21);
    l_append(ex, &ip);
    ip = make_int_pair(4, 31);
    l_append(ex, &ip);
    
    l_test_equal(ac, ex);
    
    l_free(ac);
    l_free(ex);
}

void l_each(List list, AnyFn f, Any state) {
    require_not_null(list);
    require_not_null(f);
    AnyIntAnyToVoid ff = f;
    int i = 0;
    for (ListNode *node = list->first; node != NULL; node = node->next, i++) {
        ff(node + 1, i, state);
    }
}

// @todo: add tests

void l_each2(AnyAnyIntAnyToVoid f, Any state, List l1, List l2) {
    require_not_null(f);
    require_not_null(l1);
    require_not_null(l2);
    int i = 0;
    ListNode *n1 = l1->first;
    ListNode *n2 = l2->first;
    for (; n1 != NULL && n2 != NULL; n1 = n1->next, n2 = n2->next, i++) {
        f(n1 + 1, n2 + 1, i, state);
    }
}

// @todo: add tests

void l_each3(AnyAnyAnyIntAnyToVoid f, Any state, List l1, List l2, List l3) {
    require_not_null(f);
    require_not_null(l1);
    require_not_null(l2);
    require_not_null(l3);
    int i = 0;
    ListNode *n1 = l1->first;
    ListNode *n2 = l2->first;
    ListNode *n3 = l3->first;
    for (; n1 != NULL && n2 != NULL && n3 != NULL; 
         n1 = n1->next, n2 = n2->next, n3 = n3->next, i++) 
    {
        f(n1 + 1, n2 + 1, n3 + 1, i, state);
    }
}

void l_foldl(List list, AnyFn f, Any state);

static void l_sum_j(int *state, IntPair *element, int index) {
    *state += element->j;
}

static void l_foldl_test(void) {
    printsln((String)__func__);
    List ac;
    IntPair ip;

    ac = l_create(sizeof(IntPair));
    ip = make_int_pair(1, 10);
    l_append(ac, &ip);
    ip = make_int_pair(2, 20);
    l_append(ac, &ip);
    ip = make_int_pair(3, 30);
    l_append(ac, &ip);
    
    int n = 0;
    l_foldl(ac, l_sum_j, &n);
    
    test_equal_i(n, 10 + 20 + 30);
    
    l_free(ac);
}

void l_foldl(List list, AnyFn f, Any state) {
    require_not_null(list);
    require_not_null(f);
    AnyAnyIntToVoid ff = f;
    int i = 0;
    for (ListNode *node = list->first; node != NULL; node = node->next, i++) {
        ff(state, node + 1, i);
    }

}

// @todo: add tests

void l_foldl2(AnyAnyAnyIntToVoid f, Any state, List l1, List l2) {
    require_not_null(f);
    require_not_null(l1);
    require_not_null(l2);
    int i = 0;
    ListNode *n1 = l1->first;
    ListNode *n2 = l2->first;
    for (; n1 != NULL && n2 != NULL; n1 = n1->next, n2 = n2->next, i++) {
        f(state, n1 + 1, n2 + 1, i);
    }
}

// @todo: add tests

void l_foldl3(AnyAnyAnyAnyIntToVoid f, Any state, List l1, List l2, List l3) {
    require_not_null(f);
    require_not_null(l1);
    require_not_null(l2);
    require_not_null(l3);
    int i = 0;
    ListNode *n1 = l1->first;
    ListNode *n2 = l2->first;
    ListNode *n3 = l3->first;
    for (; n1 != NULL && n2 != NULL && n3 != NULL; 
         n1 = n1->next, n2 = n2->next, n3 = n3->next, i++) 
    {
        f(state, n1 + 1, n2 + 1, n3 + 1, i);
    }
}

void l_foldr(List list, AnyFn f, Any state);

static void l_minus_j(IntPair *element, int *state, int index) {
    *state = element->j - *state;
}

static void l_foldr_test(void) {
    printsln((String)__func__);
    List ac;
    IntPair ip;

    ac = l_create(sizeof(IntPair));
    ip = make_int_pair(1, 10);
    l_append(ac, &ip);
    ip = make_int_pair(2, 20);
    l_append(ac, &ip);
    ip = make_int_pair(3, 30);
    l_append(ac, &ip);
    
    int n = 0;
    l_foldr(ac, l_minus_j, &n);
    test_equal_i(n, 10 - (20 - (30 - 0)));
    
    l_free(ac);
}

void l_foldr(List list, AnyFn f, Any state) {
    require_not_null(list);
    require_not_null(f);
    AnyAnyIntToVoid ff = f;
    List reversed = l_create(list->s);
    // assert_x("same size", reversed->s == list->s, "list->s = %d, element->s = %d", list->s, reversed->s);
    for (ListNode *node = list->first; node != NULL; node = node->next) {
        l_prepend(reversed, node + 1);
    }
    int i = 0;
    for (ListNode *node = reversed->first; node != NULL; node = node->next, i++) {
        ff(node + 1, state, i);
    }
    l_free(reversed);
}

List l_filter(List list, AnyFn predicate, Any state);

static bool l_eq2(IntPair *element, int index, Any state) {
    return element->i == 2;
}

static void l_filter_test(void) {
    printsln((String)__func__);
    List ac, ac2, ex;
    IntPair ip;

    ac = l_create(sizeof(IntPair));
    ip = make_int_pair(1, 10);
    l_append(ac, &ip);
    ip = make_int_pair(2, 20);
    l_append(ac, &ip);
    ip = make_int_pair(3, 30);
    l_append(ac, &ip);
    
    ac2 = l_filter(ac, l_eq2, NULL);
    
    ex = l_create(sizeof(IntPair));
    ip = make_int_pair(2, 20);
    l_append(ex, &ip);
    
    l_test_equal(ac2, ex);
    
    l_free(ac);
    l_free(ac2);
    l_free(ex);
}

List l_filter(List list, AnyFn predicate, Any state) {
    require_not_null(list);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    List result = l_create(list->s);
    int i = 0;
    for (ListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (f(node + 1, i, state)) {
            l_append(result, node + 1);
        }
    }
    return result;
}



List l_choose(List list, AnyIntAnyAnyToBool f, int mapped_element_size, Any state);

static bool evens_times_x(Any element, int index, Any state, Any mapped_element) {
    int e = *(int*)element;
    int x = *(int*)state;
    int *me = mapped_element;
    if ((e % 2) == 0) {
        *me = x * e;
        return true;
    }
    return false;
}

static void l_choose_test(void) {
    printsln((String)__func__);
    List a, ac, ex;
    int i;
    
    a = l_create(sizeof(int));
    i = 1; l_append(a, &i);
    i = 2; l_append(a, &i);
    i = 3; l_append(a, &i);
    i = 4; l_append(a, &i);
    i = 5; l_append(a, &i);
    i = 6; l_append(a, &i);
    int x = 3;
    ac = l_choose(a, evens_times_x, sizeof(int), &x);
    ex = l_create(sizeof(int));
    i =  6; l_append(ex, &i);
    i = 12; l_append(ex, &i);
    i = 18; l_append(ex, &i);
    l_test_equal(ac, ex);

    l_free(ac);
    l_free(ex);
    l_free(a);
}

List l_choose(List list, AnyIntAnyAnyToBool f, int mapped_element_size, Any state) {
    require_not_null(f);
    require_not_null(list);
    require("positive", mapped_element_size > 0);
    List result = l_create(mapped_element_size);
    Any mapped_content = xcalloc(1, mapped_element_size);
    int i = 0;
    for (ListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (f(node + 1, i, state, mapped_content)) {
            l_append(result, mapped_content);
        }
    }
    free(mapped_content);
    return result;
}

bool l_exists(List list, AnyFn predicate, Any state);

static void l_exists_test(void) {
    printsln((String)__func__);
    List ac;
    IntPair ip;

    ac = l_create(sizeof(IntPair));
    ip = make_int_pair(1, 10);
    l_append(ac, &ip);
    ip = make_int_pair(2, 20);
    l_append(ac, &ip);
    ip = make_int_pair(3, 30);
    l_append(ac, &ip);
    // l_println(ac, print_int_pair);
    
    int state = 29;
    test_equal_b(l_exists(ac, j_is_gt, &state), true);
    state = 30;
    test_equal_b(l_exists(ac, j_is_gt, &state), false);

    l_free(ac);
}

bool l_exists(List list, AnyFn predicate, Any state) {
    require_not_null(list);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    int i = 0;
    for (ListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (f(node + 1, i, state)) {
            return true;
        }
    }
    return false;
}

bool l_forall(List list, AnyFn predicate, Any state);

static void l_forall_test(void) {
    printsln((String)__func__);
    List ac;
    IntPair ip;

    ac = l_create(sizeof(IntPair));
    ip = make_int_pair(1, 10);
    l_append(ac, &ip);
    ip = make_int_pair(2, 20);
    l_append(ac, &ip);
    ip = make_int_pair(3, 30);
    l_append(ac, &ip);
    // l_println(ac, print_int_pair);
    
    int state = 9;
    test_equal_b(l_forall(ac, j_is_gt, &state), true);
    state = 10;
    test_equal_b(l_forall(ac, j_is_gt, &state), false);

    l_free(ac);
}

bool l_forall(List list, AnyFn predicate, Any state) {
    require_not_null(list);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    int i = 0;
    for (ListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (!f(node + 1, i, state)) {
            return false;
        }
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Testing

bool l_test_equal_file_line(const char *file, const char *function, int line, List a, List e) {
    base_count_check();
    if (a == NULL) {
        printf("%s, line %d: Actual list is NULL\n", file, line);
        return false;        
    }
    if (e == NULL) {
        printf("%s, line %d: Expected list is NULL\n", file, line);
        return false;        
    }
    if (a->s != e->s) {
        printf("%s, line %d: Actual element size %d "
            "differs from expected element size %d\n", file, line, a->s, e->s);
        return false;
    }
    ListNode *an = a->first;
    ListNode *en = e->first;
    int i = 0;
    for (; an != NULL && en != NULL; an = an->next, en = en->next, i++) {
        Any av = an + 1;
        Any ev = en + 1;
        if (memcmp(av, ev, a->s) != 0) {
            printf("%s, line %d: Actual value differs from expected value at index %d.\n", file, line, i);
            return false;
        }
    }

    if (an != NULL || en != NULL) {
        printf("%s, line %d: Actual and expected lengths differ\n", file, line);
        return false;
    }

    printf("%s, line %d: Test passed.\n", file, line);
    base_count_success();
    return true;
}

///////////////////////////////////////////////////////////////////////////////

void l_test_all(void) {
    l_create_test();
    l_of_buffer_test();
    l_fn_test();
    l_copy_test();
    l_sub_test();
    l_of_a_test();
    l_iterator_test();
    l_concat_test();
    l_index_fn_test();
    l_reverse_test();
    l_shuffle_test();
    l_sort_test();
    l_insert_test();
    l_remove_test();
    l_map_test();
    l_map2_test();
    l_map3_test();
    l_each_test();
    l_foldl_test();
    l_foldr_test();
    l_filter_test();
    l_choose_test();
    l_exists_test();
    l_forall_test();
}

#if 0
int main(void) {
    base_init();
    l_test_all();
    return 0;
}
#endif
