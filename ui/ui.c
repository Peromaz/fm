#include <ncurses.h>
#include "ui.h"

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


