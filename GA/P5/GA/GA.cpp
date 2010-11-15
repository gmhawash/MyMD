// P4.cpp : main project file.

#include "stdafx.h"
#include "thread.h"
#include<iostream>
#include<vector>
#include<string>
#include "stdCombination.h"
#include "Combination.h"
#include "qurandom.h"

using namespace System;
using namespace std;
using namespace stdcomb;

#define R 15
#define N 30
void PrintList(int* begin, int* end);
void recur(char str[N], int index, int len, int oi);
void doit();

int list[R];
char ca[]="0123456789abcdefghijklmnopqrstuvwkyz@#$%";
int ica[N];

UINT64 gCounter=0;


int main(array<System::String ^> ^args)
{
  //char ca[]="0123456789abcdefghijklmnopqrstuvwkyz@#$%";
  //char cb[]="01234567890123456789";
  // 
  //CPrintThread pth;
  //pth.Start();
  //
  //gCounter=0;
  //
  //do
  //{
  //  gCounter++;
  //  //COUT<<cb<<endl;
  //}
  //while(next_combination(ca,ca+40,cb,cb+20));
  //COUT<<"Complete!"<<endl;

#define GA
#ifdef REAL
  CCombination cmb;
  cmb.CalcWithCombinations();
#else 
gCounter = 0;
  CCombination cmb;
  cmb.SolveWithGA();
#endif  
    return 0;
}


