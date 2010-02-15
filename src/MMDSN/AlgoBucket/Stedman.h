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
	        Name = "Stedman";
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
        List<ULONGLONG>^ Input::NextOld()
        {
          List<ULONGLONG>^ list;
          
          do {
            list = Permutation::Next();
            if (m_CurrentTerm++ % 1000 == 0) cout << m_nPermutations - m_CurrentTerm << "\r";  
          } while( list != nullptr && !Valid(list));
          if (list != nullptr) {
            list->Add(m_nTerms+1);
            list->Insert(0, 0);
          }
          return list;
        }
        
        
        List<ULONGLONG>^ Input::Next()
        {
          // Use STL's next_permutation code.  Only do it once per full iteration of permutation
          //
          if ( ((void*) m_start) == NULL) {
            m_input = new vector<UINT>(m_nTerms);
            m_start = new vector<UINT>::iterator(m_input->begin());
            m_end = new vector<UINT>::iterator(m_input->end()) ;       // one past the location last element of Numbers

            for(UINT i=0; i<m_inputList.Count; i++) {
              (*m_input)[i] = m_inputList[i];
            }
          }

          bool more;          
          do {
            more = next_permutation(*m_start, *m_end);
            if ( (m_CurrentTerm++ & 0x1FFF) == 0) cout << m_nPermutations - m_CurrentTerm << "\r";
          } while (more && !Valid(m_input));

          // Reset for next cycle of iterations
          if (!more) {
            m_start = NULL;
            return nullptr;
          }  

          m_outputList.Clear();
          for (UINT i=0; i<m_nTerms; i++)
            m_outputList.Add((*m_input)[i]);

          m_outputList.Add(m_nTerms+1);
          m_outputList.Insert(0, 0);

          return %m_outputList ;
        }
        
        
        /// bool Valid(List<ULONGLONG>^ list)///   
        ///
        /// Inputs:
        ///
        /// Outputs:
        ///
        bool Input::Valid(vector<UINT>* list)
        {
          for(UINT i=1; i< list->size(); i++)
          for(UINT j=0; j < i; j++) { 
            UINT control_i = list->at(i); //& mask;
            UINT control_j = list->at(j); //& mask;
            if ( control_i  == (control_i & control_j))
                  return false;
            }
            
          return true;
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
