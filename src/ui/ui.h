#pragma once
#include <ncurses.h> 

void draw_ui();
void erase_ui();

WINDOW* create_new_window(int height, int width,
	int starty, int startx);

void destroy_window(WINDOW *local_window);
