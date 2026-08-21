/* 
 * Written by Peromaz 
 * This is meant to be a very simple file manager for unix based operating systems, although only tested on linux so far. 
 * If you see any bugs throughout the project, improvements to make, or just general inqueries related to the project, please email me at
 * peromaz@proton.me
 */

/* Libraries */ 
#include <ncurses.h>
#include "include/ui.h"
#include "include/menu.h"
#include "include/statemachine.h"

/* Macros */
#define TOPLINE 1 
#define BOTTOMLINE 1
#define LINE 1 
/* Intializing the Previous, Current and Next Windows and Menus */
WINDOW* prev_dir_win;
MENU* prev_dir_menu;

WINDOW* curr_dir_win;
MENU* curr_dir_menu;

WINDOW* next_dir_win;
MENU* next_dir_menu;
/* Boolean Debug mode controller */
int DEBUG;
/* Declarations to make main() less verbose */
void init_windows(int window_width);
void init_menus();

int main(int argc, char **argv){
    /* Parse for command line arguments */
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
    /* Intialize Ncurses */
    initscr(); //Starting ncurses
    cbreak(); //Disable line buffering
    keypad(stdscr, TRUE); //Allows all keys
    noecho();
    curs_set(0); //sets the cursor to invisible, small chance it may crash the program if terminal doesn't support it
    
    /* Calculate the width of each section, the total rows available for the menu, and total columns availble for the menu. */
    int window_width = COLS / 3;
    int total_menu_rows = LINES - (5 * LINE);
    int total_window_cols = window_width - 2;
    
    /* Intialize UI components (windows and menus) */
    init_windows(window_width);
    init_menus();
    printcwd(); 
    refresh(); 

    draw_panel(prev_dir_win, window_width, "PREVIOUS");
    wdraw_menu(prev_dir_win,
	    prev_dir_menu,
	    3,
	    1,
	    false,
	    total_menu_rows); 
    draw_panel(curr_dir_win, window_width, "CURRENT");
    wdraw_menu(curr_dir_win,
	    curr_dir_menu,
	    3,
	    1,
	    true,
	    total_menu_rows); 
    draw_panel(next_dir_win, window_width, "NEXT");
    wdraw_menu(next_dir_win,
	    next_dir_menu,
	    3,
	    1,
	    false,
	    total_menu_rows); 

    int ch; // This is the character input from the user
    /* Main input loop */
    while((ch = wgetch(curr_dir_win)) != 'q'){ //'q' to quit. 
	/* Gets menu input */
	menu_driver(curr_dir_win, curr_dir_menu, ch);
	STATE curr_state = get_current_state();
	switch (curr_state){
	    case ST_ASCENDING: {
		free_menu(prev_dir_menu);
		free_menu(curr_dir_menu);
		free_menu(next_dir_menu);

		go_to_directory("..");

		/* Create the new Current menu */
		curr_dir_menu = create_menu(".");
		/* Create the new Previous menu */
		prev_dir_menu = create_menu("..");
		/* Create the new Next menu */
		
		/* Find next directory */
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

		/* Find next directory */
		char* next_dir = get_next_directory(curr_dir_menu);
		if(next_dir){
		    /* Replace current directory with new */
		    go_to_directory(next_dir);
		    free_menu(curr_dir_menu);
		    curr_dir_menu = create_menu(".");
		}

		free(next_dir);
		next_dir = NULL;

		/* Create the new Previous menu */
		prev_dir_menu = create_menu("..");

		/* Create the new Next menu */
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
	    prev_dir_win = create_new_window(LINES - TOPLINE - BOTTOMLINE,
		    window_width,
		    1,
		    0); 
	    draw_panel(prev_dir_win,
		    window_width,
		    "PREVIOUS PREVIEW");
	    wdraw_menu(prev_dir_win,
		    prev_dir_menu,
		    3,
		    1,
		    false,
		    total_menu_rows); 

	    /* draw the current window */
	    wclear(curr_dir_win);
	    curr_dir_win = create_new_window(LINES - TOPLINE - BOTTOMLINE,
		    window_width,
		    1,
		    window_width);
	    draw_panel(curr_dir_win,
		    window_width,
		    "CURRENT");
	    wdraw_menu(curr_dir_win,
		    curr_dir_menu,
		    3,
		    1,
		    true,
		    total_menu_rows); 
	    
	    /* draw the next window */
	    wclear(next_dir_win);

	    next_dir_win = create_new_window(LINES - TOPLINE - BOTTOMLINE,
		    window_width,
		    1,
		    window_width * 2); 

	    draw_panel(next_dir_win,
		    window_width,
		    "NEXT PREVIEW");

	    wdraw_menu(next_dir_win,
		    next_dir_menu,
		    3,
		    1,
		    false,
		    total_menu_rows); 
	}
	else{
	    /* Clear and draw new Previous window */
	    wclear(prev_dir_win);
	    destroy_window(prev_dir_win);
	    prev_dir_win = create_new_window(LINES - TOPLINE, window_width,
		1, 0); 
	    draw_panel(prev_dir_win, window_width, "PREVIOUS");
	    wdraw_menu(prev_dir_win,
		    prev_dir_menu,
		    3,
		    1,
		    false,
		    total_menu_rows
	    ); 

	    /* Clear and draw new Current window */
	    wclear(curr_dir_win);
	    destroy_window(curr_dir_win);
	    curr_dir_win = create_new_window(LINES - TOPLINE, window_width,
		1, window_width);
	    draw_panel(curr_dir_win, window_width, "CURRENT");
	    wdraw_menu(curr_dir_win,
		    curr_dir_menu,
		    3,
		    1,
		    true,
		    total_menu_rows
	    ); 
	    
	    /* Clear and draw new Next window */
	    wclear(next_dir_win);
	    destroy_window(next_dir_win);
	    next_dir_win = create_new_window(LINES - TOPLINE,
		    window_width,
		    1,
		    window_width * 2); 
	    draw_panel(next_dir_win, window_width, "NEXT");
	    wdraw_menu(next_dir_win,
		    next_dir_menu,
		    3,
		    1,
		    false,
		    total_menu_rows
	    ); 
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
