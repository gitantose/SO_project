#ifndef _DATA_GUARD
#define _DATA_GUARD

#include "main.h"

void readDir(Process* proc);
void printPID(Process* proc);
void countActiveProcess(Process* proc);
double numberKB(char* path, char* str);
void setVariableProcess(Process * p);
void printProcess(Process p);

#endif
