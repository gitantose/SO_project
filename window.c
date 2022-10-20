#include "window.h"

int startx, starty, width, height;
WINDOW *my_win;
int aux = 1, cur = 0;
char buf[100], str[100];

void* win1() {
		//initscr();
		//raw();
		//keypad(stdscr, TRUE);		/* I need that nifty F2 	*/
		WINDOW* input = create_newwin(1, width, height, startx, 0);
		mvprintw(height, startx, " > ");

    int ch;
		move(height, startx + 4 + cur);
		while(1) {
			ch = getch();
			//getstr(str);
			//printw("Hai inserito: %d", (int) ch);
			if (ch == 263) {		//backspace
				if (cur != 0) {
					cur -= 1;
					mvprintw(height, startx + 4 + cur, " ");
					buf[cur] = ' ';
				}
			}
			else if (ch == 10) { // enter
				mvprintw(height, startx, " > ");
				for (int i=startx + 3; i<width-3; i++) 
					mvprintw(height, i, " ");
				buf[cur] = '\0';
				cur = 0;
				if (strcmp(buf, "quit") == 0)
					break;
				// TODO implementare kill e relive process
			}
			else {
				buf[cur] = ch;
				cur += 1;
			}
			move(height, startx + 4 + cur);
    }
		aux = 0;
		destroy_win(input);
    endwin();
    pthread_exit(NULL);
}

void* win2(Parameters* param) {
		Process* proc = param->proc;
		Global* glob = param->glob;
		char menu[12][8] = {"PID", "USER", "PR", "NI", "VIRT", "RES", "SHR", "S", "CPU %", "MEM %", "TIME+", "Command"};
		char tmp[BUF_LEN];
		destroy_win(my_win);
		my_win = create_newwin(height, width, starty, startx, 1);		
		int x = 0;
		mvprintw(starty, (width-startx)/2 - strlen(" My TOP ")/2, " My TOP %d ", x++);
		mvprintw(height - 1,(width-startx)/2 - strlen(" Run 'quit' for exit ")/2, " Run 'quit' for exit ");
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
		
		for (int i=1, e=0; i<24; i+=2, e++)
				mvprintw(starty + 7, i*(width-startx)/24 - strlen(menu[e])/2, "%s", menu[e]);
		
		mvprintw(starty + 2, (width-startx)/12 + 3, "MiB Mem:");
		sprintf(tmp, "%0.1f", glob->memTot);
		mvprintw(starty + 2, (width-startx)/4 - (strlen(tmp)+7), "%s total,", tmp); //TODO inserisci numero
		sprintf(tmp, "%0.1f", glob->memFree);
		mvprintw(starty + 2, (width-startx)/3 - (strlen(tmp)+6), "%s free,", tmp); //TODO inserisci numero 
		sprintf(tmp, "%0.1f", glob->memUsed);
		mvprintw(starty + 2, 5*(width-startx)/12 - (strlen(tmp)+5), "%s used", tmp); //TODO inserisci numero 

		mvprintw(starty + 4, (width-startx)/12 + 3, "MiB Swap:");
		sprintf(tmp, "%0.1f", glob->memSwapTot);
		mvprintw(starty + 4, (width-startx)/4 - (strlen(tmp)+7), "%s total,", tmp); //TODO inserisci numero
		sprintf(tmp, "%0.1f", glob->memSwapFree);
		mvprintw(starty + 4, (width-startx)/3 - (strlen(tmp)+6), "%s free,", tmp); //TODO inserisci numero
		sprintf(tmp, "%0.1f", glob->memSwapUsed);
		mvprintw(starty + 4, 5*(width-startx)/12 - (strlen(tmp)+5), "%s used", tmp); //TODO inserisci numero

		mvprintw(starty + 2, 7*(width-startx)/12, "Task:");
		sprintf(tmp, "%d", glob->activeProc);
		mvprintw(starty + 2, 8*(width-startx)/12 - (strlen(tmp)+7), "%s total,", tmp); //TODO inserisci numero
		sprintf(tmp, "%d", glob->sleepProc);
		mvprintw(starty + 2, 9*(width-startx)/12 - (strlen(tmp)+10), "%s sleeping,", tmp); //TODO inserisci numero
		sprintf(tmp, "%d", glob->runningProc);
		mvprintw(starty + 2, 10*(width-startx)/12 - (strlen(tmp)+8), "%s running", tmp); //TODO inserisci numero
		sprintf(tmp, "%d", glob->zombieProc);
		mvprintw(starty + 2, 11*(width-startx)/12 - (strlen(tmp)+7), "%s zombie", tmp); //TODO inserisci numero
			
		while(aux) {
			mvprintw(starty, (width-startx)/2 - strlen(" My TOP ")/2, " My TOP %d ", x++);
			for (int i=startx + 1; i<(width-startx); i++) {
				for (int j=starty+10; j<height-3; j++)
					mvprintw(starty + j, i, " ");
			}
			move(height, startx + 4 + cur);
			refresh();
			for (int p=0; p<height - 14; p++) {
				mvprintw(starty + 11 + p, 1*(width-startx)/24 - strlen(proc[p].pid)/2, "%s", proc[p].pid);
				mvprintw(starty + 11 + p, 3*(width-startx)/24 - strlen(proc[p].user)/2, "%s", proc[p].user);
				sprintf(tmp, "%d", proc[p].priority);
				mvprintw(starty + 11 + p, 5*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
				sprintf(tmp, "%d", proc[p].ni);
				mvprintw(starty + 11 + p, 7*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
				sprintf(tmp, "%ld", proc[p].virt);
				mvprintw(starty + 11 + p, 9*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
				sprintf(tmp, "%ld", proc[p].res);
				mvprintw(starty + 11 + p, 11*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
				sprintf(tmp, "%ld", proc[p].shr);
				mvprintw(starty + 11 + p, 13*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
				mvprintw(starty + 11 + p, 15*(width-startx)/24, "%c", proc[p].status);
				sprintf(tmp, "%0.1f", proc[p].cpu);
				mvprintw(starty + 11 + p, 17*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
				sprintf(tmp, "%0.1f", proc[p].mem);
				mvprintw(starty + 11 + p, 19*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
				sprintf(tmp, "%d", proc[p].time);
				mvprintw(starty + 11 + p, 21*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
				proc[p].command[(width-startx)/12 - 2] = '\0';
				mvprintw(starty + 11 + p, 23*(width-startx)/24 - strlen(proc[p].command)/2, "%s", proc[p].command);
			}
			move(height, startx + 4 + cur);
			refresh();
			sleep(1);
			//destroy_win(my_win);
    }
    destroy_win(my_win);
    endwin();
    pthread_exit(NULL);
    return NULL;
}

void window(Process* proc, Global* glob) {
    initscr();			/* Start curses mode 		*/
    raw();			/* Line buffering disabled, Pass on
				 * everty thing to me 		*/
    //noecho();

    keypad(stdscr, TRUE);		/* I need that nifty F2 	*/
    getmaxyx(stdscr,height,width);		/* get the number of rows and columns */
    height -= 2;
    starty = 0;	/* Calculating for a center placement */
    startx = 0;	/* of the window		*/
    printw("Press F2 to exit");
    //refresh();
    
    Parameters param;
    param.proc = proc;
    param.glob = glob;
    //my_win = create_newwin(height, width, starty, startx);
    pthread_t thread[2];
    int ret = pthread_create(&thread[1], NULL, (void *)win2, (void*) &param); // stampa di /proc
   	ret = pthread_create(&thread[0], NULL, win1, NULL); //terminale su cui scrivere
   	//win1();
   	pthread_join(thread[0], NULL);
		pthread_join(thread[1], NULL);
    
		pthread_exit(NULL); 
		return;
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
