#include "main.h"
#include "data.h"
#include "window.h"

Process proc[MAX_PROC] = {0x0};
Global glob;
int running;

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
			usleep(1000);
			//set the user OK
			//set the priority OK
			//set the NI (nice value of task) OK
			// set the virtual memory OK
			// set how much RAM the process used OK
			// set the shared memory used OK
			// set the status OK
			//set the percentage of cpu OK
			// set the percentage of memory
			// set the time
			// set command OK
		}
	}
}

void* run0() {
	//printf("run0\n");
	window(proc, &glob);
	printf("terminato");
/*	char c;
	while (1) {
		c = getch();
		if (c=='q') 
			break;
		print(proc, 20);
		printf("--------------------------------------------------\n");
		sleep(1);
	}
*/	
	running = 0;
	pthread_exit(NULL); 
}

void* run1() {
	char x[1000000];
	int i = 0;
	while(running) {
		readDir(proc);
		glob.activeProc = 0;
		glob.runningProc = 0;
		glob.sleepProc = 0;
		glob.zombieProc = 0;
		setMemoryVar();
		setProcessVar();
		countActiveProcess(proc, &glob);
  	qsort(&proc, glob.activeProc, sizeof(Process), (void*) processCmp);
  	//sleep(0.5);
	}
	pthread_exit(NULL); 
}

int main() {
// SEQUENZIALE
	readDir(proc);
	glob.activeProc = 0;
	glob.runningProc = 0;
	glob.sleepProc = 0;
	glob.zombieProc = 0;
	setMemoryVar();
	setProcessVar();
	countActiveProcess(proc, &glob);
  qsort(&proc, glob.activeProc, sizeof(Process), (void*) processCmp);
	//print(proc, 20);
	window(proc, &glob);
/*

	running = 1;
	pthread_t thread[2];
  int ret = pthread_create(&thread[1], NULL, run1, NULL);
  sleep(1);
  //run0();
  ret = pthread_create(&thread[0], NULL, run0, NULL);
  printf("0\n");
  pthread_join(thread[0], NULL);
  printf("1\n");
	pthread_join(thread[1], NULL);
  printf("2\n");
	//pthread_exit(NULL); 
	printf("3\n");
*/
  return 0;
}

