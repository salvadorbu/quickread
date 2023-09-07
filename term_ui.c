#include "term_ui.h"
#include <ncurses.h>

/*
Update the row and column positions after processing a character
*/
void update_win_offset(char character, int* row_offset, int* column_offset, int column_length)
{
    switch (character)
    {
        case '\n':
            ++*(row_offset);
            *column_offset = 1;
            break;
        case '\t':
            *(column_offset) += 4;
            break;
        default:
            ++*(column_offset);
            break;
    }

    if (*column_offset >= column_length)
    {
        *column_offset = (*column_offset) % column_length + 1;
        ++*(row_offset);
    }
}

/*
Get the starting character pointer for the text displayed
*/
char* get_starting_pointer(char* base, char* match_offset, int row_length, int column_length)
{
    int rows_used = 1;
    int current_column = column_length / 2;

    while (rows_used < row_length / 2 && match_offset >= base)
    {
        match_offset--;
        update_win_offset(*match_offset, &rows_used, &current_column, column_length);
    }
    
    int leading_limit = 0;

    while (*match_offset != '\n' && match_offset >= base && leading_limit < column_length / 2)
    {
        match_offset--;
        leading_limit++;
    }

    return match_offset + 1;
}

/*
Display word match result onto the terminal UI 
*/
void print_entry(WINDOW* win, char* base, int size, char* offset_in, int column_length, int row_length, int query_len)
{
    wclear(win);
    box(win, 0, 0);

    int column = 1;
    int row = 1;

    char* start = get_starting_pointer(base, offset_in, row_length, column_length);

    while (row <= row_length && start <= base + size)
    {
        if (start >= offset_in && start < offset_in + query_len)
            wattron(win, COLOR_PAIR(1));
        else
            wattroff(win, COLOR_PAIR(1));
        
        if (*start != '\n' && *start != '\t') mvwprintw(win, row, column, "%c", *start);

        update_win_offset(*start, &row, &column, column_length);
        start++;
    }
}

/*
Setup the terminal UI
*/
void initialize_ui(char* base, DoublyLinkedList results, int size, int query_len)
{
    initscr();
    noecho();
    curs_set(0);
    start_color();

    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    if (xMax < 75 || yMax < 35)
    {
        printf("Window sizing below minimum\n");
        endwin();
        return;
    }

    int results_count = results.length; 

    const int text_area_width = xMax / 2 - 2;
    const int text_area_height = yMax / 2 - 2;

    WINDOW* top_win = newwin(yMax / 4, ART_LEN + 2, 0, (xMax - ART_LEN) / 2 - 2);
    WINDOW* text_win = newwin(yMax / 2, xMax / 2, yMax / 4, xMax / 4);
    WINDOW* bottom_win = newwin(yMax / 4, xMax / 2, yMax - yMax / 4, xMax / 4);
    
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_BLACK, COLOR_WHITE);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    wattron(top_win, COLOR_PAIR(2));

    mvwprintw(top_win, 1, 2, "________        .__        __     __________                   .___");
    mvwprintw(top_win, 2, 2, "\\_____  \\  __ __|__| ____ |  | __ \\______   \\ ____ _____     __| _/");
    mvwprintw(top_win, 3, 2, " /  / \\  \\|  |  \\  |/ ___\\|  |/ /  |       _// __ \\\\__  \\   / __ | ");
    mvwprintw(top_win, 4, 2, "/   \\_/.  \\  |  /  \\  \\___|    <   |    |   \\  ___/ / __ \\_/ /_/ | ");
    mvwprintw(top_win, 5, 2, "\\_____\\ \\_/____/|__|\\___  >__|_ \\  |____|_  /\\___  >____  /\\____ | ");
    mvwprintw(top_win, 6, 2, "       \\__>             \\/     \\/         \\/     \\/     \\/      \\/ ");

    box(bottom_win, 0, 0);
    box(text_win, 0, 0);
    mvwprintw(bottom_win, 1, 1, "Quit (q)  Next (n)  Previous (p)");

    Node* curr = results.head;
    char ch;
    int result_index = 0;
    
    print_entry(text_win, base, size, curr->data, text_area_width, text_area_height, query_len);
    mvwprintw(bottom_win, 1, 1, "Quit (q)  Next (n)  Previous (p)\tResult (%d / %d)",
		    result_index % results_count + 1, results_count);

    wrefresh(top_win);
    wrefresh(bottom_win);
    wrefresh(text_win); 

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

                result_index = (result_index + 1) % results_count;
                print_entry(text_win, base, size, curr->data, text_area_width, text_area_height, query_len);
                break;

            case 'p':
                curr = curr->prev;
                if (curr == NULL) curr = results.tail;

                result_index = result_index <= 0 ? results_count - 1 : result_index - 1;
                print_entry(text_win, base, size, curr->data, text_area_width, text_area_height, query_len);
                break;
        }
        wclear(bottom_win);
        box(bottom_win, 0, 0);
	    mvwprintw(bottom_win, 1, 1, "Quit (q)  Next (n)  Previous (p)\tResult (%d / %d)", 
			result_index % results_count + 1, results_count);

        wrefresh(text_win);
        wrefresh(top_win);
        wrefresh(bottom_win);
    }
}

