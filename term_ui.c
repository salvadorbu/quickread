#include "term_ui.h"
#include <curses.h>

int calculate_radius(char* base, int size, char* offset_in, int cl, int rl)
{
    int mrad = (cl * rl) / 2;
    int lnc = 0, rnc = 0;
    for (int cc = 0; cc < mrad; cc++)
    {
        if (offset_in - cc < base || offset_in + cc >= base + size) return cc;
        if (lnc >= rl / 2 || rnc >= rl / 2) return cc;
        char cl = *(offset_in - cc), cr = *(offset_in + cc);
        if (cl == '\n') lnc++;
        if (cr == '\n') rnc++;
    }
    return mrad;
}

void initialize_ui(char* base, DoublyLinkedList results, int size)
{
    initscr();
    noecho();
    curs_set(0);
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    const int text_area_width = xMax / 2 - 2;
    const int text_area_height = yMax / 2 - 2;

    WINDOW* top_win = newwin(yMax / 4, xMax - ART_LEN, 0, (xMax - ART_LEN) / 2);
    WINDOW* text_win = newwin(yMax / 2, xMax / 2, yMax / 4, xMax / 4);
    WINDOW* bottom_win = newwin(yMax / 4, xMax / 2, yMax - yMax / 4, xMax / 4);
    box(text_win, 0, 0); 
    
    mvwprintw(text_win, 1, 1, "Enter text...");

    mvwprintw(top_win, 1, 1, "________        .__        __     __________                   .___");
    mvwprintw(top_win, 2, 1, "\\_____  \\  __ __|__| ____ |  | __ \\______   \\ ____ _____     __| _/");
    mvwprintw(top_win, 3, 1, " /  / \\  \\|  |  \\  |/ ___\\|  |/ /  |       _// __ \\\\__  \\   / __ | ");
    mvwprintw(top_win, 4, 1, "/   \\_/.  \\  |  /  \\  \\___|    <   |    |   \\  ___/ / __ \\_/ /_/ | ");
    mvwprintw(top_win, 5, 1, "\\_____\\ \\_/____/|__|\\___  >__|_ \\  |____|_  /\\___  >____  /\\____ | ");
    mvwprintw(top_win, 6, 1, "       \\__>             \\/     \\/         \\/     \\/     \\/      \\/ ");
    
    mvwprintw(bottom_win, 0, 1, "Quit (q)\tNext (n)\tPrevious (p)");
    wrefresh(top_win);
    wrefresh(bottom_win);

    Node* curr = results.head;
    wrefresh(text_win);

    char ch;
    while ((ch = wgetch(top_win)))
    {
        switch (ch)
        {
            case 'q':
                endwin();
                return;
            case 'n':
                curr = curr->next;
                if (curr == NULL) curr = results.head;
                char test[100] = "\0";
                char* temp = curr->data;
                int i = 0;
                while (*temp != '\n') {
                    test[i] = *temp;
                    i++;
                    temp++;
                }
                base = base;
                size = size;
                mvwprintw(text_win, 1, 1, "%s", test);
                break;
            case 'p':
                curr = curr->prev;
                if (curr == NULL) curr = results.tail;
                mvwprintw(text_win, 1, 1, "__RADIUS: %d__", 
                          calculate_radius(base, size, curr->data, text_area_width, text_area_height));
                break;
        }
        wrefresh(text_win);
        wrefresh(top_win);
        wrefresh(bottom_win);
    }
}

