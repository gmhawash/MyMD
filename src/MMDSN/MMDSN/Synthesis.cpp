// crt_malloc.c
// This program allocates memory with
// malloc, then frees the memory with free.
 
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <malloc.h>
#include <math.h>
#include <windows.h>
#include "synthesis.h"

using namespace std;
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

int main()
{
	ofstream f("output.txt", ios::out);
	
	__int64 t1, t2, freq;
	QueryPerformanceFrequency((LARGE_INTEGER*)&freq);
	freq = freq / 1000;	// milliseconds.
	QueryPerformanceCounter((LARGE_INTEGER*)&t1);

	for (int nSeq=0; nSeq < 1; nSeq ++) {
		nGate = 0;
		InitInput(nBits);
		InitOutput(0);
		__int64 inTerm;
		
		while ( (inTerm = NextInput()) >= 0) {
	#ifndef PROFILE	
			cout << "Term: " <<		inTerm << "\r";
	#endif		
			__int64 outTerm = NextOutput();
			
	#ifndef PROFILE	
			p[inTerm] = outTerm;

			for (__int64 i = 0; i<inTerm; i++)
				if (outTerm == p[i])
					cout << "Not Unitary - InTerm: " << inTerm << " outTerm: " << outTerm << "\n";
			f << inTerm << "  " << outTerm << "\n";

	#endif		
			ProcessTerm(inTerm, outTerm);
		}
		
	#ifndef PROFILE	
		f << "Gate:\n";
		for (__int64 i=0; i<nGate; i++) {
			f << c[i] << " " << m[i] << "\n";
		}
	#endif		
	}
	QueryPerformanceCounter((LARGE_INTEGER*)&t2);

	cout << "Total Time: " << (t2 - t1) / freq << "\n";
	f.flush();
	f.close();
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
