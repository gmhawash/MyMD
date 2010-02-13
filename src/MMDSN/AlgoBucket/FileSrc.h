#pragma once
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "permutation.h"
#include <math.h>
#include <msclr\marshal.h>
#include <windows.h>

using namespace System;
using namespace System::Collections::Generic;
using namespace std;
using namespace msclr::interop;
using namespace System::IO;

namespace AlgoBucket {
  namespace FileSrc {
	  public ref class Output {
	    private:
	      int m_nSequence; 
	      ULONGLONG m_nBits;
	      ULONGLONG m_nTerms;
	      array<String^, 1>^ m_Files;
	      String^ m_FilePrefix;
        int m_nCount;	    

	    public:
        property String^ FileName;
  
  /// Output (ULONGLONG nBits) {///        property String^ FileName;
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
	     Output (ULONGLONG nBits) {
	      Output(nBits, "*");
	    }
  
  
  /// Output (ULONGLONG nBits, String^ FilePrefix, int nCount) {/// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
	     Output (ULONGLONG nBits, String^ FilePrefix, int nCount) {
        Set(nBits, FilePrefix, nCount);
	     }
	     
  
  
  /// Output (ULONGLONG nBits, String^ FilePrefix) {/// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
	      Output (ULONGLONG nBits, String^ FilePrefix) {
          Set(nBits, FilePrefix, MAXINT);
	      }

	     void Set (ULONGLONG nBits, String^ FilePrefix, int nCount) {
        m_nCount = nCount;
	      m_FilePrefix = FilePrefix;
	      m_nSequence = 0;
	      m_nBits = nBits;
        m_nTerms = (ULONGLONG)Math::Pow(2,nBits);
        
        String^ sDir = "..\\" + m_nBits.ToString() + "-bits-src";
        if ( !Directory::Exists(sDir) )
          throw gcnew Exception(sDir + " Does not exist");
          
        m_Files = Directory::GetFiles(sDir, m_FilePrefix + ".txt");
	     }


	     
	    /// List<ULONGLONG>^ Permutation::Next()/// 
      ///
      /// Inputs:
      ///
      /// Outputs:
      ///
      List<ULONGLONG>^ Output::Next()
      {  
        if (m_nSequence >= Math::Min(m_Files->Length, m_nCount)) {
          m_nSequence = 0;
          return nullptr;
        }

        marshal_context ctx;
        String^ s = m_Files[m_nSequence++];
        String^ delim = "\\.";
        array<String^,1>^ list = s->Split(delim->ToCharArray());
        FileName = list[list->Length-2];
        ifstream fs(ctx.marshal_as<const char*>(s) , ios::in);
                
        List<ULONGLONG> l;
        int x;
        for (int i=0; i<m_nTerms; i++) {        
          fs >> x ;
          l.Add(x);
        }
        return %l;
      }	     
	  };
  }
}
