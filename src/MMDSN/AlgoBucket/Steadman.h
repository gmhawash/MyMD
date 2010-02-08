#include "stdafx.h"
#include <iostream>
#include "permutation.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace std;

namespace AlgoBucket {
  
	  public ref class Steadman : Permutation {
	
	  public:
	   Steadman(ULONGLONG nBits) : Permutation (nBits) {
	    // remove first and last from input list (00..0, 11...1).
	    m_inputList.RemoveAt(0);
	    m_inputList.RemoveAt(m_inputList.Count-1);
      m_nTerms -= 2;
      m_nPermutations = Factorial(m_nTerms);
	    
	  }
	  
	/// List<ULONGLONG>^ Permutation::Next()/// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  List<ULONGLONG>^ Steadman::Next()
  {
    List<ULONGLONG>^ list;
    
    do {
      list = Permutation::Next();
      cout << this->m_CurrentTerm << "\r";
    } while( list != nullptr && !Valid(list));
    if (list != nullptr) {
      list->Add(m_nTerms+1);
      list->Insert(0, 0);
    }
    return list;
  }
  
  /// bool Valid(List<ULONGLONG>^ list)///   
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  bool Valid(List<ULONGLONG>^ list)
  {
    for(ULONGLONG i=1; i< list->Count; i++)
    for(ULONGLONG j=0; j < i; j++) { 
//    for (ULONGLONG n = 0; n < m_nBits; n++) {
//      ULONGLONG mask = ~(1 << n);
      ULONGLONG control_i = list[i]; //& mask;
      ULONGLONG control_j = list[j]; //& mask;
      if ( control_i  == (control_i & control_j))
            return false;
      }
      
    return true;
  }
  
	};

}
