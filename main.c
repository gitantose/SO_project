#include "main.h"
#include "data.h"
#include "window.h"

Process proc[MAX_PROC] = {0x0};
int activeProc = 0;
int runningProc = 0;
int sleepProc = 0;
float memTot = 0; //memFree + memUsed + memCash
float memFree = 0;
float memUsed = 0;
float memCache = 0;
float memAvai = 0;
float memSwapTot = 0;
float memSwapFree = 0;
float memSwapUsed = 0;

static int processCmp(const void* p1, const void* p2) {
	Process* a = (Process*) p1;
	Process* b = (Process*) p2;
	return (int) (*b).cpu - (*a).cpu;
}

void printVar() {
	printf("MemTotal: %f\n", memTot);
	printf("MemFree: %f\n", memFree);
	printf("MemCache: %f\n", memCache);
	printf("MemUsed: %f\n", memUsed);
	printf("SwapTotal: %f\n", memSwapTot);
	printf("SwapFree: %f\n", memSwapFree);
	printf("SwapUsed: %f\n", memSwapFree);
	printf("Available: %f\n", memAvai);
}

void setMemoryVar() {
	memTot = numberKB("/proc/meminfo", "MemTotal") / 1024;
	memFree = numberKB("/proc/meminfo", "MemFree") / 1024;
	memCache = numberKB("/proc/meminfo", "Cached") / 1024;
	memUsed = memTot - memFree - memCache;
	memSwapTot = numberKB("/proc/meminfo", "SwapTotal") / 1024;
	memSwapFree = numberKB("/proc/meminfo", "SwapFree") / 1024;
	memSwapUsed = memSwapTot - memSwapFree;
	memAvai = numberKB("/proc/meminfo", "MemAvailable") / 1024;
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
			proc[i].mem = proc[i].res / 1,024 / memTot;
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

int main() {
  // read the '/proc' directory
	readDir(proc);
	// printPID(proc);
	countActiveProcess(proc, &activeProc);
	setMemoryVar();
	setProcessVar();
	//print(proc, activeProc);
  qsort(&proc, activeProc, sizeof(Process), (void*) processCmp);
	//printf("------------------\n");
	//print(proc, activeProc);
	window(proc);
  return 0;
}

