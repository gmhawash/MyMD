// CompareFiles.cpp : main project file.

#include "stdafx.h"

using namespace System;
using namespace AlgoBucket;
using namespace System::Collections::Generic;
using namespace std;

int main(array<System::String ^> ^args)
{
    FileSrc::Output i(4, "Nouraddin-0");
    FileSrc::Output o(4, "Stedman-*");
    
    List<UInt64>^ il = i.Next();
    List<UInt64>^ ol;
    
    while((ol = o.Next()) != nullptr) {
      Console::WriteLine(o.FileName );
      if (il->Equals(ol)) {
        Console::WriteLine("Match Found: " + o.FileName);
        break;
      }
    }
      
    return 0;
}
