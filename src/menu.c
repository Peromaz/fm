#include "include/menu.h"
#include "include/statemachine.h"

int get_dir_entry_count(DIR* dir_ptr){
    int count;
    struct dirent *dummy;
    while((dummy = readdir(dir_ptr))){
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
    
    //Check if the directory even exists, if so then display END OF DIRECTORY
    if (dir_ptr == NULL){
	menu -> n_choices = 1;
	menu -> options = (OPTION*) malloc(sizeof(OPTION));
	menu -> highlight_pos = 0;
	if(menu -> options == NULL){
	    perror("options malloc failed");
	    exit(1);
	}
	strcpy(menu -> options[menu -> highlight_pos].description,"END OF DIRECTORY");
	closedir(dir_ptr);
	return menu;
    }

    /* Reads over directory */
    menu -> n_choices = get_dir_entry_count(dir_ptr);
    menu -> options = (OPTION*) malloc(sizeof(OPTION)
	    * (menu -> n_choices)); 
    menu -> highlight_pos = 0;
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
	/* Filter out .. and . */
	if (strcmp(dir_description, ".") == 0  || strcmp(dir_description, "..") == 0){
	    menu -> n_choices -= 1;
	    //free(dir_description);
	    free(entry);
	    continue;
	}
	strcpy(entry -> description, dir_description);
	menu -> options[index] = *entry;
	//free(dir_description);
	free(entry);
	++index;
    }
    closedir(dir_ptr);
    return menu;
}

int go_to_directory(const char* directory){
    if (chdir(directory) != 0){
	perror("chdir failed");
	return 0;
    }
    return 1;
}
char* get_next_directory(MENU *curr_menu){
    char* prefix = "./";  
    char* filename = curr_menu -> options[curr_menu -> highlight_pos].description;
    size_t p_length = strlen(prefix);
    size_t f_length = strlen(filename);

    char* result = malloc(p_length + f_length + 1);

    if (!result) return NULL;

    memcpy(result, prefix, p_length);
    memcpy(result + p_length, filename, f_length);
    result[p_length + f_length] = '\0';
    return result;
    //caller has to free
}
void wdraw_menu(WINDOW* win, MENU *menu, int y, int x, int highlight){
    int i;
    for(i = 0; i < (menu -> n_choices); ++i){
	if (menu -> highlight_pos == i && highlight){
	    wattron(win, A_REVERSE);
	    mvwprintw(win, y + i, x, "%s", menu -> options[i].description);
	    wattroff(win, A_REVERSE);
	}
	else{
	    mvwprintw(win, y + i, x, "%s", menu -> options[i].description);
	}
    }
    wrefresh(win);
}

void menu_driver(WINDOW *win, MENU *menu, int ch){
    switch(ch){
	case 'h': {
	    change_state(ST_ASCENDING);
	    break;
	}
	case 'j': {
	    menu -> highlight_pos++;
	    if ( menu -> highlight_pos > menu -> n_choices - 1 ){
		menu -> highlight_pos = 0;	
	    }
	    break;
	}
	case 'k': {
	    menu -> highlight_pos--;
	    if ( menu -> highlight_pos < 0){
		menu -> highlight_pos = menu -> n_choices - 1;	
	    }
	    break;
	}
	case 'l': {
	    change_state(ST_DESCENDING);
	    break;
	}
    }
}
