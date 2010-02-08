#include <algorithm>
#include <functional>
#include <vector>
#include "synthesis.h"
#include <windows.h>

ULONGLONG gSequence;
ULONGLONG nTerms;
ULONGLONG nMask;


using namespace System::Collections::Generic;
using namespace std ;
using namespace AlgoBucket;
  
 
#define STEADMAN
  
#ifdef STEADMAN
  public ref class Globals abstract sealed {
  public:
     static Steadman^ Steadman;
     static List<ULONGLONG>^ Sequence;
  };
#endif   

  /// ULONGLONG InitInput(ULONGLONG nBits)/// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
ULONGLONG InitInput(ULONGLONG nBits)
{
	gSequence = 0;
	nTerms = 1LL << nBits;

#ifdef SEQUENTIAL
	
	nMask = 0;
	for(ULONGLONG i = 0; i<nBits; i++) 
		nMask |= 1LL << i;

#endif

#ifdef STEADMAN
  if (Globals::Steadman == nullptr)
    Globals::Steadman = gcnew Steadman(nBits);
    
  Globals::Sequence = Globals::Steadman->Next();
  if (Globals::Sequence == nullptr)
    return -1;
#endif

	return gSequence;	
}

  
  
  /// ULONGLONG NextInput()/// // natural order sequence
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
ULONGLONG NextInput()
{
#ifdef STEADMAN
  return gSequence < Globals::Sequence->Count ? Globals::Sequence[gSequence++] : -1;
#endif

#ifdef SEQUENTIAL
	return gSequence < nTerms ? gSequence++ : -1;
#endif
}


// Define a template class vector of int
typedef vector<ULONGLONG> IntVector ;

IntVector Numbers(BUF_SIZE);
ULONGLONG gOutIndex=0;


  
  
  /// ULONGLONG InitOutput(ULONGLONG nBits, int option)/// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
ULONGLONG InitOutput(ULONGLONG nBits, int option)
{
    //Define an iterator for template class vector of strings
    typedef IntVector::iterator IntVectorIt ;

    IntVectorIt start, end, it ;
    
    for (ULONGLONG i=0; i<nTerms; i++) 
			Numbers[i] = i;

		start = Numbers.begin() ;   // location of first element of Numbers
    end = Numbers.end() ;       // one past the location last element of Numbers

    // shuffle the elements in a random order
    random_shuffle(start, end) ;
		gOutIndex=0;	
	return 0LL;
}
  
  
  /// ULONGLONG NextOutput()/// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
ULONGLONG NextOutput()
{
	return gOutIndex < BUF_SIZE ? Numbers[gOutIndex++] : -1LL;
}
  
  
  /// ULONGLONG InitOutputY(ULONGLONG currentTerm, int option)/// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
ULONGLONG InitOutputY(ULONGLONG currentTerm, int option)
{	
		ULONGLONG gOut=0;

#if 0
		gOut |= SetBit(0,   Not(2) );
		gOut |= SetBit(1,   Bit(0) );
		gOut |= SetBit(2,   Not(3) );
		gOut |= SetBit(3,   Bit(1) );
		gOut |= SetBit(4,   Bit(8) );
		gOut |= SetBit(5,   Not(4) );
		gOut |= SetBit(6,   Bit(9) );
		gOut |= SetBit(7,   Not(5) );
		gOut |= SetBit(8,   Bit(6) );
		gOut |= SetBit(9,   Bit(7) );
#else
		gOut |= SetBit(0,   Not(7) );  
		gOut |= SetBit(1,   Bit(16) );
		gOut |= SetBit(2,   Not(8) );
		gOut |= SetBit(3,   Bit(0) );
		gOut |= SetBit(4,   Bit(15) );
		gOut |= SetBit(5,   Bit(12) );
		gOut |= SetBit(6,   Not(17) );
		gOut |= SetBit(7,   Bit(1));
		gOut |= SetBit(8,   Bit(18));
		gOut |= SetBit(9,   Bit(6) );
		gOut |= SetBit(10,  Not(9) );
		gOut |= SetBit(11,  Bit(19) );
		gOut |= SetBit(12,  Bit(2));
		gOut |= SetBit(13,  Bit(14) );
		gOut |= SetBit(14,  Bit(10) );
		gOut |= SetBit(15,  Bit(3));
		gOut |= SetBit(16,  Bit(5) );
		gOut |= SetBit(17,  Bit(11) );
		gOut |= SetBit(18,  Bit(4) );
		gOut |= SetBit(19,  Bit(13) );
#endif
	
	return gOut & nMask;
}



// Morphed from Nouraddin's starting_with_equations.cpp file.
// This setup does not generate a unitary in/out combination 
  
  
  /// ULONGLONG InitOutputX(ULONGLONG currentTerm, int option)/// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
ULONGLONG InitOutputX(ULONGLONG currentTerm, int option)
{	
		ULONGLONG gOut=0;
	
		gOut |= SetBit(0,   Not(1)  & Bit(14)  | Bit(7)); 
		gOut |= SetBit(1,   Bit(5)  | Bit(17)  | Bit(4)); 
		gOut |= SetBit(2,   Not(5)  & Bit(6)   | Bit(11)); 
		gOut |= SetBit(3,   Bit(9)  &  Not(3));
		gOut |= SetBit(4,   Bit(1)  | Bit(7)   | Bit(4)  & Bit(5)   | Bit(18) | Bit(4));
		gOut |= SetBit(5,   Bit(9)  &  Not(3)  &  Not(2) | Bit(14));
		gOut |= SetBit(6,   Not(3)  & Bit(17)  | Bit(11));
		gOut |= SetBit(7,   Bit(10) |  Not(7)  | Bit(4)  & Bit(15)  | Bit(8)  | Bit(4));
		gOut |= SetBit(8,   Bit(6));
		gOut |= SetBit(9,   Bit(1)  | Bit(7)   | Bit(4)  &  Not(15) | Bit(9)  | Bit(4));
		gOut |= SetBit(10,  Not(3)  & Bit(17)  | Bit(6)  &  Not(3)  | Bit(18) | Bit(0));
		gOut |= SetBit(11,  Bit(9)  &  Not(3)  | Bit(18) | Bit(0));
		gOut |= SetBit(12,  Bit(14) |  Not(0)  | Bit(6)  &  Not(3)  | Bit(18) | Bit(0));
		gOut |= SetBit(13,  Bit(1)  &  Not(3)  | Bit(18));
		gOut |= SetBit(14,  Bit(6)  &  Not(9)  | Bit(18) | Bit(10));
		gOut |= SetBit(15,  Bit(14) |  Not(6)  | Bit(6)  &  Not(3)  | Bit(18) | Bit(0));
		gOut |= SetBit(16,  Bit(1)  &  Not(13) | Bit(18) | Bit(0));
		gOut |= SetBit(17,  Bit(1)  |  Not(6)  | Bit(6)  &  Not(3)  | Bit(18) | Bit(0));
		gOut |= SetBit(18,  Bit(1)  |  Not(7));
		gOut |= SetBit(19,  Bit(4)  | Bit(7)   | Bit(4)  & Bit(15)  | Bit(19) | Bit(4));

	
	return gOut & nMask;
}


