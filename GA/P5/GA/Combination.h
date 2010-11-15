#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include "thread.h"
//#include "stdCombination.h"
#include "qurandom.h"

//using namespace stdcomb;
using namespace std;
using namespace System;
using namespace System::IO;

#define TRY2


#ifdef TRY2
 #define N_RUNS 50
 #define N_GENS 1000
 #define N_POP  200
 #define A 0.1
 #define B 0.25
#define PM 0.05
#define PC 0.45
#endif


#ifdef TRY1
 #define N_RUNS 20
 #define N_GENS 1000
 #define N_POP  200
 #define A 0.1
 #define B 0.25
#define PM 0.05
#define PC 0.6
#endif




#define CROSS_OVER CrossOverOnePoint
#define COUT m_f

 
class CCombination
{
public:
  int m_nodes[40];
  UINT64 m_grid[40];
  UINT64 m_mask;
  int m_ones[256];
  int m_minCount;
  UINT64 m_minCombo;
  UINT64 m_pop[2*N_POP];
  int m_nTotalEdges;
  int m_ParentTotalFitness;
  int m_BestFitness;
  ofstream m_f;
  
  ///   CCombination(void)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  CCombination(void)
  {
    QuRandom::Fill();
    
    // Initialize Ones Counter
    for (int i=0; i<256; i++) {
      int nCount = 0;
      for (int j=0; j<8; j++)
        nCount += (i>>j) & 1;
        
      m_ones[i] = nCount;
    }
  
    m_mask = 0;
    for (int i=0; i<40; i++) {
      m_mask |= 1ULL << i; 
      m_nodes[i] = i;
      m_grid[i] = 0;
    }
    
    // Read graph into grid.  There are 40 grid elements, each 40 bits long.
    // Each node is represented by a row,col matrix which has a '1' when the 
    // two nodes are connected.   The matrix is symmetric around the diagonal
    //
    ifstream fs("graph.txt", ios::in);
    
    int n1, n2;
    m_nTotalEdges = 0;
    while (!fs.eof()) {
      m_nTotalEdges ++;
      fs >> n1 >> n2;
      n1--,n2--;
      cout << n1 << " " << n2 << endl;
      m_grid[n2] |= 1ULL << n1;
      m_grid[n1] |= 1ULL << n2;
    }

    fs.close();

    // Output for human eyes..
    for (int i=0; i<40; i++) {
      cout << endl;
      for (int j=0; j<40; j++)
        cout << ((m_grid[i] >> j) & 1) << ", ";
    }
    cout << endl << "Total Edges: " << m_nTotalEdges << endl;
  }
  
  
  ///   int Fitness(UINT64 solution)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  int Fitness(UINT64 solution)
  {
      int P1 = A * Bisections(solution);
      int P2 = B * Math::Pow(abs(CountOnes(solution)-20), 2);
      //COUT << "Penalty 1: " << P1 << " Penalty 2: " << P2 << endl ;
      return (int)(m_nTotalEdges - P1 - P2);
  }
  
  
  ///   int Bisections(UINT64 solution)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  int Bisections(UINT64 solution)
  {
    int ones[40];
    
    // Enumerate the bits which has one in them.  Basically selecing the rows of the
    // matrix which are only ones. (group 1)
    int nOnes=0;
    for (int i=0, j=0; i<40; i++) 
      if ( (solution >> i) & 1) {
        nOnes++;
        ones[j++] = i;
      }

    // mask will have a 1 for bits in solution which has zero in them (group 2)
    UINT64 mask = ~solution & m_mask;

    // A simple and will leave the bits in this solution as a 1, indicating which members of 
    // group 1 are connected to group 2
    int nCount=0;
    for (int i=0; i<nOnes; i++) {
      nCount += CountOnes(m_grid[ones[i]] & mask);
    }
    
    if (nOnes == 20 && nCount < m_minCount) {
      m_minCombo = mask;
      m_minCount = nCount;
      COUT << " Min Bisection: " << m_minCount << " Mask: " << mask << endl;
    }
    return nCount;
  }
  
  ///   void SolveWithGA()
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  void SolveWithGA()
  {
    for (int i=0; i<N_RUNS; i++)
      DoRun(i);
  }
  
  
  ///   void DoRun(int gen)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  void DoRun(int gen)
  {
    char szTmp[20];

    m_BestFitness = 0;
    m_minCount = 5000;  
    
    sprintf_s(szTmp, "OutputGA %d.txt", gen);
    
    if (m_f.is_open()) m_f.close();
    
    m_f.open( szTmp, ofstream::out);

    // initialize population
    Init();
    
    for (int i=0; i<N_GENS; i++) {
      m_ParentTotalFitness = TotalFitness();
      
      COUT << "Gen: " << i << " Best Fit: " << m_BestFitness << " Min Edges: " << m_minCount << "\n";
    
      for (int j=0; j<N_POP; j++) {
        Breed(j);
      }

      // terminate parents
      for (int j=0; j<N_POP; j++) {
        m_pop[j] = m_pop[j+N_POP];
      }
    }
    m_f.close();
  }

 ///   double TotalFitness()
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  int TotalFitness()
  {
    int total=0;
    for (int i=0; i< N_POP; i++) {
      int fitness = Fitness(m_pop[i]);
      total += fitness;
      if (fitness > m_BestFitness) m_BestFitness = fitness;
    }
  
    return total;
  }
  
  
  ///   UINT64 Roulette()
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  UINT64 Roulette()
  {
    double rnd = QuRandom::NextDouble();
    double val=0;

    for (int i=0; i < N_POP; i++) {
      double fitness = Fitness(m_pop[i]);
      val += fitness/m_ParentTotalFitness;
      if (rnd < val)
        return m_pop[i];
    }
    
    return m_pop[N_POP-1];
  }  


  ///   void Breed(int j)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  void Breed(int j)
  {
    UINT64 p1 = Roulette();
    UINT64 p2 = Roulette();
    m_pop[N_POP+j] = CROSS_OVER(p1, p2, PC);
    
    Mutate(m_pop[N_POP+ j]);
  }
  

  
  ///   UINT64 CrossOverOnePoint(UINT64 p1, UINT64 p2, double Pc)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  UINT64 CrossOverOnePoint(UINT64 p1, UINT64 p2, double Pc)
  {
    // If Above cross over probablity, return best fit parent
    if (QuRandom::NextDouble() > Pc)
      return Fitness(p1) > Fitness(p2) ? p1 : p2;
    
    int nFirst  = QuRandom::Next(37)+1;
    UINT64 mask = -1ULL >> (64-nFirst);
    
    return p1 & mask | p2 & ~mask;
  }
  
  void Mutate(UINT64 &indv)
  {
    int bit = QuRandom::Next(40);
      indv ^= 1ULL << bit;
      
    return;
    
    for (int i=0; i<40; i++)
      if (QuRandom::NextDouble() < PM) 
        indv ^= 1ULL << i;
  }
 
   
  ///    void Init()
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  void Init()
  {
    // Allocate 40 bits for each individual in population..
    //
    for (int i=0; i<N_POP; i++) {
      m_pop[i] = (_int64)QuRandom::Next64() & m_mask;
    }
  }
  
  ///   void Process(int* begin, int* end)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  void Process(int* begin, int* end)
  {
    UINT64 mask=0;
    
    gCounter ++;
    // Build mask for group 1
    for (int* p=begin; p < end; p++)
      mask |= 1ULL << *p;
      
    mask = ~mask & m_mask;
     
    int nCount=0;
    for (int* p=begin; p < end; p++) {
      nCount += CountOnes(m_grid[*p] & mask);
    }
    
    if (nCount <= m_minCount) {
      m_minCount = nCount;
      m_minCombo = mask;
      COUT << "Current Minimum: " << m_minCount << " Combo: " << m_minCombo << endl;
    }
  }

  
  ///   int CountOnes(UINT64 n)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  int CountOnes(UINT64 n)
  {
    int nCount = 0;
    for (int i=0; i<40; i+=8) {
      nCount += m_ones[(n>>i)&0xff];
    }

    return nCount;  
  }
  
  
  ///   void CalcWithCombinations()
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  //void CalcWithCombinations()
  //{
  //
  //  CPrintThread pth;
  //  pth.Start();
  //
  //  int outNode[20];
  //  //recursive_combination(m_nodes,m_nodes+40, 0, outNode,outNode+20, 0, 20, this);

  //  pth.Stop(true);
  //      
  //  COUT << "Min Edges: " << m_minCount << endl;
  //  
  //  for (int i=0; i<40; i++) {
  //    if ( (m_minCombo >> i) == 0)
  //      COUT << "Group 1," << i << endl;
  //    else
  //      COUT << "Group 2," << i << endl;
  //  }
  //}
 
 
 
  
};
