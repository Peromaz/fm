#ifndef UI_H
#define UI_H
#include <ncurses.h> 
#include <unistd.h>
#include <limits.h>

void printcwd();
void draw_panel(WINDOW*, int, const char*);
void draw_ui();
void erase_ui();

WINDOW* create_new_window(int height, int width,
	int starty, int startx);

void destroy_window(WINDOW *local_window);
#endif
