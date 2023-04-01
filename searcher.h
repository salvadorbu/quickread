#include "term_ui.h"

typedef struct search_base
{
    char* in;
    char* target;
    int buffer;
    DoublyLinkedList* result_list;
} search_base;



void* search(void* base);

void search_multithread(search_base* sb, int threads);

