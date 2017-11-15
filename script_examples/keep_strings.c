/*
Compile: make keep_strings
Run: ./keep_strings
make keep_strings && ./keep_strings
*/

#include "base.h"        // http://hci.uni-hannover.de/files/prog1lib/base_8h.html
#include "string.h"      // http://hci.uni-hannover.de/files/prog1lib/string_8h.html
#include "list.h"        // http://hci.uni-hannover.de/files/prog1lib/list_8h.html
#include "string_list.h" // http://hci.uni-hannover.de/files/prog1lib/string__list_8h.html
#include "arrays_lists.h"

static bool contains(String element, int index, String x) {
    return s_contains(element, x);
}

void keep_strings(void) {
    String data = s_read_file("test.txt");

    List words = sl_split(data, ' ');
    printf("%d words\n", l_length(words));

    List filtered = sl_filter(words, contains, "lis");
    printf("%d filtered words\n", l_length(filtered));
    sl_println(filtered);
    
    l_free(filtered);
    sl_free(words);
    s_free(data);
}

#if 0
List keep_if_contains(List list, String part) {
    return sl_filter(list, contains, part);
}
#endif

#if 0
List keep_if_contains(List list, String part) {
    List result = sl_create();
    for (StringListNode *node = list->first; node != NULL; node = node->next) {
        if (s_contains(node->value, part)) {
            sl_append(result, node->value);
        }
    }
    return result;
}
#endif

#if 0
List keep_if_contains(List list, String part) {
    List result = sl_create();
    ListIterator iter = l_iterator(list);
    while (l_has_next(iter)) {
        String s = sl_next(&iter);
        if (s_contains(s, part)) {
            sl_append(result, s);
        }
    }
    return result;
}
#endif

#if 1
List keep_if_contains(List list, String part) {
    List result = list(String);
    Iterator iter = iterator(list);
    while (has_next(iter)) {
        String s = next_value(String, iter);
        if (s_contains(s, part)) {
            sl_append(result, s);
        }
    }
    return result;
}
#endif

static void keep_if_contains_test(void) {
    List a = sl_split("Lists are collections of variable size.", ' ');
    List b = sl_split("Lists collections variable size.", ' ');
    List c = keep_if_contains(a, "i");
    sl_test_equal(c, b);
    sl_free(b); l_free(c);

    b = sl_split("are variable", ' ');
    c = keep_if_contains(a, "ar");
    sl_test_equal(c, b);
    sl_free(a); sl_free(b); l_free(c);
}

int main(void) {
    base_init();
    base_set_memory_check(true);
//  keep_strings();
    keep_if_contains_test();
    return 0;
}
