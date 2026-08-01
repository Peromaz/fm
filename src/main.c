#include <ncurses.h>
/* My Libraries */
#include "ui/ui.h"
#include "ui/Menu/menu.h"
int main(){
    /* RULES */
    initscr(); //Starting ncurses
    cbreak(); //Disable line buffering
    keypad(stdscr, TRUE); //Allows all keys
    noecho();
    curs_set(0); //sets the cursor to invisible, small chance it may crash the program if terminal doesn't support it
    int window_width = COLS / 3;
    WINDOW* prev_dir_win = create_new_window(LINES -1, window_width,
	    1, 0);
    MENU* prev_dir_menu = create_menu("..");

    WINDOW* curr_dir_win = create_new_window(LINES -1, window_width,
	    1, window_width);
    MENU* curr_dir_menu = create_menu(".");

    WINDOW* next_dir_win = create_new_window(LINES - 1, window_width,
	    1, window_width * 2); 
    MENU* next_dir_menu;

    
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
    //wrefresh(next_dir_win);

    int ch;
    while((ch = wgetch(curr_dir_win)) != 'q'){
	menu_driver(curr_dir_win, curr_dir_menu, ch);
	wdraw_menu(curr_dir_win, curr_dir_menu, 3,1,true); 
    }

    erase_ui();
    return 0;
}


