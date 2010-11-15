#pragma once
#include "qrbg.h"
#include "math.h"

#define BUF_SIZE (sizeof(m_buffer)/sizeof(m_buffer[0]))
#define MAX_ULONG (UINT64)-1ULL
#define MAX_INT (UINT)-1

namespace QuRandom
{
	QRBG* m_rndService=NULL;
	double m_buffer[1024*1024];
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
        m_rndService->getDoubles(m_buffer, BUF_SIZE);
        m_index = 0;
      }
    }
    
    UINT64 Next64(UINT64 limit = MAX_ULONG) {Fill(); return (UINT64)(m_buffer[m_index++] * limit);}
    int Next(int limit = MAX_INT) {Fill(); return (int)(m_buffer[m_index++] * limit);}
    double Next() {Fill();return m_buffer[m_index++];}
    double NextDouble()  { Fill(); return m_buffer[m_index++];}
    unsigned long Bit()      { Fill(); return (m_buffer[m_index++] < 0.5 ? 0 : 1);}
    bool Truth()    { Fill(); return (m_buffer[m_index++] < 0.5 ? false : true);}
};

