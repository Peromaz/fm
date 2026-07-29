#pragma once
#include <ncurses.h> 

WINDOW* create_new_window(int height, int width, int starty, int startx);
void destroy_window(WINDOW *local_window);
