CFLAGS = -std=c11 -Wall -Wextra -c

all: quickread

quickread: main.o searcher.o searcher.h term_ui.o term_ui.h helper.o helper.h doublylinkedlist.o doublylinkedlist.h
	gcc -lncurses -o quickread main.o searcher.o term_ui.o helper.o doublylinkedlist.o 

main.o: main.c searcher.h term_ui.h helper.h doublylinkedlist.h
	gcc ${CFLAGS} main.c

searcher.o: searcher.c searcher.h term_ui.h
	gcc ${CFLAGS} searcher.c

term_ui.o: term_ui.c term_ui.h helper.h
	gcc ${CFLAGS} term_ui.c

helper.o: helper.c helper.h doublylinkedlist.h
	gcc ${CFLAGS} helper.c

doublylinkedlist.o: doublylinkedlist.c doublylinkedlist.h
	gcc ${CFLAGS} doublylinkedlist.c

clean:
	rm *.o
	rm quickread
