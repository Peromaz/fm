#include <ncurses.h>
#include <limits.h>
#include "menu.h"

typedef struct _OPTION{
    char description[NAME_MAX];
    char filepath[PATH_MAX];
}OPTION;

typedef struct _MENU{
    OPTION **options;
    int n_choices;
}MENU;

void draw_option(OPTION *option, int y, int x ){
    mvprintw(y, x, "%s", option -> description);  
}

void draw_menu(MENU *menu, int height, int width, int y, int x){
    int i;
    for(i = 0; i < menu -> n_choices; ++i){
    }
}
