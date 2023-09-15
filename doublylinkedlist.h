/*
 * Simple doubly linked list implementation to store pattern offsets
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef struct Node {
    char* data;
    struct Node* prev;
    struct Node* next;
} Node;

typedef struct DoublyLinkedList {
    Node* head;
    Node* tail;
    int length;
} DoublyLinkedList;

DoublyLinkedList* create_doubly_linked_list();
void destroy_doubly_linked_list(DoublyLinkedList* list);

void push_back(DoublyLinkedList* list, char* data);

