#include "stdafx.h"
#include <iostream>
#include "permutation.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace std;

namespace AlgoBucket {
  Permutation::Permutation(__int64 nBits)  {
    Set(nBits, 2);
  }
  
  
  /// Permutation::Permutation(__int64 nBits, __int64 nBase) {/// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  Permutation::Permutation(__int64 nBits, __int64 nBase) {
    Set (nBits, nBase);
  }
  
  
  /// void Permutation::Set(__int64 nBits, __int64 nBase) /// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  void Permutation::Set(__int64 nBits, __int64 nBase) 
  {
    this->m_nBase = nBase;
    m_nBits = nBits;
    m_nTerms = (__int64)Math::Pow(m_nBase,nBits);
    m_nPermutations = Factorial(m_nTerms);
    m_CurrentTerm = 0;

    for(__int64 i=0; i<m_nTerms; i++)
      m_inputList.Add(i);
  }
  
  
  /// List<__int64>^ Permutation::Next()/// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  List<__int64>^ Permutation::Next()
  {
    return Next(m_CurrentTerm);
  }
  
/// List<__int64>^ Permutation::Next()/// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  List<__int64>^ Permutation::Next(__int64 nIndex)
  {
    List<__int64>^ list;
    if (nIndex>= m_nPermutations)
      return nullptr;

    // Clone the input list
    list = gcnew List<__int64>();
    for each(__int64 m in m_inputList)
      list->Add(m);

    m_outputList.Clear();
    Branch(list, nIndex, m_nTerms);
    m_CurrentTerm++;

    return %m_outputList;
  }
    
  
  /// void Permutation::Branch(List<__int64>^ list, __int64 term, __int64 nBits)///   
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  void Permutation::Branch(List<__int64>^ list, __int64 term, __int64 nBits)
  {
    if (list->Count == 0) 
      return;

    __int64 index = term / Factorial(--nBits);
    term = term % Factorial(nBits);
    m_outputList.Add(list[index]);
    list->RemoveAt(index);
    Branch(list, term, nBits);
  }


  /// <summary>
  /// 
  /// </summary>
  /// <param name="n"></param>
  /// <returns></returns>
  __int64 Permutation::Factorial(__int64 n)
  {
    __int64 FactorialValue = 1;
    for (__int64 i = n; i > 0; i--)
    {
      FactorialValue = FactorialValue * i;
    }
    return FactorialValue;
  }		
  
  
  /// List<__int64>^ Permutation::Random()///   
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  List<__int64>^ Permutation::Random()
  {
     System::Random rand; 
     return Next(rand.Next(m_nPermutations));
  }


}
