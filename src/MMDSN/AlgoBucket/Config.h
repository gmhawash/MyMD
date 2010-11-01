#pragma once
#include "stdafx.h"

// Defines
/// 
#define NBITS 11
#define BUF_SIZE NBITS * (1LL << NBITS)

/// Macros
#define Bit(n) ((currentTerm >> n) & 1)
#define Not(n) ( ~Bit(n) & 1)
#define SetBit(n,v) ( (v & 1) << n) 

#include <iostream>
#include <windows.h>
#include <vector>
#include <algorithm>
#include <functional>
#include <math.h>
#include <msclr\marshal.h>

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
using namespace System::Collections::Generic;
using namespace std;
using namespace msclr::interop;
