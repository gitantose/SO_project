#include <dirent.h> 
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_PROC 1000

int proc[MAX_PROC] = {0x0}; // PID processi attivi

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
  			proc[size++] = value;
  	}
  closedir(d);
  }
}

void printPID() {
	printf("PID Process\n");
	for (int i = 0; i<MAX_PROC; i++) {
		if (proc[i]==0) {
			printf("\n");
			return;
		}
		printf("%d  ", proc[i]);
	}
	printf("\n");
}

void countActiveProcess() {
	for (int i = 0; i<MAX_PROC; i++) {
		if (proc[i]==0) {
			printf("Processi attivi: %d\n", i);
			break;
		}
	}
}

int main() {
  // read the '/proc' directory
	readDir();
	printPID();
	countActiveProcess();
  return 0;
}




















