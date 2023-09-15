#include "searcher.h"

#define CHARSET_LENGTH 256

/*
 * Process the characters in the pattern to implement bad character shift rule
 */
void process_pattern(void* base)
{
    search_base* sb = base;
    sb->bmh_table = (int*)malloc(256 * sizeof(int));

    for (int i = 0; i < CHARSET_LENGTH; i++)
    {
        sb->bmh_table[i] = -1;
    }

    for (int i = 0; i < sb->pattern_length; i++)
    {
        sb->bmh_table[(int)sb->pattern[i]] = i;
    }
}

/*
 * Search file for term match from the starting pointer and add matches to results linked list 
 */
void* search(void* base)
{
    search_base* sb = base;
    uint64_t offset = 0;

    while (offset <= (sb->buffer - sb->pattern_length))
    {
        int index = sb->pattern_length - 1;

        while (index >= 0 && sb->pattern[index] == sb->in[offset + index])
        {
            index--;
        }

        if (index < 0)
        {
            push_back(sb->result_list, sb->in + offset);

            if (offset + sb->pattern_length < sb->buffer)
            {
                offset += sb->pattern_length - sb->bmh_table[(int)(sb->in[offset + sb->pattern_length])];
            }
            else
            {
                offset++;
            }
        }
        else
        {
            int shift_size = index - sb->bmh_table[(int)sb->in[offset + index]];
            offset += shift_size > 1 ? shift_size : 1;
        }
    }

	free(base);
	pthread_exit(NULL);
}

/*
 * Perform search with multiple threads
 */
void search_multithread(search_base* sb, int thread_count)
{
	uint64_t buffer_length = (sb->buffer / thread_count);
	pthread_t threads[thread_count];
	DoublyLinkedList* result_lists[thread_count];

    process_pattern(sb);
	
    // create result list for each thread used
	for (int i = 0; i < thread_count; i++)
	{
		result_lists[i] = create_doubly_linked_list();
	}

    // create search base for each thread with
    // "in" offsets (search starting points) evenly spread across mmap pointer
	for (int i = 0; i < thread_count; i++)
	{
		uint64_t offset = buffer_length * i;

		search_base* newbase = malloc(sizeof(search_base));
		newbase->buffer = buffer_length;
		newbase->in = sb->in + offset;
		newbase->pattern = sb->pattern;
        newbase->pattern_length = sb->pattern_length;
		newbase->result_list = result_lists[i];
        newbase->bmh_table = sb->bmh_table;

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
    free(sb->bmh_table);
}

