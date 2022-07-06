/*
Hands-on experience for creating better memory management systems
OS Assignment - 5
	Group - 43
	- Tushar Kishor Bokade 19CS30011
	- Aditya Vallakatla 19CS30051
*/

#include "memlab.h"

void fibonacci(string k, int funcIdx1, string arr, int funcIdx2)
{
	int idx = regFunc("fibonacci");
	createVar("i", "int", idx);
	assignVar(2, "i", "int", idx);

	if (getInt(k, "int", funcIdx1) > 0)
	{
		assignArr(1, arr, "int", funcIdx2, 0);
	}
	if (getInt(k, "int", funcIdx1) > 1)
	{
		assignArr(1, arr, "int", funcIdx2, 1);
	}
	for (; getInt("i", "int", idx) < getInt(k, "int", funcIdx1); assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
	{
		assignArr(getInt(arr, "int", funcIdx2, getInt("i", "int", idx) - 2) + getInt(arr, "int", funcIdx2, getInt("i", "int", idx) - 1), arr, "int", funcIdx2, getInt("i", "int", idx));
	}
	exitFunc(idx);
	return;
}

int fibonacciProduct(string k, int funcIdx)
{
	int idx = regFunc("fibonacciProduct");
	createArr("fibo", "int", idx, getInt(k, "int", funcIdx));
	fibonacci(k, funcIdx, "fibo", idx);

	createVar("i", "int", idx);
	assignVar(0, "i", "int", idx);

	createVar("Prod", "int", idx);
	assignVar(1, "Prod", "int", idx);

	for (; getInt("i", "int", idx) < getInt(k, "int", funcIdx); assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
	{
		assignVar(getInt("Prod", "int", idx) * getInt("fibo", "int", idx, getInt("i", "int", idx)), "Prod", "int", idx);
	}
	exitFunc(idx);
	return getInt("Prod", "int", idx);
}

int main(int argc, char **argv)
{
	int size = 250 * 1000000;
	if (!createMem(size))
	{
		perror("Memory Create Error");
		exit(0);
	}
	else
	{
		cout << "Memory Allocated" << endl;
	}
	int idx = regFunc("main");
	createVar("k", "int", idx);
	assignVar(atoi(argv[1]), "k", "int", idx);

	createVar("Prod", "int", idx);
	assignVar(fibonacciProduct("k", idx), "Prod", "int", idx);

	cout << "Product: " << getInt("Prod", "int", idx) << endl;

	exitFunc(idx);
	return 0;
}
