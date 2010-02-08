#include "stdafx.h"
#include <iostream>
#include "permutation.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace std;

namespace AlgoBucket {
  Permutation::Permutation(ULONGLONG nBits)  {
    Set(nBits, 2);
  }
  
  
  /// Permutation::Permutation(ULONGLONG nBits, ULONGLONG nBase) {/// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  Permutation::Permutation(ULONGLONG nBits, ULONGLONG nBase) {
    Set (nBits, nBase);
  }
  
  
  /// void Permutation::Set(ULONGLONG nBits, ULONGLONG nBase) /// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  void Permutation::Set(ULONGLONG nBits, ULONGLONG nBase) 
  {
    this->m_nBase = nBase;
    m_nBits = nBits;
    m_nTerms = (ULONGLONG)Math::Pow(m_nBase,nBits);
    m_nPermutations = Factorial(m_nTerms);
    
    // More than 4 bits overflows the 64bit register 32!
    if (nBits > 4)
      m_nPermutations = -1LL ;
    
    m_CurrentTerm = 0;

    for(ULONGLONG i=0; i<m_nTerms; i++) {
      m_inputList.Add(i);
      }
  }
  
  
  /// List<ULONGLONG>^ Permutation::Next()/// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  List<ULONGLONG>^ Permutation::Next()
  {
    // Use STL's next_permutation code.
    m_input = new vector<UINT>(m_nTerms);
    
    for(ULONGLONG i=0; i<m_inputList.Count; i++) {
      (*m_input)[i] = m_inputList[i];
    }
    
    vector<UINT>::iterator start, end;
    
		start = m_input->begin() ;   // location of first element of Numbers
    end = m_input->end() ;       // one past the location last element of Numbers
  
    next_permutation(start, end);
    m_outputList.Clear();
    for (UINT i=0; i<m_nTerms; i++)
      m_outputList.Add((*m_input)[i]);

    return %m_outputList;
  }
  
/// List<ULONGLONG>^ Permutation::Next()/// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  List<ULONGLONG>^ Permutation::Next(ULONGLONG nIndex)
  {
    List<ULONGLONG>^ list;
    if (nIndex >= m_nPermutations)
      return nullptr;

    // Clone the input list
    list = gcnew List<ULONGLONG>();
    for each(ULONGLONG m in m_inputList)
      list->Add(m);

    m_outputList.Clear();
    Branch(list, nIndex, m_nTerms);
    m_CurrentTerm++;

    return %m_outputList;
  }
    
  
  /// void Permutation::Branch(List<ULONGLONG>^ list, ULONGLONG term, ULONGLONG nBits)///   
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  void Permutation::Branch(List<ULONGLONG>^ list, ULONGLONG term, ULONGLONG nBits)
  {
    if (list->Count == 0) 
      return;

    ULONGLONG index = term / Factorial(--nBits);
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
  ULONGLONG Permutation::Factorial(ULONGLONG n)
  {
    ULONGLONG FactorialValue = 1;
    for (ULONGLONG i = n; i > 0; i--)
    {
      FactorialValue = FactorialValue * i;
    }
    return FactorialValue;
  }		
  
  
  /// List<ULONGLONG>^ Permutation::Random()///   
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  List<ULONGLONG>^ Permutation::Random()
  {
    int *a = (int*)malloc(sizeof(int) * m_nTerms);
    
    for (int i=0; i<m_nTerms; i++) 
      a[i] = i;
    
    // shuffle the elements in a random order
    random_shuffle(&a[0], &a[m_nTerms]);
    
    List<ULONGLONG> l;
    for(int i=0; i<m_nTerms; i++)
      l.Add(a[i]);
      
    return %l;
  }


}
