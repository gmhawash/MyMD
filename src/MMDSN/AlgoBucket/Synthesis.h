#pragma once

#include "config.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;
using namespace System::Collections::Generic;

namespace AlgoBucket {

	/// <summary>
	/// Summary for Synthesis
	/// </summary>
	public ref class Synthesis 
	{
	public:
		Synthesis(ULONGLONG bits);
    ULONGLONG GateCount(){return nGate;}
    void Process(ULONGLONG inTerm, ULONGLONG outTerm);
    ULONGLONG Propogate(ULONGLONG inTerm, ULONGLONG outTerm);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
  public:
	array<ULONGLONG>^ p ;
	array<ULONGLONG>^ m ;
	array<ULONGLONG>^ c ;
  ULONGLONG nGate;
  ULONGLONG nBits;


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
#pragma endregion
	};
}
