CFLAGS = -std=c11 -Wall -Wextra -c

all: quickread

quickread: main.o searcher.o searcher.h helper.o helper.h doublylinkedlist.o doublylinkedlist.h
	gcc -o quickread main.o searcher.o helper.o doublylinkedlist.o 

main.o: main.c searcher.h helper.h doublylinkedlist.h
	gcc ${CFLAGS} main.c

searcher.o: searcher.c searcher.h helper.h
	gcc ${CFLAGS} searcher.c

helper.o: helper.c helper.h doublylinkedlist.h
	gcc ${CFLAGS} helper.c

doublylinkedlist.o: doublylinkedlist.c doublylinkedlist.h
	gcc ${CFLAGS} doublylinkedlist.c

clean:
	rm *.o
	rm quickread
