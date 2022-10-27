#include "main.h"
#include "data.h"
#include "window.h"

Process proc[MAX_PROC] = {0x0};
Global glob;
int running, cur = 0;
int height, width, starty, startx;

void sig_handler(int sig){
    endwin();
    if(sig == SIGBUS){
      printf("programm terminated with SIGBUS: %d\n", errno);
    }
    if(sig == SIGSEGV){
      printf("programm terminated with SIGSEGV: %d\n", errno);
    }
    
    exit(EXIT_FAILURE);
}

static int processCmp(const void* p1, const void* p2) {
	Process* a = (Process*) p1;
	Process* b = (Process*) p2;
	if ((*b).cpu == (*a).cpu)
		if ((*b).mem == (*a).mem)
			return ((*b).res + (*b).shr + (*b).virt) - ((*a).res + (*a).shr + (*a).virt);
		return (int) (*b).mem - (*a).mem;
	return (int) (*b).cpu - (*a).cpu;
}

void* terminale() {
	term_win(height, width, starty, startx, &cur, &glob.activeProc);
	running = 0;
}

int main() {
	WINDOW* main_win = create_main_win(&height, &width, &startx, &starty);
	running = 1;
	pthread_t term_thread;
	
	struct sigaction psig;
  psig.sa_handler = sig_handler;
  sigaction(SIGINT, &psig, NULL);
  sigaction(SIGBUS, &psig, NULL);
  sigaction(SIGSEGV, &psig, NULL);

	int ret = pthread_create(&term_thread, NULL, terminale, NULL);
	if (ret == -1)
		exit(EXIT_FAILURE);
	while (running) {
		memset(proc, 0, sizeof(proc));
		readDir(proc);
		glob.activeProc = 0;
		glob.runningProc = 0;
		glob.sleepProc = 0;
		glob.zombieProc = 0;
		setMemoryVar(&glob);
		setProcessVar(proc, &glob);
		countActiveProcess(proc, &glob);
  	qsort(&proc, glob.activeProc, sizeof(Process), (void*) processCmp);
  	
  	time_t t = time(NULL);
		struct tm *tm = localtime(&t);
		char * mytime = asctime(tm);
		print_main_win(main_win, proc, &glob, height, width, starty, startx, &cur, mytime);

		sleep(1);
	}
  return 0;
}

