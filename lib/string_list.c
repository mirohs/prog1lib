/*
@author Michael Rohs
@date 15.10.2015, 6.10.2020
@copyright Apache License, Version 2.0
*/

#include "list.h"
#include "int_list.h"
#include "string_list.h"
#include "array.h"
#include "string.h"

List sl_create() {
    ListHead *lh = xcalloc(1, sizeof(ListHead));
    lh->s = sizeof(String); // content size
    return lh;
}

List sl_repeat(int n, String value);

static void sl_repeat_test(void) {
    printsln((String)__func__);
    List ac, ex;

    ac = sl_repeat(3, "abc");
    ex = sl_create();
    sl_append(ex, "abc");
    sl_append(ex, "abc");
    sl_append(ex, "abc");
    // sl_println(ac);
    // sl_println(ex);
    sl_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);

    ac = sl_repeat(0, "abc");
    ex = sl_create();
    sl_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
}

List sl_repeat(int n, String value) {
    require("not negative", n >= 0);
    require_not_null(value);
    List list = sl_create();
    for (int i = 0; i < n; i++) {
        sl_append(list, value);
    }
    return list;
}

List sl_of_string(String s);

static void sl_of_string_test(void) {
    printsln((String)__func__);
    List ac, ex;

    ac = sl_of_string("1, 2, 3");
    ex = sl_create();
    sl_append(ex, "1");
    sl_append(ex, "2");
    sl_append(ex, "3");
    sl_test_equal(ac, ex);
    sl_free(ac);
    l_free(ex);

    ac = sl_of_string("1    ,      2,         3  ");
    ex = sl_create();
    sl_append(ex, "1    ");
    sl_append(ex, "2");
    sl_append(ex, "3  ");
    sl_test_equal(ac, ex);
    sl_free(ac); 
    l_free(ex);

    ac = sl_of_string("1    ,      2");
    ex = sl_create();
    sl_append(ex, "1    ");
    sl_append(ex, "2");
    sl_test_equal(ac, ex);
    sl_free(ac);
    l_free(ex);

    ac = sl_of_string("");
    ex = sl_create();
//    sl_append(ex, "");
    sl_test_equal(ac, ex);
    sl_free(ac);
    l_free(ex);

    ac = sl_of_string("123");
    ex = sl_create();
    sl_append(ex, "123");
    sl_test_equal(ac, ex);
    sl_free(ac);
    l_free(ex);
}

List sl_of_string(String s) {
    require_not_null(s);
    List list = sl_create();
    char *t = s;
    char *start = s;
    while (*t != '\0') {
        while (*t != ',' && *t != '\0') t++;
        if (*t == '\0') break;
        // assert: *t is comma
        sl_append(list, s_sub(start, 0, t - start));
        t++; // skip comma
        while (*t == ' ' || *t == '\t' || *t == '\n' || *t == '\r') t++; // skip whitespace
        start = t;
    }
    // don't append the empty string
    if (*start) {
        sl_append(list, s_sub(start, 0, t - start));
    }
    return list;
}

List sl_split(String s, char separator);

static void sl_split_test(void) {
    printsln((String)__func__);
    List ac, ex;

    ac = sl_split("1 2 3", ' ');
    ex = sl_create();
    sl_append(ex, "1");
    sl_append(ex, "2");
    sl_append(ex, "3");
    sl_test_equal(ac, ex);
    sl_free(ac);

    ac = sl_split("1+2+3", '+');
    sl_test_equal(ac, ex);
    sl_free(ac); 
    l_free(ex);

    ac = sl_split("1+2+3", ' ');
    ex = sl_create();
    sl_append(ex, "1+2+3");
    sl_test_equal(ac, ex);
    sl_free(ac);
    l_free(ex);

    ac = sl_split("1  2", ' ');
    ex = sl_create();
    sl_append(ex, "1");
    sl_append(ex, "");
    sl_append(ex, "2");
    sl_test_equal(ac, ex);
    sl_free(ac);
    l_free(ex);
}

List sl_split(String s, char separator) {
    require_not_null(s);
    List list = sl_create();
    require("valid separator", separator != '\0');
    char *t = s;
    char *start = s;
    while (*t != '\0') {
        while (*t != separator && *t != '\0') t++;
        if (*t == '\0') break;
        // assert: *t is separator
        sl_append(list, s_sub(start, 0, t - start));
        t++; // skip separator
        start = t;
    }
    sl_append(list, s_sub(start, 0, t - start));
    return list;
}

String s_join(List list, char joiner);

static void s_join_test(void) {
    printsln((String)__func__);
    List l;
    String ac, ex;

    l = sl_of_string("1, 2, 3");
    ac = s_join(l, '+');
    ex = "1+2+3";
    test_equal_s(ac, ex);
    s_free(ac);
    sl_free(l);

    l = sl_of_string("1");
    ac = s_join(l, '+');
    ex = "1";
    test_equal_s(ac, ex);
    s_free(ac);
    sl_free(l);

    l = sl_of_string("");
    ac = s_join(l, '+');
    ex = "";
    test_equal_s(ac, ex);
    s_free(ac);
    sl_free(l);
}

String s_join(List list, char joiner) {
    require_not_null(list);
    require_element_size_string(list);
    require("valid joiner", joiner != '\0');
    int numberOfElements = 0;
    int numberOfCharacters = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next) {
        String s = node->value;
        numberOfElements++;
        numberOfCharacters += s_length(s);
    }
    if (numberOfElements <= 0) {
        numberOfCharacters = 1; // special case of converting empty list to empty string ""
    } else {
        numberOfCharacters += numberOfElements; // joiners between elements and terminating '\0'
    }
    String result = xmalloc(numberOfCharacters);
    String p = result;
    for (StringListNode *node = list->first; node != NULL; node = node->next) {
        String s = node->value;
        int n = s_length(s);
        memcpy(p, s, n);
        p += n;
        if (node->next != NULL) {
            *p++ = joiner;
        }
    }
    *p = '\0';
    return result;
}

void sl_free(List list) {
    if (list != NULL) {
        require_element_size_string(list);
        for (StringListNode *node = list->first; node != NULL; node = node->next) {
            s_free(node->value);
        }
        l_free(list);
    }
}

String sl_get(List list, int index) {
    require_not_null(list);
    require_element_size_string(list);
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (i == index) {
            return node->value;
        }
    }
    require_x("index in range", false, "index == %d", index);
    return 0;
}

void sl_set(List list, int index, String value) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(value);
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (i == index) {
            node->value = value;
            return;
        }
    }
    require_x("index in range", false, "index == %d", index);
}

///////////////////////////////////////////////////////////////////////////////

static void sl_iterator_test(void) {
    printsln((String)__func__);
    
    List ac = sl_of_string("1, 2, 3, 4, 5");
    
    // various ways of iterating a list:
    
    ListIterator iter = l_iterator(ac);
    while (l_has_next(iter)) {
        String s = sl_next(&iter);
        printsln(s);
    }
    
    // StringListNode *first = (StringListNode*)ac->first;
    for (StringListNode *node = ac->first; node != NULL; node = node->next) {
        String s = node->value;
        printsln(s);
    }
    
#if 0
    while (iter = sl_next(iter)) {
        String d = sl_current(iter);
        printiln(d);
    }
        
    for (StringListIterator iter = sl_iterator(ac); sl_has_current(iter); iter = sl_next(iter)) {
        String d = sl_current(iter);
        printiln(d);
    }
#endif
    
    sl_free(ac);
}

String sl_next(ListIterator *iter) {
    require("iterator has more values", *iter);
    StringListNode *node = *(StringListNode**)iter;
    String value = node->value;
    *iter = (*iter)->next;
    return value;
}

///////////////////////////////////////////////////////////////////////////////

static void sl_prepend_append_test(void) {
    printsln((String)__func__);
    List ac, ex;
    ac = sl_create();
    sl_append(ac, "11");
    sl_append(ac, "22");
    sl_append(ac, "33");
    ex = sl_create();
    sl_prepend(ex, "33");
    sl_prepend(ex, "22");
    sl_prepend(ex, "11");
    sl_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
}

void sl_append(List list, String value) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(value);
    // allocate memory for next-pointer and content
    StringListNode *node = xcalloc(1, sizeof(ListNode*) + sizeof(value));
    // copy content, leave next-pointer NULL
    node->value = value;
    // if this is the first element of the list set first pointer
    if (list->first == NULL) {
        list->first = node;
    }
    // if there is a last element, update its next-pointer
    if (list->last != NULL) {
        StringListNode *list_last = list->last;
        list_last->next = node;
    }
    // the new node is the last node of the list
    list->last = node;
}

void sl_prepend(List list, String value) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(value);
    // allocate memory for next-pointer and content
    StringListNode *node = xcalloc(1, sizeof(ListNode*) + sizeof(value));
    // copy content, leave next-pointer NULL
    node->value = value;
    node->next = list->first;
    list->first = node;
    if (list->last == NULL) {
        list->last = node;
    }
}

void sl_print(List list) {
    require_not_null(list);
    require_element_size_string(list);
    StringListNode *node = (StringListNode*)list->first;
    printf("[");
    if (node != NULL) {
        printf("%s", node->value);
        node = node->next;
    }
    for (; node != NULL; node = node->next) {
        printf(", %s", node->value);
    }
    printf("]");
}

void sl_println(List list) {
    require_not_null(list);
    require_element_size_string(list);
    sl_print(list);
    printf("\n");
}

bool sl_contains(List list, String value);

static void sl_contains_test(void) {
    printsln((String)__func__);
    List list = sl_of_string("10, 20, 30");
    test_equal_b(sl_contains(list, "10"), true);
    test_equal_b(sl_contains(list, "11"), false);
    test_equal_b(sl_contains(list, "30"), true);
    sl_free(list);
}

bool sl_contains(List list, String value) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(value);
    for (StringListNode *node = list->first; node != NULL; node = node->next) {
        if (s_equals(node->value, value)) {
            return true;
        }
    }
    return false;
}

int sl_index(List list, String value);

static void sl_index_test(void) {
    printsln((String)__func__);
    List list = sl_of_string("1, 2, 3");
    int i = sl_index(list, "0");
#if 1
    if (i < 0) {
        printsln("value not found");
    } else {
        printf("value found at index %d\n", i);
    }
#endif
    test_equal_i(i, -1);
    test_equal_i(sl_index(list, "1"), 0);
    test_equal_i(sl_index(list, "2"), 1);
    
    sl_free(list);
}

int sl_index(List list, String value) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(value);
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (s_equals(node->value, value)) {
            return i;
        }
    }
    return -1;
}

int sl_index_from(List list, String value, int from);

static void sl_index_from_test(void) {
    printsln((String)__func__);
    List a = sl_of_string("10, 20, 30, 40, 50");
    test_equal_i(sl_index_from(a, "20", 0), 1);
    test_equal_i(sl_index_from(a, "20", 1), 1);
    test_equal_i(sl_index_from(a, "20", 2), -1);
    test_equal_i(sl_index_from(a, "30", -1), 2);
    test_equal_i(sl_index_from(a, "60", 0), -1);
    sl_free(a);
}

int sl_index_from(List list, String value, int from) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(value);
    if (from < 0) from = 0;
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (i >= from && s_equals(node->value, value)) {
            return i;
        }
    }
    return -1;
}

int sl_index_fn(List list, StringIntStringToBool predicate, String x);

static bool isDogOrCat(String s, int index, String x) {
    return s_equals(s, "dog") || s_equals(s, "cat");
}

static bool isShorterThan3(String s, int index, String x) {
    return s_length(s) < 3;
}

static void sl_index_fn_test(void) {
    printsln((String)__func__);
    List a = sl_of_string("test, dog, a, cat");
    test_equal_i(sl_index_fn(a, isDogOrCat, NULL), 1);
    test_equal_i(sl_index_fn(a, isShorterThan3, NULL), 2);
    sl_free(a);
}

int sl_index_fn(List list, StringIntStringToBool predicate, String x) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(predicate);
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (predicate(node->value, i, x)) {
            return i;
        }
    }
    return -1;
}

String sl_find(List list, StringIntStringToBool predicate, String x);

String sl_find(List list, StringIntStringToBool predicate, String x) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(predicate);
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (predicate(node->value, i, x)) {
            return node->value;
        }
    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////

static CmpResult String_compare(ConstAny a, ConstAny b) {
    require_not_null(a);
    require_not_null(b);
    String x = *(String*)a;
    String y = *(String*)b;
    return s_compare(x, y);
}

List sl_sort(List list);

static void sl_sort_test(void) {
    printsln((String)__func__);
    List ac, ex, as;

    ac = sl_of_string("5, 4, 3, 2, 1");
    ex = sl_of_string("1, 2, 3, 4, 5");
    as = sl_sort(ac);
    sl_test_equal(as, ex);
    sl_free(ac);
    sl_free(ex);
    l_free(as);

    ac = sl_of_string("1, 2, 1, 3, 2");
    ex = sl_of_string("1, 1, 2, 2, 3");
    as = sl_sort(ac);
    sl_test_equal(as, ex);
    sl_free(ac);
    sl_free(ex);
    l_free(as);

    ac = sl_of_string("");
    ex = sl_of_string("");
    as = sl_sort(ac);
    sl_test_equal(as, ex);
    sl_free(ac);
    sl_free(ex);
    l_free(as);

    ac = sl_of_string("-1, -2, -3, -1");
    ex = sl_of_string("-1, -1, -2, -3"); // alphabetic, not numeric sort
    as = sl_sort(ac);
    sl_test_equal(as, ex);
    sl_free(ac);
    sl_free(ex);
    l_free(as);
}

List sl_sort(List list) {
    require_not_null(list);
    require_element_size_string(list);
    return l_sort(list, String_compare);
}

static CmpResult String_compare_dec(ConstAny a, ConstAny b) {
    require_not_null(a);
    require_not_null(b);
    String x = *(String*)a;
    String y = *(String*)b;
    return s_compare(y, x);
}

List sl_sort_dec(List list);

static void sl_sort_dec_test(void) {
    printsln((String)__func__);
    List ac, ex, as;

    ac = sl_of_string("1, 2, 3, 4, 5");
    ex = sl_of_string("5, 4, 3, 2, 1");
    as = sl_sort_dec(ac);
    sl_test_equal(as, ex);
    sl_free(ac);
    sl_free(ex);
    l_free(as);

    ac = sl_of_string("1, 2, 1, 3, 2");
    ex = sl_of_string("3, 2, 2, 1, 1");
    as = sl_sort_dec(ac);
    sl_test_equal(as, ex);
    sl_free(ac);
    sl_free(ex);
    l_free(as);

    ac = sl_of_string("");
    ex = sl_of_string("");
    as = sl_sort_dec(ac);
    sl_test_equal(as, ex);
    sl_free(ac);
    sl_free(ex);
    l_free(as);

    ac = sl_of_string("-1, -2, -3, -1");
    ex = sl_of_string("-3, -2, -1, -1"); // alphabetic, not numeric sort
    as = sl_sort_dec(ac);
    sl_test_equal(as, ex);
    sl_free(ac);
    sl_free(ex);
    l_free(as);
}

List sl_sort_dec(List list) {
    require_not_null(list);
    require_element_size_string(list);
    return l_sort(list, String_compare_dec);
}

void sl_insert(List list, int index, String value);

static void sl_insert_test(void) {
    printsln((String)__func__);
    List ac, ex;

    ac = sl_of_string("1, 2, 3, 4, 5");
    sl_insert(ac, 0, s_create("9"));
    ex = sl_of_string("9, 1, 2, 3, 4, 5");
    sl_test_equal(ac, ex);
    sl_free(ac);
    sl_free(ex);
    
    ac = sl_of_string("1, 2, 3, 4, 5");
    sl_insert(ac, 5, s_create("9"));
    ex = sl_of_string("1, 2, 3, 4, 5, 9");
    sl_test_equal(ac, ex);
    sl_free(ac);
    sl_free(ex);
    
    ac = sl_of_string("1, 2, 3, 4, 5");
    sl_insert(ac, 3, s_create("9"));
    ex = sl_of_string("1, 2, 3, 9, 4, 5");
    sl_test_equal(ac, ex);
    sl_free(ac);
    sl_free(ex);
    
    ac = sl_of_string("1");
    sl_insert(ac, -1, s_create("9"));
    ex = sl_of_string("9, 1");
    sl_test_equal(ac, ex);
    sl_free(ac);
    sl_free(ex);

    ac = sl_of_string("1");
    sl_insert(ac, 1, s_create("9"));
    ex = sl_of_string("1, 9");
    sl_test_equal(ac, ex);
    sl_free(ac);
    sl_free(ex);

    ac = sl_create();
    sl_insert(ac, 0, s_create("9"));
    ex = sl_of_string("9");
    sl_test_equal(ac, ex);
    sl_free(ac);
    sl_free(ex);
}

void sl_insert(List list, int index, String value) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(value);
    l_insert(list, index, &value);
}



void sl_remove(List list, int index);

static void sl_remove_test(void) {
    printsln((String)__func__);
    List ac, ex;
    String s;

    ac = sl_of_string("1, 2, 3, 4, 5, 6");
    s = sl_get(ac, 0);
    s_free(s);
    sl_remove(ac, 0);
    ex = sl_of_string("2, 3, 4, 5, 6");
    sl_test_equal(ac, ex);
    sl_free(ac);
    sl_free(ex);
    
    ac = sl_of_string("1, 2, 3, 4, 5, 6");
    s = sl_get(ac, 5);
    s_free(s);
    sl_remove(ac, 5);
    ex = sl_of_string("1, 2, 3, 4, 5");
    sl_test_equal(ac, ex);
    sl_free(ac);
    sl_free(ex);
    
    ac = sl_of_string("1, 2, 3, 4, 5, 6");
    s = sl_get(ac, 3);
    s_free(s);
    sl_remove(ac, 3);
    ex = sl_of_string("1, 2, 3, 5, 6");
    sl_test_equal(ac, ex);
    sl_free(ac);
    sl_free(ex);
    
    ac = sl_of_string("1");
    ex = sl_of_string("");
    s = sl_get(ac, 0);
    s_free(s);
    sl_remove(ac, -1);
    sl_test_equal(ac, ex);
    sl_free(ac);
    sl_free(ex);

    ac = sl_of_string("1, 2");
    s = sl_get(ac, 0);
    s_free(s);
    sl_remove(ac, -1);
    ex = sl_of_string("2");
    sl_test_equal(ac, ex);
    sl_free(ac);
    sl_free(ex);
    
    ac = sl_of_string("1");
    sl_remove(ac, 1);
    ex = sl_of_string("1");
    sl_test_equal(ac, ex);
    sl_free(ac);
    sl_free(ex);
    
    ac = sl_of_string("1");
    s = sl_get(ac, 0);
    s_free(s);
    sl_remove(ac, 0);
    ex = sl_create();
    sl_test_equal(ac, ex);
    sl_free(ac);
    l_free(ex);
    
    ac = sl_of_string("");
    sl_remove(ac, 0);
    ex = sl_create();
    sl_test_equal(ac, ex);
    l_free(ac);
    l_free(ex);
}

void sl_remove(List list, int index) {
    require_not_null(list);
    require_element_size_string(list);
    l_remove(list, index);
}

///////////////////////////////////////////////////////////////////////////////

bool sl_index_even(String value, int index) {
    return (index & 1) == 0;
}

bool sl_index_odd(String value, int index) {
    return (index & 1) == 1;
}

bool sl_element_empty(String value, int index) {
    return s_length(value) <= 0;
}

///////////////////////////////////////////////////////////////////////////////

void sl_each(List list, StringIntStringToString f, String x);

static String concat_free(String element, int index, String x) {
    String s = s_concat(element, x);
    s_free(element);
    return s;
}

static void sl_each_test(void) {
    printsln((String)__func__);
    List ac, ex;
    
    ac = sl_of_string("a, b, c, d");
    sl_each(ac, concat_free, "...!");
    ex = sl_of_string("a...!, b...!, c...!, d...!");
    sl_test_equal(ac, ex);
    sl_free(ac);
    sl_free(ex);
    
    ac = sl_of_string("a, b, c, d");
    sl_each(ac, concat_free, "");
    ex = sl_of_string("a, b, c, d");
    sl_test_equal(ac, ex);
    sl_free(ac);
    sl_free(ex);
}

void sl_each(List list, StringIntStringToString f, String x) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(f);
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        node->value = f(node->value, i, x);
    }
}

void sl_each_state(List list, StringIntStringAnyToString f, String x, Any state);

static void sl_each_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

void sl_each_state(List list, StringIntStringAnyToString f, String x, Any state) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(f);
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        node->value = f(node->value, i, x, state);
    }
}

List sl_map(List list, StringIntStringToString f, String x);

static String append_each(String element, int index, String x) {
    return s_concat(element, x);
}

static void sl_map_test(void) {
    printsln((String)__func__);
    List l, ac, ex;
    
    l = sl_of_string("a, b, c, d");
    ac = sl_map(l, append_each, "...!");
    ex = sl_of_string("a...!, b...!, c...!, d...!");
    sl_test_equal(ac, ex);
    sl_free(l);
    sl_free(ac);
    sl_free(ex);
    
    l = sl_of_string("a, b, c, d");
    ac = sl_map(l, append_each, "");
    ex = sl_of_string("a, b, c, d");
    sl_test_equal(ac, ex);
    sl_free(l);
    sl_free(ac);
    sl_free(ex);
}

List sl_map(List list, StringIntStringToString f, String x) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(f);
    List result = sl_create();
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        sl_append(result, f(node->value, i, x));
    }
    return result;
}

List sl_map_state(List list, StringIntStringAnyToString f, String x, Any state);

static void sl_map_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

List sl_map_state(List list, StringIntStringAnyToString f, String x, Any state) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(f);
    List result = sl_create();
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        sl_append(result, f(node->value, i, x, state));
    }
    return result;
}

String sl_foldl(List list, StringStringIntToString f, String init);

static String fold_concat_free(String state, String element, int index) {
    String s = s_concat(state, element);
    s_free(state);
    return s;
}

static void sl_foldl_test(void) {
    printsln((String)__func__);
    List l;
    String s;
    
    l = sl_of_string("a, b, c");    
    s = sl_foldl(l, fold_concat_free, s_copy("init"));
    test_equal_s(s, "initabc");
    s_free(s);
    sl_free(l);

    l = sl_of_string("");    
    s = sl_foldl(l, fold_concat_free, s_copy("init"));
    test_equal_s(s, "init");
    s_free(s);
    sl_free(l);
}

String sl_foldl(List list, StringStringIntToString f, String state) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(f);
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        state = f(state, node->value, i);
    }
    return state;
}

String sl_foldr(List list, StringStringIntToString f, String init);

static String sl_concat_r(String element, String state, int index) {
    String s = s_concat(element, state);
    s_free(state);
    return s;
}

static void sl_foldr_test(void) {
    printsln((String)__func__);
    
    List l;
    String s;
    
    l = sl_of_string("a, b, c");    
    s = sl_foldr(l, sl_concat_r, s_copy("X"));
    test_equal_s(s, "abcX");
    s_free(s);
    sl_free(l);

    l = sl_of_string("");    
    s = sl_foldr(l, sl_concat_r, s_copy("X"));
    test_equal_s(s, "X");
    s_free(s);
    sl_free(l);
}

String sl_foldr(List list, StringStringIntToString f, String state) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(f);
    List rev = l_reverse(list);
    int i = l_length(list) - 1;
    for (StringListNode *node = rev->first; node != NULL; node = node->next, i--) {
        state = f(node->value, state, i);
    }
    l_free(rev);
    return state;
}

List sl_filter(List list, StringIntStringToBool predicate, String x);

static bool f_ends_width(String element, int index, String start) {
    return s_ends_with(element, start);
}

static void sl_filter_test(void) {
    printsln((String)__func__);
    List l, ac, ex;

    l = sl_of_string("Kürbis, Brombeere, Heidelbeere, Himbeere, Apfel");
    ac = sl_filter(l, f_ends_width, "beere");
    ex = sl_of_string("Brombeere, Heidelbeere, Himbeere");
    sl_test_equal(ac, ex);
    sl_free(l);
    l_free(ac);
    sl_free(ex);
}

List sl_filter(List list, StringIntStringToBool predicate, String x) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(predicate);
    List result = sl_create();
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (predicate(node->value, i, x)) {
            sl_append(result, node->value);
        }
    }
    return result;
}

List sl_filter_state(List list, StringIntStringAnyToBool predicate, String x, Any state);

static void sl_filter_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

List sl_filter_state(List list, StringIntStringAnyToBool predicate, String x, Any state) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(predicate);
    List result = sl_create();
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (predicate(node->value, i, x, state)) {
            sl_append(result, node->value);
        }
    }
    return result;
}

List sl_choose(List list, StringIntStringToStringOption f, String x);

static StringOption ends_width_0_append(String element, int index, String x) {
    if (s_ends_with(element, "0")) {
        String s = s_concat(element, x);
        return make_string_some(s);
    }
    return make_string_none();
}

static void sl_choose_test(void) {
    printsln((String)__func__);
    List l, ac, ex;

    l = sl_of_string("10, 2, 30, 4, 50, 6");
    ac = sl_choose(l, ends_width_0_append, "x");
    ex = sl_of_string("10x, 30x, 50x");
    sl_test_equal(ac, ex);

    sl_free(l);
    sl_free(ac);
    sl_free(ex);
}

List sl_choose(List list, StringIntStringToStringOption f, String x) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(f);
    List result = sl_create();
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        StringOption op = f(node->value, i, x);
        if (!op.none) {
            sl_append(result, op.some);
        }
    }
    return result;
}

List sl_choose_state(List list, StringIntStringAnyToStringOption f, String x, Any state);

static void sl_choose_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

List sl_choose_state(List list, StringIntStringAnyToStringOption f, String x, Any state) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(f);
    List result = sl_create();
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        StringOption op = f(node->value, i, x, state);
        if (!op.none) {
            sl_append(result, op.some);
        }
    }
    return result;
}

bool sl_exists(List list, StringIntStringToBool predicate, String x);

static bool sl_element_eq(String element, int index, String x) {
    return s_equals(element, x);
}

static bool sl_element_ne(String element, int index, String x) {
    return !s_equals(element, x);
}

static void sl_exists_test(void) {
    printsln((String)__func__);
    List l = sl_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(sl_exists(l, sl_element_eq, "3"), true);
    test_equal_b(sl_exists(l, sl_element_eq, "9"), false);
    test_equal_b(sl_exists(l, sl_element_ne, "9"), true);
    sl_free(l);
}

bool sl_exists(List list, StringIntStringToBool predicate, String x) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(predicate);
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (predicate(node->value, i, x)) {
            return true;
        }
    }
    return false;
}

bool sl_exists_state(List list, StringIntStringAnyToBool predicate, String x, Any state);

static void sl_exists_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

bool sl_exists_state(List list, StringIntStringAnyToBool predicate, String x, Any state) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(predicate);
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (predicate(node->value, i, x, state)) {
            return true;
        }
    }
    return false;
}

static void sl_forall_test(void) {
    printsln((String)__func__);
    List l = sl_of_string("1, 2, 3, 4, 5, 6");
    test_equal_b(sl_forall(l, sl_element_eq, "3"), false);
    test_equal_b(sl_forall(l, sl_element_ne, "9"), true);
    sl_free(l);
}

bool sl_forall(List list, StringIntStringToBool predicate, String x) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(predicate);
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (!predicate(node->value, i, x)) {
            return false;
        }
    }
    return true;
}

static void sl_forall_state_test(void) {
    printsln((String)__func__);
    // @todo: add tests
}

bool sl_forall_state(List list, StringIntStringAnyToBool predicate, String x, Any state) {
    require_not_null(list);
    require_element_size_string(list);
    require_not_null(predicate);
    int i = 0;
    for (StringListNode *node = list->first; node != NULL; node = node->next, i++) {
        if (!predicate(node->value, i, x, state)) {
            return false;
        }
    }
    return true;
}

///////////////////////////////////////////////////////////////////////////////
// Tests

bool sl_test_equal_file_line(const char *file, const char *function, int line, List a, List e) {
    base_count_check();

    if (a == NULL) {
        printf("%s, line %d: Actual list is NULL\n", file, line);
        return false;        
    }
    if (e == NULL) {
        printf("%s, line %d: Expected list is NULL\n", file, line);
        return false;        
    }
    if (a->s != sizeof(String)) {
        printf("%s, line %d: Actual list is not a String list (element size %d)\n", 
                file, line, a->s);
        return false;
    }
    if (e->s != sizeof(String)) {
        printf("%s, line %d: Expected list is not a String list (element size %d)\n", 
                file, line, e->s);
        return false;
    }
    ListNode *an = a->first;
    ListNode *en = e->first;
    int i = 0;
    for (; an != NULL && en != NULL; an = an->next, en = en->next, i++) {
        String av = *(String*)(an + 1);
        String ev = *(String*)(en + 1);
        if (strcmp(av, ev) != 0) {
            printf("%s, line %d: Actual value \"%s\" differs from expected value \"%s\" at index %d.\n", 
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

static String string2length(String element, int index, String x) {
    // index and x are not used
    int n = s_length(element);
    return s_of_int(n);
}

static void string2length2(Any element, int index, Any state, Any mapped_element) {
    // index and state are not used
    String s = *(String*)element;
    int *result = mapped_element;
    *result = s_length(s);
}

static void sum_lengths(Any state, Any element, int index) {
    // index is not used
    int *n = state;
    String s = *(String*)element;
    *n += s_length(s);
}

static StringOption starts_with_toupper(String element, int index, String x) {
    if (s_starts_with(element, x)) {
        String s = s_upper_case(element);
        return make_string_some(s);
    }
    return make_string_none();
}

///////////////////////////////////////////////////////////////////////////////

void sl_test_all(void) {
    sl_repeat_test();
    sl_of_string_test();
    sl_split_test();
    s_join_test();
    sl_prepend_append_test();
    sl_iterator_test();
    sl_contains_test();
    sl_index_test();
    sl_index_from_test();
    sl_index_fn_test();
    sl_sort_test();
    sl_sort_dec_test();
    sl_insert_test();
    sl_remove_test();
    sl_each_test();
    sl_each_state_test();
    sl_map_test();
    sl_map_state_test();
    sl_foldl_test();
    sl_foldr_test();
    sl_filter_test();
    sl_filter_state_test();
    sl_choose_test();
    sl_choose_state_test();
    sl_exists_test();
    sl_exists_state_test();
    sl_forall_test();
    sl_forall_state_test();
    
    // Example 1: Creating a list and printing its elements
    List a1 = sl_of_string("alpha, beta, gamma, delta, epsilon");
    sl_println(a1);
    sl_free(a1);

    // Example 2: Appending and prepending
    List a2 = sl_create();
    sl_append(a2, "b");
    sl_append(a2, "c");
    sl_prepend(a2, "a");
    sl_println(a2);
    l_free(a2);

    // Example 3: Iteration with loops and get
    List a3 = sl_of_string("alpha, beta, gamma, delta, epsilon");
    int n = l_length(a3);
    for (int i = 0; i < n; i++) {
        printsln(sl_get(a3, i));
    }
    sl_free(a3);

    // Example 4: Iteration with loops and list nodes
    List a4 = sl_of_string("alpha, beta, gamma, delta, epsilon");
    for (StringListNode *node = a4->first; node != NULL; node = node->next) {
        printsln(node->value);
    }
    sl_free(a4);

    // Example 5: Iteration with iterators
    List a5 = sl_of_string("alpha, beta, gamma, delta, epsilon");
    ListIterator iter = l_iterator(a5);
    while (l_has_next(iter)) {
        printsln(sl_next(&iter));
    }
    sl_free(a5);
    
    // Example 6: Sorting
    List a6 = sl_of_string("alpha, beta, gamma, delta, epsilon");
    List a7 = sl_sort_dec(a6);
    l_free(a6); // free list, but not elements
    sl_println(a7);
    sl_free(a7); // free list and elements

    // Example 7: Map string to its length (as a String).
    List a8 = sl_of_string("alpha, beta, gamma, delta, epsilon");
    List a9 = sl_map(a8, string2length, NULL);
    sl_free(a8); // free list and elements
    sl_println(a9);
    sl_free(a9); // free list and elements

    // Example 8: Map String to its length (as an int).
    List a10 = sl_of_string("alpha, beta, gamma, delta, epsilon");
    List a11 = l_map(a10, string2length2, sizeof(int), NULL); // result element size is sizeof(int)
    sl_free(a10); // free list and elements
    il_println(a11); // a11 is an int list
    l_free(a11); // free list

    // Example 9: Total number of characters of all Strings in the list
    List a12 = sl_of_string("alpha, beta, gamma, delta, epsilon");
    int sum = 0;
    l_foldl(a12, sum_lengths, &sum);
    sl_free(a12); // free list and elements
    printiln(sum);

    // Example 10: Total number of characters of all Strings in the list (with iterator)
    List a13 = sl_of_string("alpha, beta, gamma, delta, epsilon");
    sum = 0;
    ListIterator it = l_iterator(a13);
    while (l_has_next(it)) {
        String s = sl_next(&it);
        sum += s_length(s);
    }
    sl_free(a13);
    printiln(sum);

    // Example 11: Filter Strings starting with "al" and convert them to upper case.
    List a14 = sl_of_string("alpha, alfred, beta, gamma, alf");
    List a15 = sl_choose(a14, starts_with_toupper, "al");
    sl_free(a14); // free list and elements
    sl_println(a15);
    sl_free(a15); // free list and elements

    // Example 12: Read text file into a list of strings.
    /*
    line 1
    line II
    my line 3
    last line
    */
    String s = s_read_file("example.txt");
    List a16 = sl_split(s, '\n'); // split file content into lines
    s_free(s); // free file content
    sl_println(a16);
    sl_free(a16); // free list and elements

    // Example 13: Write a list of strings to a text file.
    // List a17 = sl_of_string("line 1, line II, my line 3, last line");
    List a17 = sl_create();
    sl_append(a17, "line 1");
    sl_append(a17, "line II");
    sl_append(a17, "my line 3");
    sl_append(a17, "last line");
    sl_println(a17); // output: [line 1, line II, my line 3, last line]
    s = s_join(a17, '\n');
    l_free(a17);
    s_write_file("example.txt", s);
    s_free(s); // free file content

    // Example 14: Write 100 random double values between 0 and 10 to a file.
    List a18 = sl_create();
    for (int i = 0; i < 100; i++){
        double d = d_rnd(10);
        sl_append(a18, s_of_double(d));
    }
    s = s_join(a18, '\n');
    sl_free(a18); // free list and elements
    s_write_file("random-doubles.txt", s);
    s_free(s); // joined string
}

#if 0
int main(void) {
    report_memory_leaks(true);
    base_init();
    sl_test_all();
    return 0;
}
#endif
