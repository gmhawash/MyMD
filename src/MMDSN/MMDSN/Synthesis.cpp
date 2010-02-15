// crt_malloc.c
// This program allocates memory with
// malloc, then frees the memory with free.
 
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <windows.h>
#include <msclr\marshal.h>
#include "config.h"
#include <vector>
#include <algorithm>
#include <functional>

using namespace AlgoBucket;
using namespace std;
using namespace msclr::interop;
using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;

ULONGLONG InitInput(ULONGLONG nBits);
ULONGLONG NextInput();
ULONGLONG InitOutput(ULONGLONG currentTerm, int option=0);
void ProcessTerm(ULONGLONG inTerm, ULONGLONG outTerm);
ULONGLONG NextOutput();

ULONGLONG nBits = NBITS;
ULONGLONG nGate=0;


#define COUT_SUMMARY(x) x
#define COUT_SUMMARY2(x) 

void OneInputPerFile();
void NoFile();
void AllInputsPerFile();

public delegate List<ULONGLONG>^ Next();


//void Synthesize( Permutation^ inp, Next^ fnIn, FileSrc::Output^ outp, Next^ fnOut);
void Synthesize( FileSrc::Input^ inp, Next^ fnIn, FileSrc::Input^ outp, Next^ fnOut);


//#define NOURADDIN 
//#define MILLER 
#define FILE_FILE

int main(array<System::String ^> ^args)
{


#ifdef NOURADDIN
  Nouraddin::Input inp(NBITS);
  FileSrc::Output outp(NBITS, "seq*", 50);
  Synthesize(%inp, gcnew Next(%inp, &Nouraddin::Input::Next),  %outp, gcnew Next(%outp, &FileSrc::Output::Next));
#elif  defined(MILLER)
  Miller::Input inp(NBITS);
  FileSrc::Output outp(NBITS, "seq*", 50);
  Synthesize(%inp, gcnew Next(%inp, &Miller::Input::Next),  %outp, gcnew Next(%outp, &FileSrc::Output::Next));
#elif  defined(FILE_FILE)
  if (args->Length < 2) {
    Console::WriteLine("Usage: MMDSN filespec1 \n");
    return -1;
  }

  FileSrc::Input inp(NBITS, args[0] , MAXINT);
  FileSrc::Input outp(NBITS, args[1], 50);
  Synthesize(%inp, gcnew Next(%inp, &FileSrc::Input::Next),  %outp, gcnew Next(%outp, &FileSrc::Input::Next));
#endif
  
 // AllInputsPerFile();
}


  
  
/// void Synthesize(Next^ inp, Next^ outp)/// 
///
/// Inputs:
///
/// Outputs:
///
void Synthesize(FileSrc::Input^ inp, Next^ NextIn, FileSrc::Input^ outp, Next^ NextOut)
{
	ULONGLONG t1, t2, freq;
	ULONGLONG t3;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&t1);

  marshal_context ctx;

  List<ULONGLONG>^ il;
  List<ULONGLONG>^ ol;

    COUT_SUMMARY(
      // Make sure directory exists...
      String^ sDir = nBits.ToString() + "-bits";
      if ( !Directory::Exists(sDir) )
        Directory::CreateDirectory(sDir);
      
      // Create summary file
      String^ s = sDir + "\\A-Summary-" + DateTime::Now.ToBinary() + "-" + nBits.ToString() +  ".csv" ;
      ofstream fss(ctx.marshal_as<const char*>(s) , ios::out);
	    fss << ctx.marshal_as<const char*>(inp->SeqName) << ": " <<  ctx.marshal_as<const char*>(outp->SeqName);
	    fss << "Input,Output, Ticks, Time, Min Gates, Total Time\n"  ;
	    String^ oss;
    );

  UInt64 nMin;

  // Process all Outputs
  while( (ol = NextOut()) != nullptr) {
    int nCount=0;
    t3=0; 
    
    COUT_SUMMARY2(
      // Create summary file
      String^ s = sDir + "\\Summary-" + inp->Name + "-" + outp->Name + "-" + nBits.ToString() +  ".csv" ;
      ofstream fs(ctx.marshal_as<const char*>(s) , ios::out);
	    fs << "Sequence, Function, Total Gates, Total Ticks, Frequency, Total Time\n"  ;
	    fs << "AVERAGE,=Average(C4:C10002),=Average(D4:D10002),,=Average(F4:F10002)\n"  ;
    );
    
    nCount = 0;
    nMin = UInt64::MaxValue;
    
    cout << ctx.marshal_as<const char*>(outp->Name) << "\n";
    
    while ( (il = NextIn()) != nullptr) {
      cout << nCount++ << "\r";
        
  	  QueryPerformanceCounter((LARGE_INTEGER*)&t1);
      Synthesis syn(NBITS);
      for(int i=0; i < il->Count; i++) {
        syn.Process(il[i], ol[il[i]]);    
      }
  	  QueryPerformanceCounter((LARGE_INTEGER*)&t2);
        t3 += t2 - t1;      // keep track of total time per sequence

	      if (false) if (syn.GateCount() <= nMin) {
          String^ s = sDir + "\\" + inp->Name + "-" + nBits.ToString() + "bits-" + nCount + "-" +  outp->Name +  ".csv" ;
	        ofstream f(ctx.marshal_as<const char*>(s) , ios::out);

          // Check propogation of patterns.
          for(int i=0; i < il->Count; i++) 
 			      f << 	il[i] << "," << ol[il[i]] << "\n";

	        f << "\nControl,," ;
	        for (ULONGLONG i=0; i<syn.GateCount(); i++) {
		        f << syn.c[i] << "," ;
	        }

	        f << "\nMask,," ;
	        for (ULONGLONG i=0; i<syn.GateCount(); i++) {
		        f <<  syn.m[i] << ",";
	        }
    	    
          f << "\nTotal Gates, " << syn.GateCount()  << "\n";
          f << "\nTotal Ticks, " << (t2 - t1)  << "\n";
          f << "Frequency, "   << freq << "\n";
          f << "Total Time, "  << 1000000*(t2 - t1) / freq << "\n";
          f.close();
 			  }

#ifdef CHECK_PROPAGATION
      for(int i=0; i < il->Count; i++) {
        ULONGLONG out = syn.Propogate(il[i], ol[il[i]]);
        
        if (out != il[i]) {
          f << "Failed " << "\n";
          for(int j=0; j < il->Count; j++) {
            f << "[" << il[j] << "," << ol[il[j]] << "]\n";
          }
          break;
        }
      }
#endif
      
      COUT_SUMMARY2(
        fs << "\n" << nCount ; 
        fs << ","  << ctx.marshal_as<const char*>(outp->Name);
        fs << ", " << syn.GateCount();
        fs << ", " << (t2 - t1) ;
        fs << ", "   << freq;
        fs << ", "  << 1000000*(t2 - t1) / freq ;
      );

//      COUT_SUMMARY (
        if (syn.GateCount() <= nMin) { 
          nMin = syn.GateCount();
          oss = String::Format("\n{0},{1},{2},{3},{4}", inp->Name, outp->Name, t2-t1, 1000000*(t2 - t1) / freq, nMin);
        }
//      );
    
      }

    
    COUT_SUMMARY (fss << ctx.marshal_as<const char*>(oss));
    COUT_SUMMARY (fss << "," << ctx.marshal_as<const char*>((1000000*t3/freq).ToString())); 
    COUT_SUMMARY (fss.flush());
    COUT_SUMMARY2 (fs.close());
  }
  
  COUT_SUMMARY (fss.close());
}

#ifdef ARCHIVE
void AllInputsPerFile()
{
	ULONGLONG t1, t2, freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&t1);

  marshal_context ctx;

  List<ULONGLONG>^ il;
  List<ULONGLONG>^ ol;


  FileSrc::Output outp(NBITS, "Function*");
    // Process all files
  while( (ol = outp.Next()) != nullptr) {
    int nCount=0;
    
    Miller::Input inp(NBITS);
    int maxCount = Math::Min(inp.Count(), (ULONGLONG)10000);
    
    COUT(
      // Make sure directory exists...
      String^ sDir = nBits.ToString() + "-bits";
      if ( !Directory::Exists(sDir) )
        Directory::CreateDirectory(sDir);
      
      // Create summary file
      String^ s = sDir + "\\Summary-" + inp.Name + "-" + outp.FileName + "-" + nBits.ToString() +  ".csv" ;
      ofstream fs(ctx.marshal_as<const char*>(s) , ios::out);
	    fs << "Sequence, Function, Total Gates, Total Ticks, Frequency, Total Time\n"  ;
	    fs << "AVERAGE,=Average(C4:C10002),=Average(D4:D10002),,=Average(F4:F10002)\n"  ;
    );
    
    while ( nCount++ < maxCount && (il = inp.Next()) != nullptr) {
      cout << nCount << "\n";
	    COUT(
        String^ s = sDir + "\\" + inp.Name + "-" + nBits.ToString() + "bits-" + nCount + "-" +  outp.FileName +  ".csv" ;
	      ofstream f(ctx.marshal_as<const char*>(s) , ios::out)
	      );
        
  	  QueryPerformanceCounter((LARGE_INTEGER*)&t1);
      Synthesis syn(NBITS);
      for(int i=0; i < il->Count; i++) {
        syn.Process(il[i], ol[il[i]]);    
      }
  	  QueryPerformanceCounter((LARGE_INTEGER*)&t2);
      
      // Check propogation of patterns.
      for(int i=0; i < il->Count; i++) {
        ULONGLONG out = syn.Propogate(il[i], ol[il[i]]);
 			  COUT( (f << 	il[i] << "," << ol[il[i]] << "\n") );
        
        if (out != il[i]) {
          f << "Failed " << "\n";
          for(int j=0; j < il->Count; j++) {
            f << "[" << il[j] << "," << ol[il[j]] << "]\n";
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
  	    
        f << "\nTotal Gates, " << syn.GateCount()  << "\n";
        f << "\nTotal Ticks, " << (t2 - t1)  << "\n";
        f << "Frequency, "   << freq << "\n";
        f << "Total Time, "  << 1000000*(t2 - t1) / freq << "\n";

        fs << "\n" << nCount ; 
        fs << ","  << ctx.marshal_as<const char*>(outp.FileName);
        fs << ", " << syn.GateCount()  ;
        fs << ", " << (t2 - t1) ;
        fs << ", "   << freq;
        fs << ", "  << 1000000*(t2 - t1) / freq ;

    
        f.flush();
        f.close();
      }
    )		
    fs.close();
  }
}



void OneInputPerFile()
{
	ULONGLONG t1, t2, freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&t1);

  marshal_context ctx;

  Nouraddin::Input inp(NBITS);
//  Miller::Input inp(NBITS);
//  FileSrc::Output outp(NBITS);
    FileSrc::Output outp(NBITS, "Function*");

  int maxCount = Math::Min(inp.Count(), (ULONGLONG)10000);
  List<ULONGLONG>^ il;
  List<ULONGLONG>^ ol;

  COUT(
    String^ sDir = nBits.ToString() + "-bits";
    if ( !Directory::Exists(sDir) )
      Directory::CreateDirectory(sDir);
    
    String^ s = sDir + "\\Summary-" + inp.Name + nBits.ToString() +  ".csv" ;
    ofstream fs(ctx.marshal_as<const char*>(s) , ios::out);
	  fs << "Sequence, Total Gates, Total Ticks, Frequency, Total Time\n"  ;
	  fs << "AVERAGE,=Average(B4:B10002),=Average(C4:C10002),,=Average(E4:E10002)\n"  ;
  );
    
  int nCount=0;
  while ( nCount++ < maxCount && (il = inp.Next()) != nullptr) {
    cout << nCount << "\n";
    ol = outp.Next();
    if (ol == nullptr)
      break;
      
      
	  COUT(
      String^ s = sDir + "\\" + inp.Name + "-" + nBits.ToString() + "-" + outp.FileName +  ".csv" ;
	    ofstream f(ctx.marshal_as<const char*>(s) , ios::out)
	    );
      
  	QueryPerformanceCounter((LARGE_INTEGER*)&t1);
    Synthesis syn(NBITS);
    for(int i=0; i < il->Count; i++) {
      syn.Process(il[i], ol[il[i]]);    
    }
  	QueryPerformanceCounter((LARGE_INTEGER*)&t2);
    
    // Check propogation of patterns.
    for(int i=0; i < il->Count; i++) {
      ULONGLONG out = syn.Propogate(il[i], ol[il[i]]);
 			COUT( (f << 	il[i] << "," << ol[il[i]] << "\n") );
      
      if (out != il[i]) {
        f << "Failed " << "\n";
        for(int j=0; j < il->Count; j++) {
          f << "[" << il[j] << "," << ol[il[j]] << "]\n";
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
	    

      f << "\nTotal Gates, " << syn.GateCount()  << "\n";
      f << "\nTotal Ticks, " << (t2 - t1)  << "\n";
      f << "Frequency, "   << freq << "\n";
      f << "Total Time, "  << 1000000*(t2 - t1) / freq << "\n";

      fs << "\n" << nCount ; 
      fs << ", " << syn.GateCount()  ;
      fs << ", " << (t2 - t1) ;
      fs << ", "   << freq;
      fs << ", "  << 1000000*(t2 - t1) / freq ;

  
      f.flush();
      f.close();
    )		
  }
  fs.close();

}

void NoFile()
{
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

#endif 