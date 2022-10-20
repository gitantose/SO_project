#ifndef _DATA_GUARD
#define _DATA_GUARD

#include "main.h"

void readDir(Process* proc);
void print(Process* proc, int n);
void countActiveProcess(Process* proc, Global* glob);
long numberKB(char* path, char* str);
void setVariableProcess(Process * p, Passwd* pass);
void printProcess(Process p);

#endif
