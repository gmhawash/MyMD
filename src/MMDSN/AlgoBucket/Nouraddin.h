#pragma once
#include "stdafx.h"
#include <iostream>
#include "permutation.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace std;
using namespace System::Runtime::InteropServices;

namespace AlgoBucket {
  namespace Nouraddin {
    /// public ref class Input : Permutation {///     
    ///
	  public ref class Input : Permutation {
	    List<array<UInt64>^>^ m_inputArray;
	    public:
	    
	      Input (ULONGLONG nBits) : Permutation(nBits)
	      {
	        Name = "Nouraddin";
	        m_inputArray = gcnew List<array<UInt64>^>(nBits);
	        m_inputArray->Add(gcnew array<UInt64>(1){0});
          MakeList(nBits,1);
          m_inputArray->Add(gcnew array<UInt64>(1){(1<<nBits) - 1});
   	    }
  
  
        /// void MakeList(ULONGLONG nBits, ULONGLONG nIndex)/// 
        ///
        /// Inputs:
        ///
        /// Outputs:
        ///
        void MakeList(ULONGLONG nBits, ULONGLONG nIndex)
        {
          if (nIndex >= nBits)
            return; 
            
	        m_inputArray->Add(gcnew array<UInt64>(Combination(nBits, nIndex)));
          array<UInt64>^ last = m_inputArray[nIndex-1];
                    
          int k=0;
          
          for (int i=0; i< last->Length; i++) {
            ULONGLONG l = last[i];
            for(int j=0; j< nBits; j++) {
              if (! (last[i] & (1<<j)) ) {
                ULONGLONG newValue = last[i] | (1<<j);
                if (! Exists(m_inputArray[nIndex], newValue) )
                  m_inputArray[nIndex][k++] = newValue;
              }
            }
          }
          
          MakeList(nBits, nIndex+1);
        }
  
  
        /// bool Exists(array<UInt64>^ list, ULONGLONG value)/// 
        ///
        /// Inputs:
        ///
        /// Outputs:
        ///
        bool Exists(array<UInt64>^ list, ULONGLONG value)
        {
          for each (UInt64 v in list)
            if (value == v)
              return true;
          
          return false;
        }
  
        /// ULONGLONG Input::Combination(ULONGLONG n, ULONGLONG p)/// 	     
        ///
        /// Inputs:
        ///
        /// Outputs:
        ///
	      ULONGLONG Input::Combination(ULONGLONG n, ULONGLONG p)
	      {
	        ULONGLONG res=1;
	        for (ULONGLONG i=n-p+1; i<=n; i++)
	          res *= i;
	        
	        res /= Permutation::Factorial(p);
	        return res;
	      }
	      
	      /// List<ULONGLONG>^ Permutation::Next()/// 
        ///
        /// Inputs:
        ///
        /// Outputs:
        ///
        List<ULONGLONG>^ Input::Next()
        {
          List<ULONGLONG> list;
          
          for (int j=0; j< m_inputArray->Count; j++) {
            array<UInt64>^ l = m_inputArray[j];
            vector<ULONGLONG> x(l->Length);
            for(int i=0; i<l->Length; i++)
              x[i] = l[i];
              
            random_shuffle(x.begin(), x.end());
            
            for(int i=0; i<l->Length; i++)
              list.Add(x[i]);
          }
          
          //do {
          //  list = Permutation::Next();
          //  cout << this->m_CurrentTerm << "\r";
          //} while( list != nullptr && !Valid(list));
          //if (list != nullptr) {
          //  list->Add(m_nTerms+1);
          //  list->Insert(0, 0);
          //}
          return %list;
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
