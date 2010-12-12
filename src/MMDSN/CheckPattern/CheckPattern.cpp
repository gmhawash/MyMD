// CheckPattern.cpp : main project file.

#include "stdafx.h"
#include "config.h"

using namespace System;
using namespace AlgoBucket;
using namespace System::Collections::Generic;

int main(array<System::String ^> ^args)
{
    Stedman::Input inp(NBITS);
    Stedman::Output outp(NBITS);
    
    List<ULONGLONG>^ ol, ^il;
    
    int count=0;
    while ( (ol = outp.Next()) != nullptr) {
      il = inp.Next();
      cout << count++ << "\r";
      
      Synthesis syn(NBITS);
      for(int i=0; i < il->Count; i++) {
        syn.Process(il[i], ol[i]);    
      }
      
      // Check propogation of patterns.
      for(int i=0; i < il->Count; i++) {
        //ULONGLONG out = syn.Propogate(il[i], ol[i]);
        //if (out != il[i]) {
        //  cout << "Failed " << "\n";
         // for(int j=0; j < il->Count; j++) {
         //   cout << "[" << il[j] << "," << ol[j] << "]\n";
         // }
         // break;
         // }
      }
    }
    
    
    Console::WriteLine(L"Hello World");
    return 0;
}
