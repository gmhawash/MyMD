// Defines
/// 
#define NBITS 4
#define BUF_SIZE NBITS * (1LL << NBITS)

/// Macros
#define Bit(n) ((currentTerm >> n) & 1)
#define Not(n) ( ~Bit(n) & 1)
#define SetBit(n,v) ( (v & 1) << n) 

#include <iostream>

using namespace System::Collections::Generic;
using namespace std;