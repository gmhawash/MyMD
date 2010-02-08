// crt_malloc.c
// This program allocates memory with
// malloc, then frees the memory with free.
 
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <windows.h>
#include <msclr\marshal.h>
#include "config.h"

using namespace AlgoBucket;
using namespace std;
using namespace msclr::interop;
using namespace System;
using namespace System::IO;

ULONGLONG InitInput(ULONGLONG nBits);
ULONGLONG NextInput();
ULONGLONG InitOutput(ULONGLONG currentTerm, int option=0);
void ProcessTerm(ULONGLONG inTerm, ULONGLONG outTerm);
ULONGLONG NextOutput();

ULONGLONG nBits = NBITS;
ULONGLONG p[BUF_SIZE];
ULONGLONG c[BUF_SIZE];
ULONGLONG m[BUF_SIZE];
ULONGLONG nGate=0;

#define PROFILE 1

#ifdef PROFILE
  #define COUT(x) x
#else
  #define COUT(x) 
#endif
void DoClrTest();
int main()
{
  DoClrTest();
	ULONGLONG t1, t2, freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&t1);

  marshal_context ctx;
  
  Nouraddin::Input inp(NBITS);
  Nouraddin::Output outp(NBITS);
  int nCount = Math::Min(inp.Count(), (ULONGLONG)100);
  List<ULONGLONG>^ ol, ^il;

  COUT(
    String^ sDir = nBits.ToString() + "-bits";
    if ( !Directory::Exists(sDir) )
      Directory::CreateDirectory(sDir);
    
    String^ s = sDir + "\\Summary-" + nBits.ToString() +  ".csv" ;
    ofstream fs(ctx.marshal_as<const char*>(s) , ios::out)
    );
    
    while ( nCount-- && (il = inp.Next()) != nullptr) {
    
  	  COUT(
	      String^ s = sDir + "\\Sequence-" + nBits.ToString() + "-" + nCount.ToString() +  ".csv" ;
		    ofstream f(ctx.marshal_as<const char*>(s) , ios::out)
		    );
    
      cout << nCount << "\n";
      ol = outp.Next();
    	QueryPerformanceCounter((LARGE_INTEGER*)&t1);
      Synthesis syn(NBITS);
      for(int i=0; i < il->Count; i++) {
        syn.Process(il[i], ol[i]);    
      }
    	QueryPerformanceCounter((LARGE_INTEGER*)&t2);
      
      // Check propogation of patterns.
      for(int i=0; i < il->Count; i++) {
        ULONGLONG out = syn.Propogate(il[i], ol[i]);
   			COUT( f << 	il[i] << "," << ol[i]<< "\n");
        
        if (out != il[i]) {
          f << "Failed " << "\n";
          for(int j=0; j < il->Count; j++) {
            f << "[" << il[j] << "," << ol[j] << "]\n";
          }
          break;
        }
      }
      
	    COUT(
		    f << "\nControl,," ;
		    for (ULONGLONG i=0; i<syn.GateCount(); i++) {
			    f << syn.c[i] << "," ;
		    }
		    f << "\nMask,," ;
		    for (ULONGLONG i=0; i<syn.GateCount(); i++) {
			    f <<  syn.m[i] << ",";
		    }
		    

	      f << "\nTotal Gates: " << syn.GateCount()  << "\n";
	      f << "\nTotal Ticks: " << (t2 - t1)  << "\n";
	      f << "Frequency: "   << freq << "\n";
	      f << "Total Time: "  << 1000000*(t2 - t1) / freq << "\n";

	      fs << "\n\nSequence: " << nCount ; 
	      fs << "\nTotal Gates: " << syn.GateCount()  ;
	      fs << "\nTotal Ticks: " << (t2 - t1) ;
	      fs << "\nFrequency: "   << freq;
	      fs << "\nTotal Time: "  << 1000000*(t2 - t1) / freq;
		    
	      f.flush();
	      f.close();
	    )		
    }
    fs.close();
}

void DoClrTest()
{
  List<int> m;
  for(int i=0; i< 20; i++)
    m.Add(i);
  IEnumerable<int>^ _list = %m;
  
}
