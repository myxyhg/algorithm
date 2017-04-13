#include <stdio.h>
#include <string>
#include <stdlib.h>
#include <iostream>

using namespace std;

int main()
{
    string hex = "54545F73";
    int len = hex.length();
    string str;
    for(int i = 0;i < len;i += 2)
    {
        string byte = hex.substr(i,2);
        char chr = (char)(int)strtol(byte.c_str(),NULL,16);
        str.push_back(chr);
    }
    cout << str << endl;
    return 0;
}
