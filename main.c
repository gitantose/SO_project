#include "main.h"
#include "data.h"

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


void setMemoryVar() {
	memTot = numberKB("/proc/meminfo", "MemTotal") / 1024;
	printf("MemTotal: %f\n", memTot);
	memFree = numberKB("/proc/meminfo", "MemFree") / 1024;
	printf("MemFree: %f\n", memFree);
	memCache = numberKB("/proc/meminfo", "Cached") / 1024;
	printf("MemCache: %f\n", memCache);
	memUsed = memTot - memFree - memCache;
	printf("MemUsed: %f\n", memUsed);
	memSwapTot = numberKB("/proc/meminfo", "SwapTotal") / 1024;
	printf("SwapTotal: %f\n", memSwapTot);
	memSwapFree = numberKB("/proc/meminfo", "SwapFree") / 1024;
	printf("SwapFree: %f\n", memSwapFree);
	memSwapUsed = memSwapTot - memSwapFree;
	printf("SwapUsed: %f\n", memSwapFree);
	memAvai = numberKB("/proc/meminfo", "MemAvailable") / 1024;
	printf("Available: %f\n", memAvai);
}

void setProcessVar() {
	for (int i=0; i<MAX_PROC; i++) {
		if (proc[i].pid == 1610) {
			setVariableProcess(&proc[i]);
			printProcess(proc[i]);
		}
		//set the user
		//set the priority
		//set the NI (nice value of task)
		// set the virtual memory
		// set how much RAM the process used
		// set the shared memory used
		// set the status
		//set the percentage of cpu
		// set the percentage of memory
		// set the time
		// set command
	}
}

int main() {
  // read the '/proc' directory
	readDir(proc);
	// printPID(proc);
	// countActiveProcess(proc);
	// setMemoryVar();
	setProcessVar();
  return 0;
}

