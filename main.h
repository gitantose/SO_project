#include <dirent.h> 
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#ifndef _MAIN_GUARD
#define _MAIN_GUARD

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

#endif
