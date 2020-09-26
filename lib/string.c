/*
@author Michael Rohs
@date 15.10.2015, 25.09.2020
@copyright Apache License, Version 2.0
*/

#include "string.h"

String s_create(String s) {
    require_not_null(s);
    int n = (strlen(s) + 1) * sizeof(char); // + 1 for '\0' termination
    char *a = xmalloc(n);
    memcpy(a, s, n);
    return a;
}

String s_copy(String s) {
    require_not_null(s);
    int n = (strlen(s) + 1) * sizeof(char); // + 1 for '\0' termination
    char *a = xmalloc(n);
    memcpy(a, s, n);
    return a;
}

void s_free(String s) {
    if (s != NULL) {
        free(s);
    }
}

String s_repeat(int n, char c) {
    require("non-negative length", n >= 0);
    char *a = xmalloc((n + 1) * sizeof(char)); // + 1 for '\0' termination
    for (int i = 0; i < n; i++) {
        a[i] = c;
    }
    a[n] = '\0';
    return a;
}

static void s_range_test(void) {
    printsln((String)__func__);
    String s;

    s = s_range('a', 'd');
    test_equal_s(s, "abc");
    s_free(s);

    s = s_range('c', 'a');
    test_equal_s(s, "cb");
    s_free(s);

    s = s_range('c', 'a' - 1);
    test_equal_s(s, "cba");
    s_free(s);

    s = s_range('x', 'x');
    test_equal_s(s, "");
    s_free(s);

    s = s_range('x', 'y');
    test_equal_s(s, "x");
    s_free(s);

    s = s_range('x', 'w');
    test_equal_s(s, "x");
    s_free(s);

    // s = s_range(253, 256);
    // test_equal_s(s, "\xfd\xfe\xff");
    // s_free(s);

    s = s_range(0x7e, 0x81);
    test_equal_s(s, "\x7e\x7f\x80");
    s_free(s);
}

String s_range(char _a, char _b) {
    unsigned char a = (unsigned char) _a;
    unsigned char b = (unsigned char) _b;
    if (a <= b) {
        int n = b - a;
        char *arr = xmalloc((n + 1) * sizeof(char));
        for (int i = 0; i < n; i++) {
            arr[i] = a + i;
        }
        arr[n] = '\0';
        return arr;
    } else /* a > b */ {
        int n = a - b;
        char *arr = xmalloc((n + 1) * sizeof(char));
        for (int i = 0; i < n; i++) {
            arr[i] = a - i;
        }
        arr[n] = '\0';
        return arr;
    }
}

static void s_sub_test(void) {
    printsln((String)__func__);
    String s;

    s = s_sub("abc", 0, 3);
    test_equal_s(s, "abc");
    s_free(s);

    s = s_sub("abc", 0, 2);
    test_equal_s(s, "ab");
    s_free(s);

    s = s_sub("abc", 1, 3);
    test_equal_s(s, "bc");
    s_free(s);

    s = s_sub("abc", 1, 2);
    test_equal_s(s, "b");
    s_free(s);

    s = s_sub("abc", 1, 1);
    test_equal_s(s, "");
    s_free(s);

    s = s_sub("abc", -1, 4);
    test_equal_s(s, "abc");
    s_free(s);

    s = s_sub("abc", 3, 4);
    test_equal_s(s, "");
    s_free(s);

}

String s_sub(String s, int i, int j) {
    require_not_null(s);
    int n = strlen(s);
    if (i < 0) i = 0;
    if (j > n) j = n;
    if (i >= j || i >= n || j <= 0) {
        char *a = xmalloc(1 * sizeof(char));
        a[0] = '\0';
        return a;
    }
    // assert i < j && i < n && j > 0
    n = j - i;
    char *a = xmalloc((n + 1) * sizeof(char));
    memcpy(a, s + i, n * sizeof(char));
    a[n] = '\0';
    return a;
}

/*
    // logging the call stack:
    push_func(__func__, __LINE__ - 1);
    ...
    pop_func();

typedef struct Funcs Funcs;
struct Funcs {
    Funcs* previous;
    const char* func;
    int line;
};

static Funcs* funcs = NULL;

void push_func(const char* func, int line) {
    Funcs* f = xmalloc(sizeof(Funcs));
    f->previous = funcs;
    f->func = func;
    f->line = line;
    funcs = f;
}

void pop_func(void) {
    if (funcs != NULL) {
        Funcs* f = funcs;
        funcs = funcs->previous;
        free(f);
    }
}

#define require_x0(description, condition, ...) \
if (!(condition)) {\
    fprintf(stderr, "%s, line %d: %s's precondition \"%s\" violated: ", __FILE__, __LINE__, __func__, description);\
    fprintf(stderr, __VA_ARGS__);\
    fprintf(stderr, "\n");\
    if (funcs) {\
        funcs = funcs->previous;\
        while (funcs) {\
            printf("\tcalled from %s (line %d)\n", funcs->func, funcs->line);\
            funcs = funcs->previous;\
        }\
    }\
    exit(EXIT_FAILURE);\
}
*/

char s_get(String s, int i) {
    require_not_null(s);
    int n = strlen(s);
    require_x("index in range", i >= 0 && i < n, "index == %d, length == %d", i, n);
    return s[i];
}

void s_set(String s, int i, char v) {
    require_not_null(s);
    int n = strlen(s);
    require_x("index in range", i >= 0 && i < n, "index == %d, length == %d", i, n);
    s[i] = v;
}

static void s_blit_test(void) {
    printsln((String)__func__);
    String s, t;

    s = s_create("abc");
    t = s_create("xyz");
    s_blit(s, 0, t, 0, -1);
    test_equal_s(s, "abc");
    test_equal_s(t, "xyz");
    s_free(s);
    s_free(t);

    s = s_create("abc");
    t = s_create("xyz");
    s_blit(s, 0, t, 0, 0);
    test_equal_s(s, "abc");
    test_equal_s(t, "xyz");
    s_free(s);
    s_free(t);

    s = s_create("abc");
    t = s_create("xyz");
    s_blit(s, 0, t, 0, 1);
    test_equal_s(s, "abc");
    test_equal_s(t, "ayz");
    s_free(s);
    s_free(t);

    s = s_create("abc");
    t = s_create("xyz");
    s_blit(s, 0, t, 0, 2);
    test_equal_s(s, "abc");
    test_equal_s(t, "abz");
    s_free(s);
    s_free(t);

    s = s_create("abc");
    t = s_create("xyz");
    s_blit(s, 0, t, 0, 3);
    test_equal_s(s, "abc");
    test_equal_s(t, "abc");
    s_free(s);
    s_free(t);

    s = s_create("abc");
    t = s_create("xyz");
    s_blit(s, 0, t, 1, 0);
    test_equal_s(s, "abc");
    test_equal_s(t, "xyz");
    s_free(s);
    s_free(t);

    s = s_create("abc");
    t = s_create("xyz");
    s_blit(s, 0, t, 1, 1);
    test_equal_s(s, "abc");
    test_equal_s(t, "xaz");
    s_free(s);
    s_free(t);

    s = s_create("abc");
    t = s_create("xyz");
    s_blit(s, 0, t, 1, 2);
    test_equal_s(s, "abc");
    test_equal_s(t, "xab");
    s_free(s);
    s_free(t);

    s = s_create("abc");
    t = s_create("xyz");
    s_blit(s, 1, t, 1, 2);
    test_equal_s(s, "abc");
    test_equal_s(t, "xbc");
    s_free(s);
    s_free(t);

    s = s_create("abc");
    t = s_create("xyz");
    s_blit(s, 1, t, 1, 2);
    test_equal_s(s, "abc");
    test_equal_s(t, "xbc");
    s_free(s);
    s_free(t);
}

void s_blit(String source, int source_index, String destination, int destination_index, int count) {
    require_not_null(source);
    require_not_null(destination);
    if (count <= 0) return;
    require("non-negative source index", source_index >= 0);
    require("non-negative destination index", destination_index >= 0);
    for (int i = source_index, j = destination_index; i < source_index + count; i++, j++) {
        s_set(destination, j, s_get(source, i));
    }
}

static void s_concat_test(void) {
    printsln((String)__func__);
    String s;

    s = s_concat("", "");
    test_equal_s(s, "");
    s_free(s);

    s = s_concat("a", "");
    test_equal_s(s, "a");
    s_free(s);

    s = s_concat("", "b");
    test_equal_s(s, "b");
    s_free(s);

    s = s_concat("a", "b");
    test_equal_s(s, "ab");
    s_free(s);

    s = s_concat("ab", "");
    test_equal_s(s, "ab");
    s_free(s);

    s = s_concat("", "cd");
    test_equal_s(s, "cd");
    s_free(s);

    s = s_concat("ab", "cd");
    test_equal_s(s, "abcd");
    s_free(s);
}

String s_concat(String x, String y) {
    require_not_null(x);
    require_not_null(y);
    int nx = strlen(x);
    int ny = strlen(y) + 1;
    int n = nx + ny;
    char *a = xmalloc(n * sizeof(char));
    memcpy(a,      x, nx * sizeof(char));
    memcpy(a + nx, y, ny * sizeof(char));
    return a;
}

int s_length(String s) {
    require_not_null(s);
    return strlen(s);
}

String s_lower_case(String s) {
    require_not_null(s);
    int n = strlen(s);
    String t = xmalloc((n + 1) * sizeof(char));
    for (int i = 0; i < n; i++) {
        t[i] = tolower(s[i]);
    }
    t[n] = '\0';
    return t;
}

String s_upper_case(String s) {
    require_not_null(s);
    int n = strlen(s);
    String t = xmalloc((n + 1) * sizeof(char));
    for (int i = 0; i < n; i++) {
        t[i] = toupper(s[i]);
    }
    t[n] = '\0';
    return t;
}

CmpResult s_compare(String s, String t) {
    require_not_null(s);
    require_not_null(t);
    int c = strcmp(s, t);
    return (c == 0) ? EQ : ((c < 0) ? LT : GT);
}

CmpResult s_compare_ignore_case(String s, String t) {
    require_not_null(s);
    require_not_null(t);
    String sl = s_lower_case(s);
    String tl = s_lower_case(t);
    int c = strcmp(sl, tl);
    s_free(sl);
    s_free(tl);
    return (c == 0) ? EQ : ((c < 0) ? LT : GT);
}

bool s_equals(String s, String t) {
    require_not_null(s);
    require_not_null(t);
    return strcmp(s, t) == 0;
}

bool s_equals_ignore_case(String s, String t) {
    return s_compare_ignore_case(s, t) == EQ;
}

bool s_contains(String s, String part) {
    require_not_null(s);
    require_not_null(part);
    return strstr(s, part) != NULL;
}

bool s_starts_with(String s, String start) {
    require_not_null(s);
    require_not_null(start);
    String p = strstr(s, start);
    return p == s;
}

bool s_ends_with(String s, String end) {
    require_not_null(s);
    require_not_null(end);
    int ns = strlen(s);
    int nend = strlen(end);
    if (nend > ns) return false;
    if (nend <= 0) return true;
    String s_end = s + ns - nend;
    return strcmp(s_end, end) == 0;
}

int s_index(String s, String part) {
    require_not_null(s);
    require_not_null(part);
    String p = strstr(s, part);
    if (p == NULL) return -1;
    return p - s;
}

int s_index_from(String s, String part, int from) {
    require_not_null(s);
    require_not_null(part);
    if (from < 0) from = 0;
    int n = strlen(s);
    if (from >= n) return -1;
    String p = strstr(s + from, part);
    if (p == NULL) return -1;
    return p - s;
}

String s_reverse(String s) {
    require_not_null(s);
    int n = strlen(s);
    String t = xmalloc((n + 1) * sizeof(char));
    for (int i = 0; i < n; i++) {
        t[i] = s[n - i - 1];
    }
    t[n] = '\0';
    return t;
}

static void s_last_index_test(void) {
    printsln((String)__func__);
    test_equal_i(s_last_index("", ""), 0);
    test_equal_i(s_last_index("a", ""), 1);
    test_equal_i(s_last_index("ab", ""), 2);
    test_equal_i(s_last_index("", "a"), -1);
    test_equal_i(s_last_index("a", "a"), 0);
    test_equal_i(s_last_index("aa", "a"), 1);
    test_equal_i(s_last_index("aaa", "a"), 2);
    test_equal_i(s_last_index("abc", "a"), 0);
    test_equal_i(s_last_index("abc", "b"), 1);
    test_equal_i(s_last_index("abc", "c"), 2);
    test_equal_i(s_last_index("abc", "ab"), 0);
    test_equal_i(s_last_index("abc", "bc"), 1);
    test_equal_i(s_last_index("abcd", "ab"), 0);
    test_equal_i(s_last_index("abcd", "bc"), 1);
    test_equal_i(s_last_index("abcd", "cd"), 2);
    test_equal_i(s_last_index("abcd", "de"), -1);
}

int s_last_index(String s, String part) {
    require_not_null(s);
    require_not_null(part);
    String rs = s_reverse(s);
    String rpart = s_reverse(part);
    String p = strstr(rs, rpart);
    int i = (strlen(s) - strlen(part)) - (p - rs);
    s_free(rs);
    s_free(rpart);
    if (p == NULL) return -1;
    return i;
}

bool s_is_empty(String s) {
    require_not_null(s);
    return *s  == '\0';
}

static void s_trim_test(void) {
    printsln((String)__func__);
    String s;

    s = s_trim("");
    test_equal_s(s, "");
    s_free(s);

    s = s_trim(" ");
    test_equal_s(s, "");
    s_free(s);

    s = s_trim("  ");
    test_equal_s(s, "");
    s_free(s);

    s = s_trim("a");
    test_equal_s(s, "a");
    s_free(s);

    s = s_trim(" a");
    test_equal_s(s, "a");
    s_free(s);

    s = s_trim("  a");
    test_equal_s(s, "a");
    s_free(s);

    s = s_trim("a ");
    test_equal_s(s, "a");
    s_free(s);

    s = s_trim("a  ");
    test_equal_s(s, "a");
    s_free(s);

    s = s_trim("  a  ");
    test_equal_s(s, "a");
    s_free(s);

    s = s_trim(" ab");
    test_equal_s(s, "ab");
    s_free(s);

    s = s_trim("  ab");
    test_equal_s(s, "ab");
    s_free(s);

    s = s_trim("ab ");
    test_equal_s(s, "ab");
    s_free(s);

    s = s_trim("ab \t ");
    test_equal_s(s, "ab");
    s_free(s);

    s = s_trim("ab\t");
    test_equal_s(s, "ab");
    s_free(s);

    s = s_trim("ab\n");
    test_equal_s(s, "ab");
    s_free(s);

    s = s_trim(" \n\r ab \t ");
    test_equal_s(s, "ab");
    s_free(s);
}

#define IS_WHITESPACE(c) ((c) == ' ' || (c) == '\t' || (c) == '\n' || (c) == '\r')

String s_trim(String s) {
    require_not_null(s);
    int n = strlen(s);
    int l = 0;
    while (l < n && IS_WHITESPACE(s[l])) l++;
    int r = n - 1;
    while (r >= 0 && IS_WHITESPACE(s[r])) r--;
    if (r < l || l >= n || r < 0) {
        return s_create("");
    }
    // assert: r >= l and 0 <= r < n and 0 <= l < n
    n = r - l + 2;
    String t = xmalloc(n * sizeof(char));
    int j = 0;
    for (int i = l; i <= r; i++, j++) {
        t[j] = s[i];
    }
    t[j] = '\0';
    return t;
}

static void s_replace_test(void) {
    printsln((String)__func__);
    String s;

    s = s_replace("abc", "k", "x");
    test_equal_s(s, "abc");
    s_free(s);

    s = s_replace("abc", "a", "");
    test_equal_s(s, "bc");
    s_free(s);

    s = s_replace("abc", "b", "");
    test_equal_s(s, "ac");
    s_free(s);

    s = s_replace("abc", "c", "");
    test_equal_s(s, "ab");
    s_free(s);

    s = s_replace("abc", "ab", "");
    test_equal_s(s, "c");
    s_free(s);

    s = s_replace("abc", "bc", "");
    test_equal_s(s, "a");
    s_free(s);

    s = s_replace("abc", "abc", "");
    test_equal_s(s, "");
    s_free(s);

    s = s_replace("abc", "a", "x");
    test_equal_s(s, "xbc");
    s_free(s);

    s = s_replace("abc", "b", "x");
    test_equal_s(s, "axc");
    s_free(s);

    s = s_replace("abc", "c", "x");
    test_equal_s(s, "abx");
    s_free(s);

    s = s_replace("abc", "ab", "x");
    test_equal_s(s, "xc");
    s_free(s);

    s = s_replace("abc", "bc", "x");
    test_equal_s(s, "ax");
    s_free(s);

    s = s_replace("abc", "abc", "x");
    test_equal_s(s, "x");
    s_free(s);

    s = s_replace("abc", "a", "xy");
    test_equal_s(s, "xybc");
    s_free(s);

    s = s_replace("abc", "b", "xy");
    test_equal_s(s, "axyc");
    s_free(s);

    s = s_replace("abc", "c", "xy");
    test_equal_s(s, "abxy");
    s_free(s);

    s = s_replace("abc", "ab", "xy");
    test_equal_s(s, "xyc");
    s_free(s);

    s = s_replace("abc", "bc", "xy");
    test_equal_s(s, "axy");
    s_free(s);

    s = s_replace("abc", "abc", "xy");
    test_equal_s(s, "xy");
    s_free(s);
}

String s_replace(String s, String part, String replacement) {
    require_not_null(s);
    require_not_null(part);
    require_not_null(replacement);
    int i = s_index(s, part);
    if (i < 0) {
        return s_copy(s);
    }
    int n = strlen(s);
    int np = strlen(part);
    int nr = strlen(replacement);
    int n_result = n - np + nr;
    String result = xmalloc((n_result + 1) * sizeof(char));
    memcpy(result, s, i);
    memcpy(result + i, replacement, nr);
    memcpy(result + i + nr, s + i + np, n_result - i - nr);
    result[n_result] = '\0';
    return result;
}

static void s_replace_all_test(void) {
    printsln((String)__func__);
    String s;

    s = s_replace_all("abc", "k", "x");
    test_equal_s(s, "abc");
    s_free(s);

    s = s_replace_all("abc", "a", "");
    test_equal_s(s, "bc");
    s_free(s);

    s = s_replace_all("abc", "b", "");
    test_equal_s(s, "ac");
    s_free(s);

    s = s_replace_all("abc", "c", "");
    test_equal_s(s, "ab");
    s_free(s);

    s = s_replace_all("abc", "ab", "");
    test_equal_s(s, "c");
    s_free(s);

    s = s_replace_all("abc", "bc", "");
    test_equal_s(s, "a");
    s_free(s);

    s = s_replace_all("abc", "abc", "");
    test_equal_s(s, "");
    s_free(s);

    s = s_replace_all("abc", "a", "x");
    test_equal_s(s, "xbc");
    s_free(s);

    s = s_replace_all("abc", "b", "x");
    test_equal_s(s, "axc");
    s_free(s);

    s = s_replace_all("abc", "c", "x");
    test_equal_s(s, "abx");
    s_free(s);

    s = s_replace_all("abc", "ab", "x");
    test_equal_s(s, "xc");
    s_free(s);

    s = s_replace_all("abc", "bc", "x");
    test_equal_s(s, "ax");
    s_free(s);

    s = s_replace_all("abc", "abc", "x");
    test_equal_s(s, "x");
    s_free(s);

    s = s_replace_all("abc", "a", "xy");
    test_equal_s(s, "xybc");
    s_free(s);

    s = s_replace_all("abc", "b", "xy");
    test_equal_s(s, "axyc");
    s_free(s);

    s = s_replace_all("abc", "c", "xy");
    test_equal_s(s, "abxy");
    s_free(s);

    s = s_replace_all("abc", "ab", "xy");
    test_equal_s(s, "xyc");
    s_free(s);

    s = s_replace_all("abc", "bc", "xy");
    test_equal_s(s, "axy");
    s_free(s);

    s = s_replace_all("abc", "abc", "xy");
    test_equal_s(s, "xy");
    s_free(s);

    s = s_replace_all("aaa", "a", "x");
    test_equal_s(s, "xxx");
    s_free(s);

    s = s_replace_all("aaa", "a", "xy");
    test_equal_s(s, "xyxyxy");
    s_free(s);

    s = s_replace_all("aba", "a", "x");
    test_equal_s(s, "xbx");
    s_free(s);

    s = s_replace_all("bab", "a", "x");
    test_equal_s(s, "bxb");
    s_free(s);

    s = s_replace_all("aaa", "a", "");
    test_equal_s(s, "");
    s_free(s);

    s = s_replace_all("abab", "a", "x");
    test_equal_s(s, "xbxb");
    s_free(s);

    s = s_replace_all("baba", "a", "x");
    test_equal_s(s, "bxbx");
    s_free(s);

    s = s_replace_all("aaa", "a", "xyz");
    test_equal_s(s, "xyzxyzxyz");
    s_free(s);

    s = s_replace_all("a a a", "a", "xyz");
    test_equal_s(s, "xyz xyz xyz");
    s_free(s);

    s = s_replace_all("aaa", "aa", "xy");
    test_equal_s(s, "xya");
    s_free(s);

    s = s_replace_all("aba", "ab", "xyz");
    test_equal_s(s, "xyza");
    s_free(s);

    s = s_replace_all("abab", "ab", "xyz");
    test_equal_s(s, "xyzxyz");
    s_free(s);
}

String s_replace_all(String s, String part, String replacement) {
    require_not_null(s);
    require_not_null(part);
    require_not_null(replacement);
    int n = strlen(s);
    int np = strlen(part);
    int count = 0;
    int from = 0;
    while (true) {
        int i = s_index(s + from, part);
        if (i < 0) break;
        count++;
        from += i + np;
    }
    if (count <= 0) {
        return s_copy(s);
    }
    // assert: count >= 1
    int nr = strlen(replacement);
    int n_result = n - count * (np - nr);
    String result = xmalloc((n_result + 1) * sizeof(char));
    String t = result;
    while (true) {
        int i = s_index(s, part);
        if (i < 0) break;
        memcpy(t, s, i);
        s += i; // s at part
        t += i; // i characters copied
        memcpy(t, replacement, nr);
        s += np; // s after part
        t += nr; // nr characters copied
    }
    strcpy(t, s);
    return result;
}

// split string --> see string list

String s_of_int(int i) {
    char buf[16];
    sprintf(buf, "%d", i);
    int n = strlen(buf) + 1;
    String s = xmalloc(n * sizeof(char));
    memcpy(s, buf, n);
    return s;
}

String s_of_double(double d) {
    char buf[32];
    sprintf(buf, "%.16g", d);
    int n = strlen(buf) + 1;
    String s = xmalloc(n * sizeof(char));
    memcpy(s, buf, n);
    return s;
}

String s_of_boolean(bool b) {
    return b ? s_create("true") : s_create("false");
}



///////////////////////////////////////////////////////////////////////////////

#if 0
int main(void) {
    base_init();
    
    String s = NULL, t = NULL, u = NULL;
    
    // s_sub(s, 0, 10);
    // s_create(s);
    // s_copy(s);
    // s_trim(s);
    // s_get(s, 4);

    s = s_create("hello");
    printsln(s);

    s_free(s);
    s = s_copy("hello");
    printsln(s);

    s_free(s);
    s = s_repeat(20, 'x');
    printsln(s);

    s_free(s);
    s = s_range(' ', '~' + 1);
    printsln(s);

    s_free(s);
    s = s_range('~', ' ' - 1);
    printsln(s);

    s_free(s);
    s = s_range('0', '9');
    t = s_copy(s);
    s_set(t, 0, 'X');
    printsln(s);
    printsln(t);

    s_free(t);
    t = s_sub(s, 1, 4);
    printsln(t);
    printsln(s);
    
    s_free(s);
    s = s_create("hello");
    s_free(t);
    t = s_create(", world");
    u = s_concat(s, t);
    printsln(u);
    
    s_blit(t, 2, u, 0, 5);
    printsln(u);

    char *sd = xmalloc(10);
    strcpy(sd, "strdup");
    printsln(sd);
    free(sd);

    s_free(s);
    s_free(t);
    s_free(u);
    
    s = s_lower_case("ABC");
    t = s_upper_case("abc");
    s_free(s);
    s_free(t);
    
    for (int i = 0; i < 1000; i++) {
        s = s_create("abc");
        s_free(s);
    }
    
#if 0
    printiln(s_compare("", ""));
    printiln(s_compare("aaa", "aaa"));
    printiln(s_compare("aac", "aaa"));
    printiln(s_compare("aaa", "aab"));
    printiln(s_compare("a", "aa"));
    
    printiln(s_compare("", ""));
    printiln(s_compare_ignore_case("aaA", "aaa"));
    printiln(s_compare_ignore_case("aaA", "aaa"));
    printiln(s_compare_ignore_case("aAa", "aab"));
    printiln(s_compare_ignore_case("Aa", "aa"));
#endif
    
#if 0    
    printbln(s_starts_with("", ""));
    printbln(s_starts_with("abc", "ab"));
    printbln(s_starts_with("abc", "abc"));
    printbln(s_starts_with("abc", "abcd"));
    printbln(s_starts_with("abc", "a"));
    printbln(s_starts_with("abc", ""));
#endif

#if 0
    printbln(s_ends_with("", ""));
    printbln(s_ends_with("abc", "bc"));
    printbln(s_ends_with("abc", "abcd"));
    printbln(s_ends_with("abc", "c"));
    printbln(s_ends_with("abc", "abc"));
#endif
    
#if 0
    printiln(s_index("", ""));
    printiln(s_index("abc", "bc"));
    printiln(s_index("abc", "abcd"));
    printiln(s_index("abc", "c"));
    printiln(s_index("                      abc", "abc"));
#endif
        
#if 0
    printiln(s_last_index("a", "a"));
    printiln(s_last_index("a", "ab"));
    printiln(s_last_index("abc", "bc"));
    printiln(s_last_index("abcabc", "bc"));
    printiln(s_last_index("abcabcabc", "bc"));
#endif
    
#if 0
    printiln(s_index("", ""));
    printiln(s_index("abc", "bc"));
    printiln(s_index("abc", "abcd"));
    printiln(s_index("abc", "c"));
    printiln(s_index("                      abc", "abc"));
#endif
    

#if 0
    String s1 = s_trim("");
    String s2 = s_trim("a");
    String s3 = s_trim(" a");
    String s4 = s_trim("a ");
    String s5 = s_trim("  abc d  ");
    printf("'%s'\n", s1);
    printf("'%s'\n", s2);
    printf("'%s'\n", s3);
    printf("'%s'\n", s4);
    printf("'%s'\n", s5);
    s_free(s1);
    s_free(s2);
    s_free(s3);
    s_free(s4);
    s_free(s5);
#endif

#if 0
    printsln(s_of_int(0));
    printsln(s_of_int(1));
    printsln(s_of_int(123456));
    printsln(s_of_int(-123456));
#endif

#if 0
    printsln(s_of_double(0.123));
    printsln(s_of_double(1.234));
    printsln(s_of_double(123456.123));
    printsln(s_of_double(-123456.123));
#endif

#if 0
    printsln(s_of_boolean(false));
    printsln(s_of_boolean(true));
#endif

#if 0
    printsln(s_reverse(""));
    printsln(s_reverse("a"));
    printsln(s_reverse("ab"));
    printsln(s_reverse("abc"));
#endif
    
}
#endif

void s_test_all(void) {
    s_range_test();
    s_sub_test();
    s_blit_test();
    s_concat_test();
    s_last_index_test();
    s_trim_test();
    s_replace_test();
    s_replace_all_test();
}

#if 0
int main(void) {
    base_init();
    report_memory_leaks(true);
    s_test_all();
    return 0;
}
#endif
