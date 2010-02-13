// AlgoBucket.h

#pragma once
#include "config.h"
using namespace System;

namespace AlgoBucket {

	public interface class AlgoClass
	{
  public: 
    //property String^ Name;
    virtual List<ULONGLONG>^ Next();
    virtual List<ULONGLONG>^ Random();
    property ULONGLONG Count;	
    property String^ FileName;
    property String^ Name;
	};
}
