#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include "searcher.h"
#include <string.h>

#define FILE_FLAG "-f"
#define SEARCH_FLAG "-s"
#define THREAD_FLAG "-t"

/*
Command Ussage:
./editor -f <file.ext> -s <search term> -t <thread count>
*/
int main(int argc, char* argv[])
{
	if (argc < 5 || argc > 7 || argc % 2 == 0)
	{
		printf("Usage: %s -f <file.ext> -s <search term> | OPTIONAL -t <thread count>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	char* file_name = NULL;
	char* search_term = NULL;
	int thread_num = 25;

	for (int argi = 1; argi < argc; argi += 2)
	{
		if (strcmp(argv[argi], FILE_FLAG) == 0)
			file_name = argv[argi + 1];
		else if (strcmp(argv[argi], SEARCH_FLAG) == 0)
			search_term = argv[argi + 1];
		else if (strcmp(argv[argi], THREAD_FLAG) == 0)
			thread_num = atoi(argv[argi + 1]);
		else {
			printf("Incorrect flag usage [Required Flags: -f, -s] \n");
			exit(EXIT_FAILURE);
		}
	}
	
	if (file_name == NULL || search_term == NULL)
	{
		printf("Missing required flags\n");
		exit(EXIT_FAILURE);
	}

	int fd = open(file_name, O_RDONLY, S_IRUSR | S_IWUSR);
	struct stat sb;

	if (fstat(fd, &sb) == -1)
	{
		perror("Couldn't find file size\n");
		exit(EXIT_FAILURE);
	}
	
	char* buff = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
	
	search_base* base = malloc(sizeof(search_base));
	base->in = buff;
	base->target = search_term;
	base->buffer = sb.st_size;
	
	search_multithread(base, thread_num);

	if (base->result_list->head != NULL) 
		initialize_ui(buff, *(base->result_list), sb.st_size, strlen(search_term));

	destroy_doubly_linked_list(base->result_list);
	free(base);
	munmap(buff, sb.st_size);
	close(fd);
}
