/*
Compile: make postfix_parser
Run: ./postfix_parser
make postfix_parser && ./postfix_parser
*/

#include "base.h"
#include "string.h"
#include "list.h"
#include "double_list.h"

List stack; // external double list

void push(double f) {
    dl_append(stack, f);
}

double pop(void) {
    int n = l_length(stack) - 1;
    double d = dl_get(stack, n);
    dl_remove(stack, n);
    return d;
}

int isEmpty(void) {
    return l_length(stack) == 0;
}

enum TokenType {
    UNKNOWN, 
    OPERAND, 
    OPERATOR
};

typedef enum TokenType TokenType;

struct Token {
    TokenType type;
    int start; // inclusive
    int end; // exclusive
};

typedef struct Token Token;

Token make_token(TokenType type, int start, int end) {
    Token t = { type, start, end };
    return t;
}

bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

bool is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

bool is_float_character(char c) {
    return c == '.' || c == '-' || (c >= '0' && c <= '9');
}

bool is_unknown_character(char c) {
    return !is_operator(c) && !is_float_character(c);
}

bool line_test_equal_i(int line, int actual, int expected) {
    return base_test_equal_i(__FILE__, line, actual, expected);
}

bool test_equal_token(int line, Token actual, Token expected) {
    bool b1 = line_test_equal_i(line * 100 + 1, actual.type, expected.type);
    bool b2 = line_test_equal_i(line * 100 + 2, actual.start, expected.start);
    bool b3 = line_test_equal_i(line * 100 + 3, actual.end, expected.end);
    return b1 && b2 && b3;
}

Token next_token(String s, int start);

void next_token_test() {
    test_equal_token(__LINE__, next_token(" 1.34 + 89", 0), make_token(OPERAND, 1, 5));
    test_equal_token(__LINE__, next_token(" 1.34 + 89", 5), make_token(OPERATOR, 6, 7));
    test_equal_token(__LINE__, next_token(" 1.34 + 89", 7), make_token(OPERAND, 8, 10));
    test_equal_token(__LINE__, next_token("", 0), make_token(UNKNOWN, 0, 0));
    test_equal_token(__LINE__, next_token("", 5), make_token(UNKNOWN, 0, 0));
    test_equal_token(__LINE__, next_token(" ", 0), make_token(UNKNOWN, 0, 1));
    test_equal_token(__LINE__, next_token(" ", 5), make_token(UNKNOWN, 1, 1));
    test_equal_token(__LINE__, next_token(" abc", 0), make_token(UNKNOWN, 0, 4));
    test_equal_token(__LINE__, next_token(" abc", 1), make_token(UNKNOWN, 1, 4));
    test_equal_token(__LINE__, next_token(" ab3", 0), make_token(UNKNOWN, 0, 3));
    test_equal_token(__LINE__, next_token(" ab3", 1), make_token(UNKNOWN, 1, 3));
}

Token next_token(String s, int start) {
    int i = start;
    int n = s_length(s);
    if (start >= n || n <= 0) return make_token(UNKNOWN, n, n);
    // assert: start < n && n > 0
    // skip whitespace
    while (i < n && is_whitespace(s_get(s, i))) i++;
    // end of string reached while skipping whitespace?
    if (i >= n) return make_token(UNKNOWN, start, n);
    // assert: i < n
    char c = s_get(s, i);
    if (is_operator(c)) { // we only have single-character operators
        return make_token(OPERATOR, i, i + 1);
    }
    if (is_float_character(c)) { // a float number may consist of multiple characters
        int j = i;
        while (is_float_character(c)) {
            j++;
            if (j >= n) break;
            c = s_get(s, j);
        }
        // assert: j >= n || !is_float_character(s_get(s, j))
        return make_token(OPERAND, i, j);
    }
    // skip unknown characters
    int j = i;
    while (is_unknown_character(c)) {
        j++;
        if (j >= n) break;
        c = s_get(s, j);
    }
    // assert: j >= n || !is_unknown_character(s_get(s, j))
    return make_token(UNKNOWN, start, j);
}

void apply_operator(char operator) {
    double operand2;
    switch(operator) {
    case '+': 
        push(pop() + pop()); 
        break;
    case '-':
        operand2 = pop();
        push(pop() - operand2);
        break;
    case '*':    
        push(pop() * pop());
        break;
    case '/':    
        operand2 = pop();
        push(pop() / operand2);
        break;
    }
}

/*
15  7  1  1  + - / 2 *
15  7  2 - / 2 *
15  5 / 2 *
3 2 *
6
*/

int main(void) {
    // next_token_test();
    stack = dl_create();
    String input = "15  7  1  1  + - / 2 *";
    int i = 0;
    int n = s_length(input);
    while (true) {
        Token t = next_token(input, i);
        printsln(s_sub(input, t.start, t.end));
        if (t.start >= n) break;
        if (t.type == OPERAND) {
            String s = s_sub(input, t.start, t.end);
            push(d_of_s(s)); // convert string to double, then push double
        } else if (t.type == OPERATOR) {
            apply_operator(s_get(input, t.start));
        }
        i = t.end;
    }
    if (!isEmpty()) printf("= %f\n", pop());
    return 0;
}
