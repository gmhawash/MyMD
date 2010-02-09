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
int main(array<System::String ^> ^args)
{
  marshal_context ctx;

    for (int nBits=3; nBits<5; nBits++) {
      Miller::Input inp(nBits);
      Miller::Output outp(nBits);
      
      // Create directory if it does not exist
      String^ sDir = "..\\" + nBits.ToString() + "-bits-src";
      if ( !Directory::Exists(sDir) )
        Directory::CreateDirectory(sDir);
        
      for (int nCount=0; nCount < 10000; nCount++) {
        String^ s = sDir + "\\seq-" + nCount.ToString() +  ".txt" ;
        ofstream fs(ctx.marshal_as<const char*>(s) , ios::out);
        cout << ctx.marshal_as<const char*>(s) << "\r";
                
        List<ULONGLONG>^ il = inp.Next();
        List<ULONGLONG>^ ol = outp.Next();
        if (il == nullptr)
          break;
        
        for (int i=0; i<il->Count; i++)
          fs << il[i] << " " << ol[i] << "\n";
      }
    }
    
  
		//Steadman s(4);
		//List<ULONGLONG>^x;
		//
		//int count=0;
		//while ( (x = s.Next()) != nullptr) {
		//  cout << "count: " << count ++ << "\n";
		//  
		//  for each (int i in x)
  //      Console::WriteLine(i);
  //  }
    return 0;
}

