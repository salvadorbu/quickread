#include "doublylinkedlist.h"

/*
 * Create new linked list
 */
DoublyLinkedList* create_doubly_linked_list() {
    DoublyLinkedList* list = malloc(sizeof(DoublyLinkedList));

    if (list == NULL) {
        printf("malloc fail\n");
        exit(EXIT_FAILURE);
    }

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;

    return list;
}

/*
 * Free linked list
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
 * Append item to linked list
 */
void push_back(DoublyLinkedList* list, char* data) {
    Node* node = malloc(sizeof(Node));
    list->length = list->length + 1;

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

