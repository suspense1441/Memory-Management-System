/*
Hands-on experience for creating better memory management systems
OS Assignment - 5
	Group - 43
	- Tushar Kishor Bokade 19CS30011
	- Aditya Vallakatla 19CS30051
*/

#include "memlab.h"

// Structure of Page
class Page
{
public:
	int location;			// address of the word/frame
	char varName[NAMESIZE]; // Name of the variable
	int type;				// 0-char, 1-boolean, 2-median int, 3-int (based on their size)
	int ArrayBit;			// 0-normal variable, 1-Array
	int ValidBit;			// 0 - The location is free, 1 - The location is allocated
	int size;
};

// Structure of Function
class Function
{
public:
	int funcValidBit;								// 0 - Function Invalid, 1 - Function Valid
	int nextLoc;									// next location for next var in symbolTable
	int offset;										// Function offset inside the memory
	char symbolTableString[SYMTABLESIZE][NAMESIZE]; // Stores variable information
	int symbolTableInt[SYMTABLESIZE];				// Stores variable information
};

// Structure of LocationMap
class LocationMap
{
public:
	int location; // location inside the allocated memory
	int idx;	  // index inside the page table
};

// Structure of Memory
class Memory
{
public:
	void *init;								  // Starting Address of the Memory segment allocated
	int size;								  // size created using createMem (in bytes)
	int counter;							  // points to the next free memory space
	int nextPageLoc;						  // points to the next free page Table entry
	int nextFuncLoc;						  // points to the next free Function Table entry
	int nextMapLoc;							  // points to the next free location Table entry
	int usedSpace;							  // Space used by the user program
	Function functionTable[FUNCTABLESIZE];	  // function table - stores info about the function and its local variables/arrays
	Page pageTable[PAGETABLESIZE];			  // page table
	LocationMap locationTable[PAGETABLESIZE]; // location table - used by garbage collector for compacting process
};

Memory *memory;

pthread_t gc;
pthread_mutex_t mlock;

// Function to sleep - msec is in milliseconds
int msleep(long msec)
{
	struct timespec ts;
	int res;

	if (msec < 0)
	{
		errno = EINVAL;
		return -1;
	}

	ts.tv_sec = msec / 1000;
	ts.tv_nsec = (msec % 1000) * 1000000;

	do
	{
		res = nanosleep(&ts, &ts);
	} while (res && errno == EINTR);
	return res;
}

// GC_Initialize to initialize the garbageCollector
void gc_initialize()
{
	if (pthread_create(&gc, NULL, &garbageCollector, NULL) < 0)
	{
		perror("Garbage Collector Thread Initialization Error!");
		exit(0);
	}
	cout << "Garbage Collector Initialized" << endl;
	return;
}

// Comparsion function for sorting the LocationMap Elements
bool comp(LocationMap a, LocationMap b)
{
	return a.location < b.location;
}

// Function to print Page Table
void printPageTable()
{
	if (DEBUG == 0)
		return;
	cout << "PRINTING PAGE TABLE: " << endl;
	for (int i = 0; i < memory->nextPageLoc; ++i)
	{
		cout << "loc: " << memory->pageTable[i].location << " name: " << memory->pageTable[i].varName << " type: " << memory->pageTable[i].type << " arraybit: " << memory->pageTable[i].ArrayBit << " valid: " << memory->pageTable[i].ValidBit << " size: " << memory->pageTable[i].size << endl;
	}
}

// Garbage Collector Implementation and Compaction
void gc_run()
{
	if (memory == NULL)
		return;
	for (int i = 0; i < memory->nextFuncLoc; i++)
	{
		if (memory->functionTable[i].funcValidBit == 0)
		{
			for (int j = 0; j < memory->functionTable[i].nextLoc; j++)
			{
				memory->pageTable[memory->functionTable[i].symbolTableInt[j]].ValidBit = 0;
				memory->usedSpace -= memory->pageTable[memory->functionTable[i].symbolTableInt[j]].size;
			}
			memory->functionTable[i].funcValidBit = -1;
		}
	}
	pthread_mutex_lock(&mlock);
	cout << "COMPACTION INITIATED" << endl;
	printPageTable();
	if (memory->nextMapLoc == 0)
	{
		cout << "COMPACTION ENDED" << endl;
		pthread_mutex_unlock(&mlock);
		return;
	}

	sort(memory->locationTable, memory->locationTable + memory->nextMapLoc, comp);
	char *x, *y;
	int ct = 0;
	int sfree = 0;
	int pos = 0;
	for (; pos < memory->nextMapLoc; pos++)
	{
		int id = memory->locationTable[pos].idx;
		if (memory->pageTable[id].ValidBit == 0 && memory->pageTable[id].location != -1 && memory->pageTable[id].size > 0)
		{
			x = (char *)((char *)memory->init + memory->pageTable[id].location);
			memory->locationTable[pos].location = memory->counter;
			memory->pageTable[id].location = -1;
			memory->pageTable[id].size == 0;
			ct++;
			sfree += memory->pageTable[id].size;
			pos++;
			break;
		}
	}
	for (; pos < memory->nextMapLoc; pos++)
	{
		int id = memory->locationTable[pos].idx;
		if (memory->pageTable[id].ValidBit == 1 && memory->pageTable[id].location != -1 && memory->pageTable[id].size > 0)
		{
			y = (char *)((char *)memory->init + memory->pageTable[id].location);
			memory->pageTable[id].location = x - (char *)memory->init;
			int size = memory->pageTable[id].size;
			for (int i = 0; i < size; ++i)
			{
				*(x + i) = *(y + i);
			}
			x += size;
		}
		else if (memory->pageTable[id].location != -1 && memory->pageTable[id].size > 0)
		{
			memory->locationTable[pos].location = memory->counter;
			ct++;
			memory->pageTable[id].location = -1;
			sfree += memory->pageTable[id].size;
		}
	}
	sort(memory->locationTable, memory->locationTable + memory->nextMapLoc, comp);
	memory->nextMapLoc -= ct;
	cout << sfree << " Space cleared" << endl;
	// memory->usedSpace -= sfree;
	memory->counter = memory->counter - sfree;
	printPageTable();
	cout << "COMPACTION ENDED" << endl;
	pthread_mutex_unlock(&mlock);
	return;
}

// Function to Call gc_run Periodically
void *garbageCollector(void *args)
{
	cout << "GC_Started" << endl;
	while (memory != NULL)
	{
		gc_run();
		msleep(10);
	}
	return NULL;
}

// Function to Create Memory
bool createMem(int size)
{
	cout << "Creating Memory, Size :" << size << endl;
	int storage = sizeof(Memory); // space for page Table
	void *start = (void *)malloc(size);
	memory = (Memory *)start;
	memory->init = static_cast<void *>(static_cast<char *>(start) + storage);
	memory->size = size;
	memory->counter = 0;
	memory->nextFuncLoc = 0;
	memory->nextPageLoc = 0;
	memory->nextMapLoc = 0;
	memory->usedSpace = 0;
	pthread_mutexattr_t att;
	pthread_mutexattr_init(&att);
	pthread_mutexattr_setrobust(&att, PTHREAD_MUTEX_ROBUST);
	if (pthread_mutex_init(&mlock, &att) < 0)
	{
		perror("Mutex Lock Initialization Error");
		exit(0);
	}
	if (memory->init == NULL)
		return false;
	gc_initialize();
	return true;
}

// function to register a function and return the index
int regFunc(string funcName)
{
	if (DEBUG)
		cout << "Registering Function: " << funcName << endl;
	int pos = memory->nextFuncLoc;
	memory->nextFuncLoc++;
	memory->functionTable[pos].funcValidBit = 1;
	memory->functionTable[pos].nextLoc = 0;
	return memory->nextFuncLoc - 1;
}

// function to exit the function passed in the argument
void exitFunc(int idx)
{
	memory->functionTable[idx].funcValidBit = 0;
	if (DEBUG)
		cout << "Function " << idx << " Ended!" << endl;
	printPageTable();
	if (idx == 0)
	{
		free(memory);
		memory = NULL;
		pthread_cancel(gc);
		pthread_mutex_destroy(&mlock);
		cout << "GC_EXITED" << endl;
		msleep(0.5);
	}
	return;
}

// function to search free space in pageTable
int searchIndex(int size)
{
	int fs = sizeof(Memory) + memory->counter;
	if (memory->nextPageLoc < PAGETABLESIZE && memory->size - fs > size)
	{
		return memory->nextPageLoc;
	}
	for (int i = 0; i < PAGETABLESIZE; ++i)
	{
		if (memory->pageTable[i].ValidBit == 0 && memory->pageTable[i].location == -1)
		{
			int arr = memory->pageTable[i].ArrayBit;
			int sz = 4;
			if (arr)
				sz = arr * 4;
			if (sz >= size)
				return i;
		}
	}
	return -1;
}

// deducing variable's type
int varType(string type)
{
	int t = 0;
	if (type == "char")
		t = 1;
	else if (type == "medium int")
		t = 2;
	else if (type == "int")
		t = 3;
	return t;
}

// Function to Check if the given integer fits in medium int
bool checkMediumInt(int value)
{
	if (abs(value) <= (1 << 23))
		return true;
	return false;
}

// Checking if any other variable with same name exists or not
bool checkVarName(string varName, int funcIdx)
{
	char name[NAMESIZE];
	bzero(name, NAMESIZE);
	for (int i = 0; i < varName.size(); i++)
		name[i] = varName[i];
	for (int i = 0; i < memory->functionTable[funcIdx].nextLoc; ++i)
	{
		if (!strcmp(memory->functionTable[funcIdx].symbolTableString[i], name))
			return true;
	}
	return false;
}

// Function to Create a Variable
void createVar(string varName, string type, int funcIdx)
{
	pthread_mutex_lock(&mlock);
	if (DEBUG)
		cout << "Creating Variable, Name: " << varName << " Type: " << type << " Function: " << funcIdx << endl;
	int t = varType(type);

	if (checkVarName(varName, funcIdx))
	{
		if (DEBUG)
			cout << "A variable with the same name " << varName << " already exists!!" << endl;
		pthread_mutex_unlock(&mlock);
		return;
	}

	// search a proper index for the variable inside the pageTable
	int idx = searchIndex(4);
	// storing info inside the symbolTable of the function
	int loc = memory->functionTable[funcIdx].nextLoc;
	char name[NAMESIZE];
	bzero(name, NAMESIZE);
	for (int i = 0; i < varName.size(); i++)
		name[i] = varName[i];
	strcpy(memory->functionTable[funcIdx].symbolTableString[loc], name);
	memory->functionTable[funcIdx].symbolTableInt[loc] = idx;
	memory->functionTable[funcIdx].nextLoc++;

	int pos = memory->nextPageLoc;
	memory->pageTable[pos].location = memory->counter;
	memory->pageTable[pos].ArrayBit = 0;
	memory->pageTable[pos].type = t;
	memory->pageTable[pos].size = 4;
	memory->pageTable[pos].ValidBit = 1;
	strcpy(memory->pageTable[pos].varName, name);
	int val = memory->nextMapLoc;
	memory->locationTable[val].location = memory->counter;
	memory->locationTable[val].idx = pos;
	memory->counter += 4;
	memory->usedSpace += 4;
	memory->nextPageLoc++;
	memory->nextMapLoc++;
	if (DEBUG)
		cout << "Variable Created, Name: " << varName << " Type: " << type << " Function: " << funcIdx << endl;
	pthread_mutex_unlock(&mlock);
}

// AssignVar for Int and MediumInt
void assignVar(int value, string varName, string type, int funcIdx)
{
	pthread_mutex_lock(&mlock);
	if (DEBUG)
		cout << "Assigning Variable: " << varName << " type: " << type << " funcIdx: " << funcIdx << " value: " << value << endl;
	int t = varType(type);
	if (t == 2)
	{
		if (!checkMediumInt(value))
		{
			cout << "Value out of Bounds for Medium Int!! {" << varName << "}" << endl;
			pthread_mutex_unlock(&mlock);
			return;
		}
	}
	char name[NAMESIZE];
	bzero(name, NAMESIZE);
	for (int i = 0; i < varName.size(); i++)
		name[i] = varName[i];
	for (int i = 0; i < memory->functionTable[funcIdx].nextLoc; i++)
	{
		if (!strcmp(memory->functionTable[funcIdx].symbolTableString[i], name))
		{
			int idx = memory->functionTable[funcIdx].symbolTableInt[i]; // position inside the page Table
			int original_t = memory->pageTable[idx].type;
			if (original_t >= t)
			{
				int *loc = (static_cast<int *>(memory->init) + memory->pageTable[idx].location);
				*loc = value;
			}
			else
			{
				cout << "Variable Type Error" << endl;
			}
			pthread_mutex_unlock(&mlock);
			return;
		}
	}
	pthread_mutex_unlock(&mlock);
	cout << "Variable Doesn't Exist!!" << endl;
	return;
}

// Get Integer
int getInt(string varName, string type, int funcIdx, int index)
{
	pthread_mutex_lock(&mlock);
	if (DEBUG)
		cout << "Getting Variable: " << varName << " type: " << type << " funcIdx: " << funcIdx << endl;
	int t = varType(type);
	char name[NAMESIZE];
	bzero(name, NAMESIZE);
	for (int i = 0; i < varName.size(); i++)
		name[i] = varName[i];
	for (int i = 0; i < memory->functionTable[funcIdx].nextLoc; i++)
	{
		if (!strcmp(memory->functionTable[funcIdx].symbolTableString[i], name))
		{
			int idx = memory->functionTable[funcIdx].symbolTableInt[i]; // position inside the page Table
			int original_t = memory->pageTable[idx].type;
			if (original_t >= t)
			{
				if (index < memory->pageTable[idx].ArrayBit || index == 0)
				{
					int *loc = (static_cast<int *>(memory->init) + memory->pageTable[idx].location + 4 * index);
					pthread_mutex_unlock(&mlock);
					return *loc;
				}
				else
				{
					cout << "Out of Bounds Error{" << varName << "}" << endl;
				}
			}
			else
			{
				cout << "Variable Type Error{" << varName << "}" << endl;
			}
			pthread_mutex_unlock(&mlock);
			return 0;
		}
	}
	pthread_mutex_unlock(&mlock);
	cout << "Variable Doesn't Exist!!{" << varName << "}" << endl;
	return 0;
}

// Assign value for bool variables
void assignVar(bool value, string varName, string type, int funcIdx)
{
	pthread_mutex_lock(&mlock);
	if (DEBUG)
		cout << "Assigning Variable: " << varName << " type: " << type << " funcIdx: " << funcIdx << " value: " << value << endl;
	int t = varType(type);
	char name[NAMESIZE];
	bzero(name, NAMESIZE);
	for (int i = 0; i < varName.size(); i++)
		name[i] = varName[i];
	for (int i = 0; i < memory->functionTable[funcIdx].nextLoc; i++)
	{
		if (!strcmp(memory->functionTable[funcIdx].symbolTableString[i], name))
		{
			int idx = memory->functionTable[funcIdx].symbolTableInt[i]; // position inside the page Table
			int original_t = memory->pageTable[idx].type;
			if (original_t >= t)
			{
				int *loc = (static_cast<int *>(memory->init) + memory->pageTable[idx].location);
				*loc = value;
			}
			else
			{
				cout << "Variable Type Error{" << varName << "}" << endl;
			}
			pthread_mutex_unlock(&mlock);
			return;
		}
	}
	pthread_mutex_unlock(&mlock);
	cout << "Variable Doesn't Exist!!{" << varName << "}" << endl;
	return;
}

bool getBool(string varName, string type, int funcIdx, int index)
{
	pthread_mutex_lock(&mlock);
	if (DEBUG)
		cout << "Getting Variable: " << varName << " type: " << type << " funcIdx: " << funcIdx << endl;
	int t = varType(type);
	char name[NAMESIZE];
	bzero(name, NAMESIZE);
	for (int i = 0; i < varName.size(); i++)
		name[i] = varName[i];
	for (int i = 0; i < memory->functionTable[funcIdx].nextLoc; i++)
	{
		if (!strcmp(memory->functionTable[funcIdx].symbolTableString[i], name))
		{
			int idx = memory->functionTable[funcIdx].symbolTableInt[i]; // position inside the page Table
			int original_t = memory->pageTable[idx].type;
			if (original_t >= t)
			{
				if (index < memory->pageTable[idx].ArrayBit || index == 0)
				{
					int *loc = (static_cast<int *>(memory->init) + memory->pageTable[idx].location + 4 * index);
					pthread_mutex_unlock(&mlock);
					return *loc;
				}
				else
				{
					cout << "Out of Bounds Error{" << varName << "}" << endl;
				}
			}
			else
			{
				cout << "Variable Type Error{" << varName << "}" << endl;
			}
			pthread_mutex_unlock(&mlock);
			return 0;
		}
	}
	cout << "Variable Doesn't Exist!!{" << varName << "}" << endl;
	pthread_mutex_unlock(&mlock);
	return 0;
}

// Assign Variable for Characters
void assignVar(char value, string varName, string type, int funcIdx)
{
	pthread_mutex_lock(&mlock);
	if (DEBUG)
		cout << "Assigning Variable: " << varName << " type: " << type << " funcIdx: " << funcIdx << " value: " << value << endl;
	int t = varType(type);
	char name[NAMESIZE];
	bzero(name, NAMESIZE);
	for (int i = 0; i < varName.size(); i++)
		name[i] = varName[i];
	for (int i = 0; i < memory->functionTable[funcIdx].nextLoc; i++)
	{
		if (!strcmp(memory->functionTable[funcIdx].symbolTableString[i], name))
		{
			int idx = memory->functionTable[funcIdx].symbolTableInt[i]; // position inside the page Table
			int original_t = memory->pageTable[idx].type;
			if (original_t >= t)
			{
				int *loc = (static_cast<int *>(memory->init) + memory->pageTable[idx].location);
				*loc = value;
			}
			else
			{
				cout << "Variable Type Error{" << varName << "}" << endl;
			}
			pthread_mutex_unlock(&mlock);
			return;
		}
	}
	cout << "Variable Doesn't Exist!!{" << varName << "}" << endl;
	pthread_mutex_unlock(&mlock);
	return;
}

// Function to get Character
char getChar(string varName, string type, int funcIdx, int index)
{
	pthread_mutex_lock(&mlock);
	if (DEBUG)
		cout << "Getting Variable: " << varName << " type: " << type << " funcIdx: " << funcIdx << endl;
	int t = varType(type);
	char name[NAMESIZE];
	bzero(name, NAMESIZE);
	for (int i = 0; i < varName.size(); i++)
		name[i] = varName[i];
	for (int i = 0; i < memory->functionTable[funcIdx].nextLoc; i++)
	{
		if (!strcmp(memory->functionTable[funcIdx].symbolTableString[i], name))
		{
			int idx = memory->functionTable[funcIdx].symbolTableInt[i]; // position inside the page Table
			int original_t = memory->pageTable[idx].type;
			if (original_t >= t)
			{
				if (index < memory->pageTable[idx].ArrayBit || index == 0)
				{
					int *loc = (static_cast<int *>(memory->init) + memory->pageTable[idx].location + 4 * index);
					pthread_mutex_unlock(&mlock);
					return *loc;
				}
				else
				{
					cout << "Out of Bounds Error{" << varName << "}" << endl;
				}
			}
			else
			{
				cout << "Variable Type Error{" << varName << "}" << endl;
			}
			pthread_mutex_unlock(&mlock);
			return 0;
		}
	}
	cout << "Variable Doesn't Exist!!{" << varName << "}" << endl;
	pthread_mutex_unlock(&mlock);
	return 0;
}

// Function to Create Array
void createArr(string varName, string type, int funcIdx, int count)
{
	pthread_mutex_lock(&mlock);
	if (DEBUG)
		cout << "Creating Array, Name: " << varName << " Type: " << type << " Function: " << funcIdx << endl;
	int t = varType(type);
	int size = 4 * count;

	if (checkVarName(varName, funcIdx))
	{
		cout << "A variable with the same name already exists!!" << endl;
		pthread_mutex_unlock(&mlock);
		return;
	}

	// search a proper index for the variable inside the pageTable
	int idx = searchIndex(size);
	// storing info inside the symbolTable of the function
	int loc = memory->functionTable[funcIdx].nextLoc;
	char name[NAMESIZE];
	bzero(name, NAMESIZE);
	for (int i = 0; i < varName.size(); i++)
		name[i] = varName[i];
	strcpy(memory->functionTable[funcIdx].symbolTableString[loc], name);
	memory->functionTable[funcIdx].symbolTableInt[loc] = idx;
	memory->functionTable[funcIdx].nextLoc++;

	int pos = memory->nextPageLoc;
	memory->pageTable[pos].location = memory->counter;
	memory->pageTable[pos].ArrayBit = count;
	memory->pageTable[pos].type = t;
	memory->pageTable[pos].ValidBit = 1;
	memory->pageTable[pos].size = size;
	strcpy(memory->pageTable[pos].varName, name);
	int val = memory->nextMapLoc;
	memory->locationTable[val].location = memory->counter;
	memory->locationTable[val].idx = pos;
	memory->nextMapLoc++;
	memory->counter += size;
	memory->usedSpace += size;
	memory->nextPageLoc++;
	if (DEBUG)
		cout << "Array Created, Name: " << varName << " Type: " << type << " Function: " << funcIdx << endl;
	pthread_mutex_unlock(&mlock);
}

// assignArr for Int and MediumInt arrays
void assignArr(int value, string varName, string type, int funcIdx, int index)
{
	pthread_mutex_lock(&mlock);
	if (DEBUG)
		cout << "Assigning Array: " << varName << " Index: " << index << " type: " << type << " funcIdx: " << funcIdx << " value: " << value << endl;
	int t = varType(type);
	if (t == 2)
	{
		if (!checkMediumInt(value))
		{
			cout << "Value out of Bounds for Medium Int!!" << endl;
			pthread_mutex_unlock(&mlock);
			return;
		}
	}
	char name[NAMESIZE];
	bzero(name, NAMESIZE);
	for (int i = 0; i < varName.size(); i++)
		name[i] = varName[i];
	for (int i = 0; i < memory->functionTable[funcIdx].nextLoc; i++)
	{
		if (!strcmp(memory->functionTable[funcIdx].symbolTableString[i], name))
		{
			int idx = memory->functionTable[funcIdx].symbolTableInt[i]; // position inside the page Table
			int original_t = memory->pageTable[idx].type;

			// Checking if the index is out of bounds or not
			if (memory->pageTable[idx].ArrayBit == 0)
			{
				cout << "Given Variable is not an array{" << varName << "}" << endl;
				pthread_mutex_unlock(&mlock);
				return;
			}
			if (memory->pageTable[idx].ArrayBit <= index)
			{
				cout << "Index out of bound{" << varName << "}" << endl;
				pthread_mutex_unlock(&mlock);
				return;
			}

			if (original_t >= t)
			{
				int *loc = (static_cast<int *>(memory->init) + memory->pageTable[idx].location + 4 * index);
				*loc = value;
			}
			else
			{
				cout << "Array Type Error{" << varName << "}" << endl;
			}
			pthread_mutex_unlock(&mlock);
			return;
		}
	}
	cout << "Array Doesn't Exist!!{" << varName << "}" << endl;
	pthread_mutex_unlock(&mlock);
	return;
}

// Get Integer array
void getIntArr(string varName, string type, int funcIdx, int *arr)
{
	pthread_mutex_lock(&mlock);
	if (DEBUG)
		cout << "Getting Array: " << varName << " type: " << type << " funcIdx: " << funcIdx << endl;
	int t = varType(type);
	char name[NAMESIZE];
	bzero(name, NAMESIZE);
	for (int i = 0; i < varName.size(); i++)
		name[i] = varName[i];
	for (int i = 0; i < memory->functionTable[funcIdx].nextLoc; i++)
	{
		if (!strcmp(memory->functionTable[funcIdx].symbolTableString[i], name))
		{
			int idx = memory->functionTable[funcIdx].symbolTableInt[i]; // position inside the page Table
			int original_t = memory->pageTable[idx].type;
			if (original_t >= t)
			{
				for (int i = 0; i < memory->pageTable[idx].ArrayBit; ++i)
				{
					int *loc = (static_cast<int *>(memory->init) + memory->pageTable[idx].location + 4 * i);
					arr[i] = *loc;
				}
				// cout << "Got Array: " << varName << " type: " << type << " funcIdx: " << funcIdx << endl;
				pthread_mutex_unlock(&mlock);
				return;
			}
			else
			{
				cout << "Array Type Error{" << varName << "}" << endl;
			}
			pthread_mutex_unlock(&mlock);
			return;
		}
	}
	cout << "Array Doesn't Exist!!{" << varName << "}" << endl;
	pthread_mutex_unlock(&mlock);
	return;
}

// assignArr for bool arrays
void assignArr(bool value, string varName, string type, int funcIdx, int index)
{
	pthread_mutex_lock(&mlock);
	if (DEBUG)
		cout << "Assigning Array: " << varName << " Index: " << index << " type: " << type << " funcIdx: " << funcIdx << " value: " << value << endl;
	int t = varType(type);
	char name[NAMESIZE];
	bzero(name, NAMESIZE);
	for (int i = 0; i < varName.size(); i++)
		name[i] = varName[i];
	for (int i = 0; i < memory->functionTable[funcIdx].nextLoc; i++)
	{
		if (!strcmp(memory->functionTable[funcIdx].symbolTableString[i], name))
		{
			int idx = memory->functionTable[funcIdx].symbolTableInt[i]; // position inside the page Table
			int original_t = memory->pageTable[idx].type;

			// Checking if the index is out of bounds or not
			if (memory->pageTable[idx].ArrayBit == 0)
			{
				cout << "Given Variable is not an array{" << varName << "}" << endl;
				pthread_mutex_unlock(&mlock);
				return;
			}
			if (memory->pageTable[idx].ArrayBit <= index)
			{
				cout << "Index out of bound" << endl;
				pthread_mutex_unlock(&mlock);
				return;
			}

			if (original_t >= t)
			{
				int *loc = (static_cast<int *>(memory->init) + memory->pageTable[idx].location + 4 * index);
				*loc = value;
			}
			else
			{
				cout << "Array Type Error{" << varName << "}" << endl;
			}
			pthread_mutex_unlock(&mlock);
			return;
		}
	}
	cout << "Array Doesn't Exist!!{" << varName << "}" << endl;
	pthread_mutex_unlock(&mlock);
	return;
}

// Get Boolean array
void getBoolArr(string varName, string type, int funcIdx, bool *arr)
{
	pthread_mutex_lock(&mlock);
	if (DEBUG)
		cout << "Getting Array: " << varName << " type: " << type << " funcIdx: " << funcIdx << endl;
	int t = varType(type);
	char name[NAMESIZE];
	bzero(name, NAMESIZE);
	for (int i = 0; i < varName.size(); i++)
		name[i] = varName[i];
	for (int i = 0; i < memory->functionTable[funcIdx].nextLoc; i++)
	{
		if (!strcmp(memory->functionTable[funcIdx].symbolTableString[i], name))
		{
			int idx = memory->functionTable[funcIdx].symbolTableInt[i]; // position inside the page Table
			int original_t = memory->pageTable[idx].type;
			if (original_t >= t)
			{
				for (int i = 0; i < memory->pageTable[idx].ArrayBit; ++i)
				{
					int *loc = (static_cast<int *>(memory->init) + memory->pageTable[idx].location + 4 * i);
					arr[i] = *loc;
				}
				pthread_mutex_unlock(&mlock);
				return;
			}
			else
			{
				cout << "Array Type Error{" << varName << "}" << endl;
			}
			pthread_mutex_unlock(&mlock);
			return;
		}
	}
	cout << "Array Doesn't Exist!!{" << varName << "}" << endl;
	pthread_mutex_unlock(&mlock);
	return;
}

// assignArr for char arrays
void assignArr(char value, string varName, string type, int funcIdx, int index)
{
	pthread_mutex_lock(&mlock);
	if (DEBUG)
		cout << "Assigning Array: " << varName << " Index: " << index << " type: " << type << " funcIdx: " << funcIdx << " value: " << value << endl;
	int t = varType(type);
	char name[NAMESIZE];
	bzero(name, NAMESIZE);
	for (int i = 0; i < varName.size(); i++)
		name[i] = varName[i];
	for (int i = 0; i < memory->functionTable[funcIdx].nextLoc; i++)
	{
		if (!strcmp(memory->functionTable[funcIdx].symbolTableString[i], name))
		{
			int idx = memory->functionTable[funcIdx].symbolTableInt[i]; // position inside the page Table
			int original_t = memory->pageTable[idx].type;

			// Checking if the index is out of bounds or not
			if (memory->pageTable[idx].ArrayBit == 0)
			{
				cout << "Given Variable is not an array{" << varName << "}" << endl;
				pthread_mutex_unlock(&mlock);
				return;
			}
			if (memory->pageTable[idx].ArrayBit <= index)
			{
				cout << "Index out of bound" << endl;
				pthread_mutex_unlock(&mlock);
				return;
			}

			if (original_t >= t)
			{
				int *loc = (static_cast<int *>(memory->init) + memory->pageTable[idx].location + 4 * index);
				*loc = value;
			}
			else
			{
				cout << "Array Type Error{" << varName << "}" << endl;
			}
			pthread_mutex_unlock(&mlock);
			return;
		}
	}
	cout << "Array Doesn't Exist!!{" << varName << "}" << endl;
	pthread_mutex_unlock(&mlock);
	return;
}

// Get Character array
void getCharArr(string varName, string type, int funcIdx, char *arr)
{
	pthread_mutex_lock(&mlock);
	if (DEBUG)
		cout << "Getting Array: " << varName << " type: " << type << " funcIdx: " << funcIdx << endl;
	int t = varType(type);
	char name[NAMESIZE];
	bzero(name, NAMESIZE);
	for (int i = 0; i < varName.size(); i++)
		name[i] = varName[i];
	for (int i = 0; i < memory->functionTable[funcIdx].nextLoc; i++)
	{
		if (!strcmp(memory->functionTable[funcIdx].symbolTableString[i], name))
		{
			int idx = memory->functionTable[funcIdx].symbolTableInt[i]; // position inside the page Table
			int original_t = memory->pageTable[idx].type;
			if (original_t >= t)
			{
				for (int i = 0; i < memory->pageTable[idx].ArrayBit; ++i)
				{
					int *loc = (static_cast<int *>(memory->init) + memory->pageTable[idx].location + 4 * i);
					arr[i] = *loc;
				}
			}
			else
			{
				cout << "Array Type Error{" << varName << "}" << endl;
			}
			pthread_mutex_unlock(&mlock);
			return;
		}
	}
	cout << "Array Doesn't Exist!!{" << varName << "}" << endl;
	pthread_mutex_unlock(&mlock);
	return;
}

// Free a Variable/Array
void freeElem(string varName, string type, int funcIdx)
{
	if (!checkVarName(varName, funcIdx))
	{
		if (DEBUG)
			cout << "Variable with the given name doesn't exist!!{" << varName << "}" << endl;
		pthread_mutex_unlock(&mlock);
		return;
	}
	int t = varType(type);
	char name[NAMESIZE];
	bzero(name, NAMESIZE);
	for (int i = 0; i < varName.size(); i++)
		name[i] = varName[i];
	for (int i = 0; i < memory->functionTable[funcIdx].nextLoc; i++)
	{
		if (!strcmp(memory->functionTable[funcIdx].symbolTableString[i], name))
		{
			int idx = memory->functionTable[funcIdx].symbolTableInt[i]; // position inside the page Table
			memory->pageTable[idx].ValidBit = 0;
			int sz = 4;
			if (memory->pageTable[idx].ArrayBit)
			{
				sz = memory->pageTable[idx].ArrayBit * 4;
			}
			// memory->usedSpace -= sz;
			if (DEBUG)
				cout << "Memory of variable: " << varName << " of size: " << sz << " freed!!" << endl;
			pthread_mutex_unlock(&mlock);
			return;
		}
	}
	if (DEBUG)
		cout << "Free Operation unsuccessful!!" << endl;
	pthread_mutex_unlock(&mlock);
	return;
}
