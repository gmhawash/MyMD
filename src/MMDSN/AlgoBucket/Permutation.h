#pragma once
#include "stdafx.h"
#include <iostream>

using namespace System;
using namespace System::Collections::Generic;
using namespace std;

namespace AlgoBucket {
  public ref class Permutation {
  protected:  
    __int64 m_CurrentTerm;
    __int64 m_nPermutations;
    __int64 m_nBits,m_nBase;
    __int64 m_nTerms;
    List<__int64> m_inputList;
    List<__int64> m_outputList;
    


  public: 
    Permutation(__int64 nBits);
    Permutation(__int64 nBits, __int64 nBase);
    void Set(__int64 nBits, __int64 nBase);
    List<__int64>^ Next();
    List<__int64>^ Next(__int64 nIndex);
    List<__int64>^ Random();

  protected:
    void Branch(List<__int64>^ list, __int64 term, __int64 nBits);
    __int64 Factorial(__int64 n);
  };

}
