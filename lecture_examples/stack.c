/*
Compile: make stack
Run: ./stack
make stack && ./stack
*/

#include "base.h"

#define STACK_SIZE 100    // stack implemented as array has fixed capacity
double stack[STACK_SIZE];    // external double array
int stack_index = 0;     // next free stack entry, external int variable

void push(double f) {
    // valid stack indices: 0 ≤ stack_index ≤ STACK_SIZE-1
    if (stack_index < STACK_SIZE) {
        // assert: 0 ≤ stack_index ≤ STACK_SIZE-1
        stack[stack_index++] = f;
    } else {
        // assert: stack_index ≥ STACK_SIZE
        printf("error: push: stack full\n");
        exit(1);
    }
}

double pop(void) {
    if (stack_index > 0) {
        return stack[--stack_index];
    } else {
        printf("error: pop: stack empty\n");
        exit(1);
        return 0.0;
    }
}

int is_empty(void) {
    return stack_index <= 0;
}

////////////////////////////////////////////////////////

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

Token next_token(String s, int start) {
    int i = start;
    int n = strlen(s);
    if (start >= n || n <= 0) return make_token(UNKNOWN, n, n);
    // assert: start < n && n > 0
    // skip whitespace
    while (i < n && is_whitespace(s[i])) i++;
    // end of string reached while skipping whitespace?
    if (i >= n) return make_token(UNKNOWN, start, n);
    // assert: i < n
    char c = s[i];
    if (is_operator(c)) { // we only have single-character operators
        return make_token(OPERATOR, i, i + 1);
    }
    if (is_float_character(c)) { // a float number may consist of multiple characters
        int j = i;
        while (is_float_character(c)) {
            j++;
            if (j >= n) break;
            c = s[j];
        }
        // assert: j >= n || !is_float_character(s_get(s, j))
        return make_token(OPERAND, i, j);
    }
    // skip unknown characters
    int j = i;
    while (is_unknown_character(c)) {
        j++;
        if (j >= n) break;
        c = s[j];
    }
    // assert: j >= n || !is_unknown_character(s_get(s, j))
    return make_token(UNKNOWN, start, j);
}

////////////////////////////////////////////////////////

void apply(char operator, double operand1, double operand2) {
    switch(operator) {
        case '+':
            push(operand1 + operand2); 
            break;
        case '-':
            push(operand1 - operand2);
            break;
        case '*':
            push(operand1 * operand2);
            break;
        case '/':
            push(operand1 / operand2);
            break;
    }
}

double evaluate(String expression);

void evaluate_test(void) {
    test_within_d(evaluate(" 1.5 2.5 +"), 4.0, EPSILON);
    test_within_d(evaluate("1.5 2.5 -"), -1.0, EPSILON);
    test_within_d(evaluate("100 0.01 *"), 1.0, EPSILON);
    test_within_d(evaluate(" 15  7  1  1  + - / 2 * "), 6.0, EPSILON);
}

double evaluate(String expression) {
    Token token = make_token(UNKNOWN, 0, 0);
    while (token.end < strlen(expression)) {
        token = next_token(expression, token.end);
        if (token.type == OPERAND) {
            double operand = d_of_s(expression + token.start);
            push(operand);
        } else if (token.type == OPERATOR) {
            char operator = expression[token.start];
            double operand2 = pop();
            double operand1 = pop();
            apply(operator, operand1, operand2);
        }
    }
    return pop();
}

#if 1
int main(void) {
    evaluate_test();
    String expression;
    while (strcmp(expression = s_input(100), "exit") != 0) {
        printdln(evaluate(expression));
    }
    return 0;
}
#endif

#if 0
int main(void) {
    push(3.5);
    push(2.5);
    printdln(pop());
    printbln(is_empty());
    printdln(pop());
    printbln(is_empty());
    return 0;
}
#endif

#if 0
void stack_test(void) {
    push(3.5);
    push(2.5);
    test_within_d(pop(), 2.5, EPSILON);
    test_equal_b(is_empty(), false);
    test_within_d(pop(), 3.5, EPSILON);
    test_equal_b(is_empty(), true);
}

int main(void) {
    stack_test();
    return 0;
}
#endif
