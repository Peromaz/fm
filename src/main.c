#include <ncurses.h>
/* My Libraries */
#include "ui/ui.h"
#include "ui/Menu/menu.h"

#define TOPLINE 1
int main(){
    /* RULES */
    initscr(); //Starting ncurses
    cbreak(); //Disable line buffering
    keypad(stdscr, TRUE); //Allows all keys
    noecho();
    curs_set(0); //sets the cursor to invisible, small chance it may crash the program if terminal doesn't support it
    int window_width = COLS / 3;
    WINDOW* prev_dir_win = create_new_window(LINES - TOPLINE, window_width,
	    1, 0);
    MENU* prev_dir_menu = create_menu("..");

    WINDOW* curr_dir_win = create_new_window(LINES - TOPLINE, window_width,
	    1, window_width);
    MENU* curr_dir_menu = create_menu(".");

    WINDOW* next_dir_win = create_new_window(LINES - TOPLINE, window_width,
	    1, window_width * 2); 
    MENU* next_dir_menu = create_menu(get_next_directory(curr_dir_menu));

    
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
	menu_driver(curr_dir_win, curr_dir_menu, ch);
	printcwd();

	char* next_dir = get_next_directory(curr_dir_menu);
	if(next_dir){
	    next_dir_menu = create_menu(next_dir);
	    free(next_dir);
	    next_dir = NULL;
	}

	wclear(prev_dir_win);
	prev_dir_win = create_new_window(LINES - TOPLINE, window_width,
	    1, 0); 
	draw_panel(prev_dir_win, window_width, "PREVIOUS");
	wdraw_menu(prev_dir_win, prev_dir_menu, 3,1,false); 
	
	wclear(curr_dir_win);
	curr_dir_win = create_new_window(LINES - TOPLINE, window_width,
	    1, window_width);
	draw_panel(curr_dir_win, window_width, "CURRENT");
	wdraw_menu(curr_dir_win, curr_dir_menu, 3,1,true); 
	
	wclear(next_dir_win);
	next_dir_win = create_new_window(LINES - TOPLINE, window_width,
	    1, window_width * 2); 
	draw_panel(next_dir_win, window_width, "NEXT");
	wdraw_menu(next_dir_win, next_dir_menu, 3,1,false); 
    }

    erase_ui();
    return 0;
}


