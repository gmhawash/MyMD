// Inputs.cpp : main project file.

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include "config.h"
#include <msclr\marshal.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace AlgoBucket;
using namespace std;
using namespace System::IO;

using namespace msclr::interop;

public delegate List<ULONGLONG>^ Next();
void GenerateAlgoSeq( Permutation^ inp, Next^ fnIn, String^ sDir);
void GenerateRandomSeq();

#define NOURADDIN
//#define MILLER
#define STEDMAN

int main(array<System::String ^> ^args)
{

  for (int nBits=4; nBits<20; nBits++) {
    // Create directory if it does not exist
    String^ sDir = "..\\" + nBits.ToString() + "-bits-src";
    if ( !Directory::Exists(sDir) )
      Directory::CreateDirectory(sDir);
  
#ifdef NOURADDIN
  Nouraddin::Input inp(nBits);
  GenerateAlgoSeq(%inp, gcnew Next(%inp, &Nouraddin::Input::Next), sDir);
#elif  defined(MILLER)
  Miller::Input inp(nBits);
  GenerateAlgoSeq(%inp, gcnew Next(%inp, &Miller::Input::Next), sDir);
#elif  defined(STEDMAN)
  Stedman::Input inp(nBits);
  GenerateAlgoSeq(%inp, gcnew Next(%inp, &Stedman::Input::Next), sDir);
#endif
  }
  

}

void GenerateRandomSeq()
{
    marshal_context ctx;

    for (int nBits=3; nBits<5; nBits++) {
      Permutation outp(nBits);
      
      // Create directory if it does not exist
      String^ sDir = "..\\" + nBits.ToString() + "-bits-src";
      if ( !Directory::Exists(sDir) )
        Directory::CreateDirectory(sDir);

      int nCount = 0;

      for (int nCount=0; nCount < 10000; nCount++) {
        String^ s = sDir + "\\Random" + "-" + nCount.ToString() +  ".txt" ;
        ofstream fs(ctx.marshal_as<const char*>(s) , ios::out);
        cout << ctx.marshal_as<const char*>(s) << "\r";
                
        List<ULONGLONG>^ ol = outp.Random();
        
        for (int i=0; i<ol->Count; i++)
          fs << ol[i] << "\n";
        
        nCount++;
      }
    }
}

void GenerateAlgoSeq(Permutation^ inp, Next^ fnNext, String^ sDir)
{
    marshal_context ctx;

    String^ s = sDir + "\\" + inp->Name + ".txt" ;
    ofstream fs(ctx.marshal_as<const char*>(s) , ios::out);
    cout << ctx.marshal_as<const char*>(s) << "\n";

    List<ULONGLONG>^ il;
    int nCount = 0;
    while( (il = fnNext()) != nullptr) {
      cout << nCount << "\r";
      fs << nCount << " ";  
      for (int i=0; i<il->Count; i++)
        fs << il[i] << " ";
        
      fs << "\n";
      nCount++;
    }
    fs.close();
}

void GenerateStedmanSeq()
{
    marshal_context ctx;

    for (int nBits=3; nBits<5; nBits++) {
      Stedman::Input inp(nBits);
      //Miller::Input inp(nBits);
      //Miller::Output outp(nBits);
      
      // Create directory if it does not exist
      String^ sDir = "..\\" + nBits.ToString() + "-bits-src";
      if ( !Directory::Exists(sDir) )
        Directory::CreateDirectory(sDir);

      List<ULONGLONG>^ il;
      int nCount = 0;
      while( (il = inp.Next()) != nullptr) {
      //for (int nCount=0; nCount < 10000; nCount++) {
        String^ s = sDir + "\\" + inp.Name + "-" + nCount.ToString() +  ".txt" ;
        ofstream fs(ctx.marshal_as<const char*>(s) , ios::out);
        cout << ctx.marshal_as<const char*>(s) << "\r";
                
        for (int i=0; i<il->Count; i++)
          fs << il[i] << "\n";
        
        nCount++;
      }
    }
    
}

