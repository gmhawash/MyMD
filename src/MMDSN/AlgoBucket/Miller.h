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
	    public:
	     Input (ULONGLONG nBits) : Permutation (nBits) {}

	    /// List<ULONGLONG>^ Permutation::Next()/// 
      ///
      /// Inputs:
      ///
      /// Outputs:
      ///
      List<ULONGLONG>^ Input::Next()
      {
        // Always return the first list (natural order).
        m_CurrentTerm = 0;
        return Permutation::Next();
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
