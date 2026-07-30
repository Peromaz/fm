#ifndef MENU_H
#define MENU_H

#include <limits.h>
#include <dirent.h>
#include <ncurses.h>

/* This data structure represents one option in a menu */
typedef struct _OPTION{
    int highlighted;
    char description[NAME_MAX];
    //char filepath[PATH_MAX];
}OPTION;

/* This data structure represents the data within a menu, it contains a list of options and its amount of choices */
typedef struct _MENU{
    OPTION *options;
    int n_choices;
}MENU;

/* For the dirent library, this counts the amount of entries ina directory stream */
int get_dir_entry_count(DIR* dir_ptr);

/* This actually creates the MENU data structure and stores the directory entries as options in the menu */
MENU *create_menu(const char* filepath);

/* draws a given menu to a given window */
void wdraw_menu(WINDOW* win, MENU *menu, int height, int width, int y, int x);
#endif
