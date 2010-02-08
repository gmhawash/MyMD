#include "StdAfx.h"
#include "Synthesis.h"


namespace AlgoBucket {
  Synthesis::Synthesis(ULONGLONG bits)
  {
  	p = gcnew array<ULONGLONG>(BUF_SIZE);
	  m = gcnew array<ULONGLONG>(BUF_SIZE);
  	c = gcnew array<ULONGLONG>(BUF_SIZE);

    nGate = 0;
    nBits = bits;
  }
  
  
  
  
  /// void Synthesis::Process(ULONGLONG inTerm, ULONGLONG outTerm)
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  void Synthesis::Process(ULONGLONG inTerm, ULONGLONG outTerm)
  {
    outTerm = Propogate(inTerm, outTerm);
    
	  // Do we still have a difference?
	  // TODO: redo this thing with hash tables
	  ULONGLONG diff = inTerm ^ outTerm;
  	
	  if (diff > 0) {
		  // Flip the 0 bits first
		  ULONGLONG mask = 1;
		  for (int j = 0; j< nBits; j++) {
			  if ( (diff & mask) && !(outTerm & mask)) {
				  c[nGate] = outTerm & ~mask;		// clear inTerm bit at current bit position
				  m[nGate++] = mask;						// Mark bit position for as NOT gate
				  diff &= ~mask;								// reset bit in diff, since it should be same in outTerm
				  outTerm ^= mask;
			  }
			  mask <<= 1;
		  }
		  
		  mask = 1;
		  // Flip the 1 bits second
		  for (int j = 0; j< nBits; j++) {
			  if ( (diff & mask) && (outTerm & mask)) {
				  c[nGate] = outTerm & ~mask;		// clear inTerm bit at current bit position
				  m[nGate++] = mask;						// Mark bit position for as NOT gate
				  diff &= ~mask;								// reset bit in diff, since it should be same in outTerm
				  outTerm ^= mask;
			  }
			  mask <<= 1;
		  }
		  
	  }	
  }
  
  ULONGLONG Synthesis::Propogate(ULONGLONG inTerm, ULONGLONG outTerm)
  {
	  // Apply current list of gates..
	  for (ULONGLONG i=0; i<nGate; i++) {
		  ULONGLONG x = outTerm & c[i];
		  if (x == c[i])
			  outTerm ^= m[i];
	  }
	  return outTerm;
  }  
  
}