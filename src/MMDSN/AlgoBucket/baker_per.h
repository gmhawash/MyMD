#pragma once
#include "stdafx.h"
#include <iostream>
#include "permutation.h"
#include <string>


using namespace System;
using namespace System::Collections::Generic;
using namespace std;
using namespace System::Runtime::InteropServices;
using namespace System::IO;


typedef vector<int, allocator<int> > INTVECTOR;
typedef vector<ULONGLONG , allocator<ULONGLONG>> ULONGVECTOR;


namespace AlgoBucket {
 namespace Baker_per {
    /// public ref class Input : Permutation {///     
    ///
	 public  ref class Input: public Permutation {
      

    public:
		Input(ULONGLONG nBits);
		List<ULONGLONG>^ Input::Next();
		
	private:
		vector<vector<ULONGLONG>>& current_h; //curent hase diagrame
		vector<vector<ULONGLONG>> hasse(ULONGLONG n); //construct first hasse diagrame
		//List<ULONGLONG>^ linear_hasse(vector<vector<ULONGLONG>^>* band); //convert hasse diagram  to linear representation 
		
	}  ;
      
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
 
  
;  }



