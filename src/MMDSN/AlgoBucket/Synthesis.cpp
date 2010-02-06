#include "StdAfx.h"
#include "Synthesis.h"


namespace AlgoBucket {
  Synthesis::Synthesis(__int64 bits)
  {
  	p = gcnew array<__int64>(BUF_SIZE);
	  m = gcnew array<__int64>(BUF_SIZE);
  	c = gcnew array<__int64>(BUF_SIZE);

    nGate = 0;
    nBits = bits;
  }
  
  void Synthesis::Process(__int64 inTerm, __int64 outTerm)
  {
    outTerm = Propogate(inTerm, outTerm);
    
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
  
  __int64 Synthesis::Propogate(__int64 inTerm, __int64 outTerm)
  {
	  // Apply current list of gates..
	  for (__int64 i=0; i<nGate; i++) {
		  __int64 x = outTerm & c[i];
		  if (x == c[i])
			  outTerm ^= m[i];
	  }
	  return outTerm;
  }  
  
}