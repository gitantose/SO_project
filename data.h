#ifndef _DATA_GUARD
#define _DATA_GUARD

#include "main.h"

void readDir(Process* proc);
void countActiveProcess(Process* proc, Global* glob);
long find_first_occ(char* path, char* str);
void setVariableProcess(Process * p, Passwd* pass);

#endif
