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
	      Input (__int64 nBits) : Permutation (nBits) 
	      {
	        // remove first and last from input list (00..0, 11...1).
	        m_inputList.RemoveAt(0);
	        m_inputList.RemoveAt(m_inputList.Count-1);
          m_nTerms -= 2;
          m_nPermutations = Factorial(m_nTerms);
   	    }
	     
	      /// List<__int64>^ Permutation::Next()/// 
        ///
        /// Inputs:
        ///
        /// Outputs:
        ///
        List<__int64>^ Input::Next()
        {
          List<__int64>^ list;
          
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
        
        /// bool Valid(List<__int64>^ list)///   
        ///
        /// Inputs:
        ///
        /// Outputs:
        ///
        bool Input::Valid(List<__int64>^ list)
        {
          for(__int64 i=1; i< list->Count; i++)
          for(__int64 j=0; j < i; j++) { 
      //    for (__int64 n = 0; n < m_nBits; n++) {
      //      __int64 mask = ~(1 << n);
            __int64 control_i = list[i]; //& mask;
            __int64 control_j = list[j]; //& mask;
            if ( control_i  == (control_i & control_j))
                  return false;
            }
            
          return true;
        }
	  };
	
	  public ref class Output : Permutation {
	    public:
	      Output (__int64 nBits) : Permutation (nBits) {
	     }
	    /// List<__int64>^ Permutation::Next()/// 
      ///
      /// Inputs:
      ///
      /// Outputs:
      ///
      List<__int64>^ Output::Next()
      {
        return Permutation::Random();
      }	     
	  };
}
}
