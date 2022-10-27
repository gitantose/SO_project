#include "window.h"

int position = 0;

void clean_term(int height, int width, int startx) {
	mvprintw(height, startx, " > ");
	for (int i=startx + 3; i<width-3; i++) 
		mvprintw(height, i, " ");	
}

void check_ret(int ret, int height, int startx) {
	if(ret==-1)
		mvprintw(height, startx + 4, "Errore nell' invio del segnale al processo");
  else 
   	mvprintw(height, startx + 4, "Segnale inviato correttamente");
  refresh();
  sleep(3);
}

void term_win(int height, int width, int starty, int startx, int* cur, int* active_proc) {
	WINDOW* input = create_newwin(1, width, height, startx, 0);
	mvprintw(height, startx, " > ");
  int ch;
  char buf[100];
	move(height, startx + 4 + *cur);
	while(1) {
		ch = getch();
		if (ch == 263) {		//backspace
			if (*cur > 0) {
				*cur -= 1;
				mvprintw(height, startx + 4 + *cur, " ");
				buf[*cur] = ' ';
			}
		}
		else if (ch == KEY_DOWN) {
			if (position < *active_proc - 32)
				position += 1;
		}
		else if (ch == KEY_UP) {
			if (position > 0)
				position -= 1;		
		}
		else if (ch == 10) { // enter
			clean_term(height, width, startx);
			buf[*cur] = '\0';
			*cur = 0;
			char* command = strtok(buf, " ");
			pid_t pid;
			if (strcmp(command, SHUTDOWN) == 0) {
				break;
			}
			else if (strcmp(command, KILL) == 0) {
				command = strtok(NULL, " ");
				pid = atoi(command);
				if (pid != 0) {
					int ret=kill(pid, SIGTERM);
  	    	check_ret(ret, height, startx);
				}
			}
			else if (strcmp(command, RESUME) == 0) {
				command = strtok(NULL, " ");
				pid = atoi(command);
				if (pid != 0) {
					int ret=kill(pid, SIGCONT);
  	    	check_ret(ret, height, startx);
				}
			}
			else if (strcmp(command, SUSPEND) == 0) {
				command = strtok(NULL, " ");
				pid = atoi(command);
				if (pid != 0) {
					int ret=kill(pid, SIGSTOP);
  	    	check_ret(ret, height, startx);
				}
			}
			else if (strcmp(command, QUIT) == 0) {
				command = strtok(NULL, " ");
				pid = atoi(command);
				if (pid != 0) {
					int ret=kill(pid, SIGQUIT);
  	    	check_ret(ret, height, startx);
				}
			}
			clean_term(height, width, startx);
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

void print_main_win(WINDOW* win, Process* proc, Global* glob, int height, int width, int starty, int startx, int* cur, char* t) {
	char menu[12][8] = {"PID", "USER", "PR", "NI", "VIRT", "RES", "SHR", "S", "CPU %", "MEM %", "TIME+", "Command"};
	char tmp[BUF_LEN];
	for (int i=startx; i<width-startx; i++)
		for (int j=starty; j<height-starty; j++)
			mvprintw(j, i, " ");
	refresh();
	mvprintw(starty, (width-startx)/2 - strlen(" My TOP ")/2, " My TOP ");
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
	
	mvprintw(starty + 4, 7*(width-startx)/12, "%s", t);
	mvprintw(starty + 4, 9*(width-startx)/12, "View from %d° to %d° proc", position+1, position+32);


	for (int p=0; p<height - 14; p++) {
		Process process = proc[p + position];
		mvprintw(starty + 11 + p, 1*(width-startx)/24 - strlen(process.pid)/2, "%s", process.pid);
		mvprintw(starty + 11 + p, 3*(width-startx)/24 - strlen(process.user)/2, "%s", process.user);
		sprintf(tmp, "%d", process.priority);
		mvprintw(starty + 11 + p, 5*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
		sprintf(tmp, "%d", process.ni);
		mvprintw(starty + 11 + p, 7*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
		sprintf(tmp, "%ld", process.virt);
		mvprintw(starty + 11 + p, 9*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
		sprintf(tmp, "%ld", process.res);
		mvprintw(starty + 11 + p, 11*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
		sprintf(tmp, "%ld", process.shr);
		mvprintw(starty + 11 + p, 13*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
		mvprintw(starty + 11 + p, 15*(width-startx)/24, "%c", process.status);
		sprintf(tmp, "%0.1f", process.cpu);
		mvprintw(starty + 11 + p, 17*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
		sprintf(tmp, "%0.1f", process.mem);
		mvprintw(starty + 11 + p, 19*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
		sprintf(tmp, "%0.3f", process.time);
		mvprintw(starty + 11 + p, 21*(width-startx)/24 - strlen(tmp)/2, "%s", tmp);
		process.command[(width-startx)/12 - 2] = '\0';
		mvprintw(starty + 11 + p, 23*(width-startx)/24 - strlen(process.command)/2, "%s", process.command);
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
 
  if (has_colors()){
    start_color();
    
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    bkgd(COLOR_PAIR(1));
  }

  return my_win;
}


WINDOW *create_newwin(int height, int width, int starty, int startx, int box) {	
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    //if (box)
	    box(local_win, 0, 0);		/* 0, 0 gives default characters 
					   for the vertical and horizontal
					    lines			*/
    wrefresh(local_win);		/* Show that box 		*/
    return local_win;
}

void destroy_win(WINDOW *local_win) {    
    wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');

    wrefresh(local_win);
    delwin(local_win);
}
