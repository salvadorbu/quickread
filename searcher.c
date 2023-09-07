#include "searcher.h"

/*
Search file for term match from the starting pointer and add matches to results linked list 
*/
void* search(void* base)
{
	search_base* sb = base;
    
	for (int offset = 0; offset < sb->buffer; offset++)
	{
        if (memcmp(sb->in + offset, sb->target, sb->target_length) == 0)
        {
            push_back(sb->result_list, sb->in + offset);
        }
	}

	free(base);
	pthread_exit(NULL);
}

/*
Perform search with multiple threads
*/
void search_multithread(search_base* sb, int thread_count)
{
	int buffer_length = (sb->buffer / thread_count);
	pthread_t threads[thread_count];
	DoublyLinkedList* result_lists[thread_count];
	
    // Create result list for each thread used
	for (int i = 0; i < thread_count; i++)
	{
		result_lists[i] = create_doubly_linked_list();
	}

    // Create search base for each thread with
    // "in" offsets (search starting points) evenly spread across mmap pointer
	for (int i = 0; i < thread_count; i++)
	{
		int offset = buffer_length * i;

		search_base* newbase = malloc(sizeof(search_base));
		newbase->buffer = buffer_length;
		newbase->in = sb->in + offset;
		newbase->target = sb->target;
        newbase->target_length = sb->target_length;
		newbase->result_list = result_lists[i];

		int rc = pthread_create(&threads[i], NULL, search, newbase);
		if (rc)
		{
			printf("Thread creation error\n");
			exit(EXIT_FAILURE);
		}
	}

	int total_length = 0;
	
    // terminate all threads and add results count to total
	for (int i = 0; i < thread_count; i++)
	{
		pthread_join(threads[i], NULL);
		total_length += result_lists[i]->length;
	}

    // combine the linked lists for each thread into one
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

		free(next_list);
	}

	joined_list->length = total_length;
	sb->result_list = joined_list;
}

