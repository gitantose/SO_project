#include "window.h"

int startx, starty, width, height;
WINDOW *my_win;
int aux = 1;

void* win1() {
		initscr();
		raw();
		keypad(stdscr, TRUE);		/* I need that nifty F2 	*/
		WINDOW* input = create_newwin(1, width, height, startx, 0);
		mvprintw(height, startx, " > ");
		move(height, startx+3);

    int ch;
		while((ch = getch()) != KEY_F(2)) {
    }
		aux = 0;
		destroy_win(input);
    endwin();
}

void* win2() {
		
		while(aux) {

			my_win = create_newwin(height, width, starty, startx, 1);
			mvprintw(starty, (width-5)/2 , " My TOP ");
			mvprintw(height - 1,(width-16)/2 , " Press F2 for exit ");
			for (int i=startx+1; i<width-1; i++) {
				for (int j=starty+1; j<height-3; j++) 
					mvprintw(j, i, " ");
			}
			for (int i=startx + 1; i<(width-startx)/12; i++) {
				for (int j=starty+5; j<height-3; j++)
					mvprintw(starty + j, i*(width-startx)/12, "|");
			}
			for (int i=startx + 2; i<width-2; i++) {
				mvprintw(starty + 5, i, "_");
				mvprintw(starty + 9, i, "-");
			}
			move(height, startx+3);
			refresh();
			sleep(1);
			
    }
    destroy_win(my_win);
    endwin();
}

void window() {
    initscr();			/* Start curses mode 		*/
    raw();			/* Line buffering disabled, Pass on
				 * everty thing to me 		*/
    //noecho();

    keypad(stdscr, TRUE);		/* I need that nifty F2 	*/
    getmaxyx(stdscr,height,width);		/* get the number of rows and columns */
    height -= 1;
    starty = 0;	/* Calculating for a center placement */
    startx = 0;	/* of the window		*/
    //printw("Press F2 to exit");
    //refresh();
     
    //my_win = create_newwin(height, width, starty, startx);
    pthread_t thread[2];
    int ret = pthread_create(&thread[0], NULL, win1, NULL);
   	ret = pthread_create(&thread[1], NULL, win2, NULL);
   	//pthread_join(&thread[0], NULL);
		//pthread_join(&thread[1], NULL);
    
		pthread_exit(NULL);    
}

WINDOW *create_newwin(int height, int width, int starty, int startx, int box) {	
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    if (box) // solo se box è 1
	    box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					   for the vertical and horizontal
					    lines			*/

    wrefresh(local_win);		/* Show that box 		*/
    return local_win;
}

void destroy_win(WINDOW *local_win) {
    	/* box(local_win, ' ', ' '); : This won't produce the desired
  	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window.  */
    
    wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');

	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
 	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window */
    
    wrefresh(local_win);
    delwin(local_win);
}
