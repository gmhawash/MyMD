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
	     Input (__int64 nBits) : Permutation (nBits) {}

	    /// List<__int64>^ Permutation::Next()/// 
      ///
      /// Inputs:
      ///
      /// Outputs:
      ///
      List<__int64>^ Input::Next()
      {
        // Always return the first list (natural order).
        m_CurrentTerm = 0;
        return Permutation::Next();
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
