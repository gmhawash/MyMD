
#include "stdafx.h"
#include "baker_per.h"
#include <algorithm>
#include <vector>

namespace AlgoBucket {
  namespace Baker_per {
    /// public ref class Input : Permutation {///     
    ///
 	Input::Input (ULONGLONG nBits) : Permutation(nBits), current_h(hasse(nBits))
		{
			Name = "Baker";
			current_h = hasse(nBits); 
			cout<< "initialized " <<current_h.size()<<"\n";
		}



vector<vector<ULONGLONG>> Input::hasse(ULONGLONG n)
	//this function is to create hasse diagram
	//n number oent f bits
{
	
	vector<vector<ULONGLONG>> band(n+1);
	unsigned x=1;
	for(unsigned  b=0; b<=n;b++)
	{
		if (b==0) band[0].push_back(0); // zero band
		else
			if (b==1) // create ones band
				for(unsigned i=0;i<n;i++) 
				{ 
					band[1].push_back(x);
					x=x<<1; //by shifting 
				}
			else
				for(unsigned i=0;i<n;i++) //iterate on the ones band
					for(unsigned j=0;j<band[b-1].size();j++) //iterate on the previous band 
					{
						x=band[1][i]|band[b-1][j]; // use ones band to add one to the current (mutate the current)
						if ((x!=band[b-1][j])&&(0==count(band[b].begin(),band[b].end(),x)))// it is real mutation(not equal to the previous band) and dos not exist in the new band
								band[b].push_back(x);
					}
		
	sort(band[b].begin(),band[b].end());
	}
return band;
}

/*List<ULONGLONG>^ Input::linear_hasse(vector<vector<ULONGLONG>>* band)
{
	List<ULONGLONG> result;
	vector<vector<ULONGLONG>> b= *band;
	for( int i =0; i<b.size();i++) 
		for(int j =0; j<b[i].size();j++)
		{ ULONGLONG v=b[i][j];
			result.Add(v);
	}
	return %result;
}
*/
List<ULONGLONG>^ Input::Next()
{
	
	List<ULONGLONG> list;
	cout<<current_h.size();
	int bands = current_h.size()-2;
	cout<<bands;//Why?????????????????????????????????????????????????????????????????????/ 
	int i=bands;
	bool endband;
	while((i>0)&&(endband= !next_permutation(current_h[i].begin(),current_h[i].end()))) 
		{
			sort(current_h[i].begin(),current_h[i].end());
			i--;
		}

	if (i==0)
		return nullptr;
	else 
	{
		
		vector<vector<ULONGLONG>> b= current_h;
		for( int i =0; i<b.size();i++) 
			for(int j =0; j<b[i].size();j++)
			{ ULONGLONG v=b[i][j];
				list.Add(v);
				cout<<v;
			}
		
		return %list;
	}
}
  }  
      
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
        return Permutation::Next();
      }	     

	};

  }
