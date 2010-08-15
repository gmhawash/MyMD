#pragma once
#include "stdafx.h"
#include <iostream>
#include "permutation.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace std;

namespace AlgoBucket {
  namespace Func30 {
    /// public ref class Input : Permutation {///     
    ///
	  public ref class Input : Permutation {
	    bool m_called;
	    public:
  	    
	      Input ()  {
          Name = "Func30";
          SeqName = "Func30";
          m_nTerms = (ULONGLONG)Math::Pow(2,30);
	      }
        
      virtual void Reset() override
      {
        m_CurrentTerm = 0;
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
