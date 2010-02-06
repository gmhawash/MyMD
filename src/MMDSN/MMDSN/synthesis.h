#define NBITS 3
#define BUF_SIZE (1LL << NBITS)




// Macros 
#define Bit(n) ((currentTerm >> n) & 1)
#define Not(n) ( ~Bit(n) & 1)
#define SetBit(n,v) ( (v & 1) << n) 
