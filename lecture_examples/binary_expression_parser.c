/*
Compile: make binary_expression_parser3
Run: ./binary_expression_parser3
*/

#include "base.h"

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

bool is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

bool is_operand_character(char c) {
    return c == '.' || c == '-' || (c >= '0' && c <= '9');
}

bool is_unknown_character(char c) {
    return !is_operator(c) && !is_operand_character(c);
}

bool test_equal_token(int line, Token actual, Token expected) {
    bool b1 = base_test_equal_i(__FILE__, line, actual.type, expected.type);
    bool b2 = base_test_equal_i(__FILE__, line, actual.start, expected.start);
    bool b3 = base_test_equal_i(__FILE__, line, actual.end, expected.end);
    return b1 && b2 && b3;
}

Token next_token(String s, int start);

void next_token_test() {
    test_equal_token(__LINE__, next_token(" 1.34 + 89", 0), make_token(OPERAND, 1, 5));
    test_equal_token(__LINE__, next_token(" 1.34 + 89", 5), make_token(OPERATOR, 6, 7));
    test_equal_token(__LINE__, next_token(" 1.34 + 89", 7), make_token(OPERAND, 8, 10));
    test_equal_token(__LINE__, next_token(" 1.34 + 89", 10), make_token(UNKNOWN, 10, 10));
    test_equal_token(__LINE__, next_token("", 0), make_token(UNKNOWN, 0, 0));
    test_equal_token(__LINE__, next_token("", 5), make_token(UNKNOWN, 0, 0));
    test_equal_token(__LINE__, next_token(" ", 0), make_token(UNKNOWN, 0, 1));
    test_equal_token(__LINE__, next_token(" ", 5), make_token(UNKNOWN, 1, 1));
    test_equal_token(__LINE__, next_token(" abc", 0), make_token(UNKNOWN, 0, 4));
    test_equal_token(__LINE__, next_token(" ab3", 0), make_token(OPERAND, 3, 4));
}

Token next_token(String s, int start) {
    int i = start;
	int n = strlen(s);
	if (n <= 0 || i >= n) return make_token(UNKNOWN, n, n);
	// assert: n > 0 && start < n

	// skip unknown characters (whitespace, letters, etc.)
	while (i < n && is_unknown_character(s[i])) i++;

	// end of string reached while skipping whitespace?
	if (i >= n) return make_token(UNKNOWN, start, n);
	// assert: i < n
	char c = s[i];
	if (is_operator(c)) { // we only have single-character operators
		return make_token(OPERATOR, i, i + 1);
	}
	if (is_operand_character(c)) { // an operand may consist of multiple characters
		int j = i;
		while (is_operand_character(c)) {
			j++;
			if (j >= n) break;
			c = s[j];
		}
		// assert: j >= n || !is_operand_character(s_get(s, j))
		return make_token(OPERAND, i, j);
	}
    return make_token(UNKNOWN, n, n);
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
    if (token1.type != OPERAND) printsln("Error: The first token is not an operand!");
    if (token2.type != OPERATOR) printsln("Error: The second token is not an operator!");
    if (token3.type != OPERAND) printsln("Error: The third token is not an operand!");
    double operand1 = d_of_s(expression + token1.start);
    char operator = expression[token2.start];
    double operand2 = d_of_s(expression + token3.start);
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
    next_token_test();
    evaluate_test();
    String expression;
    while (strcmp(expression = s_input(100), "exit") != 0) {
        prints("= ");
        printdln(evaluate(expression));
    }
    return 0;
}
