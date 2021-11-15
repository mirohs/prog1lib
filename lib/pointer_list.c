/*
@author Michael Rohs
@date 15.10.2015
@copyright Apache License, Version 2.0
*/

#include "list.h"
#include "array.h"
#include "pointer_list.h"
#include "string.h"
#include "string_list.h"

List pl_create() {
    ListHead *lh = xcalloc(1, sizeof(ListHead));
    lh->s = sizeof(Any); // content size
    return lh;
}

#if 0
List pl_repeat(int n, Any value);

static void pl_repeat_test(void) {
    printsln((Any)__func__);
    List ac, ex;

    ac = pl_repeat(3, "abc");
    ex = pl_create();
    pl_append(ex, "abc");
    pl_append(ex, "abc");
    pl_append(ex, "abc");
    // pl_println(ac);
    // pl_println(ex);
    pl_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = pl_repeat(0, "abc");
    ex = pl_create();
    pl_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
}

List pl_repeat(int n, Any value) {
    require("non-negative length", n >= 0);
    List list = pl_create();
    for (int i = 0; i < n; i++) {
        pl_append(list, value);
    }
    return list;
}
#endif

void pl_free(List list) {
    if (list != NULL) {
        require_element_size_pointer(list);
        for (PointerListNode *node = list->first; node != NULL; node = node->next) {
            free(node->value);
        }
        l_free(list);
    }
}

void pl_free_with_destructor(List list, AnyFn element_destructor) {
    if (list != NULL) {
        require_element_size_pointer(list);
        require_not_null(element_destructor);
        AnyToVoid f = element_destructor;
        for (PointerListNode *node = list->first; node != NULL; node = node->next) {
            f(node->value);
        }
        l_free(list);
    }
}

Any pl_get(List list, int index) {
    require_not_null(list);
    require_element_size_pointer(list);
    int i = 0;
    for (PointerListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (i == index) {
            return node->value;
        }
    }
    require_x("index in range", false, "index == %d", index);
    return 0;
}

void pl_set(List list, int index, Any value) {
    require_not_null(list);
    require_element_size_pointer(list);
    int i = 0;
    for (PointerListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (i == index) {
            node->value = value;
            return;
        }
    }
    require_x("index in range", false, "index == %d", index);
}

///////////////////////////////////////////////////////////////////////////////

static void pl_iterator_test(void) {
    printsln((Any)__func__);
    
    // various ways of iterating a list:
    List ac = pl_create();
    pl_append(ac, "a");
    pl_append(ac, "b");
    pl_append(ac, "c");
    
    ListIterator iter = l_iterator(ac);
    while (l_has_next(iter)) {
        Any s = pl_next(&iter);
        printsln(s);
    }
    
    // PointerListNode *first = (PointerListNode*)ac->first;
    for (PointerListNode *node = ac->first; node != NULL; node = node->next) {
        Any s = node->value;
        printsln(s);
    }
    
#if 0
    while (iter = pl_next(iter)) {
        Any d = pl_current(iter);
        printiln(d);
    }
        
    for (AnyListIterator iter = pl_iterator(ac); pl_has_current(iter); iter = pl_next(iter)) {
        Any d = pl_current(iter);
        printiln(d);
    }
#endif
    
    l_free(ac);
}

Any pl_next(ListIterator *iter) {
    require("iterator has more values", *iter);
    PointerListNode *node = *(PointerListNode**)iter;
    Any value = node->value;
    *iter = (*iter)->next;
    return value;
}

///////////////////////////////////////////////////////////////////////////////

static void pl_prepend_append_test(void) {
    printsln((Any)__func__);
    List ac, ex;
    ac = pl_create();
    pl_append(ac, "11");
    pl_append(ac, "22");
    pl_append(ac, "33");
    ex = pl_create();
    pl_prepend(ex, "33");
    pl_prepend(ex, "22");
    pl_prepend(ex, "11");
    pl_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
}

void pl_append(List list, Any value) {
    require_not_null(list);
    require_element_size_pointer(list);
    // allocate memory for next-pointer and content
    PointerListNode *node = xcalloc(1, sizeof(ListNode*) + sizeof(value));
    // copy content, leave next-pointer NULL
    node->value = value;
    // if this is the first element of the list set first pointer
    if (list->first == NULL) {
        list->first = node;
    }
    // if there is a last element, update its next-pointer
    if (list->last != NULL) {
        PointerListNode *list_last = list->last;
        list_last->next = node;
    }
    // the new node is the last node of the list
    list->last = node;
}

void pl_prepend(List list, Any value) {
    require_not_null(list);
    require_element_size_pointer(list);
    // allocate memory for next-pointer and content
    PointerListNode *node = xcalloc(1, sizeof(ListNode*) + sizeof(value));
    // copy content, leave next-pointer NULL
    node->value = value;
    node->next = list->first;
    list->first = node;
    if (list->last == NULL) {
        list->last = node;
    }
}

static void print_elem(String elem) {
    prints(elem);
}

static void pl_print_test(void) {
    printsln((Any)__func__);
    
    List ac = pl_create();
    pl_append(ac, "a");
    pl_append(ac, "b");
    pl_append(ac, "c");
    
    pl_print(ac, print_elem);
    pl_println(ac, print_elem);
    
    l_free(ac);
}

void pl_print(List list, AnyFn print_element) {
    require_not_null(list);
    require_element_size_pointer(list);
    require_not_null(print_element);
    AnyToVoid f = print_element;
    PointerListNode *node = (PointerListNode*)list->first;
    printf("[");
    if (node != NULL) {
        f(node->value);
        node = node->next;
    }
    for (; node != NULL; node = node->next) {
        prints(", ");
        f(node->value);
    }
    printf("]");
}

void pl_println(List list, AnyFn print_element) {
    require_not_null(list);
    require_element_size_pointer(list);
    require_not_null(print_element);
    pl_print(list, print_element);
    printf("\n");
}

static void pl_contains_test(void) {
    printsln((Any)__func__);
    String l1 = "10";
    String l2 = "20";
    String l3 = s_create("20");
    List list = pl_create();
    pl_append(list, l1);
    pl_append(list, l2);
    test_equal_b(pl_contains(list, l1), true);
    test_equal_b(pl_contains(list, l2), true);
    test_equal_b(pl_contains(list, l3), false);
    l_free(list);
    s_free(l3);
}

bool pl_contains(List list, Any value) {
    require_not_null(list);
    require_element_size_pointer(list);
    for (PointerListNode *node = list->first; node != NULL; node = node->next) {
        if (node->value == value) {
            return true;
        }
    }
    return false;
}

static void pl_index_test(void) {
    printsln((Any)__func__);
    String l1 = "10";
    String l2 = "20";
    String l3 = s_create("20");
    List list = pl_create();
    pl_append(list, l1);
    pl_append(list, l2);
    int i = pl_index(list, l3);
#if 1
    if (i < 0) {
        printsln("value not found");
    } else {
        printf("value found at index %d\n", i);
    }
#endif
    test_equal_i(i, -1);
    test_equal_i(pl_index(list, l1), 0);
    test_equal_i(pl_index(list, l2), 1);
    l_free(list);
    s_free(l3);
}

int pl_index(List list, Any value) {
    require_not_null(list);
    require_element_size_pointer(list);
    int i = 0;
    for (PointerListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (node->value == value) {
            return i;
        }
    }
    return -1;
}

static void pl_index_from_test(void) {
    printsln((Any)__func__);
    List a = sl_of_string("10, 20, 30, 40, 50");
    test_equal_i(pl_index_from(a, "20", 0), 1);
    test_equal_i(pl_index_from(a, "20", 1), 1);
    test_equal_i(pl_index_from(a, "20", 2), -1);
    test_equal_i(pl_index_from(a, "30", -1), 2);
    test_equal_i(pl_index_from(a, "60", 0), -1);
    pl_free(a);
}

int pl_index_from(List list, Any value, int from) {
    require_not_null(list);
    require_element_size_pointer(list);
    if (from < 0) from = 0;
    int i = 0;
    for (PointerListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (i >= from && s_equals(node->value, value)) {
            return i;
        }
    }
    return -1;
}

static bool isDogOrCat(Any s, int index, Any x) {
    return s_equals(s, "dog") || s_equals(s, "cat");
}

static bool isShorterThan(Any s, int index, int *n) {
    return s_length(s) < *n;
}

static void pl_index_fn_test(void) {
    printsln((Any)__func__);
    List a = sl_of_string("test, dog, a, cat");
    test_equal_i(pl_index_fn(a, isDogOrCat, NULL), 1);
    int n = 3;
    test_equal_i(pl_index_fn(a, isShorterThan, &n), 2);
    sl_free(a);
}

int pl_index_fn(List list, AnyFn predicate, Any x) {
    require_not_null(list);
    require_element_size_pointer(list);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    int i = 0;
    for (PointerListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (f(node->value, i, x)) {
            return i;
        }
    }
    return -1;
}

// @todo: add test

Any pl_find(List list, AnyFn predicate, Any x) {
    require_not_null(list);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    int i = 0;
    for (PointerListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (f(node->value, i, x)) {
            return node->value;
        }
    }
    return NULL;
}

static void pl_insert_test(void) {
    printsln((Any)__func__);
    List ac, ex;

    ac = sl_of_string("1, 2, 3, 4, 5");
    pl_insert(ac, 0, s_create("9"));
    ex = sl_of_string("9, 1, 2, 3, 4, 5");
    sl_test_equal(ac, ex);
    pl_free(ac);
    pl_free(ex);
    
    ac = sl_of_string("1, 2, 3, 4, 5");
    pl_insert(ac, 5, s_create("9"));
    ex = sl_of_string("1, 2, 3, 4, 5, 9");
    sl_test_equal(ac, ex);
    pl_free(ac);
    pl_free(ex);
    
    ac = sl_of_string("1, 2, 3, 4, 5");
    pl_insert(ac, 3, s_create("9"));
    ex = sl_of_string("1, 2, 3, 9, 4, 5");
    sl_test_equal(ac, ex);
    pl_free(ac);
    pl_free(ex);
    
    ac = sl_of_string("1");
    pl_insert(ac, -1, s_create("9"));
    ex = sl_of_string("9, 1");
    sl_test_equal(ac, ex);
    pl_free(ac);
    pl_free(ex);

    ac = sl_of_string("1");
    pl_insert(ac, 1, s_create("9"));
    ex = sl_of_string("1, 9");
    sl_test_equal(ac, ex);
    pl_free(ac);
    pl_free(ex);

    ac = pl_create();
    pl_insert(ac, 0, s_create("9"));
    ex = sl_of_string("9");
    sl_test_equal(ac, ex);
    pl_free(ac);
    pl_free(ex);
}

void pl_insert(List list, int index, Any value) {
    require_not_null(list);
    require_element_size_pointer(list);
    l_insert(list, index, &value);
}

static void pl_remove_test(void) {
    printsln((Any)__func__);
    List ac, ex;
    Any s;

    ac = sl_of_string("1, 2, 3, 4, 5, 6");
    s = pl_get(ac, 0);
    s_free(s);
    pl_remove(ac, 0);
    ex = sl_of_string("2, 3, 4, 5, 6");
    sl_test_equal(ac, ex);
    pl_free(ac);
    pl_free(ex);
    
    ac = sl_of_string("1, 2, 3, 4, 5, 6");
    s = pl_get(ac, 5);
    s_free(s);
    pl_remove(ac, 5);
    ex = sl_of_string("1, 2, 3, 4, 5");
    sl_test_equal(ac, ex);
    pl_free(ac);
    pl_free(ex);
    
    ac = sl_of_string("1, 2, 3, 4, 5, 6");
    s = pl_get(ac, 3);
    s_free(s);
    pl_remove(ac, 3);
    ex = sl_of_string("1, 2, 3, 5, 6");
    sl_test_equal(ac, ex);
    pl_free(ac);
    pl_free(ex);
    
    ac = sl_of_string("1");
    s = pl_get(ac, 0);
    s_free(s);
    pl_remove(ac, -1);
    ex = sl_of_string("");
    sl_test_equal(ac, ex);
    pl_free(ac);
    pl_free(ex);
    
    ac = sl_of_string("1");
    pl_remove(ac, 1);
    ex = sl_of_string("1");
    sl_test_equal(ac, ex);
    pl_free(ac);
    pl_free(ex);
    
    ac = sl_of_string("1");
    s = pl_get(ac, 0);
    s_free(s);
    pl_remove(ac, 0);
    ex = pl_create();
    sl_test_equal(ac, ex);
    pl_free(ac);
    l_free(ex);
    
    ac = sl_of_string("");
    ex = pl_create();
    sl_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
}

void pl_remove(List list, int index) {
    require_not_null(list);
    require_element_size_pointer(list);
    l_remove(list, index);
}

static Any pl_upper_case_free(String s, int index, Any x) {
    String u = s_upper_case(s);
    s_free(s);
    return u;
}

static void pl_each_test(void) {
    printsln((Any)__func__);
    List ac, ex;
    
    ac = sl_of_string("a, b, c, d");
    pl_each(ac, pl_upper_case_free, NULL);
    ex = sl_of_string("A, B, C, D");
    sl_test_equal(ac, ex);
    sl_free(ac);
    sl_free(ex);
    
    ac = sl_of_string("");
    pl_each(ac, pl_upper_case_free, NULL);
    ex = sl_of_string("");
    sl_test_equal(ac, ex);
    sl_free(ac);
    sl_free(ex);
}

void pl_each(List list, AnyFn f, Any x) {
    require_not_null(list);
    require_element_size_pointer(list);
    require_not_null(f);
    AnyIntAnyToAny ff = f;
    int i = 0;
    for (PointerListNode *node = list->first; node != NULL; node = node->next, i++) {
        node->value = ff(node->value, i, x);
    }
}

static Any append_each(String element, int index, String x) {
    return s_concat(element, x);
}

static void pl_map_test(void) {
    printsln((Any)__func__);
    List l, ac, ex;
    
    l = sl_of_string("a, b, c, d");
    ac = pl_map(l, append_each, "...!");
    ex = sl_of_string("a...!, b...!, c...!, d...!");
    sl_test_equal(ac, ex);
    pl_free(l);
    pl_free(ac);
    pl_free(ex);
    
    l = sl_of_string("a, b, c, d");
    ac = pl_map(l, append_each, "");
    ex = sl_of_string("a, b, c, d");
    sl_test_equal(ac, ex);
    pl_free(l);
    pl_free(ac);
    pl_free(ex);
}

List pl_map(List list, AnyFn f, Any x) {
    require_not_null(list);
    require_element_size_pointer(list);
    require_not_null(f);
    AnyIntAnyToAny ff = f;
    List result = pl_create();
    int i = 0;
    for (PointerListNode *node = list->first; node != NULL; node = node->next, i++) {
        pl_append(result, ff(node->value, i, x));
    }
    return result;
}

static Any fold_concat_free(Any state, Any element, int index) {
    Any s = s_concat(state, element);
    s_free(state);
    return s;
}

static void pl_foldl_test(void) {
    printsln((Any)__func__);
    List l;
    Any s;
    
    l = sl_of_string("a, b, c");    
    s = pl_foldl(l, fold_concat_free, s_copy("init"));
    test_equal_s(s, "initabc");
    s_free(s);
    pl_free(l);

    l = sl_of_string("");    
    s = pl_foldl(l, fold_concat_free, s_copy("init"));
    test_equal_s(s, "init");
    s_free(s);
    pl_free(l);
}

Any pl_foldl(List list, AnyFn f, Any state) {
    require_not_null(list);
    require_element_size_pointer(list);
    require_not_null(f);
    AnyAnyIntToAny ff = f;
    int i = 0;
    for (PointerListNode *node = list->first; node != NULL; node = node->next, i++) {
        state = ff(state, node->value, i);
    }
    return state;
}

static String pl_concat_r(String element, String state, int index) {
    String s = s_concat(element, state);
    s_free(state);
    return s;
}

static void pl_foldr_test(void) {
    printsln((Any)__func__);
    
    List l;
    Any s;
    
    l = sl_of_string("a, b, c");    
    s = pl_foldr(l, pl_concat_r, s_copy("X"));
    test_equal_s(s, "abcX");
    s_free(s);
    pl_free(l);

    l = sl_of_string("");    
    s = pl_foldr(l, pl_concat_r, s_copy("X"));
    test_equal_s(s, "X");
    s_free(s);
    pl_free(l);
}

Any pl_foldr(List list, AnyFn f, Any init) {
    require_not_null(list);
    require_element_size_pointer(list);
    require_not_null(f);
    AnyAnyIntToAny ff = f;
    List rev = l_reverse(list);
    int i = l_length(list) - 1;
    for (PointerListNode *node = rev->first; node != NULL; node = node->next, i--) {
        init = ff(node->value, init, i);
    }
    l_free(rev);
    return init;
}

static bool f_ends_with(String element, int index, String x) {
    return s_ends_with(element, x);
}

static void pl_filter_test(void) {
    printsln((Any)__func__);
    List l, ac, ex;

    l = sl_of_string("Kürbis, Brombeere, Heidelbeere, Himbeere, Apfel");
    ac = pl_filter(l, f_ends_with, "beere");
    ex = sl_of_string("Brombeere, Heidelbeere, Himbeere");
    sl_test_equal(ac, ex);
    pl_free(l);
    l_free(ac);
    pl_free(ex);
}

List pl_filter(List list, AnyFn predicate, Any x) {
    require_not_null(list);
    require_element_size_pointer(list);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    List result = pl_create();
    int i = 0;
    for (PointerListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (f(node->value, i, x)) {
            pl_append(result, node->value);
        }
    }
    return result;
}

static String ends_width_0_append(String element, int index, String x) {
    if (s_ends_with(element, "0")) {
        return s_concat(element, x);
    }
    return NULL;
}

static void pl_choose_test(void) {
    printsln((Any)__func__);
    List l, ac, ex;
    l = sl_of_string("10, 2, 30, 4, 50, 6");
    printiln(l_element_size(l));
    printiln(sizeof(Any));
    ac = pl_choose(l, ends_width_0_append, "x");
    ex = sl_of_string("10x, 30x, 50x");
    sl_test_equal(ac, ex);
    sl_free(l);
    sl_free(ac);
    sl_free(ex);
}

List pl_choose(List list, AnyFn f, Any x) {
    require_not_null(list);
    require_element_size_pointer(list);
    require_not_null(f);
    AnyIntAnyToAny ff = f;
    List result = pl_create();
    int i = 0;
    for (PointerListNode *node = list->first; node != NULL; node = node->next, i++) {
        Any a = ff(node->value, i, x);
        if (a != NULL) {
            pl_append(result, a);
        }
    }
    return result;
}

static bool pl_element_eq(String element, int index, String x) {
    return s_equals(element, x);
}

static bool pl_element_ne(String element, int index, String x) {
    return !s_equals(element, x);
}

static void pl_exists_test(void) {
    printsln((Any)__func__);
    List l = sl_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(pl_exists(l, pl_element_eq, "3"), true);
    test_equal_b(pl_exists(l, pl_element_eq, "9"), false);
    test_equal_b(pl_exists(l, pl_element_ne, "9"), true);
    sl_free(l);
}

bool pl_exists(List list, AnyFn predicate, Any x) {
    require_not_null(list);
    require_element_size_pointer(list);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    int i = 0;
    for (PointerListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (f(node->value, i, x)) {
            return true;
        }
    }
    return false;
}

static void pl_forall_test(void) {
    printsln((Any)__func__);
    List l = sl_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(pl_forall(l, pl_element_eq, "3"), false);
    test_equal_b(pl_forall(l, pl_element_ne, "9"), true);
    pl_free(l);
}

bool pl_forall(List list, AnyFn predicate, Any x) {
    require_not_null(list);
    require_element_size_pointer(list);
    require_not_null(predicate);
    AnyIntAnyToBool f = predicate;
    int i = 0;
    for (PointerListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (!f(node->value, i, x)) {
            return false;
        }
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////

bool pl_test_equal_file_line(const char *file, const char *function, int line, List a, List e) {
    base_count_check();

    if (a == NULL) {
        printf("%s, line %d: Actual list is NULL\n", file, line);
        return false;        
    }
    if (e == NULL) {
        printf("%s, line %d: Expected list is NULL\n", file, line);
        return false;        
    }
    if (a->s != sizeof(Any)) {
        printf("%s, line %d: Actual list is not a pointer list (element size %d)\n", 
                file, line, a->s);
        return false;
    }
    if (e->s != sizeof(Any)) {
        printf("%s, line %d: Expected list is not a pointer list (element size %d)\n", 
                file, line, e->s);
        return false;
    }
    ListNode *an = a->first;
    ListNode *en = e->first;
    int i = 0;
    for (; an != NULL && en != NULL; an = an->next, en = en->next, i++) {
        Any av = *(Any*)(an + 1);
        Any ev = *(Any*)(en + 1);
        if (av != ev) {
            printf("%s, line %d: Actual pointer \"%p\" differs from expected pointer \"%p\" at index %d.\n", 
                    file, line, av, ev, i);
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

void pl_test_all(void) {
//    pl_repeat_test();
    pl_prepend_append_test();
    pl_iterator_test();
    pl_print_test();
    pl_contains_test();
    pl_index_test();
    pl_index_from_test();
    pl_index_fn_test();
#if 0
    pl_sort_test();
    pl_sort_dec_test();
#endif
    pl_insert_test();
    pl_remove_test();
    pl_each_test();
    pl_map_test();
    pl_foldl_test();
    pl_foldr_test();
    pl_filter_test();
    pl_choose_test();
    pl_exists_test();
    pl_forall_test();
}

#if 0
int main(void) {
    base_init();
    pl_test_all();
    return 0;
}
#endif
