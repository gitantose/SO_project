#include <dirent.h> 
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#ifndef _MAIN_GUARD
#define _MAIN_GUARD

#define MAX_PROC 500 
#define MAX_SIZE_USER 20
#define MAX_SIZE_COMMAND 20
#define BUF_LEN 20
#define SIZE_PATH 1000

#define PASSWD_SIZE 50

typedef struct {
	char pid[5]; // PID
	char status; // r -> running | s -> sleeping 
	char user[MAX_SIZE_USER]; // user name of owner of task
	int priority; // priority
	long virt; // virtual memory used
	float cpu; // percentual of CPU usage
	int time; // CPU time
	long shr; // shared memory used
	int ni; // rapresent a nice value of task. A negative value implies higher priority
	float mem; // percentual of memory used
	long res; // RAM memory used
	char command[MAX_SIZE_COMMAND]; // name of the command that started the process
} Process;

typedef struct {
	char uid[BUF_LEN];
	char name[BUF_LEN];
} Passwd;


#endif
