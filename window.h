#ifndef _WINDOW_GUARD
#define _WINDOW_GUARD

#define QUIT "quit"
#define RESUME "resume"
#define KILL "kill"
#define SUSPEND "suspend"
#define EXIT "exit"
#define SHUTDOWN "shutdown"

#include "main.h"


WINDOW *create_newwin(int height, int width, int starty, int startx, int box);
void destroy_win(WINDOW *local_win);
void term_win(int height, int width, int starty, int startx, int* cur, int* active_proc);
void print_main_win(WINDOW* win, Process* proc, Global* glob, int height, int width, int starty, int startx, int* cur, char* t);
WINDOW* create_main_win(int* height, int* width, int* starty, int* startx);

typedef struct {
	Process* proc;
	Global* glob;
} Parameters;
	
#endif
