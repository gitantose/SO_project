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
void term_win(int height, int width, int starty, int startx, int* cur);
void print_main_win(WINDOW* win, Process* proc, Global* glob, int height, int width, int starty, int startx, int* cur, int* x);
WINDOW* create_main_win(int* height, int* width, int* starty, int* startx);
void clean_term(int height, int width, int startx);

typedef struct {
	Process* proc;
	Global* glob;
} Parameters;
	
#endif
