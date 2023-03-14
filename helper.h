#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include "doublylinkedlist.h"
#include <bits/pthreadtypes.h>

char* load_next(char* in, char* val, int len);

void print_region(char* in, char* offset_in, int radius, int size);

void print_region_list(char* in, int radius, int size, Node* head);
