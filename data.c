#include "data.h"

// read all directory in /proc and save pid process
void readDir(Process* proc) {
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
void printPID(Process* proc) {
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
void countActiveProcess(Process* proc) {
	for (int i = 0; i<MAX_PROC; i++) {
		if (proc[i].pid==0) {
			printf("Processi attivi: %d\n", i);
			break;
		}
	}
}

// return the final position of the world in the file
// assuming that not have repeat of word
float numberKB(char* path, char* str) {
	FILE * f = fopen(path, "r");
	char c;
	char number[30];
	int n = 0;
	int ret = 0, index = 0, len = strlen(str);
	int key = 0;
	while (c != EOF && !key) {
		c = fgetc(f);
		if (len == index)
			key = 1;
		else if (index < len && c == str[index]) {
			index++;
		} else 
			index = 0;
		ret++;
	}
	
	if (key) {
		c = fgetc(f);
		while (c == ' ')
			c = fgetc(f);
		while (c=='0' || c=='1' || c=='2' || c=='3' || c=='4' || c=='5' ||
						c=='6' || c=='7' || c=='8' || c=='9') {
			number[n++] = c;
			c = fgetc(f);
		}
		number[n] = '\0';
		return atoi(number);		
	} else 
		printf("String not found\n");
	return -1;
}



