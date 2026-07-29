#include <ncurses.h>
/* My Libraries */
#include "ui/ui.h"
int main(){
    int ch;
    /* RULES */
    draw_ui();
    while ((ch = getch()) != 'q'){

    }
    erase_ui();
    return 0;
}


