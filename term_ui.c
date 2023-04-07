#include "term_ui.h"
#include <curses.h>

void print_entry(WINDOW* win, char* base, int size, char* offset_in, int cl, int rl, int query_len)
{
    int cx = 0;
    int offset = -50;
    
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_BLACK, COLOR_WHITE);

    for (int row = 0; row < rl + 1; row++)
    {
        while (cx < cl)
        {
            if (offset_in + offset >= base + size)
            {
                mvwprintw(win, row, cx, " ");
                cx++;
                continue;
            }

            if (offset_in + offset >= offset_in && offset_in + offset < offset_in + query_len)
                wattron(win, COLOR_PAIR(1));
            else
                wattroff(win, COLOR_PAIR(1));

            char curr = *(offset_in + offset);

            if (curr == '\n')
            {
                cx = 0;
                row++;
            }
            mvwprintw(win, row, cx, "%c", curr);
            offset++;
            cx++;
        }
        cx = 1;
    }
}

void initialize_ui(char* base, DoublyLinkedList results, int size, int query_len)
{
    initscr();
    noecho();
    curs_set(0);
    start_color();
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    
    if (xMax < 150 || yMax < 100)
    {
        printf("Window sizing below minimum\n");
    }

    const int text_area_width = xMax / 2 - 2;
    const int text_area_height = yMax / 2 - 2;

    WINDOW* top_win = newwin(yMax / 4, xMax - ART_LEN, 0, (xMax - ART_LEN) / 2);
    WINDOW* text_win = newwin(yMax / 2, xMax / 2, yMax / 4, xMax / 4);
    WINDOW* bottom_win = newwin(yMax / 4, xMax / 2, yMax - yMax / 4, xMax / 4);

    mvwprintw(text_win, 1, 1, "Loading...");

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
                print_entry(text_win, base, size, curr->data, text_area_width, text_area_height, query_len);
                break;
            case 'p':
                curr = curr->prev;
                if (curr == NULL) curr = results.tail;
                break;
        }
        wrefresh(text_win);
        wrefresh(top_win);
        wrefresh(bottom_win);
    }
}

