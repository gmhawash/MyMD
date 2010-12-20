
#include <time.h>

typedef vector<string, allocator<string> > STRINGVECTOR;
typedef vector<ULONGLONG, allocator<ULONGLONG> > ULONGVector;


namespace AlgoBucket {
	public ref class CPermutation
	{
	private:
		ULONGLONG nBits;
		STRINGVECTOR *Cubes;
		ULONGLONG Bits;
		ULONGLONG requestedBits;
		array<vector<ULONGLONG> *>^ m_inArray;
		array<vector<ULONGLONG> *>^ m_outArray;
		
		array<ULONGLONG>^ m ;
		array<ULONGLONG>^ c ;
		ULONGLONG nGate;
		int numberOfHasse;
		int in_position;
		int out_position;
		int numberOfBands;
		

	public:
		List<ULONGLONG> in_list;
		List<ULONGLONG> out_list;
		array<int>^ fitness;
		CPermutation(ULONGLONG);
		STRINGVECTOR* duplicateVector(STRINGVECTOR arrayOfVectors[], int , int );
		STRINGVECTOR generateVector(int );
		int power(ULONGLONG );
		STRINGVECTOR *generateCube(STRINGVECTOR , ULONGLONG );
		int numberOfOnes(string s, ULONGLONG );
		STRINGVECTOR* insertZeroAndOne(STRINGVECTOR Cubes[], int , int );
		int convertToDecimal(string );
		STRINGVECTOR *generatePermutation(STRINGVECTOR cubes[], int );
		void Synthesise();
		void print();
		void openFile(ULONGLONG nBits, String^ FilePrefix);
		void process(ULONGLONG inTerm, ULONGLONG outTerm);
		ULONGLONG QuantumCost();
		ULONGLONG Propogate(ULONGLONG outTerm);
		ULONGLONG GateCost(int i);
		ULONGLONG ControlLines(ULONGLONG n);
		void next();
		bool nextOut();
		void printList();
		void resetPosition();
		ULONGVector *getHasse(int);
		void crossOver(ULONGVector Hasse1[], ULONGVector Hasse2[]);

		//int CalcFitness()
		//{
			//Synthesize();
			//int fitness = GateCost();
		//}
	};


CPermutation::CPermutation(ULONGLONG nBits)
{
	numberOfHasse = 10;
	in_position = 0;
	out_position = 0;
	fitness = gcnew array<int>(numberOfHasse);
	nGate = 0;
	requestedBits = 1;
	STRINGVECTOR theVector;
	this->nBits =nBits;
	Bits = nBits-requestedBits;
	theVector = generateVector(Bits);
	STRINGVECTOR *arrayOfVectors = new STRINGVECTOR[Bits + 1];
	arrayOfVectors = generateCube(theVector, Bits);

	numberOfBands = power(requestedBits) * (Bits+1);
	Cubes = new STRINGVECTOR[numberOfBands];
	Cubes = duplicateVector(arrayOfVectors, requestedBits, Bits);
	if(requestedBits>0) Cubes = insertZeroAndOne(Cubes, requestedBits, Bits);
	m_inArray = gcnew array<vector<ULONGLONG>*>(numberOfBands*numberOfHasse); // array of pointers to vector (+1 means 1 more band hasse diagram)

	int counter = 0;
	for(int k=0;k<numberOfHasse;k++)
	{
		Cubes = generatePermutation(Cubes,numberOfBands);
		for(int i=0;i<numberOfBands;i++)
		{
			m_inArray[counter] = new vector<ULONGLONG>(Cubes[i].size());
				for(int j=0;j<Cubes[i].size();j++)
					m_inArray[counter]->at(j)=convertToDecimal(Cubes[i].at(j));
			counter++;
		}
	}

	for(int i=0;i<power(nBits);i++)
	{
		in_list.Add(0);
		out_list.Add(0);
	}
	m = gcnew array<ULONGLONG>(Bits*1024);
	c = gcnew array<ULONGLONG>(Bits*1024);
}

STRINGVECTOR* CPermutation::duplicateVector(STRINGVECTOR arrayOfVectors[], int requestedBits, int bits)
	{
		STRINGVECTOR* Cube = new STRINGVECTOR[numberOfBands];
		int counter = 0;
		for(int j=0;j<power(requestedBits);j++)
		for(int i=0;i<bits+1;i++)
		{
			Cube[counter] = arrayOfVectors[i];
			counter++;
		}
		return Cube;
	}
STRINGVECTOR CPermutation::generateVector(int bits)
	{
		char buffer[20];
		STRINGVECTOR theVector;
		for(int i=0;i<power(bits);i++)
			theVector.push_back(itoa(i,buffer,2));
		for(int i=0;i<power(bits);i++)
		{
			int size = bits - theVector.at(i).size();
			if(size<bits)
				for(int j=0;j<size;j++)
					theVector.at(i).insert(0,"0");
		}
		return theVector;
	}
int CPermutation::power(ULONGLONG a)
	{
		ULONGLONG p=1;
		for(int i=1;i<=a;i++)
			p *= 2;
		return p;
	}
STRINGVECTOR * CPermutation::generateCube(STRINGVECTOR theVector, ULONGLONG bits)
	{
		STRINGVECTOR *arrayOfVectors = new STRINGVECTOR[bits + 1];
		ULONGLONG number;
		for(int i=0;i<power(bits); i++)
		{
			number = numberOfOnes(theVector.at(i), bits);
			arrayOfVectors[number].push_back(theVector.at(i));
		}
	
		return arrayOfVectors;
	}
int CPermutation::numberOfOnes(string s, ULONGLONG nBits)
	{
		int counter = 0;
		for(int i=0;i<nBits;i++)
		{
			if(s[i]=='1') counter++;
		}
		return counter;
	}
STRINGVECTOR* CPermutation::insertZeroAndOne(STRINGVECTOR Cubes[], int requestedBits, int bits)
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
int CPermutation::convertToDecimal(string x)
	{
		int value=0;
		int position=0;
		for(int i = x.size()-1;i>=0;i--)
		{
			if(x[i]=='1') value = value + pow(2, double(position++)); else position++;
		}
		return value;
	}
STRINGVECTOR * CPermutation::generatePermutation(STRINGVECTOR cubes[], int sequences)
	{
		srand ( time(NULL) );
		int r;
		do
			r = rand() % (sequences);
		while(cubes[r].size()==1);
			random_shuffle(cubes[r].begin(), cubes[r].end());
		return cubes;
	}
void CPermutation::openFile(ULONGLONG nBits, String^ FilePrefix)
	{
		 ifstream *fs;
		int m_nSequence=0; 
		ULONGLONG m_nTerms;
		array<String^, 1>^ m_Files;
		String^ FileName;
		 m_nTerms = (ULONGLONG)Math::Pow(2,nBits);
		String^ sDir = "..\\" + nBits.ToString() + "-bits-src";
        if ( !Directory::Exists(sDir) )
          throw gcnew Exception(sDir + " Does not exist");
          
        m_Files = Directory::GetFiles(sDir, FilePrefix + ".txt");
		String^ s = m_Files[m_nSequence++];
        String^ delim = "\\.";
        array<String^,1>^ list = s->Split(delim->ToCharArray());
        FileName = list[list->Length-2];
		marshal_context ctx;
        fs = new ifstream (ctx.marshal_as<const char*>(s) , ios::in);
        
        
		m_outArray = gcnew array<vector<ULONGLONG>*>(10);
        char x[100];
		int counter=0;
          while(!(fs->eof()))  
          {
			  *fs >> x ;
			  m_outArray[counter] = new vector<ULONGLONG>;
			  for(int i=0;i<power(nBits);i++)
			  {
				*fs >> x ;
				if(strlen(x)!=0) m_outArray[counter]->push_back(atoi(x));
				//out_list.Add(atoi(x));
			  }
			  counter++;
		  }
		  fs->close();
	}
void CPermutation::print()
	{
		for(int i=0;i<power(requestedBits)*(Bits+1)*numberOfHasse;i++)
			{
				for(int j=0;j<m_inArray[i]->size();j++)
					cout << m_inArray[i]->at(j) << "  ";
				cout << endl;
			}
	}
void CPermutation::process(ULONGLONG inTerm, ULONGLONG outTerm)
	{
		outTerm = Propogate(outTerm);
		ULONGLONG diff = inTerm ^ outTerm;
		
		if (diff > 0) {
			// Flip the 0 bits first
			ULONGLONG mask = 1;
			for (int j = 0; j< nBits; j++) {
				if ( (diff & mask) && !(outTerm & mask)) {
					if (nGate > c->Length - 8)  // make sure there is enough memory
						c->Resize(c, c->Length + 1024), m->Resize(m, c->Length + 1024);
						
					c[nGate] = outTerm & ~mask;		// clear inTerm bit at current bit position
					m[nGate++] = mask;						// Mark bit position for as NOT gate
					diff &= ~mask;								// reset bit in diff, since it should be same in outTerm
					outTerm ^= mask;
				}
				mask <<= 1;
			}
			
			mask = 1;
			// Flip the 1 bits second
			for (int j = 0; j< nBits; j++) {
				if ( (diff & mask) && (outTerm & mask)) {
					if (nGate > c->Length - 8)  // make sure there is enough memory
						c->Resize(c, c->Length + 1024), m->Resize(m, c->Length + 1024);

					c[nGate] = outTerm & ~mask;		// clear inTerm bit at current bit position
					m[nGate++] = mask;						// Mark bit position for as NOT gate
					diff &= ~mask;								// reset bit in diff, since it should be same in outTerm
					outTerm ^= mask;
				}
				mask <<= 1;
			}
			
		}	
	}
ULONGLONG CPermutation::Propogate(ULONGLONG outTerm)
	{
		// Apply current list of gates..
		for (ULONGLONG i=0; i<nGate; i++) {
			ULONGLONG x = outTerm & c[i];
			if (x == c[i])
				outTerm ^= m[i];
		}
		return outTerm;
	}  
ULONGLONG CPermutation::QuantumCost()
	{
		int nCost=0;
		
		for (int i=0; i<nGate; i++) 
			nCost += GateCost(i);
			
		return nCost;
	}
ULONGLONG CPermutation::GateCost(int i)
	{
		return Math::Max((int)1, (int)Math::Pow(2, 1 + ControlLines(c[i])) - 3);
	}
ULONGLONG CPermutation::ControlLines(ULONGLONG n)
	{
		ULONGLONG nCount=0;
		
		for (int i=0; i<nBits; i++) 
			nCount += (n >> i) & 1;
			
		return nCount;
	}
void CPermutation::next()
	{
		int someCounter = 0;
		for(int i=in_position * numberOfBands;i<in_position * numberOfBands+numberOfBands;i++)
		{
				for(int j=0;j<m_inArray[i]->size();j++)
				{
					in_list[someCounter++]=m_inArray[i]->at(j);
				}
		}
		in_position++;
	}
	
bool CPermutation::nextOut()
{
	if(m_outArray[out_position]->size())
	{
		for(int i=0;i<m_outArray[out_position]->size();i++)
			out_list[i]=m_outArray[out_position]->at(i);
		out_position++;
		return true;
	}
	return false;
}
void CPermutation::printList()
	{
		for(int i=0;i<power(nBits);i++)
			cout << in_list[i]<< "  ";
	}
void CPermutation::resetPosition()
{
	in_position = 0;
}
ULONGVector * CPermutation::getHasse(int pos)
{
	pos--;
	ULONGVector* Hasse = new ULONGVector[numberOfBands];
	int counter=0;
	for(int i=pos * numberOfBands; i< pos * numberOfBands + numberOfBands; i++)
	{
		for(int j=0;j<m_inArray[i]->size();j++)
		{
			Hasse[counter].push_back(m_inArray[i]->at(j));
		}
		counter++;
	}
	return Hasse;
}
void CPermutation::crossOver(ULONGVector Hasse1[], ULONGVector Hasse2[])
{
	srand ( time(NULL) );
	int r = rand() % (numberOfBands);
	ULONGVector temp;
	for(int i= r; i< numberOfBands-i;i++)
	{
		temp = Hasse1[i];
		Hasse1[i] = Hasse2[i];
		Hasse2[i] = temp;
	}
}
void CPermutation::Synthesise()
	{

	}
}