#include "qurandom.h"


using namespace System;
using namespace System::IO;

#define N 8
#define M 2
#define RATIO (4.0/MAX_INT)

class CSolution
{
public:

  UINT  m_x[N];       // n=8: x0-x7
  double m_f[M];     // m=2; two objectives to meet.
  double m_rank;
  double m_selProp;  // Selection Probability
  bool   m_dominated;

  CSolution()
  {
    m_dominated = false;
    // Initialize Individual
    for (int i=0; i<N; i++)
      m_x[i] = QuRandom::Next32();
      
    Refresh();
  }
  
  
  ///   void Refresh()
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  void Refresh()
  {
    double d = 1/Math::Sqrt(N);
    double s1=0, s2=0;
    
    // sums of variables
    for (int i=0; i<N; i++) {
      double x =  (-2.0 + m_x[i] * RATIO);
      s1 += (x-d)*(x-d);
      s2 += (x+d)*(x+d);
    }
    
    m_f[0] = 1 - exp(-s1);
    m_f[1] = 1 - exp(-s2);
  }

  inline CSolution& operator=(CSolution &s2)
  {
    memcpy(m_x, s2.m_x, sizeof(m_x));
    Refresh();
    return *this;
  }
  
  
  ///   void SetDominance(CSolution& s2)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  void SetDominance(CSolution& s2)
  {
    // If this solution is dominated by either f1 or f2, say so.
    // We have to compare all elements of both solutions despite finding one 
    // dominating the other at earlier steps.
    //
    int gt=0,lt=0;
    for (int i=0; i<M; i++) {
      gt += m_f[i] > s2.m_f[i];
      lt += m_f[i] < s2.m_f[i];
      //cout << m_f[i] << " " << s2.m_f[i] << endl;
    }
    s2.m_dominated = s2.m_dominated || (gt == 0) && (lt > 0);
    m_dominated = m_dominated  || (lt == 0) && (gt > 0);
    
    //cout << m_dominated << " " << s2.m_dominated << endl;
  }
  
  inline bool operator<(CSolution &s2)
  {
    return true;
  }

  inline bool operator>(CSolution &s2)
  {
    return true;
  }

  
  inline bool operator<(CSolution s2[])
  {
    return true;
  }


  inline bool operator>(CSolution s2[])
  {
    return true;
  }

  void Mutate(double Pm)
  {
    if (Pm > QuRandom::Double())
      return;

    for(int i=0; i<N; i++)
      m_x[i] ^= 1 << QuRandom::Next32(31);
      
    Refresh();
  }
};