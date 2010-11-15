#pragma once
#include "qrbg.h"
#include "math.h"

#define BUF_SIZE (sizeof(m_buffer)/sizeof(m_buffer[0]))
#define MAX_ULONG (UINT64)-1ULL
#define MAX_INT (UINT)-1

using namespace System;
using namespace std;

#ifdef _DEBUG
  #define N_SIZE 1024
#else
  #define N_SIZE 1024*1024
#endif

namespace QuRandom
{
	QRBG* m_rndService=NULL;
	double m_buffer[N_SIZE];
	int m_index=BUF_SIZE;

  void Init(void)
  {
    if (m_rndService)
      return;
    
    // Create random service object
	  try {
		  m_rndService = new QRBG;
	  } catch (QRBG::NetworkSubsystemError) {
		  printf("Network error!");
		  return ;
	  } catch (...) {
		  printf("Failed to create QRBG client object!");
		  return ;
	  }

    // Login to server
	  try {
		  m_rndService->defineServer("random.irb.hr", 1227);
		  m_rndService->defineUser("mhawash", "seven11");
	  } catch (QRBG::InvalidArgumentError e) {
		  printf("Invalid hostname/port, or username/password specified! \n");
		  delete m_rndService;
		  return ;
	  }
  };
  
    void TearDown(void)
    {
      delete m_rndService;
    }
    
    void Fill()
    {
      Init();
      if (m_index == BUF_SIZE) {
        Console::Write("Filling Random Buffer\n");
        m_rndService->getDoubles(m_buffer, BUF_SIZE);
        m_index = 0;
      }
    }
    
    inline UINT64 Next64(UINT64 limit = MAX_ULONG) {Fill(); return (UINT64)(m_buffer[m_index++] * limit);}
    inline UINT Next32(UINT limit = MAX_INT) {Fill(); return (UINT)(m_buffer[m_index++] * limit);}
    inline UINT Next(UINT limit = MAX_INT) {Fill(); return (UINT)(m_buffer[m_index++] * limit);}
    inline double Double()  { Fill(); return m_buffer[m_index++];}
    double NextDouble()  { return Double();}
    unsigned long Bit()      { Fill(); return (m_buffer[m_index++] < 0.5 ? 0 : 1);}
    bool Truth()    { Fill(); return (m_buffer[m_index++] < 0.5 ? false : true);}
};

