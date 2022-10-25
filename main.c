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
	return (int) (*b).cpu - (*a).cpu;
}

void printVar() {
	printf("MemTotal: %f\n", glob.memTot);
	printf("MemFree: %f\n", glob.memFree);
	printf("MemCache: %f\n", glob.memCache);
	printf("MemUsed: %f\n", glob.memUsed);
	printf("SwapTotal: %f\n", glob.memSwapTot);
	printf("SwapFree: %f\n", glob.memSwapFree);
	printf("SwapUsed: %f\n", glob.memSwapFree);
	printf("Available: %f\n", glob.memAvai);
}

void setMemoryVar() {
	glob.memTot = numberKB("/proc/meminfo", "MemTotal") / 1024;
	glob.memFree = numberKB("/proc/meminfo", "MemFree") / 1024;
	glob.memCache = numberKB("/proc/meminfo", "Cached") / 1024;
	glob.memUsed = glob.memTot - glob.memFree - glob.memCache;
	glob.memSwapTot = numberKB("/proc/meminfo", "SwapTotal") / 1024;
	glob.memSwapFree = numberKB("/proc/meminfo", "SwapFree") / 1024;
	glob.memSwapUsed = glob.memSwapTot - glob.memSwapFree;
	glob.memAvai = numberKB("/proc/meminfo", "MemAvailable") / 1024;
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
			proc[i].mem = proc[i].res / 1,024 / glob.memTot;
			usleep(10);
			//set the user OK
			//set the priority OK
			//set the NI (nice value of task) OK
			// set the virtual memory OK
			// set how much RAM the process used OK
			// set the shared memory used OK
			// set the status OK
			//set the percentage of cpu OK
			// set the percentage of memory
			// set the time OK
			// set command OK
		}
	}
}

void* terminale() {
	term_win(height, width, starty, startx, &cur);
	running = 0;
}

int main() {
	// ALTERNATIVA
	//crea window e setta dati iniziali
	WINDOW* main_win = create_main_win(&height, &width, &startx, &starty);
	running = 1;
	// crea thread per abilitare lettura di dati da terminale
	pthread_t term_thread;
	int ret = pthread_create(&term_thread, NULL, terminale, NULL);
	if (ret == -1)
		exit(EXIT_FAILURE);
	int x=0;
	while (running) {
		// leggi dati e riempi array
		readDir(proc);
		glob.activeProc = 0;
		glob.runningProc = 0;
		glob.sleepProc = 0;
		glob.zombieProc = 0;
		setMemoryVar();
		setProcessVar();
		countActiveProcess(proc, &glob);
  	qsort(&proc, glob.activeProc, sizeof(Process), (void*) processCmp);
		// stampa su window
		print_main_win(main_win, proc, &glob, height, width, starty, startx, &cur, &x);
		x++;
		sleep(1);
	}
  return 0;
}

