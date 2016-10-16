/*
@author Michael Rohs
@date 15.10.2015
@copyright Apache License, Version 2.0
*/

#include "string.h"

String s_create(String s) {
    assert_argument_not_null(s);
    int n = (strlen(s) + 1) * sizeof(char); // + 1 for '\0' termination
    char *a = xmalloc(n);
    memcpy(a, s, n);
    return a;
}

String s_copy(String s) {
    assert_argument_not_null(s);
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
    if (n < 0) {
        printf("%s: length cannot be negative (is %d)\n", __func__, n);
        exit(EXIT_FAILURE);
    }
    char *a = xmalloc((n + 1) * sizeof(char)); // + 1 for '\0' termination
    for (int i = 0; i < n; i++) {
        a[i] = c;
    }
    a[n] = '\0';
    return a;
}

String s_range(char a, char b) {
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

String s_sub(String s, int i, int j) {
    assert_argument_not_null(s);
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

char s_get(String s, int i) {
    assert_argument_not_null(s);
    int n = strlen(s);
    if (i < 0 || i >= n) {
        printf("%s: index %d is out of range "
            "(string length: %d, allowed indices: 0..%d)\n", 
        __func__, i, n, n - 1);
        exit(EXIT_FAILURE);
    }
    return s[i];
}

void s_set(String s, int i, char v) {
    assert_argument_not_null(s);
    int n = strlen(s);
    if (i < 0 || i >= n) {
        printf("%s: index %d is out of range "
            "(string length: %d, allowed indices: 0..%d)\n", 
        __func__, i, n, n - 1);
        exit(EXIT_FAILURE);
    }
    s[i] = v;
}

void s_blit(String source, int source_index, String destination, int destination_index, int count) {
    assert_argument_not_null(source);
    assert_argument_not_null(destination);
    if (count <= 0) return;
    // do not check for string lengths, as that would require 
    // finding '\0' and would not allow binary strings
    if (source_index < 0) {
        printf("%s: source_index cannot be negative (is %d)\n", 
        __func__, source_index);
    }
    if (destination_index < 0) {
        printf("%s: destination_index cannot be negative (is %d)\n", 
        __func__, destination_index);
    }
    for (int i = source_index, j = destination_index; i < source_index + count; i++, j++) {
        s_set(destination, j, s_get(source, i));
    }
}

String s_concat(String x, String y) {
    assert_argument_not_null(x);
    assert_argument_not_null(y);
    int nx = strlen(x);
    int ny = strlen(y) + 1;
    int n = nx + ny;
    char *a = xmalloc(n * sizeof(char));
    memcpy(a,      x, nx * sizeof(char));
    memcpy(a + nx, y, ny * sizeof(char));
    return a;
}

int s_length(String s) {
    assert_argument_not_null(s);
    return strlen(s);
}

String s_lower_case(String s) {
    assert_argument_not_null(s);
    int n = strlen(s);
    String t = xmalloc(n + 1);
    for (int i = 0; i < n; i++) {
        t[i] = tolower(s[i]);
    }
    t[n] = '\0';
    return t;
}

String s_upper_case(String s) {
    assert_argument_not_null(s);
    int n = strlen(s);
    String t = xmalloc(n + 1);
    for (int i = 0; i < n; i++) {
        t[i] = toupper(s[i]);
    }
    t[n] = '\0';
    return t;
}

CmpResult s_compare(String s, String t) {
    assert_argument_not_null(s);
    assert_argument_not_null(t);
    int c = strcmp(s, t);
    return (c == 0) ? EQ : ((c < 0) ? LT : GT);
}

CmpResult s_compare_ignore_case(String s, String t) {
    assert_argument_not_null(s);
    assert_argument_not_null(t);
    String s2 = s_lower_case(s);
    String t2 = s_lower_case(t);
    int c = strcmp(s2, t2);
    s_free(s2);
    s_free(t2);
    return (c == 0) ? EQ : ((c < 0) ? LT : GT);
}

bool s_equals(String s, String t) {
    if (s == NULL && t == NULL) return true;
    if (s == NULL && t != NULL) return false;
    if (s != NULL && t == NULL) return false;
    return s_compare(s, t) == EQ;
}

bool s_equals_ignore_case(String s, String t) {
    assert_argument_not_null(s);
    assert_argument_not_null(t);
    return s_compare_ignore_case(s, t) == EQ;
}

bool s_contains(String s, String part) {
    assert_argument_not_null(s);
    assert_argument_not_null(part);
    return strstr(s, part) != NULL;
}

bool s_starts_with(String s, String start) {
    assert_argument_not_null(s);
    assert_argument_not_null(start);
    String p = strstr(s, start);
    return p == s;
}

bool s_ends_with(String s, String end) {
    assert_argument_not_null(s);
    assert_argument_not_null(end);
    int ns = strlen(s);
    int nend = strlen(end);
    if (nend > ns) return false;
    if (nend <= 0) return true;
    String s_end = s + ns - nend;
    return strcmp(s_end, end) == 0;
}

int s_index(String s, String part) {
    assert_argument_not_null(s);
    assert_argument_not_null(part);
    String p = strstr(s, part);
    if (p == NULL) return -1;
    return p - s;
}

int s_index_from(String s, String part, int from) {
    assert_argument_not_null(s);
    assert_argument_not_null(part);
    if (from < 0) from = 0;
    int n = strlen(s);
    if (from >= n) return -1;
    String p = strstr(s + from, part);
    if (p == NULL) return -1;
    return p - s;
}

String s_reverse(String s) {
    assert_argument_not_null(s);
    int n = strlen(s);
    String t = xmalloc(n + 1);
    for (int i = 0; i < n; i++) {
        t[i] = s[n - i - 1];
    }
    t[n] = '\0';
    return t;
}

int s_last_index(String s, String part) {
    assert_argument_not_null(s);
    assert_argument_not_null(part);
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
    assert_argument_not_null(s);
    return strlen(s) == 0;
}

// @todo: add tests

String s_trim(String s) {
    assert_argument_not_null(s);
    int n = strlen(s);
    int l = 0;
    while (l < n && s[l] == ' ') l++;
    int r = n - 1;
    while (r >= 0 && s[r] == ' ') r--;
    if (r < l || l >= n || r < 0) {
        return s_create("");
    }
    // assert: r >= l && r < n && l >= 0
    n = r - l + 2;
    String t = xmalloc(n);
    int j = 0;
    for (int i = l; i <= r; i++, j++) {
        t[j] = s[i];
    }
    t[j] = '\0';
    return t;
}

// @todo: replace, replace_all

// split string --> see string list

String s_of_int(int i) {
    char buf[16];
    sprintf(buf, "%d", i);
    int n = strlen(buf) + 1;
    String s = xmalloc(n);
    memcpy(s, buf, n);
    return s;
}

String s_of_double(double d) {
    char buf[32];
    sprintf(buf, "%.16g", d);
    int n = strlen(buf) + 1;
    // printf("'%s' %d\n", buf, n);
    String s = xmalloc(n);
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
