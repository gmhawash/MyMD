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
		Synthesis(__int64 bits);
    __int64 GateCount(){return nGate;}
    void Process(__int64 inTerm, __int64 outTerm);
    __int64 Propogate(__int64 inTerm, __int64 outTerm);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
	array<__int64>^ p ;
	array<__int64>^ m ;
	array<__int64>^ c ;
  __int64 nGate;
  __int64 nBits;


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
#pragma endregion
	};
}
