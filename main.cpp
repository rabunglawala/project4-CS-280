/*
 * main.cpp
 *
 * CS280
 * Spring 2020
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>

#include "lex.h"
#include "parse.h"

map<string,Value>symMap;

using namespace std;
ifstream fin;
istream *in=&cin;

int main(int argc,char *argv[])
{
    if(argc>2)
    {
        cout<<"ONLY ONE FILE NAME ALLOWED"<<endl;
        return -1;
    }
    if(argc > 1)
    {
        fin.open(argv[1]);
        if(fin)
        {
            in = &fin;
        }
        if(!fin)
        {
            cout<<"CANNOT OPEN "<<argv[1]<<endl;
            return-1;
        }
        in = &fin;
    }
    int linenum=0;
    
    
     Pt *prog = Prog(*in, linenum); 
   
   
    if( prog == 0 )
    {
		return 0;
    }
    prog->Eval(symMap);
    
   
return 0;
}
