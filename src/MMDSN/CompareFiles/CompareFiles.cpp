// CompareFiles.cpp : main project file.

#include "stdafx.h"

using namespace System;
using namespace AlgoBucket;
using namespace System::Collections::Generic;
using namespace std;

bool Same(List<UInt64>^ il, List<UInt64>^ ol);
int main(array<System::String ^> ^args)
{
    FileSrc::Output i(4, args->Length > 0 ? args[0] : "Nouraddin-0");
    FileSrc::Output o(4, args->Length > 1 ? args[1] : "Stedman-*");

    List<UInt64>^ il;
    List<UInt64>^ ol;
    
    while ( (il = i.Next()) != nullptr ) { 
      Console::WriteLine(i.FileName );
    while((ol = o.Next()) != nullptr) {
      Console::WriteLine(o.FileName + "\r");
      if ((i.FileName != o.FileName) && Same(il, ol)) {
        Console::WriteLine("Match Found: " + i.FileName + " " + o.FileName);
        return 0;
      }
    }
    }
      
    return 0;
}


bool Same(List<UInt64>^ il, List<UInt64>^ ol)
{

  for (int i=0; i< il->Count; i++) {
    if (il[i] != ol[i])
      return false;
  }

  return true;
}
