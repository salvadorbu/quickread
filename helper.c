#include "helper.h"

/*
Read from memory mapped region and add characters to corresponding value variable
*/
char* load_next(char* in, char* val, int len)
{
	for (int i = 0; i < len; i++)
	{
		val[i] = in[i];
	}
	val[len] = '\0';
	return in + 1;
}

/*
Print out a chunk of the file relative to an offset
*/
void print_region(char* in, char* offset_in, int radius, int size)
{
	char* begin = offset_in - radius;
	char* end = offset_in + radius;

	begin = begin > in ? begin : in;
	end = end <= in + size ? end : in + size;
	char* curr = begin;

	while (curr != end)
	{
		printf("%c", *curr);
		curr += 1;
	}
	printf("\n");
}

/*
Print list of results stored in linked list
*/
void print_region_list(char* in, int radius, int size, Node* head)
{
	Node* curr = head;
	int index = 1;
	while (curr != NULL)
	{
		printf("Result (%d)\n", index);
		print_region(in, curr->data, 200, size);
		index++;
		curr = curr->next;
	}

}

