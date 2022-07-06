/*
Hands-on experience for creating better memory management systems
OS Assignment - 5
    Group - 43
    - Tushar Kishor Bokade 19CS30011
    - Aditya Vallakatla 19CS30051
*/

#include "memlab.h"

#define SZ 50000

void func1(string a, string b, string type, int funcIdx)
{
    int idx = regFunc("func1");
    createVar("i", "int", idx);
    assignVar(0, "i", "int", idx);
    cout << "Value of I: " << getInt("i", "int", idx) << endl;
    createArr("arr", "int", idx, SZ);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        assignArr(rand(), "arr", "int", idx, getInt("i", "int", idx));
    }
    int arr[SZ];
    assignVar(0, "i", "int", idx);
    getIntArr("arr", "int", idx, arr);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        cout << "arr[" << getInt("i", "int", idx) << "] = " << arr[getInt("i", "int", idx)] << endl;
    }

    freeElem("arr", "int", idx);
    exitFunc(idx);
    return;
}

void func2(string a, string b, string type, int funcIdx)
{
    int idx = regFunc("func2");
    createVar("i", "int", idx);
    assignVar(0, "i", "int", idx);
    cout << "Value of I: " << getInt("i", "int", idx) << endl;
    createArr("arr", "char", idx, SZ);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        assignArr((char)(rand() % 26 + 97), "arr", "char", idx, getInt("i", "int", idx));
    }
    char arr[SZ];
    assignVar(0, "i", "int", idx);
    getCharArr("arr", "char", idx, arr);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        cout << "arr[" << getInt("i", "int", idx) << "] = " << arr[getInt("i", "int", idx)] << endl;
    }

    freeElem("arr", "char", idx);

    exitFunc(idx);
    return;
}

void func3(string a, string b, string type, int funcIdx)
{
    int idx = regFunc("func3");
    createVar("i", "int", idx);
    assignVar(0, "i", "int", idx);
    cout << "Value of I: " << getInt("i", "int", idx) << endl;
    createArr("arr", "bool", idx, SZ);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        assignArr(rand() % 2, "arr", "bool", idx, getInt("i", "int", idx));
    }
    bool arr[SZ];
    assignVar(0, "i", "int", idx);
    getBoolArr("arr", "bool", idx, arr);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        cout << "arr[" << getInt("i", "int", idx) << "] = " << arr[getInt("i", "int", idx)] << endl;
    }

    freeElem("arr", "bool", idx);
    exitFunc(idx);
    return;
}

void func4(string a, string b, string type, int funcIdx)
{
    int idx = regFunc("func4");
    createVar("i", "int", idx);
    assignVar(0, "i", "int", idx);
    cout << "Value of I: " << getInt("i", "int", idx) << endl;
    createArr("arr", "medium int", idx, SZ);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        assignArr(rand() % (1 << 22), "arr", "medium int", idx, getInt("i", "int", idx));
    }
    int arr[SZ];
    assignVar(0, "i", "int", idx);
    getIntArr("arr", "medium int", idx, arr);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        cout << "arr[" << getInt("i", "int", idx) << "] = " << arr[getInt("i", "int", idx)] << endl;
    }

    freeElem("arr", "medium int", idx);
    exitFunc(idx);
    return;
}

void func5(string a, string b, string type, int funcIdx)
{
    int idx = regFunc("func5");
    createVar("i", "int", idx);
    assignVar(0, "i", "int", idx);
    cout << "Value of I: " << getInt("i", "int", idx) << endl;
    createArr("arr", "int", idx, SZ);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        assignArr(rand(), "arr", "int", idx, getInt("i", "int", idx));
    }
    int arr[SZ];
    assignVar(0, "i", "int", idx);
    getIntArr("arr", "int", idx, arr);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        cout << "arr[" << getInt("i", "int", idx) << "] = " << arr[getInt("i", "int", idx)] << endl;
    }

    freeElem("arr", "int", idx);
    exitFunc(idx);
    return;
}

void func6(string a, string b, string type, int funcIdx)
{
    int idx = regFunc("func6");
    createVar("i", "int", idx);
    assignVar(0, "i", "int", idx);
    cout << "Value of I: " << getInt("i", "int", idx) << endl;
    createArr("arr", "char", idx, SZ);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        assignArr((char)(rand() % 26 + 97), "arr", "char", idx, getInt("i", "int", idx));
    }
    char arr[SZ];
    assignVar(0, "i", "int", idx);
    getCharArr("arr", "char", idx, arr);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        cout << "arr[" << getInt("i", "int", idx) << "] = " << arr[getInt("i", "int", idx)] << endl;
    }

    freeElem("arr", "char", idx);

    exitFunc(idx);
    return;
}

void func7(string a, string b, string type, int funcIdx)
{
    int idx = regFunc("func7");
    createVar("i", "int", idx);
    assignVar(0, "i", "int", idx);
    cout << "Value of I: " << getInt("i", "int", idx) << endl;
    createArr("arr", "bool", idx, SZ);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        assignArr(rand() % 2, "arr", "bool", idx, getInt("i", "int", idx));
    }
    bool arr[SZ];
    assignVar(0, "i", "int", idx);
    getBoolArr("arr", "bool", idx, arr);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        cout << "arr[" << getInt("i", "int", idx) << "] = " << arr[getInt("i", "int", idx)] << endl;
    }

    freeElem("arr", "bool", idx);
    exitFunc(idx);
    return;
}

void func8(string a, string b, string type, int funcIdx)
{
    int idx = regFunc("func4");
    createVar("i", "int", idx);
    assignVar(0, "i", "int", idx);
    cout << "Value of I: " << getInt("i", "int", idx) << endl;
    createArr("arr", "medium int", idx, SZ);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        assignArr(rand() % (1 << 22), "arr", "medium int", idx, getInt("i", "int", idx));
    }
    int arr[SZ];
    assignVar(0, "i", "int", idx);
    getIntArr("arr", "medium int", idx, arr);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        cout << "arr[" << getInt("i", "int", idx) << "] = " << arr[getInt("i", "int", idx)] << endl;
    }

    freeElem("arr", "medium int", idx);
    exitFunc(idx);
    return;
}

void func9(string a, string b, string type, int funcIdx)
{
    int idx = regFunc("func9");
    createVar("i", "int", idx);
    assignVar(0, "i", "int", idx);
    cout << "Value of I: " << getInt("i", "int", idx) << endl;
    createArr("arr", "int", idx, SZ);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        assignArr(rand(), "arr", "int", idx, getInt("i", "int", idx));
    }
    int arr[SZ];
    assignVar(0, "i", "int", idx);
    getIntArr("arr", "int", idx, arr);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        cout << "arr[" << getInt("i", "int", idx) << "] = " << arr[getInt("i", "int", idx)] << endl;
    }

    freeElem("arr", "int", idx);
    exitFunc(idx);
    return;
}

void func10(string a, string b, string type, int funcIdx)
{
    int idx = regFunc("func10");
    createVar("i", "int", idx);
    assignVar(0, "i", "int", idx);
    cout << "Value of I: " << getInt("i", "int", idx) << endl;
    createArr("arr", "char", idx, SZ);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        assignArr((char)(rand() % 26 + 97), "arr", "char", idx, getInt("i", "int", idx));
    }
    char arr[SZ];
    assignVar(0, "i", "int", idx);
    getCharArr("arr", "char", idx, arr);
    for (; getInt("i", "int", idx) < SZ; assignVar(getInt("i", "int", idx) + 1, "i", "int", idx))
    {
        cout << "arr[" << getInt("i", "int", idx) << "] = " << arr[getInt("i", "int", idx)] << endl;
    }

    freeElem("arr", "char", idx);

    exitFunc(idx);
    return;
}

int main()
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
    auto start = high_resolution_clock::now();
    int idx = regFunc("main");

    createVar("x1", "int", idx);
    assignVar(20, "x1", "int", idx);
    cout << "Value of x1: " << getInt("x1", "int", idx) << endl;
    createVar("x2", "int", idx);
    assignVar(10, "x2", "int", idx);
    cout << "Value of x2: " << getInt("x2", "int", idx) << endl;
    func1("x1", "x2", "int", idx);

    createVar("a1", "char", idx);
    assignVar('p', "a1", "char", idx);
    cout << "Value of a1: " << getChar("a1", "char", idx) << endl;
    createVar("a2", "char", idx);
    assignVar('c', "a2", "char", idx);
    cout << "Value of a2: " << getChar("a2", "char", idx) << endl;
    func2("a1", "a2", "int", idx);

    createVar("b1", "bool", idx);
    assignVar(true, "b1", "bool", idx);
    cout << "Value of b1: " << getBool("b1", "bool", idx) << endl;
    createVar("b2", "bool", idx);
    assignVar(false, "b2", "bool", idx);
    cout << "Value of b2: " << getBool("b2", "bool", idx) << endl;
    func3("b1", "b2", "bool", idx);

    createVar("m1", "medium int", idx);
    assignVar(1, "m1", "medium int", idx);
    cout << "Value of m1: " << getInt("m1", "medium int", idx) << endl;
    createVar("m2", "medium int", idx);
    assignVar(0, "m2", "medium int", idx);
    cout << "Value of m2: " << getChar("m2", "medium int", idx) << endl;
    func4("m1", "m2", "medium int", idx);

    createVar("p1", "int", idx);
    assignVar(20, "p1", "int", idx);
    cout << "Value of p1: " << getInt("p1", "int", idx) << endl;
    createVar("p2", "int", idx);
    assignVar(10, "p2", "int", idx);
    cout << "Value of p2: " << getInt("p2", "int", idx) << endl;
    func5("p1", "p2", "int", idx);

    createVar("q1", "char", idx);
    assignVar('p', "q1", "char", idx);
    cout << "Value of q1: " << getChar("q1", "char", idx) << endl;
    createVar("q2", "char", idx);
    assignVar('c', "q2", "char", idx);
    cout << "Value of B: " << getChar("q2", "char", idx) << endl;
    func6("q1", "q2", "int", idx);

    createVar("c1", "bool", idx);
    assignVar(true, "c1", "bool", idx);
    cout << "Value of c1: " << getBool("c1", "bool", idx) << endl;
    createVar("c2", "bool", idx);
    assignVar(false, "c2", "bool", idx);
    cout << "Value of c2: " << getBool("c2", "bool", idx) << endl;
    func7("c1", "c2", "bool", idx);

    createVar("d1", "medium int", idx);
    assignVar(1, "d1", "medium int", idx);
    cout << "Value of d1: " << getInt("d1", "medium int", idx) << endl;
    createVar("d2", "medium int", idx);
    assignVar(0, "d2", "medium int", idx);
    cout << "Value of d2: " << getChar("d2", "medium int", idx) << endl;
    func8("d1", "d2", "medium int", idx);

    createVar("t1", "int", idx);
    assignVar(20, "t1", "int", idx);
    cout << "Value of t1: " << getInt("t1", "int", idx) << endl;
    createVar("t2", "int", idx);
    assignVar(10, "t2", "int", idx);
    cout << "Value of t2: " << getInt("t2", "int", idx) << endl;
    func9("t1", "t2", "int", idx);

    createVar("r1", "char", idx);
    assignVar('p', "r1", "char", idx);
    cout << "Value of r1: " << getChar("r1", "char", idx) << endl;
    createVar("r2", "char", idx);
    assignVar('c', "r2", "char", idx);
    cout << "Value of r2: " << getChar("r2", "char", idx) << endl;
    func10("r1", "r2", "int", idx);

    exitFunc(idx);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Demo EXITED - Time taken : " << duration.count() * 1.00 / 1000000.0 << " sec" << endl;
    return 0;
}
