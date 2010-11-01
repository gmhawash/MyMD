#pragma once
#include "stdafx.h"
#include <iostream>
#include "permutation.h"
#include <string>


using namespace System;
using namespace System::Collections::Generic;
using namespace std;
using namespace System::Runtime::InteropServices;
using namespace System::IO;

typedef vector<string, allocator<string> > STRINGVECTOR;
typedef vector<int, allocator<int> > INTVECTOR;
typedef vector<ULONGLONG , allocator<ULONGLONG>> ULONGVECTOR;

namespace AlgoBucket {
	namespace Hawash_one_bit {
		/// public ref class Input : Permutation {///     
		///
		public ref class Input : Permutation {
			List<array<UInt64>^>^ m_inputArray;
			array<int>^ m_permList;
			array<vector<ULONGLONG> *>^ m_inArray;

		public:

			Input (ULONGLONG nBits) : Permutation(nBits)
			{

				Name = "Hawash_one_bit";
        SeqName = Name;
				STRINGVECTOR theVector, anotherVector, permutationVector;
				theVector= generateTwoBitsVector();
				anotherVector = generateTwoBitsVector();

				for(int i=2;i<nBits;i++)
				{
					theVector = randomizeVector(theVector);
					anotherVector = randomizeVector(anotherVector);
					theVector = rearrangeBands(theVector);
					anotherVector = rearrangeBands(anotherVector);
					theVector = duplicateVector(theVector, anotherVector);
					theVector = insertZeroAndOne(theVector);
					anotherVector = theVector;
				}

				STRINGVECTOR *arrayOfVectors = new STRINGVECTOR[nBits * 2];
				arrayOfVectors = generateCube(arrayOfVectors, theVector, nBits);

				m_inArray = gcnew array<vector<ULONGLONG>*>(2 * nBits); // array of pointers to vector (+1 means 1 more band hasse diagram)
				for(int i=0;i<2 * nBits;i++)
				{
					m_inArray[i] = new vector<ULONGLONG>(arrayOfVectors[i].size());
					for(int j=0;j<arrayOfVectors[i].size();j++)
						m_inArray[i]->at(j)=convertToDecimal(arrayOfVectors[i].at(j));
				}


				m_nPermutations = 1;
				for each (vector<ULONGLONG>* a in m_inArray)
						m_nPermutations *= Factorial(a->size());
			}

		public:
			array<vector<ULONGLONG>*>^ get()
			{
				return m_inArray;
			}

			/// void MakeList(ULONGLONG nBits, ULONGLONG nIndex)/// 
			///
			/// Inputs:
			///
			/// Outputs:
			///

			int convertToDecimal(string x)
			{
				int value=0;
				int position=0;
				for(int i = x.size()-1;i>=0;i--)
				{
					if(x[i]=='1') value = value + pow(2, double(position++)); else position++;
				}
				return value;
			}
			STRINGVECTOR generateTwoBitsVector()
			{
				STRINGVECTOR theVector;
				theVector.push_back("00");
				theVector.push_back("01");
				theVector.push_back("10");
				theVector.push_back("11");

				return theVector;
			}
			STRINGVECTOR randomizeVector(STRINGVECTOR theVector)
			{
				random_shuffle(theVector.begin(), theVector.end());
				return theVector;
			}

			STRINGVECTOR rearrangeBands(STRINGVECTOR theVector)
			{
				INTVECTOR Positions;
				int length = theVector.size();
				for(int i=0;i<length;i++)
				{
					int counter = 0;
					for(int j=0;j<theVector[i].size();j++)
					{
						if(theVector[i].at(j)=='1') counter++;
					}
					Positions.push_back(counter);
				}
				for (int i=0;i<length;i++)
				{
					for(int j=i;j<length;j++)
					{
						if(Positions.at(j)<Positions.at(i)) 
						{
							swap(Positions.at(j),Positions.at(i));
							swap(theVector.at(j), theVector.at(i));
						}
					}
				}
				return theVector;
			}

			int power(ULONGLONG a)
			{
				ULONGLONG p=1;
				for(int i=1;i<=a;i++)
					p *= 2;
				return p;
			}
			STRINGVECTOR duplicateVector(STRINGVECTOR theVector, STRINGVECTOR anotherVector)
			{
				int length=theVector.size();
				for(int i=0;i<length;i++)
					theVector.push_back(anotherVector.at(i));
				return theVector;
			}
			STRINGVECTOR insertZeroAndOne(STRINGVECTOR theVector)
			{
				int length=theVector.size();
				for(int i=0;i<length/2;i++)
					theVector.at(i).insert(0,"0");
				for(int i=length/2;i<length;i++)
					theVector.at(i).insert(0,"1");
				return theVector;
			}

			STRINGVECTOR *generateCube(STRINGVECTOR arrayOfVectors[], STRINGVECTOR theVector, ULONGLONG nBits)
			{
				ULONGLONG number;
				for(int i=0;i<power(nBits)/2; i++)
				{
					number = numberOfOnes(theVector.at(i), nBits);
					arrayOfVectors[number].push_back(theVector.at(i));
				}
				ULONGLONG counter = number;
				for(int i=power(nBits)/2;i<power(nBits); i++)
				{
					number = numberOfOnes(theVector.at(i), nBits);
					arrayOfVectors[number + counter].push_back(theVector.at(i));
				}
				return arrayOfVectors;
			}

			int numberOfOnes(string s, ULONGLONG nBits)
			{
				int counter = 0;
				for(int i=0;i<nBits;i++)
				{
					if(s[i]=='1') counter++;
				}
				return counter;
			}

			void MakeList(ULONGLONG nBits, ULONGLONG nIndex) // band index
			{
				if (nIndex >= nBits)
					return; 

				m_inArray[nIndex] = new vector<ULONGLONG>(Combination(nBits, nIndex));  // this array contains a combination for a sequence of bits
				vector<ULONGLONG>* last = m_inArray[nIndex-1];  //last points to the previous band

				int k=0;
				// what do the following loops for? does it generate permutations for a sequence of numbers?
				//generates the permutations and saves them inside the vectors
				for (int i=0; i< last->size(); i++) {
					ULONGLONG l = last->at(i);
					for(int j=0; j< nBits; j++) {
						if (! (last->at(i) & (1<<j)) ) {
							ULONGLONG newValue = last->at(i)| (1<<j);
							vector<ULONGLONG>::iterator it =  find(m_inArray[nIndex]->begin(), m_inArray[nIndex]->end(), newValue);
							if (it == m_inArray[nIndex]->end())
								m_inArray[nIndex]->at(k++) = newValue;
						}
					}
				}

				MakeList(nBits, nIndex+1);
			}


			/// bool Exists(array<UInt64>^ list, ULONGLONG value)/// 
			///
			/// Inputs:
			///
			/// Outputs:
			///
			bool Exists(array<UInt64>^ list, ULONGLONG value)
			{
				for each (UInt64 v in list)
					if (value == v)
						return true;

				return false;
			}

			/// ULONGLONG Input::Combination(ULONGLONG n, ULONGLONG p)/// 	     
			///
			/// Inputs:
			///
			/// Outputs:
			///
			ULONGLONG Input::Combination(ULONGLONG n, ULONGLONG p)//calculation of combination (select p elements out of n)
			{
				ULONGLONG res=1;
				for (ULONGLONG i=n-p+1; i<=n; i++)
					res *= i;

				res /= Permutation::Factorial(p);
				return res;
			}

			/// List<ULONGLONG>^ Permutation::Next()/// 
			///
			/// Inputs:
			///
			/// Outputs:
			///
			List<ULONGLONG>^ Input::Next()
			{
				if (m_CurrentTerm++ >= m_nPermutations) {
					m_CurrentTerm = 0;
					return nullptr;
				}

				List<ULONGLONG> list;

				if (m_permList == nullptr)
					m_permList = gcnew array<int>(m_inArray->Length);

				int carry = 1;
				for (int i = 0; i < m_inArray->Length; i++) {
					if (carry) {
						++m_permList[i] ;
						carry = m_permList[i] / m_inArray[i]->size();
						m_permList[i] %=  m_inArray[i]->size();
						next_permutation(m_inArray[i]->begin(), m_inArray[i]->end());
					}

					// Add the current list to array
					for (int j=0; j < m_inArray[i]->size(); j++)
						list.Add(m_inArray[i]->at(j));
				}          
				return %list;
			}


			List<ULONGLONG>^ Input::Random()
			{
				// Select which band to shuffle randomly..
				System::Random rnd;
				int x = rnd.Next(m_inArray->Length);  //does this select a band from m_inArray?

				random_shuffle(m_inArray[x]->begin(), m_inArray[x]->end());  //how does this work? what do begin and end mean in terms of the hasse diagram?

				// Copy list back
				List<ULONGLONG> list;  //does this list brings one of the permutations?
				for (int i = 0; i < m_inArray->Length; i++) 
					for (int j=0; j < m_inArray[i]->size(); j++)
						list.Add(m_inArray[i]->at(j));

				return %list;
			}



			/// bool Valid(List<ULONGLONG>^ list)///   
			///
			/// Inputs:
			///
			/// Outputs:
			///
			bool Input::Valid(List<ULONGLONG>^ list)
			{
				for(ULONGLONG i=1; i< list->Count; i++)
					for(ULONGLONG j=0; j < i; j++) { 
						//    for (ULONGLONG n = 0; n < m_nBits; n++) {
						//      ULONGLONG mask = ~(1 << n);
						ULONGLONG control_i = list[i]; //& mask;
						ULONGLONG control_j = list[j]; //& mask;
						if ( control_i  == (control_i & control_j))
							return false;
					}

					return true;
			}
		};

		public ref class Output : Permutation {
		public:
			Output (ULONGLONG nBits) : Permutation (nBits) {

			}
			/// List<ULONGLONG>^ Permutation::Next()/// 
			///
			/// Inputs:
			///
			/// Outputs:
			///
			List<ULONGLONG>^ Output::Next()
			{
				return Permutation::Random();
			}	     

		};

	}
}
