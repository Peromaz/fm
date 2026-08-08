#include "include/ui.h"

void printcwd(){
    char cwd[PATH_MAX];
    getcwd(cwd, sizeof(cwd));
    move(0,0);
    clrtoeol();
    printw("Current working directory: %s", cwd);
    refresh();
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

