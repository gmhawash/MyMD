#pragma once
#include "stdafx.h"
#include <iostream>
#include "permutation.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace std;

namespace AlgoBucket {
  namespace Stedman {
    /// public ref class Input : Permutation {///     
    ///
	  public ref class Input : Permutation {
	    public:
	      Input (ULONGLONG nBits) : Permutation (nBits) 
	      {
	        // remove first and last from input list (00..0, 11...1).
	        m_inputList.RemoveAt(0);
	        m_inputList.RemoveAt(m_inputList.Count-1);
          m_nTerms -= 2;
          
          m_nPermutations = Factorial(m_nTerms);
          if (nBits > 4)
            m_nPermutations = -1LL ;
   	    }
	     
	      /// List<ULONGLONG>^ Permutation::Next()/// 
        ///
        /// Inputs:
        ///
        /// Outputs:
        ///
        List<ULONGLONG>^ Input::Next()
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
        bool Input::Valid(List<ULONGLONG>^ list)
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
	
	  public ref class Output : Permutation {
	    public:
	      Output (ULONGLONG nBits) : Permutation (nBits) {
	        
	     }
	    /// List<ULONGLONG>^ Permutation::Next()/// 
      ///
      /// Inputs:
      ///
      /// Outputs:
      ///
      List<ULONGLONG>^ Output::Next()
      {
        return Permutation::Random();
      }	     
	  };
}
}
