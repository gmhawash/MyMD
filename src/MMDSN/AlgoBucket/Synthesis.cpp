#include "StdAfx.h"
#include "Synthesis.h"


namespace AlgoBucket {
	Synthesis::Synthesis(ULONGLONG bits)
	{
		m = gcnew array<ULONGLONG>(bits*1024);
		c = gcnew array<ULONGLONG>(bits*1024);

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
		outTerm = Propogate(outTerm);
		
		// Do we still have a difference?
		// TODO: redo this thing with hash tables
		ULONGLONG diff = inTerm ^ outTerm;
		
		if (diff > 0) {
			// Flip the 0 bits first
			ULONGLONG mask = 1;
			for (int j = 0; j< nBits; j++) {
				if ( (diff & mask) && !(outTerm & mask)) {
					if (nGate > c->Length - 8)  // make sure there is enough memory
						c->Resize(c, c->Length + 1024), m->Resize(m, c->Length + 1024);
						
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
					if (nGate > c->Length - 8)  // make sure there is enough memory
						c->Resize(c, c->Length + 1024), m->Resize(m, c->Length + 1024);

					c[nGate] = outTerm & ~mask;		// clear inTerm bit at current bit position
					m[nGate++] = mask;						// Mark bit position for as NOT gate
					diff &= ~mask;								// reset bit in diff, since it should be same in outTerm
					outTerm ^= mask;
				}
				mask <<= 1;
			}
			
		}	
	}
	
	ULONGLONG Synthesis::Propogate(ULONGLONG outTerm)
	{
		// Apply current list of gates..
		for (ULONGLONG i=0; i<nGate; i++) {
			ULONGLONG x = outTerm & c[i];
			if (x == c[i])
				outTerm ^= m[i];
		}
		return outTerm;
	}  

	ULONGLONG Synthesis::QuantumCost()
	{
		int nCost=0;
		
		for (int i=0; i<nGate; i++) 
			nCost += GateCost(i);
			
		return nCost;
	}

	
	///   ULONGLONG Synthesis::GateCost(int i)
	///
	///
	/// Inputs:
	///
	/// Outputs:
	///
	ULONGLONG Synthesis::GateCost(int i)
	{
		return Math::Max((int)1, (int)Math::Pow(2, 1 + ControlLines(c[i])) - 3);
	}
	
	
	///   ULONGLONG Synthesis::ControlLines(ULONGLONG n)
	///
	///
	/// Inputs:
	///
	/// Outputs:
	///
	ULONGLONG Synthesis::ControlLines(ULONGLONG n)
	{
		ULONGLONG nCount=0;
		
		for (int i=0; i<nBits; i++) 
			nCount += (n >> i) & 1;
			
		return nCount;
	}

	
	
	///   ULONGLONG Synthesis::AddGate(ULONGLONG control, ULONGLONG target)
	///
	///
	/// Inputs:
	///
	/// Outputs:
	///
	ULONGLONG Synthesis::AddGate(ULONGLONG control, ULONGLONG target)
	{
		if (nGate > c->Length - 8)  // make sure there is enough memory
			c->Resize(c, c->Length + 1024), m->Resize(m, c->Length + 1024);

		c[nGate] = control;
		m[nGate++] = target;
		
		return 0;
	}
	
	
	///   void Synthesis::Init30Bits_1()
	///
	///
	/// Inputs:
	///
	/// Outputs:
	///
	void Synthesis::Init30Bits_1()
	{
		for (int i=0; i<29; i++)
			AddGate((ULONGLONG)Math::Pow(2,i+1), (ULONGLONG)Math::Pow(2,i));
	
		AddGate(1, (ULONGLONG)Math::Pow(2,29));
	}
	
	
}