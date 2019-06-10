#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
using namespace std;

void testRun()
{
    vector<string> msg {"Hello", "C++Header", "World", "from", "VS Code!"};

    for (const string& word : msg)
    {
        cout << word << " ";
    }
    cout << endl;
    getch();
}