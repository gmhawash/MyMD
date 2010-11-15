// P4.cpp : main project file.

#include "stdafx.h"
#include "thread.h"
#include<iostream>
#include<vector>
#include<string>
#include "population.h"
#include "qurandom.h"


using namespace System;
using namespace std;


UINT64 gCounter=0;

 #define N_RUNS 1


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

  for (int i=0; i<N_RUNS; i++) {
    CPopulation pop;
    pop.DoRun(i);
  }
  
  //cmb.SolveWithGA();
#endif  
    return 0;
}


