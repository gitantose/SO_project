#include "window.h"

void clean_term(int height, int width, int startx) {
	mvprintw(height, startx, " > ");
	for (int i=startx + 3; i<width-3; i++) 
		mvprintw(height, i, " ");	
}

void term_win(int height, int width, int starty, int startx, int* cur) {
	WINDOW* input = create_newwin(1, width, height, startx, 0);
	mvprintw(height, startx, " > ");
  int ch;
  char buf[100];
	move(height, startx + 4 + *cur);
	while(1) {
		ch = getch();
		//getstr(str);
		//printw("Hai inserito: %d", (int) ch);
		if (ch == 263) {		//backspace
			if (*cur != 0) {
				*cur -= 1;
				mvprintw(height, startx + 4 + *cur, " ");
				buf[*cur] = ' ';
			}
		}
		else if (ch == 10) { // enter
			clean_term(height, width, startx);
			buf[*cur] = '\0';
			*cur = 0;
			char* command = strtok(buf, " ");
			if (strcmp(command, SHUTDOWN) == 0) {
				mvprintw(height, startx + 4, "Adioss");
				refresh();
				break;
			}
			else if (strcmp(command, KILL) == 0) {
				pid_t pid = atoi(strtok(NULL, " "));
				int ret=kill(pid, SIGTERM);
      	if(ret==-1)
					mvprintw(height, startx + 4, "Errore nell' invio del segnale al processo");
        else 
        	mvprintw(height, startx + 4, "Segnale inviato correttamente");
        refresh();
        sleep(3);
        clean_term(height, width, startx);
			}
			else if (strcmp(command, RESUME) == 0) {
				pid_t pid = atoi(strtok(NULL, " "));
				int ret=kill(pid, SIGCONT);
      	if(ret==-1)
					mvprintw(height, startx + 4, "Errore nell' invio del segnale al processo");
        else 
        	mvprintw(height, startx + 4, "Segnale inviato correttamente");
        refresh();
        sleep(3);
			}
			else if (strcmp(command, SUSPEND) == 0) {
				pid_t pid = atoi(strtok(NULL, " "));
				int ret=kill(pid, SIGSTOP);
      	if(ret==-1)
					mvprintw(height, startx + 4, "Errore nell' invio del segnale al processo");
        else 
        	mvprintw(height, startx + 4, "Segnale inviato correttamente");
        refresh();
        sleep(3);
			}
			else if (strcmp(command, QUIT) == 0) {
				pid_t pid = atoi(strtok(NULL, " "));
				int ret=kill(pid, SIGQUIT);
      	if(ret==-1)
					mvprintw(height, startx + 4, "Errore nell' invio del segnale al processo");
        else 
        	mvprintw(height, startx + 4, "Segnale inviato correttamente");
        refresh();
        sleep(3);
			}
		}
		else {
			buf[*cur] = ch;
			*cur += 1;
		}
		move(height, startx + 4 + *cur);
  }
	destroy_win(input);
	endwin();
}

void print_main_win(WINDOW* win, Process* proc, Global* glob, int height, int width, int starty, int startx, int* cur, int* x) {
	char menu[12][8] = {"PID", "USER", "PR", "NI", "VIRT", "RES", "SHR", "S", "CPU %", "MEM %", "TIME+", "Command"};
	char tmp[BUF_LEN];
	mvprintw(starty, (width-startx)/2 - strlen(" My TOP ")/2, " My TOP %d ", *x);
	mvprintw(height - 1,(width-startx)/2 - strlen(" Run 'shutdown' for exit ")/2, " Run 'shutdown' for exit ");
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
	mvprintw(starty + 2, (width-startx)/4 - (strlen(tmp)+7), "%s total,", tmp); 
	sprintf(tmp, "%0.1f", glob->memFree);
	mvprintw(starty + 2, (width-startx)/3 - (strlen(tmp)+6), "%s free,", tmp);
	sprintf(tmp, "%0.1f", glob->memUsed);
	mvprintw(starty + 2, 5*(width-startx)/12 - (strlen(tmp)+5), "%s used", tmp);

	mvprintw(starty + 4, (width-startx)/12 + 3, "MiB Swap:");
	sprintf(tmp, "%0.1f", glob->memSwapTot);
	mvprintw(starty + 4, (width-startx)/4 - (strlen(tmp)+7), "%s total,", tmp);
	sprintf(tmp, "%0.1f", glob->memSwapFree);
	mvprintw(starty + 4, (width-startx)/3 - (strlen(tmp)+6), "%s free,", tmp);
	sprintf(tmp, "%0.1f", glob->memSwapUsed);
	mvprintw(starty + 4, 5*(width-startx)/12 - (strlen(tmp)+5), "%s used", tmp);

	mvprintw(starty + 2, 7*(width-startx)/12, "Task:");
	sprintf(tmp, "%d", glob->activeProc);
	mvprintw(starty + 2, 8*(width-startx)/12 - (strlen(tmp)+7), "%s total,", tmp);
	sprintf(tmp, "%d", glob->sleepProc);
	mvprintw(starty + 2, 9*(width-startx)/12 - (strlen(tmp)+10), "%s sleeping,", tmp);
	sprintf(tmp, "%d", glob->runningProc);
	mvprintw(starty + 2, 10*(width-startx)/12 - (strlen(tmp)+8), "%s running", tmp);
	sprintf(tmp, "%d", glob->zombieProc);
	mvprintw(starty + 2, 11*(width-startx)/12 - (strlen(tmp)+7), "%s zombie", tmp);

	mvprintw(starty, (width-startx)/2 - strlen(" My TOP ")/2, " My TOP ");
	for (int i=startx + 1; i<(width-startx); i++) {
		for (int j=starty+10; j<height-3; j++)
			mvprintw(starty + j, i, " ");
	}
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
		sprintf(tmp, "%0.3f", proc[p].time);
		mvprintw(starty + 11 + p, 21*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
		proc[p].command[(width-startx)/12 - 2] = '\0';
		mvprintw(starty + 11 + p, 23*(width-startx)/24 - strlen(proc[p].command)/2, "%s", proc[p].command);
	}
	move(height, startx + 4 + (*cur));
	refresh();
}

WINDOW* create_main_win(int* height, int* width, int* starty, int* startx) {
	WINDOW* my_win;
	initscr();
	raw();
	keypad(stdscr, TRUE);
	getmaxyx(stdscr, *height, *width);
	*height -= 2;
  *starty = 0;	/* Calculating for a center placement */
  *startx = 0;	/* of the window		*/
  my_win = create_newwin(*height, *width, *starty, *startx, 1);
  return my_win;
}


WINDOW *create_newwin(int height, int width, int starty, int startx, int box) {	
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    if (box) // solo se box è 1
	    box(local_win, 0, 0);		/* 0, 0 gives default characters 
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
