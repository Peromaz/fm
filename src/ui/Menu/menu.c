#include <ncurses.h>
#include <limits.h>
#include <errno.h>
#include <dirent.h>
#include "menu.h"
#include <stdlib.h>
#define ARRAY_SIZE(a) (sizeof(a)) / (sizeof(a[0])

typedef struct _OPTION{
    int highlighted;
    char description[NAME_MAX];
    char filepath[PATH_MAX];
}OPTION;

typedef struct _MENU{
    OPTION *options;
    int n_choices;
}MENU;

void draw_option(OPTION *option, int y, int x ){
    mvprintw(y, x, "%s", option -> description);  
}

int get_dir_entry_count(DIR* dir_ptr){
    int count;
    while(readdir(dir_ptr)){
	++count;	
    }
    return count;
}

MENU *create_menu(const char* filepath){
    MENU *menu;

    struct dirent *dir_entry;
    DIR *dir_ptr;
    
    dir_ptr = opendir(filepath);
    //Check if the directory even exists
    if (dir_ptr != NULL){
	perror("Could not open the directory");
	exit(1);
    }
    /* Reads over directory */
    menu -> n_choices = get_dir_entry_count(dir_ptr);
    menu -> options = (OPTION*) malloc(sizeof(OPTION) * (menu -> n_choices)); /* allocate memory for options array */
    int i = 0;
    while((dir_entry = readdir(dir_ptr))){
	char *dir_description = dir_entry -> d_name;	
    }

    closedir(dir_ptr);
    return menu;
}
void draw_menu(MENU *menu, int height, int width, int y, int x){
    int i;
    for(i = 0; i < menu -> n_choices; ++i){
    }
}
