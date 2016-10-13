/*
Compile: make basic_list
Run: ./basic_list
Compile & run: make basic_list && ./basic_list
*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Node {
    int value;
    struct Node *next;
} Node;

typedef struct List {
    Node *first;
} List;

List *new_list(void) {
    return calloc(1, sizeof(List));
}

Node *new_node(int value, Node *next) {
    Node *node = calloc(1, sizeof(Node));
    node->value = value;
    node->next = next;
    return node;
}

void free_list(List *list) {
    Node *next = NULL;
    for (Node *n = list->first; n != NULL; n = next) {
        next = n->next;
        free(n);
    }
    list->first = NULL;
    free(list);
}

void print_list(List *list) {
    printf("[");
    for (Node *n = list->first; n != NULL; n = n->next) {
        printf("%d", n->value);
        if (n->next != NULL) printf(" ");
    }
    printf("]\n");
}

void print_elements_rec(Node *node) {
    if (node == NULL) return;
    printf("%d", node->value);
    if (node->next != NULL) printf(" ");
    print_elements_rec(node->next);
}

void print_list_rec(List *list) {
    printf("[");
    print_elements_rec(list->first);
    printf("]\n");
}

int length_list(List *list) {
    int i = 0;
    for (Node *n = list->first; n != NULL; n = n->next) {
        i++;
    }
    return i;
}

int length_list_help(Node *node) {
    if (node == NULL) return 0;
    else return 1 + length_list_help(node->next);
}

int length_list_rec(List *list) {
    return length_list_help(list->first);
}

void prepend_list(List *list, int value) {
    Node *n = new_node(value, list->first);
    list->first = n;
}

void append_list(List *list, int value) {
    Node *n = new_node(value, NULL);
    Node *m = list->first;
    if (m == NULL) {
        list->first = n;
    } else {
        while (m->next != NULL) m = m->next;
        m->next = n;
    }
}

void insert_list(List *list, int index, int value) {
    if (index < 0) return; // invalid index
    if (index == 0) { // insert at front
        prepend_list(list, value);
        return;
    }
    Node *node = list->first;
    int k = 1;
    while (node != NULL && k < index) { // skip index - 1 nodes
        node = node->next;
        k++;
    }
    if (node != NULL) { // insert after node
        Node *node2 = new_node(value, node->next);
        node->next = node2;
    }
}

void remove_list(List *list, int index) {
    if (index < 0 || list->first == NULL) return;
    if (index == 0) {
        Node *del = list->first;
        list->first = del->next;
        free(del);
        return;
    }
    Node *node = list->first;
    int k = 1;
    while (node != NULL && k < index) {
        node = node->next;
        k++;
    }
    if (node != NULL && node->next != NULL) {
        Node *del = node->next;
        node->next = del->next;
        free(del);
    }
}

void insert_ordered(List *list, int value) {
    Node *node = new_node(value, NULL);
    if (list->first == NULL) { // empty list
        list->first = node;
    } else if (value < list->first->value) { // insert before first
        node->next = list->first;
        list->first = node;
    } else { // find insertion position in non-empty list, after first node
        struct Node *n;
        for (n = list->first; n != NULL; n = n->next) {
            if (n->next == NULL || value < n->next->value) { // last element or > next
                node->next = n->next;
                n->next = node;
                break; // leave for-loop
            }
        }
    }
    
}

List *list_of_string(char *s) {
    List *list = new_list();
    char *t = s;
    while (*t != '\0') {
        if (isdigit(*t)) {
            if ((t > s) && (*(t - 1) == '-')) t--; // check for minus sign, no whitespace between '-' and digit
            append_list(list, atoi(t)); // convert digit string to int
            t++; // assert: *t is a digit or '-', skip
            while (isdigit(*t)) t++; // skip int
            // assert: *t is not a digit, *t may be '\0'
        } else {
            // assert: *t is not a digit, *t is not '\0'
            t++; // not a digit, skip
        }
    }
    return list;
}

int main(void) {
    List *list = list_of_string("2, 4, 6");
    append_list(list, -1);
    insert_list(list, 3, 7);
    insert_list(list, 2, 5);
    insert_list(list, 1, 3);
    insert_list(list, 0, 1);
    append_list(list, -2);
    prepend_list(list, 99);
    print_list(list);
    print_list_rec(list);
    printf("length = %d\n", length_list_rec(list));
    free_list(list);
    
    list = new_list();
    for (int i = 0; i < 10; i++) {
        insert_ordered(list, rand() % 10);
    }
    print_list(list);
    free_list(list);

    return 0;
}
