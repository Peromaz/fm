#include <ncurses.h>
#include <unistd.h>
#include "ui.h"

/* Main three windows */
WINDOW *prev_dir_win;
WINDOW *curr_dir_win;
WINDOW *next_dir_win;

void draw_ui(){
    initscr(); //Starting ncurses
    cbreak(); //Disable line buffering
    keypad(stdscr, TRUE); //Allows all keys
    noecho();
    
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));
    
    printw("Current working directory: %s\n", cwd);

    int window_width = COLS / 3;
    refresh(); 
    prev_dir_win = create_new_window(LINES - 1, window_width,
	    1, 0);
    curr_dir_win = create_new_window(LINES - 1, window_width,
	    1, window_width);
    next_dir_win = create_new_window(LINES - 1, window_width,
	    1, window_width * 2);
}
void erase_ui(){
    endwin();
}
WINDOW* create_new_window(int height, int width, int starty, int startx){
    WINDOW *local_window;

    local_window = newwin(height, width, starty, startx);
    box(local_window, 0, 0);
    wrefresh(local_window);
    return local_window;
}

void destroy_window(WINDOW *local_window){
    wborder(local_window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(local_window);
    delwin(local_window);
}


