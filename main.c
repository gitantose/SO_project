#include "main.h"
#include "data.h"
#include "window.h"

Process proc[MAX_PROC] = {0x0};
Global glob;
int running, cur = 0;
int height, width, starty, startx;

static int processCmp(const void* p1, const void* p2) {
	Process* a = (Process*) p1;
	Process* b = (Process*) p2;
	if ((*b).cpu == (*a).cpu)
		return (int) (*b).mem - (*a).mem;
	return (int) (*b).cpu - (*a).cpu;
}

void setMemoryVar() {
	glob.memTot = find_first_occ("/proc/meminfo", "MemTotal") / 1024;
	glob.memFree = find_first_occ("/proc/meminfo", "MemFree") / 1024;
	glob.memCache = find_first_occ("/proc/meminfo", "Cached") / 1024;
	glob.memUsed = glob.memTot - glob.memFree - glob.memCache;
	glob.memSwapTot = find_first_occ("/proc/meminfo", "SwapTotal") / 1024;
	glob.memSwapFree = find_first_occ("/proc/meminfo", "SwapFree") / 1024;
	glob.memSwapUsed = glob.memSwapTot - glob.memSwapFree;
	glob.memAvai = find_first_occ("/proc/meminfo", "MemAvailable") / 1024;
}

void setProcessVar() {
	FILE* f = fopen("/etc/passwd", "r");
	char string[200], *tok, *res;
	Passwd pass[PASSWD_SIZE];
	int index = 0;
	while (1) {
		res = fgets(string, 200, f);
		if (res == NULL)
			break;
		tok = strtok(string, ":");
		strcpy(pass[index].name, tok);
		tok = strtok(NULL, ":");
		tok = strtok(NULL, ":");
		strcpy(pass[index].uid, tok);
		index++;
	}
	fclose(f);
	
	for (int i=0; i<MAX_PROC; i++) {
		if (atoi(proc[i].pid) != 0) {

			setVariableProcess(&proc[i], pass);
			proc[i].mem = proc[i].res / 10.24 / glob.memTot;
			usleep(10);
		}
	}
}

void* terminale() {
	term_win(height, width, starty, startx, &cur, &glob.activeProc);
	running = 0;
}

int main() {
	WINDOW* main_win = create_main_win(&height, &width, &startx, &starty);
	running = 1;
	pthread_t term_thread;
	int ret = pthread_create(&term_thread, NULL, terminale, NULL);
	if (ret == -1)
		exit(EXIT_FAILURE);
	int x=0;
	while (running) {
		memset(proc, 0, sizeof(proc));
		readDir(proc);
		glob.activeProc = 0;
		glob.runningProc = 0;
		glob.sleepProc = 0;
		glob.zombieProc = 0;
		setMemoryVar();
		setProcessVar();
		countActiveProcess(proc, &glob);
  	qsort(&proc, glob.activeProc, sizeof(Process), (void*) processCmp);

		print_main_win(main_win, proc, &glob, height, width, starty, startx, &cur, &x);
		x++;
		sleep(1);
	}
  return 0;
}

