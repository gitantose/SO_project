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
double numberKB(char* path, char* str) {
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
		while (c == ' ' || c == '\t')
			c = fgetc(f);
		while (c=='0' || c=='1' || c=='2' || c=='3' || c=='4' || c=='5' ||
						c=='6' || c=='7' || c=='8' || c=='9') {
			number[n++] = c;
			printf("%c", c);
			c = fgetc(f);
		}
		number[n] = '\0';
		fclose(f);
		return atoi(number);		
	}
	fclose(f);
	return -1;
}

void setVariableProcess(Process * p) {
	if ((*p).pid == 0) return;
	char pid[BUF_LEN], ni[4], priority[4], virtual[BUF_LEN], starttime[BUF_LEN], utime[BUF_LEN], stime[BUF_LEN];
	int index=0, i_ni=0, i_priority=0, i_virtual=0, i_starttime=0, i_utime=0, i_stime=0;
	char c = 'a';
	
	sprintf(pid, "%d", (*p).pid);
	char path[SIZE_PATH] = "";
	strcat(path, "/proc/");
	strcat(path, pid);
	strcat(path, "/stat");
	FILE * f = fopen(path, "r");
	if (f == NULL) {
		printf("ERRORE nell' apertura del file\n");
		return;
	}
	while (c != EOF) {
		c = fgetc(f);
		if (c == ' ') 
			index++;
		else if (index == 2) // setto lo stato del processo;
			(*p).status = c;
		else if (index == 17) // setto la priorità
			priority[i_priority++] = c;
		else if (index == 18) // setto il nice value
			ni[i_ni++] = c;
		else if (index == 22) // virtual memory in byte
			virtual[i_virtual++] = c;
		else if (index == 13)
			utime[i_utime++] = c;
		else if (index == 14)
			stime[i_stime] = c;
		else if (index == 21)
			starttime[i_starttime] = c;
	}
	
	priority[i_priority] = '\0';
	ni[i_ni] = '\0';
	virtual[i_virtual] = '\0';
	utime[i_utime] = '\0';
	stime[i_stime] = '\0';
	starttime[i_starttime] = '\0';
	(*p).priority = atoi(priority);
	(*p).ni = atoi(ni);
	(*p).virt = atol(virtual);
	// (*p).cpu = (atoi(utime) + atoi(stime)) / atoi(starttime); // TODO gestire errore
	fclose(f);
	
	// read status 
	strcat(path, "us");
	
	printf("%s", path);
	
	(*p).shr = (long) numberKB(path, "RssShmem");
	
	// read cmdline
	path[0] = '\0';
	strcat(path, "/proc/");
	strcat(path, pid);
	strcat(path, "/cmdline");

	f = fopen(path, "r");
	if (f == NULL) {
		printf("ERRORE nell'apertura del file\n");
	} else {
		c = 'a';
		index = 0;
		while (c != EOF) {
			c = fgetc(f);
			(*p).command[index++] = c;
		}
	}
	fclose(f);
	
}

void printProcess(Process p) {
	printf("Pid: %d, Status: %c, User: %s, Priority: %d, NI: %d, Virt: %ld, CPU: %0.1f, Time: %d, SHR: %ld, Mem: %0.1f, Res: %ld, Command: %s\n", p.pid, p.status, p.user, p.priority, p.ni, p.virt, p.cpu, p.time, p.shr, p.mem, p.res, p.command); 
}

