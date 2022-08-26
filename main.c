#include <dirent.h> 
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#define MAX_PROC 1000
#define MAX_SIZE_USER 20
#define MAX_SIZE_COMMAND 20
#define BUF_LEN 20

typedef struct {
	int pid; // PID
	char status; // r -> running | s -> sleeping 
	char user[MAX_SIZE_USER]; // user name of owner of task
	int prioriry; // priority
	int virt; // virtual memory used
	int cpu; // percentual of CPU usage
	int time; // CPU time
	int shr; // shared memory used
	int ni; // rapresent a nice value of task. A negative value implies higher priority
	int mem; // percentual of memory used
	int res; // RAM memory used
	char command[MAX_SIZE_COMMAND]; // name of the command that started the process
} Process;

Process proc[MAX_PROC] = {0x0};
int activeProc = 0;
int runningProc = 0;
int sleepProc = 0;
float memTot = 0; //memFree + memUsed + memCash
float memFree = 0;
float memUsed = 0;
float memCash = 0;
float memSwapTot = 0;
float memSwapFree = 0;
float memSwapUsed = 0;

// read all directory in /proc and save pid process
void readDir() {
	int size = 0;
	DIR *d;
	struct dirent *dir;
	int value = -1;
	d = opendir("/proc");
  if (d) {
  	while ((dir = readdir(d)) != NULL) {
  		//printf("%s\n", dir->d_name);
  		value = atoi(dir->d_name);
  		if (value != 0) 
  			proc[size++].pid = value;
  	}
  closedir(d);
  }
}

// print pid active process
void printPID() {
	printf("PID Process\n");
	for (int i = 0; i<MAX_PROC; i++) {
		if (proc[i].pid==0) {
			printf("\n");
			return;
		}
		printf("%d  ", proc[i].pid);
	}
	printf("\n");
}

// count active process and set the variable "activeProc"
void countActiveProcess() {
	for (int i = 0; i<MAX_PROC; i++) {
		if (proc[i].pid==0) {
			printf("Processi attivi: %d\n", i);
			break;
		}
	}
}

// return the final position of the world in the file
// assuming that not have repeat of word
int offsetWord(char* path, char* str) {
  FILE * f = fopen(path, "r");
	char c; 
	int ret = 0, index = 0, len = strlen(str);
	while (c != EOF) {
		c = fgetc(f);
		if (len == index)
			return ret;
		else if (index < len && c == str[index]) {
			index++;
		} else 
			index = 0;
		ret++;
	}
  return ret;
}

void setMemoryVar() {
	int pos = offsetWord("/proc/meminfo", "MemFree");
	printf("'MemFree' end at char #%d\n", pos);
}

int main() {
  // read the '/proc' directory
	readDir();
	// printPID();
	// countActiveProcess();
	setMemoryVar();
	
  return 0;
}




















