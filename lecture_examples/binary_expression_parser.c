/*
Compile: make binary_expression_parser
Run: ./binary_expression_parser
*/

#include "base.h"
#include "string.h"

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

double evaluate(String expression);

void evaluate_test(void) {
    test_within_d(evaluate(" 1.5 + 2.5"), 4.0, EPSILON);
    test_within_d(evaluate(" 2 * 30 "), 60, EPSILON);
    test_within_d(evaluate("1.5 - 2.5 "), -1.0, EPSILON);
    test_within_d(evaluate(" 9.0  /  2.0"), 4.5, EPSILON);
    test_within_d(evaluate("100 *   0.01 "), 1.0, EPSILON);
}

double evaluate(String expression) {
    Token token1 = next_token(expression, 0);
    Token token2 = next_token(expression, token1.end);
    Token token3 = next_token(expression, token2.end);
    String s = s_sub(expression, token1.start, token1.end);
    double operand1 = d_of_s(s);
    char operator = s_get(expression, token2.start);
    s = s_sub(expression, token3.start, token3.end);
    double operand2 = d_of_s(s);
    // printf("%g %c %g\n", operand1, operator, operand2);
    switch (operator) {
        case '+': return operand1 + operand2;
        case '-': return operand1 - operand2;
        case '*': return operand1 * operand2;
        case '/': return operand1 / operand2;
    }
    return 0.0;
}

int main(void) {
    // next_token_test();
    // evaluate_test();
    String expression;
    while (!s_contains(expression = s_input(100), "exit")) {
        printdln(evaluate(expression));
    }
    return 0;
}
