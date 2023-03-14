#include "doublylinkedlist.h"

/*
Create new linked list
*/
DoublyLinkedList* create_doubly_linked_list() {
    DoublyLinkedList* list = malloc(sizeof(DoublyLinkedList));
    if (list == NULL) {
        printf("malloc fail\n");
        exit(EXIT_FAILURE);
    }
    list->head = NULL;
    list->tail = NULL;
    return list;
}

/*
Free linked list
*/
void destroy_doubly_linked_list(DoublyLinkedList* list) {
    Node* current = list->head;
    while (current != NULL) {
        Node* next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

/*
Append item to linked list
*/
void push_back(DoublyLinkedList* list, char* data) {
    Node* node = malloc(sizeof(Node));
    if (node == NULL) {
        printf("malloc fail\n");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->prev = list->tail;
    node->next = NULL;
    if (list->tail != NULL) list->tail->next = node;
    list->tail = node;
    if (list->head == NULL) list->head = node;
}

/*
Print pointer addresses (logging purposes)
*/
void print_list(DoublyLinkedList* list) {
    printf("[ ");
    Node* current = list->head;
    while (current != NULL) {
        printf("%p\n", current->data);
        current = current->next;
    }
    printf("]\n");
}
