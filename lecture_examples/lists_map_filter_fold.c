/*
Compile: make lists_map_filter_fold
Run: ./lists_map_filter_fold
make lists_map_filter_fold && ./lists_map_filter_fold
*/

#include "base.h"

void string2length(String* element, int index, Any state, /*out*/ int* mapped_element) {
    *mapped_element = s_length(*element);
}

void sum_lengths(Any state, Any element, int index) {
    // index is not used
    int *n = state;
    String s = *(String*)element;
    *n += s_length(s);
}

int sum_rec(StringListNode * l) {
    if (l == NULL) return 0;
    else return s_length(l->value) + sum_rec(l->next);
}

int sumf(List l) {
    return sum_rec(l->first);
}

int sum_rec2(StringListNode * l, int acc) {
    if (l == NULL) return acc;
    else return sum_rec2(l->next, acc + s_length(l->value));
}

int sumf2(List l) {
    return sum_rec2(l->first, 0);
}

int sum_elements(int state, int element, int index) {
    return state + element;
}

void lists_map_filter_fold_test(void) {
    List words = sl_of_string("alpha, be, gam, delta, epsilon");
    sl_println(words); // words is a String list
    List lengths = l_map(words, string2length, sizeof(int), NULL);
    il_println(lengths); // lengths is an int list
    
    int sum = il_foldl(lengths, sum_elements, 0);
    printiln(sum);
    l_free(lengths); // free list
    
    sum = 0;
    l_foldl(words, sum_lengths, &sum);
    printiln(sum);
    
    printiln(sumf(words));
    printiln(sumf2(words));
    
    sum = 0;
    ListIterator it = l_iterator(words);
    while (l_has_next(it)) {
        String s = sl_next(&it);
        sum += s_length(s);
    }
    printiln(sum);
    
    sl_free(words); // free list and elements
}

int main(void) {
    base_init();
    base_set_memory_check(true);
    lists_map_filter_fold_test();
    return 0;
}
