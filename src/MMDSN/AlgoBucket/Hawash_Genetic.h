#pragma once
#include "stdafx.h"
#include <iostream>
#include "permutation.h"
#include <string>
#include <time.h>

using namespace AlgoBucket;
using namespace System;
using namespace msclr::interop;
using namespace System::Collections::Generic;
using namespace std;
using namespace System::Runtime::InteropServices;
using namespace System::IO;



typedef vector<string, allocator<string> > STRINGVECTOR;
typedef vector<int, allocator<int> > INTVECTOR;
typedef vector<ULONGLONG , allocator<ULONGLONG>> ULONGVECTOR;

ULONGLONG someCounter=0;
ULONGLONG numberOfChromosomes = 50;

namespace AlgoBucket {
	namespace Hawash_Genetic {
		/// public ref class Input : Permutation {///     
		///
		public ref class Input : Permutation {
			List<array<UInt64>^>^ m_inputArray;
			array<int>^ m_permList;
			array<vector<ULONGLONG> *>^ m_inArray;

		public:


			Input (ULONGLONG nBits) : Permutation(nBits)
			{

				SeqName = "Hawash_Genetic";
				int requestedBits;
				cout << "Enter number of most significant bits:";
				cin>> requestedBits;

				STRINGVECTOR theVector, anotherVector, permutationVector;
				int bits = nBits-requestedBits;

				srand ( unsigned ( time (NULL) ) );

				theVector = generateVector(bits);
				STRINGVECTOR *arrayOfVectors = new STRINGVECTOR[bits + 1];
				arrayOfVectors = generateCube(theVector, bits);

				STRINGVECTOR *Cubes = new STRINGVECTOR[power(requestedBits) * (bits+1)];
				Cubes = duplicateVector(arrayOfVectors, requestedBits, bits);

				Cubes = insertZeroAndOne(Cubes, requestedBits, bits);

				m_inArray = gcnew array<vector<ULONGLONG>*>(power(requestedBits)*(bits+1)*numberOfChromosomes); // array of pointers to vector (+1 means 1 more band hasse diagram)

				for(int i=0;i<power(requestedBits)*(bits+1);i++)
				{
					m_inArray[i] = new vector<ULONGLONG>(Cubes[i].size());
					for(int j=0;j<Cubes[i].size();j++)
						m_inArray[i]->at(j)=convertToDecimal(Cubes[i].at(j));
				}
				someCounter = power(requestedBits) * (bits+1)-1;
				for(int i=1;i<numberOfChromosomes;i++)
				{
					Cubes = generatePermutation(Cubes, power(requestedBits) * (bits+1));
					for(int k=0;k<power(requestedBits)*(bits+1);k++)
					{
						someCounter++;
						m_inArray[someCounter] = new vector<ULONGLONG>(Cubes[k].size());
						for(int j=0;j<Cubes[k].size();j++)
							m_inArray[someCounter]->at(j)=convertToDecimal(Cubes[k].at(j));
					}
				}



				for(int i=0;i<power(requestedBits)*(bits+1)*numberOfChromosomes;i++)
				{
				for(int j=0;j<m_inArray[i]->size();j++)
				cout << m_inArray[i]->at(j) << "  ";
				cout << endl;
				}

				int *fitness = new int[numberOfChromosomes];

				m_nPermutations = 1;
				for each (vector<ULONGLONG>* a in m_inArray)
					m_nPermutations *= a->size();


				// up to this point m_inArray contains 50 different chromosomes
				// put the necessary code to synthesis these 50 chromosomes
				// and put the quantum number inside the fitness array, then
				// I will continue.

				synthesis(nBits);





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
			void synthesis(ULONGLONG nBits)
			{
				/*
				FileSrc::Input  outp(nBits, "function*", 50);
				List<ULONGLONG>^ il;
				List<ULONGLONG>^ ol;




				ULONGLONG t1, t2, freq;
				ULONGLONG t3;
				QueryPerformanceFrequency((LARGE_INTEGER*)&freq); // what this for? is it to calculte times?
				QueryPerformanceCounter((LARGE_INTEGER*)&t1);// more accurate that time interrupt

				marshal_context ctx;  //what is this for? conversion between old string definition and managed classes

				String^ sDir = nBits.ToString() + "-bits";
				if ( !Directory::Exists(sDir) )
				Directory::CreateDirectory(sDir);

				// Create summary file
				String^ s = sDir + "\\Summary-" + inp->Name +  DateTime::Now.ToBinary() + "-" + nBits.ToString() +  ".csv" ;
				ofstream fss(ctx.marshal_as<const char*>(s) , ios::out);  //what is this? to write srtings on file stream
				fss << "Input,Output, Ticks, Time, Min Gates, Total Time\n"  ;
				String^ oss;


				UInt64 nMin;

				*/

			}

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
			STRINGVECTOR generateVector(ULONGLONG bits)
			{
				STRINGVECTOR theVector;
				char buffer[10];
				for(ULONGLONG i=0;i<power(bits);i++)
					theVector.push_back(itoa(i,buffer,2));

				for(ULONGLONG i=0;i<theVector.size();i++)
				{
					ULONGLONG length = theVector.at(i).length();
					for(ULONGLONG j=1;j<=bits-length;j++)
						theVector.at(i).insert(0,"0");
				}

				return theVector;
			}
			STRINGVECTOR* duplicateVector(STRINGVECTOR arrayOfVectors[], int requestedBits, int bits)
			{
				STRINGVECTOR* Cube = new STRINGVECTOR[power(requestedBits)*(bits+1)];
				int counter = 0;
				for(int j=0;j<power(requestedBits);j++)
					for(int i=0;i<bits+1;i++)
					{
						Cube[counter] = arrayOfVectors[i];
						counter++;
					}
					return Cube;
			}
			STRINGVECTOR* insertZeroAndOne(STRINGVECTOR Cubes[], int requestedBits, int bits)
			{
				int counter = 0;
				char buffer[20];
				for(int j=0;j<power(requestedBits);j++)
					for(int i=0;i<bits+1;i++)
					{
						for(int k=0;k<Cubes[counter].size();k++)
							Cubes[counter].at(k).insert(0,itoa(j,buffer,2));
						counter++;
					}

					return Cubes;
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
			STRINGVECTOR *duplicateCubeFourTimes(STRINGVECTOR theCube[], ULONGLONG bits)
			{
				STRINGVECTOR *theCubeFourTimes = new STRINGVECTOR[4*(bits+1)];
				int counter=0;
				for(int k=1;k<=4;k++)
					for(int i=0;i<bits+1;i++)
					{
						for(int j=0;j<theCube[i].size();j++)
							theCubeFourTimes[counter].push_back(theCube[i].at(j));
						counter++;
					}
					return theCubeFourTimes;
			}
			STRINGVECTOR *insertZeroAndOne(STRINGVECTOR theVector[], ULONGLONG bits)
			{
				for(int i=0;i<bits+1;i++)
					for(int j=0;j<theVector[i].size();j++)
						theVector[i].at(j).insert(0,"00");

				for(int i=bits+1;i<2*(bits+1);i++)
					for(int j=0;j<theVector[i].size();j++)
						theVector[i].at(j).insert(0,"01");

				for(int i=2*(bits+1);i<3*(bits+1);i++)
					for(int j=0;j<theVector[i].size();j++)
						theVector[i].at(j).insert(0,"10");

				for(int i=3*(bits+1);i<4*(bits+1);i++)
					for(int j=0;j<theVector[i].size();j++)
						theVector[i].at(j).insert(0,"11");



				return theVector;
			}

			STRINGVECTOR *generateCube(STRINGVECTOR theVector, ULONGLONG bits)
			{
				 STRINGVECTOR*arrayOfVectors = new STRINGVECTOR[bits + 1];
				ULONGLONG number;
				for(int i=0;i<power(bits); i++)
				{
					number = numberOfOnes(theVector.at(i), bits);
					arrayOfVectors[number].push_back(theVector.at(i));
				}

				return arrayOfVectors;
			}

			STRINGVECTOR *generatePermutation(STRINGVECTOR cubes[], int sequences)
			{
				int r;
				do
				r = rand() % (sequences);
				while(cubes[r].size()==1);
				next_permutation(cubes[r].begin(), cubes[r].end());
				return cubes;
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


