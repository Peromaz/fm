#include <ncurses.h>
#include <unistd.h>
#include "ui.h"
#include "Menu/menu.h"

/* Main three windows */
WINDOW *prev_dir_win;
WINDOW *curr_dir_win;
WINDOW *next_dir_win;

void draw_ui(){
    initscr(); //Starting ncurses
    cbreak(); //Disable line buffering
    keypad(stdscr, TRUE); //Allows all keys
    noecho();
    curs_set(0); //sets the cursor to invisible, small chance it may crash the program if terminal doesn't support it
    
    MENU* curr_dir_menu;
    curr_dir_menu = create_menu(".");  
    MENU* prev_dir_menu;
    prev_dir_menu = create_menu("..");

    printcwd();
    int window_width = COLS / 3;
    refresh();
    /* draw the panels */
    /* draw previous panel */
    prev_dir_win = create_new_window(LINES -1, window_width, 1, 0);
    draw_panel(prev_dir_win, window_width, "PREVIOUS");
    wdraw_menu(prev_dir_win, prev_dir_menu, 3,1); 
    wrefresh(prev_dir_win);
    /* draw current panel */
    curr_dir_win = create_new_window(LINES -1, window_width, 1, window_width);
    draw_panel(curr_dir_win, window_width, "CURRENT");
    wdraw_menu(curr_dir_win, curr_dir_menu, 3,1); 
    wrefresh(curr_dir_win);
    /* draw next panel */ 
    next_dir_win = create_new_window(LINES - 1, window_width, 1, window_width * 2);
    draw_panel(next_dir_win, window_width, "NEXT");
}
void printcwd(){
    char cwd[4096];
    getcwd(cwd, sizeof(cwd));
    printw("Current working directory: %s\n", cwd);
}
void draw_panel(WINDOW *win, int width, const char* panel_description ){
    mvwprintw(win, 1, 1, "%s", panel_description);
    /* Adds line flush with rest of window */
    mvwaddch(win, 2, 0, ACS_LTEE);
    mvwhline(win, 2, 1, 0, width - 2);
    mvwaddch(win, 2, width-1, ACS_RTEE); 
    // draw
    wrefresh(win);
}
WINDOW* create_new_window(int height, int width, int starty, int startx){
    WINDOW *local_window;

    local_window = newwin(height, width, starty, startx);
    box(local_window, 0, 0);
    //wrefresh(local_window);
    return local_window;
}

void destroy_window(WINDOW *local_window){
    wborder(local_window, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(local_window);
    delwin(local_window);
}

void erase_ui(){
    endwin();
}

