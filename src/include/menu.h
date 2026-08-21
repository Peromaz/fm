#ifndef MENU_H
#define MENU_H

#include <ncurses.h>
#include <limits.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* This data structure represents one option in a menu */
typedef struct _OPTION{
    char description[NAME_MAX];
    int isDirectory;
}OPTION;

/* This data structure represents the data within a menu, it contains a list of options and its amount of choices */
typedef struct _MENU{
    OPTION *options;
    int highlight_pos;
    int n_choices;
}MENU;

/* For the dirent library, this counts the amount of entries ina directory stream */
int get_dir_entry_count(DIR* dir_ptr);

/* This actually creates the MENU data structure and stores the directory entries as options in the menu */
MENU *create_menu(const char* filepath);
void free_menu(MENU* menu);
int go_to_directory(const char* directory);
char* get_next_directory(MENU *curr_menu);
void menu_driver(WINDOW *win, MENU *menu, int ch);

/* draws a given menu to a given window */
void wdraw_menu(WINDOW* win, MENU *menu, int y, int x, int highlight, int total_menu_rows);
#endif
