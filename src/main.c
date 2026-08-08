#include <ncurses.h>
/* My Libraries */
#include "include/ui.h"
#include "include/menu.h"
#include "include/statemachine.h"
#define TOPLINE 1 
#define BOTTOMLINE 1

WINDOW* prev_dir_win;
MENU* prev_dir_menu;

WINDOW* curr_dir_win;
MENU* curr_dir_menu;

WINDOW* next_dir_win;
MENU* next_dir_menu;

int DEBUG;

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

    prev_dir_menu = create_menu("..");
    curr_dir_menu = create_menu(".");
    next_dir_menu = create_menu(get_next_directory(curr_dir_menu));

    /* Draw static and intial UI components */ 
    printcwd(); 
    refresh(); 

    draw_panel(prev_dir_win, window_width, "PREVIOUS");
    wdraw_menu(prev_dir_win, prev_dir_menu, 3,1,false); 
    //wrefresh(prev_dir_win);

    draw_panel(curr_dir_win, window_width, "CURRENT");
    wdraw_menu(curr_dir_win, curr_dir_menu, 3,1,true); 
    //wrefresh(curr_dir_win);

    draw_panel(next_dir_win, window_width, "NEXT");
    wdraw_menu(next_dir_win, next_dir_menu, 3,1,false); 
    //wrefresh(next_dir_win);

    int ch;
    while((ch = wgetch(curr_dir_win)) != 'q'){
	/* Gets menu input */
	menu_driver(curr_dir_win, curr_dir_menu, ch);
	/* prints current working directory at the top */
	
	STATE curr_state = get_current_state();
	switch (curr_state){
	    case ST_ASCENDING: {
		/* Free up space for the current directory menu and resassign */
		free(curr_dir_menu -> options);
		free(curr_dir_menu);
			
		go_to_directory("..");
		curr_dir_menu = create_menu(".");
		change_state(ST_BROWSING);
		printcwd();
		break;
	    }
	    case ST_DESCENDING: {
		char* next_dir = get_next_directory(curr_dir_menu);
		if(next_dir){
		/* Free up space for the current directory menu and reassign */
		    free(curr_dir_menu -> options);
		    free(curr_dir_menu);
		    go_to_directory(next_dir);
		    curr_dir_menu = create_menu(".");
		    free(next_dir);
		    next_dir = NULL;
		}
		change_state(ST_BROWSING);
		printcwd();
		break;		
	    }
	    case ST_BROWSING: {
		char* next_dir = get_next_directory(curr_dir_menu);
		if (DEBUG){
		    move(LINES - 1, 0);
		    clrtoeol();
		    printw("%s", next_dir);
		}
		free(next_dir);
		next_dir = NULL;
		printcwd();
	    }
	    default: break;
	}
	
	if (DEBUG){
	    wclear(prev_dir_win);
	    prev_dir_win = create_new_window(LINES - TOPLINE - BOTTOMLINE, window_width,
		1, 0); 
	    draw_panel(prev_dir_win, window_width, "PREVIOUS");
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
	    draw_panel(next_dir_win, window_width, "NEXT");
	    wdraw_menu(next_dir_win, next_dir_menu, 3,1,false); 
	}
	else{
	    wclear(prev_dir_win);
	    prev_dir_win = create_new_window(LINES - TOPLINE, window_width,
		1, 0); 
	    draw_panel(prev_dir_win, window_width, "PREVIOUS");
	    wdraw_menu(prev_dir_win, prev_dir_menu, 3,1,false); 

	    /* draw the current window */
	    wclear(curr_dir_win);
	    curr_dir_win = create_new_window(LINES - TOPLINE, window_width,
		1, window_width);
	    draw_panel(curr_dir_win, window_width, "CURRENT");
	    wdraw_menu(curr_dir_win, curr_dir_menu, 3,1,true); 
	    
	    /* draw the next window */
	    wclear(next_dir_win);
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


