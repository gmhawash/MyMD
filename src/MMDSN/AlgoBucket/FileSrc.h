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
    public ref class Input {
      UINT m_nCount;
      ULONGLONG m_nFiles, m_nSequence, m_nBits, m_nTerms;
      array<String^, 1>^ m_Files;
     ifstream *m_fsCurrent;

    public:
      property String^ Name;
      property String^ SeqName;
  
  
  /// Input(ULONG nBits, String^ FilePrefix) {///       
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
      Input(ULONG nBits, String^ FilePrefix) {
        Set(nBits, FilePrefix,  UInt32::MaxValue);
      }
  
	    Input (ULONGLONG nBits, String^ FilePrefix, UINT nCount) {
	      Set(nBits, FilePrefix,  nCount);
	    }
  
  /// void Set (ULONGLONG nBits, String^ FilePrefix, UINT nCount) {/// 
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
	    void Set (ULONGLONG nBits, String^ FilePrefix, UINT nCount) {
	     
        m_nCount = nCount;
	      SeqName = FilePrefix->Replace('*', ' ')->TrimEnd();
	      m_nSequence = 0;
	      m_nBits = nBits;
        m_nTerms = (ULONGLONG)Math::Pow(2,nBits);
        
        String^ sDir = "..\\" + m_nBits.ToString() + "-bits-src";
        if ( !Directory::Exists(sDir) )
          throw gcnew Exception(sDir + " Does not exist");
          
        m_nFiles = 0;
        m_Files = Directory::GetFiles(sDir, FilePrefix + ".txt");
	     }
  
  
  /// List<ULONGLONG>^ Next()/// 	     
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
        List<ULONGLONG>^ Input::Next()
        {  
retry:        
          if ( (m_nSequence++ > m_nCount) || (m_nFiles >= m_Files->Length) ) {
            m_nSequence = 0;
            m_nFiles = 0;
            return nullptr;
          }

          marshal_context ctx;
          
          if (m_fsCurrent == NULL) {
            String^ s=m_Files[m_nFiles];
            m_fsCurrent = new ifstream(ctx.marshal_as<const char*>(s) , ios::in);
          }
          
          // Open file if not open
          // Read Next list until end of file reached

            List<ULONGLONG> l;
            char x[100];
            *m_fsCurrent >> x;

            if (m_fsCurrent->eof()) {
              m_fsCurrent->close();
              m_fsCurrent = NULL;
              m_nFiles++;
              goto retry;
            }
            
            Name = gcnew String(x);
            int num;
            for (int i=0; i<m_nTerms; i++) {        
              *m_fsCurrent >> num;
              l.Add(num);
            }
            return %l;
        }	     
    };
    
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
