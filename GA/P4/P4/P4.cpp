// P4.cpp : main project file.

#include "stdafx.h"
#include "permutation.h"

using namespace System;

int main(array<System::String ^> ^args)
{
  CPermutation perm;
  
    //perm.SolveWithPermutation();
    perm.SolveWithGA();
    Console::WriteLine(L"Hello World");
    return 0;
}


