#pragma once
#include "stdafx.h"
#include <iostream>
#include "permutation.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace std;
using namespace System::Runtime::InteropServices;
using namespace System::IO;

namespace AlgoBucket {
  namespace Nouraddin {
    /// public ref class Input : Permutation {///     
    ///
	  public ref class Input : Permutation {
	    List<array<UInt64>^>^ m_inputArray;
	    array<int>^ m_permList;
	    array<vector<ULONGLONG> *>^ m_inArray;
	    public:
  	    
	      Input (ULONGLONG nBits) : Permutation(nBits)
	      {
	        SeqName = "Nouraddin";
          m_inArray = gcnew array<vector<ULONGLONG>*>(nBits+1);
          m_inArray[0] = new vector<ULONGLONG>(1, 0);
          MakeList(nBits,1);
          m_inArray[nBits] = new vector<ULONGLONG>(1, (1<<nBits) - 1);
          
          m_nPermutations = 1;
          for each (vector<ULONGLONG>* a in m_inArray)
            m_nPermutations *= a->size();
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
          
          m_inArray[nIndex] = new vector<ULONGLONG>(Combination(nBits, nIndex));
          vector<ULONGLONG>* last = m_inArray[nIndex-1];
          
          int k=0;
          
          for (int i=0; i< last->size(); i++) {
            ULONGLONG l = last->at(i);
            for(int j=0; j< nBits; j++) {
              if (! (last->at(i) & (1<<j)) ) {
                ULONGLONG newValue = last->at(i)| (1<<j);
                vector<ULONGLONG>::iterator it =  find(m_inArray[nIndex]->begin(), m_inArray[nIndex]->end(), newValue);
                if (it == m_inArray[nIndex]->end())
                  m_inArray[nIndex]->at(k++) = newValue;
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
          if (m_CurrentTerm++ >= m_nPermutations) {
            m_CurrentTerm = 0;
            return nullptr;
          }
          
          List<ULONGLONG> list;

          if (m_permList == nullptr)
            m_permList = gcnew array<int>(m_inArray->Length);

          int carry = 1;
          for (int i = 0; i < m_inArray->Length; i++) {
            if (carry) {
              ++m_permList[i] ;
              carry = m_permList[i] / m_inArray[i]->size();
              m_permList[i] %=  m_inArray[i]->size();
              next_permutation(m_inArray[i]->begin(), m_inArray[i]->end());
            }
            
            // Add the current list to array
            for (int j=0; j < m_inArray[i]->size(); j++)
              list.Add(m_inArray[i]->at(j));
          }          
          return %list;
        }
  

      List<ULONGLONG>^ Input::Random()
        {
          // Select which band to shuffle randomly..
          System::Random rnd;
          int x = rnd.Next(m_inArray->Length);
          
          random_shuffle(m_inArray[x]->begin(), m_inArray[x]->end());

          // Copy list back
          List<ULONGLONG> list;
          for (int i = 0; i < m_inArray->Length; i++) 
          for (int j=0; j < m_inArray[i]->size(); j++)
            list.Add(m_inArray[i]->at(j));

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
