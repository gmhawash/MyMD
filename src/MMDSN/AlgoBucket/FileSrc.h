#pragma once
#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "permutation.h"
#include <msclr\marshal.h>

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
	    
	    public:
	     Output (ULONGLONG nBits) {
	      m_nSequence = 0;
	      m_nBits = nBits;
        m_nTerms = (ULONGLONG)Math::Pow(2,nBits);
        
        String^ sDir = "..\\" + m_nBits.ToString() + "-bits-src";
        if ( !Directory::Exists(sDir) )
          throw gcnew Exception(sDir + " Does not exist");
          
        m_Files = Directory::GetFiles(sDir, "seq-*.txt");
	    }
	     
	    /// List<ULONGLONG>^ Permutation::Next()/// 
      ///
      /// Inputs:
      ///
      /// Outputs:
      ///
      List<ULONGLONG>^ Output::Next()
      {  
        marshal_context ctx;
        String^ s = m_Files[m_nSequence++];
        ifstream fs(ctx.marshal_as<const char*>(s) , ios::in);
        
        List<ULONGLONG> l;
        int x,y;
        for (int i=0; i<m_nTerms; i++) {        
          fs >> x >> y;
          l.Add(y);
        }
        return %l;
      }	     
	  };
}
}
