#include <algorithm>
#include <functional>
#include <vector>
#include "synthesis.h"

__int64 gSequence;
__int64 nTerms;
__int64 nMask;

using namespace std ;

__int64 InitInput(__int64 nBits)
{
	gSequence = 0;
	nTerms = 1LL << nBits;
	
	nMask = 0;
	for(__int64 i = 0; i<nBits; i++) 
		nMask |= 1LL << i;

	return gSequence;	
}

// natural order sequence
__int64 NextInput()
{
		
	return gSequence < nTerms ? gSequence++ : -1;
}




// Define a template class vector of int
typedef vector<__int64> IntVector ;

IntVector Numbers(BUF_SIZE);
__int64 gOutIndex=0;

__int64 InitOutput(__int64 nBits, int option)
{
    //Define an iterator for template class vector of strings
    typedef IntVector::iterator IntVectorIt ;

    IntVectorIt start, end, it ;
    
    for (__int64 i=0; i<nTerms; i++) 
			Numbers[i] = i;

		start = Numbers.begin() ;   // location of first element of Numbers
    end = Numbers.end() ;       // one past the location last element of Numbers

    // shuffle the elements in a random order
    random_shuffle(start, end) ;
		gOutIndex=0;	
	return 0LL;
}


__int64 NextOutput()
{
	return gOutIndex < BUF_SIZE ? Numbers[gOutIndex++] : -1LL;
}

__int64 InitOutputY(__int64 currentTerm, int option)
{	
		__int64 gOut=0;

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
__int64 InitOutputX(__int64 currentTerm, int option)
{	
		__int64 gOut=0;
	
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


