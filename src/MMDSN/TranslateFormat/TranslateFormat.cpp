// TranslateFormat.cpp : main project file.

#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <msclr\marshal.h>


using namespace System;
using namespace System;
using namespace AlgoBucket;
using namespace System::Collections::Generic;
using namespace std;
using namespace msclr::interop;

int main(array<System::String ^> ^args)
{
  // We need to arg on the commmand line
  //
  if (args->Length < 2) {
    Console::WriteLine("Usage: TranslateFormat filespec1 filespec2\n");
    return -1;
  }
  
  // Iterate through all files that match the specs of first command line arg
  FileSrc::Output i(4, args[0]);
  List<UInt64>^ il;
  
  marshal_context ctx;

  array<ofstream*>^ fsList = gcnew array<ofstream*>(1000);

  while ( (il = i.Next()) != nullptr ) {
    Console::WriteLine(i.FileName);

    array<wchar_t>^ delim = {'-'};
    String ^x = i.FileName->Split(delim)[1];
    UInt64 index = Convert::ToUInt64(x) / 100000;
    
    if (fsList[index] == NULL) {
      String^ s = args[1] + "-" + index.ToString() + ".txt";
      fsList[index] = new ofstream(ctx.marshal_as<const char*>(s) , ios::out);
    }
    *fsList[index] << ctx.marshal_as<const char*>(x) << " "; 
    
    for (int i=0; i< il->Count; i++) 
      *fsList[index] << il[i] << " ";
    
    *fsList[index] << "\n";
  }
  
  return 0;
}
