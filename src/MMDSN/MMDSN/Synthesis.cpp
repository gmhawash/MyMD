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
#include "synthesis.h"

using namespace std;
using namespace msclr::interop;
using namespace System;

__int64 InitInput(__int64 nBits);
__int64 NextInput();
__int64 InitOutput(__int64 currentTerm, int option=0);
void ProcessTerm(__int64 inTerm, __int64 outTerm);
__int64 NextOutput();

__int64 nBits = NBITS;
__int64 p[BUF_SIZE];
__int64 c[BUF_SIZE];
__int64 m[BUF_SIZE];
__int64 nGate=0;

#define PROFILE 1

#ifdef PROFILE
  #define COUT(x) x
#else
  #define COUT(x) 
#endif

int main()
{
	__int64 t1, t2, freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	QueryPerformanceCounter((LARGE_INTEGER*)&t1);

  marshal_context ctx;
	for (int nSeq=0; nSeq < 100; nSeq ++) {

		nGate = 0;
		if (InitInput(nBits) < 0)
		  break;
		  
		InitOutput(0);
		__int64 inTerm, outTerm;

	  COUT(
	    String^ s = "Sequence-" + nBits.ToString() + "-" + nSeq.ToString() +  ".csv" ;
		  ofstream f(ctx.marshal_as<const char*>(s) , ios::out)
		  );
		
		while ( (inTerm = NextInput()) >= 0) {
			outTerm = NextOutput();
			
  		COUT(cout << "[in,out]: [" <<	inTerm << "," << outTerm << "]\n");
			ProcessTerm(inTerm, outTerm);
			COUT( f << 	inTerm << "," << outTerm << "\n");
		}
		
		COUT (cout << "Gates: " << nGate << "\n");
	  COUT(
		  f << "\nControl,," ;
		  for (__int64 i=0; i<nGate; i++) {
			  f << c[i] << "," ;
		  }
		  f << "\nMask,," ;
		  for (__int64 i=0; i<nGate; i++) {
			  f <<  m[i] << ",";
		  }
	    f.flush();
	    f.close();
	  )		
	}
	QueryPerformanceCounter((LARGE_INTEGER*)&t2);

	cout << "Total Ticks: " << (t2 - t1)  << "\n";
	cout << "Frequency: "   << freq << "\n";
	cout << "Total Time: "  << 1000*(t2 - t1) / freq << "\n";
}

/*
	Apply existing gates to inTerm
		foreach gate
			if ( (bitControl & inTerm) == bitControl)
				inTerm = inTerm XOR bitMask;
	
	diff = inTerm XOR outTerm
	foreach different bit
	  gate.bitMask = 1 << bitPosition
	  gate.bitControl = outTerm AND ~bitMask
*/
void ProcessTerm(__int64 inTerm, __int64 outTerm)
{
	// Apply current list of gates..
	for (__int64 i=0; i<nGate; i++) {
		__int64 x = outTerm & c[i];
		if (x == c[i])
			outTerm ^= m[i];
	}

	// Do we still have a difference?
	__int64 diff = inTerm ^ outTerm;
	
	if (diff > 0) {
		__int64 mask = 1;
		for (int j = 0; j< nBits; j++) {
			if (diff & mask) {
				c[nGate] = outTerm & ~mask;		// clear inTerm bit at current bit position
				m[nGate++] = mask;						// Mark bit position for as NOT gate
				diff &= ~mask;								// reset bit in diff, since it should be same in outTerm
				outTerm ^= mask;
			}
			mask <<= 1;
		}
	}	
}
