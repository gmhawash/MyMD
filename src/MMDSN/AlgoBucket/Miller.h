#pragma once
#include "stdafx.h"
#include <iostream>
#include "permutation.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace std;

namespace AlgoBucket {
  namespace Miller {
    /// public ref class Input : Permutation {///     
    ///
	  public ref class Input : Permutation {
	    bool m_called;
	    public:
  	    
	      Input (ULONGLONG nBits) : Permutation (nBits) {
          Name = "Miller";
	        m_called  = false;
	        m_nPermutations = 1;
	      }
        
      virtual void Reset() override
      {
        m_called = false;
      }
      
	    /// List<ULONGLONG>^ Permutation::Next()/// 
      ///
      /// Inputs:
      ///
      /// Outputs:
      ///
      List<ULONGLONG>^ Input::Next()
      {
        // Always return the first list (natural order).
        if (m_called) {
          m_called = false;
          return nullptr;
        }
          
        m_called =  true;
        return %m_inputList;
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
