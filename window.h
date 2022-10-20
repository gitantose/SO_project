#ifndef _WINDOW_GUARD
#define _WINDOW_GUARD

#include "main.h"

WINDOW *create_newwin(int height, int width, int starty, int startx, int box);
void destroy_win(WINDOW *local_win);
void window(Process* proc);
	
#endif
