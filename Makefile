CFLAGS = -std=c11 -Wall -Wextra -c

all: quickread

quickread: main.o searcher.o searcher.h term_ui.o term_ui.h doublylinkedlist.o doublylinkedlist.h
	gcc -o quickread main.o searcher.o term_ui.o doublylinkedlist.o -lncurses -pthread 

main.o: main.c searcher.h term_ui.h doublylinkedlist.h
	gcc ${CFLAGS} main.c

searcher.o: searcher.c searcher.h term_ui.h
	gcc ${CFLAGS} searcher.c

term_ui.o: term_ui.c term_ui.h
	gcc ${CFLAGS} term_ui.c

doublylinkedlist.o: doublylinkedlist.c doublylinkedlist.h
	gcc ${CFLAGS} doublylinkedlist.c

clean:
	rm *.o
	rm quickread
