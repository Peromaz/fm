#include <ncurses.h>
/* My Libraries */
#include "ui/ui.h"
int main(){
    WINDOW *prev_dir_win;
    WINDOW *curr_dir_win;
    WINDOW *next_dir_win;

    int ch;
    /* RULES */
    initscr(); //Starting ncurses
    cbreak(); //Disable line buffering
    keypad(stdscr, TRUE); //Allows all keys
    noecho();
    printw("File path goes here");
    mvprintw(0, COLS - 25,"LINES = %d COLS = %d", LINES, COLS); 
    int window_width = COLS / 3;
    refresh(); 
    prev_dir_win = create_new_window(LINES - 1, window_width, 1, 0);
    curr_dir_win = create_new_window(LINES - 1, window_width, 1, window_width);
    next_dir_win = create_new_window(LINES - 1, window_width, 1, window_width * 2);
    
    while ((ch = getch()) != 'q'){

    }
    endwin();
    return 0;
}


