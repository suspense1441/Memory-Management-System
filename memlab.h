/*
Hands-on experience for creating better memory management systems
OS Assignment - 5
    Group - 43
    - Tushar Kishor Bokade 19CS30011
    - Aditya Vallakatla 19CS30051
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <time.h>
#include <iostream>
#include <bits/stdc++.h>
#include <pthread.h>

using namespace std;
using namespace std::chrono;

#define PAGETABLESIZE 10000
#define SYMTABLESIZE 100
#define FUNCTABLESIZE 100
#define NAMESIZE 100
#define DEBUG 0

bool createMem(int size);

void gc_initialize();

void *garbageCollector(void *args);

void gc_run(void *args);

int regFunc(string funcName);

void createVar(string varName, string type, int funcIdx);

void assignVar(int value, string varName, string type, int funcIdx);
int getInt(string varName, string type, int funcIdx, int index = 0);

void assignVar(char value, string varName, string type, int funcIdx);
char getChar(string varName, string type, int funcIdx, int index = 0);

void assignVar(bool value, string varName, string type, int funcIdx);
bool getBool(string varName, string type, int funcIdx, int index = 0);

void createArr(string varName, string type, int funcIdx, int count);

void assignArr(int value, string varName, string type, int funcIdx, int index);
void getIntArr(string varName, string type, int funcIdx, int *arr);

void assignArr(char value, string varName, string type, int funcIdx, int index);
void getCharArr(string varName, string type, int funcIdx, char *arr);

void assignArr(bool value, string varName, string type, int funcIdx, int index);
void getBoolArr(string varName, string type, int funcIdx, bool *arr);

void freeElem(string varName, string type, int funcIdx);

void exitFunc(int idx);
