#include "term_ui.h"

typedef struct search_base
{
    char* in; //mmap pointer
    char* target; //user search pattern
    int target_length; // # of chars in pattern
    int buffer; //size of the search region
    DoublyLinkedList* result_list;
} search_base;



void* search(void* base);

void search_multithread(search_base* sb, int threads);

