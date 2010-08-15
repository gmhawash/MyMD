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

ULONGLONG nBits = 4;
ULONGLONG nGate=0;


#define COUT_SUMMARY(x) x
#define COUT_SUMMARY2(x) 

void OneInputPerFile();
void NoFile();
void AllInputsPerFile();

public delegate List<ULONGLONG>^ Next();


void SynthesizeFromFunction( Permutation^ inp, Next^ fnIn, FileSrc::Input^ outp, Next^ fnOut);
void SynthesizeFromFile( FileSrc::Input^ inp, Next^ fnIn, FileSrc::Input^ outp, Next^ fnOut);


//#define LARGE_FUNCTION
#define NOURADDIN 
//#define MILLER 
//#define FILE_FILE

  
int main(array<System::String ^> ^args)
{

#ifdef LARGE_FUNCTION
    Synthesis outp(30);
    outp.Init30Bits_1 ();
  	ULONGLONG t1, t2, freq;
	  ULONGLONG t3;
    marshal_context ctx;
  	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);


	  QueryPerformanceCounter((LARGE_INTEGER*)&t1);

    Synthesis syn(30);
    for(ULONGLONG i=0; i < (1LL << 30); i++) {
      syn.Process(i, outp.Propogate (i));    
    }
	  QueryPerformanceCounter((LARGE_INTEGER*)&t2);
    t3 = t2 - t1;

    // Make sure directory exists...
    String^ sDir = "30-bits";
    if ( !Directory::Exists(sDir) )
      Directory::CreateDirectory(sDir);

    String^ s = sDir +  "\\Summary-ahp30_1" +  DateTime::Now.ToBinary() + "-" +  ".csv" ;
    ofstream fss(ctx.marshal_as<const char*>(s) , ios::out);
    fss << "Function AHP30_1 \n";
    fss << "Input,Output, Ticks, Time, Min Gates, Total Time\n"  ;

    fss << "Input Gates\n";
	        fss << "\nControl,," ;
	        for (ULONGLONG i=0; i<outp.GateCount(); i++) {
		        fss << outp.c[i] << "," ;
	        }

	        fss << "\nMask,," ;
	        for (ULONGLONG i=0; i<outp.GateCount(); i++) {
		        fss <<  outp.m[i] << ",";
	        }
    
    fss << "Synthesized Gates\n";
	        fss << "\nControl,," ;
	        for (ULONGLONG i=0; i<syn.GateCount(); i++) {
		        fss << syn.c[i] << "," ;
	        }

	        fss << "\nMask,," ;
	        for (ULONGLONG i=0; i<syn.GateCount(); i++) {
		        fss <<  syn.m[i] << ",";
	        }
  
    fss << "\nTotal Time, " << ctx.marshal_as<const char*>((1000000*t3/freq).ToString()); 
    fss.flush();

    
    
#elif defined(NOURADDIN)
  for (nBits=4; nBits<= NBITS; nBits++) {
    Nouraddin::Input inp(nBits);
    FileSrc::Input  outp(nBits, "function*", 50);
    SynthesizeFromFunction(%inp, gcnew Next(%inp, &Nouraddin::Input::Random),  %outp, gcnew Next(%outp, &FileSrc::Input::Next));
  }
#elif  defined(MILLER)
  for (nBits=4; nBits<= NBITS; nBits++) {
    Miller::Input inp(nBits);
    FileSrc::Input outp(nBits, "function*", 50);
    SynthesizeFromFunction(%inp, gcnew Next(%inp, &Miller::Input::Next),  %outp, gcnew Next(%outp, &FileSrc::Input::Next));
  }
#elif  defined(FILE_FILE)
  if (args->Length < 2) {
    Console::WriteLine("Usage: MMDSN filespec1 filespec2 \n");
    return -1;
  }

  FileSrc::Input inp(nBits, args[0] , MAXINT);
  FileSrc::Input outp(nBits, args[1], 50);
  SynthesizeFromFile(%inp, gcnew Next(%inp, &FileSrc::Input::Next),  %outp, gcnew Next(%outp, &FileSrc::Input::Next));
#endif
 // AllInputsPerFile();
}


  
/// void SynthesizeFromFunction(Permutation::Input^ inp, Next^ NextIn, FileSrc::Input^ outp, Next^ NextOut)/// 
///
/// Inputs:
///
/// Outputs:
///
void SynthesizeFromFunction(Permutation^ inp, Next^ NextIn, FileSrc::Input^ outp, Next^ NextOut)
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
      String^ s = sDir + "\\Summary-" + inp->Name +  DateTime::Now.ToBinary() + "-" + nBits.ToString() +  ".csv" ;
      ofstream fss(ctx.marshal_as<const char*>(s) , ios::out);
	    fss << ctx.marshal_as<const char*>(inp->Name) << ": " <<  ctx.marshal_as<const char*>(outp->Name);
	    fss << "Input,Output, Ticks, Time, Min Gates, Total Time\n"  ;
	    String^ oss;
    );

  UInt64 nMin;


#ifdef MILLER      
  int k=1;
#else
  int k=5000;
#endif
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
    
    cout << "\n" << ctx.marshal_as<const char*>(outp->Name) << "\n";
    
    while ( k--) {
      il = NextIn();
      cout << nCount++ << "\r";

  	  QueryPerformanceCounter((LARGE_INTEGER*)&t1);
      Synthesis syn(NBITS);
      for(int i=0; i < il->Count; i++) {
        syn.Process(il[i], ol[il[i]]);    
      }
  	  QueryPerformanceCounter((LARGE_INTEGER*)&t2);
        t3 += t2 - t1;      // keep track of total time per sequence

	      if (syn.GateCount() <= nMin) {
          String^ s = sDir + "\\" + inp->Name + "-" + nBits.ToString() + "bits-" + syn.GateCount() + "gates-" +  outp->Name +  ".csv" ;
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

  
/// void Synthesize(Next^ inp, Next^ outp)/// 
///
/// Inputs:
///
/// Outputs:
///
void SynthesizeFromFile(FileSrc::Input^ inp, Next^ NextIn, FileSrc::Input^ outp, Next^ NextOut)
{
	ULONGLONG t1, t2, freq;
	ULONGLONG t3;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&t1);

  marshal_context ctx;

  List<ULONGLONG>^ il;
  List<ULONGLONG>^ ol;

    //COUT_SUMMARY(
      // Make sure directory exists...
      String^ sDir = nBits.ToString() + "-bits";
      if ( !Directory::Exists(sDir) )
        Directory::CreateDirectory(sDir);
      
      // Create summary file
      String^ s = sDir + "\\A-Summary-" + inp->SeqName +  DateTime::Now.ToBinary() + "-" + nBits.ToString() +  ".csv" ;
      ofstream fss(ctx.marshal_as<const char*>(s) , ios::out);
	    fss << ctx.marshal_as<const char*>(inp->SeqName) << ": " <<  ctx.marshal_as<const char*>(outp->SeqName);
	    fss << "Input,Output, Ticks, Time, Min Gates, Quantum Cost, Total Time\n"  ;
	    String^ oss;
    //);

  UInt64 nMin;

  // Process all Outputs
  while( (ol = NextOut()) != nullptr) {
    int nCount=0;
    t3=0; 
    
    COUT_SUMMARY2(
      // Create summary file
      String^ s = sDir + "\\A-Summary-" + inp->SeqName + "-" + outp->SeqName + "-" + nBits.ToString() +  ".csv" ;
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

	      if (syn.GateCount() <= nMin) {
          String^ s = sDir + "\\" + inp->SeqName + "-" + inp->Name + "-" + nBits.ToString() + "bits-" + syn.GateCount() + "gates-" +  outp->SeqName + "-" + outp->Name + ".csv" ;
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

	        f << "\nQuantum Cost," << syn.QuantumCost() << ",";
	        
	        for (ULONGLONG i=0; i<syn.GateCount(); i++) {
		        f <<  syn.GateCost(i) << ",";
	        }
    	    
          f << "\nTotal Gates, " << syn.GateCount() ;
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
        fs << ", " << syn.QuantumCost();
        fs << ", " << (t2 - t1) ;
        fs << ", "   << freq;
        fs << ", "  << 1000000*(t2 - t1) / freq ;
      );

//      COUT_SUMMARY (
        if (syn.GateCount() <= nMin) { 
          nMin = syn.GateCount();
          oss = String::Format("\n{0},{1},{2},{3},{4},{5}", inp->Name, outp->Name, t2-t1, 1000000*(t2 - t1) / freq, nMin, syn.QuantumCost());
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