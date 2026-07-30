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
    MENU *menu;
    menu = (MENU*) malloc(sizeof(MENU)); 
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
    menu -> options = (OPTION*) malloc(sizeof(OPTION) * (menu -> n_choices)); /* allocate memory for options array */
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
	entry -> highlighted = false;	
	menu -> options[index] = *entry;
	free(entry);
	++index;
    }

    closedir(dir_ptr);
    return menu;
}
void wdraw_menu(WINDOW* win, MENU *menu, int height, int width, int y, int x){
    int i;
    for(i = 0; i < (menu -> n_choices); ++i){
	mvwprintw(win, 3 + i, 1, "%s", menu -> options[i].description);
    }
}
