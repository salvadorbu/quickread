#include "term_ui.h"

typedef struct search_base
{
    char* in;
    char* pattern;
    int pattern_length;
    uint64_t buffer;
    int* bmh_table;
    DoublyLinkedList* result_list;
} search_base;



void* search(void* base);

void search_multithread(search_base* sb, int threads);

