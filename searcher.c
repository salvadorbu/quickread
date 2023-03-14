#include "searcher.h"

/*
* Search
* i.e buffer_len = 3; in=alphabet
* abcdefghijkl
* abc
* bcd
* cde
*/
void* search(void* base)
{
	search_base* sb = base;
	int len = strlen((*sb).target);
	char curr[len + 1];
	for (int i = 0; i < (*sb).buffer; i++)
	{
		(*sb).in = load_next((*sb).in, curr, len);
		if (strcmp(curr, (*sb).target) == 0)
		{
			push_back(sb->result_list, sb->in - 1);
		}
	}
	free(base);
	pthread_exit(NULL);
}

void search_multithread(search_base* sb, int thread_count)
{
	int target_length = strlen(sb->target);
	int buffer_length = (sb->buffer / thread_count);
	pthread_t threads[thread_count];
	DoublyLinkedList* result_lists[thread_count];
	int thread_ids[thread_count];
	
	for (int i = 0; i < thread_count; i++)
	{
		result_lists[i] = create_doubly_linked_list();
	}

	for (int i = 0; i < thread_count; i++)
	{
		int offset = buffer_length * i;

		search_base* newbase = malloc(sizeof(search_base));
		newbase->buffer = buffer_length;
		newbase->in = sb->in + offset;
		newbase->target = sb->target;
		newbase->result_list = result_lists[i];
		int rc = pthread_create(&threads[i], NULL, search, newbase);
		if (rc)
		{
			printf("Thread creation error\n");
			exit(EXIT_FAILURE);
		}
	}
	
	for (int i = 0; i < thread_count; i++)
	{
		pthread_join(threads[i], NULL);
	}
	
	DoublyLinkedList* joined_list = result_lists[0];
	for (int i = 1; i < thread_count; i++)
	{
		DoublyLinkedList* next_list = result_lists[i];
		if (joined_list->head == NULL)
		{
			destroy_doubly_linked_list(joined_list);
			joined_list = next_list;
			continue;
		}
		if (next_list->head == NULL)
		{
			destroy_doubly_linked_list(next_list);
			continue;
		}
		joined_list->tail->next = next_list->head;
		next_list->head->prev = joined_list->tail;
		joined_list->tail = next_list->tail;
	}
	sb->result_list = joined_list;
}

