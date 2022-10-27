#include "data.h"

// read all directory in /proc and save pid process
void readDir(Process* proc) {
	int size = 0;
	DIR *d;
	struct dirent *dir;
	char value[20] = "";
	d = opendir("/proc");
  if (d) {
  	while ((dir = readdir(d)) != NULL) {
  		if (atoi(dir->d_name) != 0)
  			strcpy(proc[size++].pid, dir->d_name);
  	}
  	closedir(d);
  }
}

// count active process and set the variable "activeProc"
void countActiveProcess(Process* proc, Global* glob) {
	for (int i = 0; i<MAX_PROC; i++) {
		if (proc[i].status == 'R')
			glob->runningProc += 1;
		else if(proc[i].status == 'S')
			glob->sleepProc += 1;
		else if(proc[i].status == 'Z')
			glob->zombieProc += 1;
		if (strcmp(proc[i].pid,"")==0) {
			glob->activeProc = i;
			break;
		}
	}
}

// restituisce il primo numero che segue a str
long find_first_occ(char* path, char* str) {
	FILE * f = fopen(path, "r");
	if (f == NULL) 
		exit(EXIT_FAILURE);
	char c = ' ';
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
			c = fgetc(f);
		}
		number[n] = '\0';
		fclose(f);
		return atoi(number);		
	}
	fclose(f);
	return -1;
}

void setVariableProcess(Process * p, Passwd* pass) {
	if ((*p).pid == 0) return;
	char pid[5]="", ni[4], priority[4], virtual[BUF_LEN], starttime[BUF_LEN], utime[BUF_LEN], stime[BUF_LEN], uptime[BUF_LEN], cutime[BUF_LEN], cstime[BUF_LEN];
	int index=0, i_ni=0, i_priority=0, i_virtual=0, i_starttime=0, i_utime=0, i_stime=0, i_uptime=0, i_cutime=0, i_cstime=0;
	char c = 'a';
	
	char path[SIZE_PATH] = "";
	strcat(path, "/proc/");
	strcat(path, (*p).pid);
	strcat(path, "/stat");

	
	FILE * f = fopen(path, "r");
	if (f == NULL)
		exit(EXIT_FAILURE);

	while (c != EOF) {
		c = fgetc(f);
		if (c == ' ') 
			index++;
		else if (index == 2)
			(*p).status = c;
		else if (index == 17)
			priority[i_priority++] = c;
		else if (index == 18)
			ni[i_ni++] = c;
		else if (index == 13)
			utime[i_utime++] = c;
		else if (index == 14)
			stime[i_stime++] = c;
		else if (index == 15)
			cutime[i_cutime++] = c;
		else if (index == 16)
			cstime[i_cstime++] = c;
		else if (index == 21)
			starttime[i_starttime++] = c;
		else if (index == 22) // virtual memory in byte
			virtual[i_virtual++] = c;
		else if (index == 24)
			c = EOF;
	}
	fclose(f);
	
	priority[i_priority] = '\0';
	ni[i_ni] = '\0';
	virtual[i_virtual] = '\0';
	utime[i_utime] = '\0';
	stime[i_stime] = '\0';
	starttime[i_starttime] = '\0';
	(*p).priority = atoi(priority);
	(*p).ni = atoi(ni);
	(*p).virt = (long) atol(virtual) / 1024; //rappresentata in kB
	
	// uptime	
	f = fopen("/proc/uptime", "r");
	if (f == NULL)
		exit(EXIT_FAILURE);
	c = 'a';
	while(c != EOF) {
		c = fgetc(f);
		if (c == ' ') {
			uptime[i_uptime + 1] = '\0';
			c = EOF;
		}
		else
			uptime[i_uptime++] = c;
	}
	fclose(f);

	double a = atof(utime)/sysconf(_SC_CLK_TCK);
	double b = atof(stime)/sysconf(_SC_CLK_TCK);
	double cc = atof(starttime)/sysconf(_SC_CLK_TCK);
	(*p).cpu = (double) (( a+ b)) * 100 / (atof(uptime) - cc);
	//long h = atol(utime) + atol(stime) + atoi(cutime) + atoi(cstime);
	(*p).time = atof(utime) + atof(stime);
	
	// read /proc/[pid]/status 
	strcat(path, "us");
	
	long RSsh =  find_first_occ(path, "RssShmem");
	long RSan = find_first_occ(path, "RssAnon");
	long RSfd = find_first_occ(path, "RssFile");
	if (RSsh == -1) RSsh = 0; 		// not found
	if (RSan == -1) RSan = 0;			// not found
	if (RSfd == -1) RSfd = 0;			// not found
	// printf("%ld %ld %ld\n", RSsh, RSan, RSfd);
	(*p).shr = RSsh;
	(*p).res = RSsh + RSan + RSfd;
	(*p).mem = (*p).shr + (*p).res;
	
	int x = find_first_occ(path, "Uid:");
	char temp[BUF_LEN];
	sprintf(temp, "%d", x);
	index = 0;
	while(index < PASSWD_SIZE) {
		if (strcmp(temp, pass[index].uid) == 0) {
			strcpy((*p).user, pass[index].name);
			index = PASSWD_SIZE;
		}
		index++;
	}
	
	// read /proc/[pid]/cmdline
	path[0] = '\0';
	strcat(path, "/proc/");
	strcat(path, (*p).pid);
	strcat(path, "/cmdline");

	f = fopen(path, "r");
	
	if (f == NULL) {
		exit(EXIT_FAILURE);
	} 
	else {
		c = 'a';
		index = 0;
		while (c != EOF && index<MAX_SIZE_COMMAND-1) {
			c = fgetc(f);
			(*p).command[index++] = c;
		}
		(*p).command[index] = '\0';
	}
	fclose(f);
}



void setMemoryVar(Global* glob) {
	(*glob).memTot = find_first_occ("/proc/meminfo", "MemTotal") / 1024;
	(*glob).memFree = find_first_occ("/proc/meminfo", "MemFree") / 1024;
	(*glob).memCache = find_first_occ("/proc/meminfo", "Cached") / 1024;
	(*glob).memUsed = (*glob).memTot - (*glob).memFree - (*glob).memCache;
	(*glob).memSwapTot = find_first_occ("/proc/meminfo", "SwapTotal") / 1024;
	(*glob).memSwapFree = find_first_occ("/proc/meminfo", "SwapFree") / 1024;
	(*glob).memSwapUsed = (*glob).memSwapTot - (*glob).memSwapFree;
	(*glob).memAvai = find_first_occ("/proc/meminfo", "MemAvailable") / 1024;
}

void setProcessVar(Process* proc, Global* glob) {
	FILE* f = fopen("/etc/passwd", "r");
	if (f == NULL)
		exit(EXIT_FAILURE);
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
			proc[i].mem = proc[i].res / 10.24 / (*glob).memTot;
			usleep(10);
		}
	}
}


