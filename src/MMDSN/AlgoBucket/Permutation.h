#pragma once
#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <functional>

using namespace System;
using namespace System::Collections::Generic;
using namespace std;

namespace AlgoBucket {
  public ref class Permutation {
  protected:  
    ULONGLONG m_CurrentTerm;
    ULONGLONG m_nPermutations;
    ULONGLONG m_nBits,m_nBase;
    ULONGLONG m_nTerms;
    List<ULONGLONG> m_inputList;
    List<ULONGLONG> m_outputList;
    
    
    // TODO: EXTENSION NEEDED FOR MORE THAN 32 BITS.
    vector<UINT>* m_input;
    vector<UINT>::iterator* m_start;
    vector<UINT>::iterator* m_end;
 

  public: 
    static String^ Name = "Permutation";

    //property String^ Name;
    Permutation(ULONGLONG nBits);
    Permutation(ULONGLONG nBits, ULONGLONG nBase);
    void Set(ULONGLONG nBits, ULONGLONG nBase);
    List<ULONGLONG>^ Next();
    List<ULONGLONG>^ Next(ULONGLONG nIndex);
    List<ULONGLONG>^ Random();
    ULONGLONG Count(){return m_nPermutations;}

  protected:
    void Branch(List<ULONGLONG>^ list, ULONGLONG term, ULONGLONG nBits);
    ULONGLONG Factorial(ULONGLONG n);
  };

}
