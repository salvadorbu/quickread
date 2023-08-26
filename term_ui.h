#include <ncurses.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <bits/pthreadtypes.h>

#include "doublylinkedlist.h"

#define ART_LEN 67

#define NEWLINE_SPACE 1
#define TAB_SPACE 4

void initialize_ui(char* base, DoublyLinkedList results, int size, int query_len);

