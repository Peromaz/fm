#include <ncurses.h>
#include <limits.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "menu.h"

int get_dir_entry_count(DIR* dir_ptr){
    int count;
    while(readdir(dir_ptr)){
	++count;	
    }
    rewinddir(dir_ptr);
    return count;
}

MENU *create_menu(const char* filepath){
    //Allocate memory for the menu
    MENU *menu = (MENU*) malloc(sizeof(MENU)); 

    struct dirent *dir_entry;
    DIR *dir_ptr;  
    dir_ptr = opendir(filepath);

    //Check if the directory even exists
    if (dir_ptr == NULL){
	perror("Could not open the directory");
	exit(1);
    }

    /* Reads over directory */
    menu -> n_choices = get_dir_entry_count(dir_ptr);
    menu -> options = (OPTION*) malloc(sizeof(OPTION) * (menu -> n_choices)); 
    if(menu -> options == NULL){
	perror("options malloc failed");
	exit(1);
    }
    int index = 0;
    while((dir_entry = readdir(dir_ptr))){
	/* get the name of entry */
	char *dir_description = dir_entry -> d_name;
	/* at runtime, allocate memory for an option */
	OPTION* entry = (OPTION*) malloc(sizeof(OPTION));
	strcpy(entry -> description, dir_description);
	entry -> highlighted = 0;	
	menu -> options[index] = *entry;
	free(entry);
	++index;
    }

    closedir(dir_ptr);
    return menu;
}
void highlight_option(MENU* menu, int index, int highlight){
    if (highlight){
	menu -> options[index].highlighted = 1;
    }
    else{
	menu -> options[index].highlighted = 0;
    }
}
void wdraw_menu(WINDOW* win, MENU *menu, int y, int x){
    int i;
    highlight_option(menu, 0, 1);
    for(i = 0; i < (menu -> n_choices); ++i){
	if (menu -> options[i].highlighted == 1){
	    wattron(win, A_REVERSE);
	    mvwprintw(win, y + i, x, "%s", menu -> options[i].description);
	    wattroff(win, A_REVERSE);
	}
	else{
	    mvwprintw(win, y + i, x, "%s", menu -> options[i].description);
	}
    }
}
/* Create a pointer to the current option and increment/decrement with ++options or --options */
void menu_driver(WINDOW *win, MENU *menu){
    int ch;
    ch = wgetch(win);
    switch(ch){
	case 'h':
	    break;
	case 'j':
	    break;
	case 'k':
	    break;
	case 'l':
	    break;
    }
}
