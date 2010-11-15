// PrintSolution.cpp : main project file.

#include "stdafx.h"
#include<iostream>
#include <string>
#include <windows.h>

using namespace System;
using namespace std;

int main(array<System::String ^> ^args)
{
  
  UINT64 xa[] = {410527040636, 727505075074, 723344325506, 376167302269, 372006552701, 688984587139};
  
  for (int j=0; j<sizeof(xa)/sizeof(xa[0]); j++) {
    UINT64 x = xa[j];
    x = 377241044092;
    cout << "\nV1 = ";
    for (int i=0; i<40; i++) {
      if ((x >> i) & 1)
        cout << i+1 << ", " ; 
    }


    cout << "\nV2 = ";
    for (int i=0; i<40; i++) {
      if (!((x >> i) & 1))
        cout << i+1 << ", " ; 
    }
  }
    
  Console::WriteLine(L"Hello World");
  return 0;
}
