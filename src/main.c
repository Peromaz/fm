#include <ncurses.h>
#include "include/ui.h"
#include "include/menu.h"
#include "include/statemachine.h"

#define TOPLINE 1 
#define BOTTOMLINE 1
#define LINE 1 

WINDOW* prev_dir_win;
MENU* prev_dir_menu;

WINDOW* curr_dir_win;
MENU* curr_dir_menu;

WINDOW* next_dir_win;
MENU* next_dir_menu;

int DEBUG;
/* Declarations */
void init_windows(int window_width);
void init_menus();

int main(int argc, char **argv){
    /* parse debug arg */
    if (argc == 2){
	char* debug_string = strstr(argv[1], "--debug");
	if (debug_string != NULL){
	    DEBUG = true;
	}
	else {
	    DEBUG = false;
	}
    }
    else if( argc > 2){
	printf("Program takes either no args or just the --debug flag for debug mode");
	return 1;
    }
    /* RULES */
    initscr(); //Starting ncurses
    cbreak(); //Disable line buffering
    keypad(stdscr, TRUE); //Allows all keys
    noecho();
    curs_set(0); //sets the cursor to invisible, small chance it may crash the program if terminal doesn't support it
    int window_width = COLS / 3;
    int total_menu_rows = LINES - (4 * LINE);
    int total_window_cols = window_width - 2;
    
    init_windows(window_width);
    init_menus();

    /* Draw static and intial UI components */ 
    printcwd(); 
    refresh(); 

    draw_panel(prev_dir_win, window_width, "PREVIOUS");
    wdraw_menu(prev_dir_win, prev_dir_menu, 3,1,false); 

    draw_panel(curr_dir_win, window_width, "CURRENT");
    wdraw_menu(curr_dir_win, curr_dir_menu, 3,1,true); 

    draw_panel(next_dir_win, window_width, "NEXT");
    wdraw_menu(next_dir_win, next_dir_menu, 3,1,false); 

    int ch;
    while((ch = wgetch(curr_dir_win)) != 'q'){
	/* Gets menu input */
	menu_driver(curr_dir_win, curr_dir_menu, ch);
	STATE curr_state = get_current_state();
	switch (curr_state){
	    case ST_ASCENDING: {
		free_menu(prev_dir_menu);
		free_menu(curr_dir_menu);
		free_menu(next_dir_menu);

		go_to_directory("..");
		// Create the new current menu	
		curr_dir_menu = create_menu(".");
		// create the new previous menu
		prev_dir_menu = create_menu("..");
		//create the new next menu
		char* next_dir = get_next_directory(curr_dir_menu);
		if (next_dir){
		    next_dir_menu = create_menu(next_dir);
		}
		free(next_dir);
		next_dir = NULL;

		change_state(ST_BROWSING);
		break;
	    }
	    case ST_DESCENDING: {
		free_menu(prev_dir_menu);
		free_menu(next_dir_menu);
		// Create the new current menu	
		char* next_dir = get_next_directory(curr_dir_menu);
		if(next_dir){
		/* Free up space for the current directory menu and reassign */
		    go_to_directory(next_dir);
		    free_menu(curr_dir_menu);
		    curr_dir_menu = create_menu(".");
		}
		free(next_dir);
		next_dir = NULL;
		//Create previous menu 
		prev_dir_menu = create_menu("..");

		next_dir = get_next_directory(curr_dir_menu);
		if (next_dir){
		    next_dir_menu = create_menu(next_dir);
		}
		free(next_dir);
		next_dir = NULL;

		change_state(ST_BROWSING);
		break;		
	    }
	    case ST_BROWSING: {
		free_menu(next_dir_menu);

		char* next_dir = get_next_directory(curr_dir_menu);
		if (next_dir){
		    next_dir_menu = create_menu(next_dir);
		}
		free(next_dir);
		next_dir = NULL;

		if (DEBUG){
		    char* next_dir = get_next_directory(curr_dir_menu);
		    move(LINES - 1, 0);
		    clrtoeol();
		    printw("%s", next_dir);
		    free(next_dir);
		    next_dir = NULL;
		}
	    }
	    default: break;
	}
	printcwd();

	if (DEBUG){

	    wclear(prev_dir_win);
	    prev_dir_win = create_new_window(LINES - TOPLINE - BOTTOMLINE, window_width,
		1, 0); 
	    draw_panel(prev_dir_win, window_width, "PREVIOUS PREVIEW");
	    wdraw_menu(prev_dir_win, prev_dir_menu, 3,1,false); 

	    /* draw the current window */
	    wclear(curr_dir_win);
	    curr_dir_win = create_new_window(LINES - TOPLINE - BOTTOMLINE, window_width,
		1, window_width);
	    draw_panel(curr_dir_win, window_width, "CURRENT");
	    wdraw_menu(curr_dir_win, curr_dir_menu, 3,1,true); 
	    
	    /* draw the next window */
	    wclear(next_dir_win);
	    next_dir_win = create_new_window(LINES - TOPLINE - BOTTOMLINE, window_width,
		1, window_width * 2); 
	    draw_panel(next_dir_win, window_width, "NEXT PREVIEW");
	    wdraw_menu(next_dir_win, next_dir_menu, 3,1,false); 
	}
	else{
	    wclear(prev_dir_win);
	    destroy_window(prev_dir_win);
	    prev_dir_win = create_new_window(LINES - TOPLINE, window_width,
		1, 0); 
	    draw_panel(prev_dir_win, window_width, "PREVIOUS");
	    wdraw_menu(prev_dir_win, prev_dir_menu, 3,1,false); 

	    /* draw the current window */
	    wclear(curr_dir_win);
	    destroy_window(curr_dir_win);
	    curr_dir_win = create_new_window(LINES - TOPLINE, window_width,
		1, window_width);
	    draw_panel(curr_dir_win, window_width, "CURRENT");
	    wdraw_menu(curr_dir_win, curr_dir_menu, 3,1,true); 
	    
	    /* draw the next window */
	    wclear(next_dir_win);
	    destroy_window(next_dir_win);
	    next_dir_win = create_new_window(LINES - TOPLINE, window_width,
		1, window_width * 2); 
	    draw_panel(next_dir_win, window_width, "NEXT");
	    wdraw_menu(next_dir_win, next_dir_menu, 3,1,false); 
	}
	printcwd();
    }

    erase_ui();
    return 0;
}

void init_windows(int window_width){
    if (DEBUG){
	prev_dir_win = create_new_window(LINES - TOPLINE - BOTTOMLINE , window_width, 1, 0);
	curr_dir_win = create_new_window(LINES - TOPLINE - BOTTOMLINE, window_width, 1, window_width);
	next_dir_win = create_new_window(LINES - TOPLINE - BOTTOMLINE, window_width, 1, window_width * 2); 
    }
    else{
	prev_dir_win = create_new_window(LINES - TOPLINE, window_width, 1, 0);
	curr_dir_win = create_new_window(LINES - TOPLINE, window_width, 1, window_width);
	next_dir_win = create_new_window(LINES - TOPLINE, window_width, 1, window_width * 2); 
    }
}
void init_menus(){
    prev_dir_menu = create_menu("..");
    curr_dir_menu = create_menu(".");
    next_dir_menu = create_menu(get_next_directory(curr_dir_menu));
}
