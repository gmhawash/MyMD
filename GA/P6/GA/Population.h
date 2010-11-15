#pragma once
#include<iostream>
#include<fstream>
#include <vector>
#include<string>
#include "thread.h"
//#include "stdCombination.h"
#include "qurandom.h"
#include "solution.h"

//using namespace stdcomb;
using namespace std;
using namespace System;
using namespace System::IO;

#define Tu 2
#define TRY2

#define CrossOver UniformCrossOver

#ifdef TRY2
 #define N_GENS 200
 #define N_POP  100
#define PM 0.05
#define PC 0.7
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

 
class CPopulation
{
public:
  CSolution m_sol[2*N_POP];   // should be initialized by now.
  ofstream m_f;
  
  ///   CPopulation(void)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  CPopulation(void)
  {
        

  }
 
 void PrintFrontier(int g)
 {
    m_f << "Generation: " << g << endl;
    
    for (int i=0; i<N_POP; i++) 
      if (m_sol[i].m_rank == 1) {
        m_f << "(" << m_sol[i].m_f[0] << ", " << m_sol[i].m_f[1] << "): ";
        for (int n=0; n<8; n++)
          m_f << "0x" << hex << m_sol[i].m_x[n] << ", ";
          
        m_f << endl;
      }
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

    sprintf_s(szTmp, "OutputGA %d.txt", gen);
    
    if (m_f.is_open()) m_f.close();
    m_f.open( szTmp, ofstream::out);

    for (int g=0; g<N_GENS; g++) {
      cout << "Generation: " << g << endl;
      // Copy population to a list for ranking sake.
      vector<CSolution*> sol;

      for (int i=0; i<N_POP; i++) { 
        m_sol[i].Refresh();
        sol.push_back(&m_sol[i]);
      }

      Rank(sol, 1);
      PrintFrontier(g);
      CalcProbability();
      
      // Now do the breeding based on the rank...
      for (int j=0; j<N_POP; j++) 
        Breed(j);

      // Now cull the parents and keep children...
      for (int j=0; j<N_POP; j++) 
        m_sol[j] = m_sol[N_POP+j];
    }
    m_f.close();
  }

  
  void CalcProbability()
  {
    double sum = 0;

    // Caclulate the sum of probabilities: Prob of selecting sol i = exp(-Tu * Rank(i))    
    for (int i=0; i<N_POP; i++) {
      m_sol[i].m_selProp = exp(-m_sol[i].m_rank * Tu);
      sum += m_sol[i].m_selProp;
    } 

    // Now find the ratio where all will add up to 1.0
    for (int i=0; i<N_POP; i++) {
      m_sol[i].m_selProp /= sum;
    } 
  }
  
  ///   void Dominance()
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  void CalcDominance(vector<CSolution*> &sol)
  {
    for (int i=0; i<sol.size(); i++) 
      sol[i]->m_dominated = false;            // reset dominance bit first    

    for (int i=0; i<sol.size()-1; i++) {
      for (int j=i+1; j<sol.size(); j++) {
        sol[i]->SetDominance(m_sol[j]);
      }
    }
  }

  
  ///   void Rank(vector<CSolution*> &sol, int rank)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  void Rank(vector<CSolution*> &sol, int rank)
  { 
    // We are done...
    if (sol.size() == 0)
      return;
      
    CalcDominance(sol);
    
    for (int i=0; i<sol.size(); i++) {
      if (!sol[i]->m_dominated) {
        sol[i]->m_rank = rank;
        sol.erase(sol.begin() + i);
      }
    }

    Rank(sol, ++rank);
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
    CSolution &p1 = Roulette();
    CSolution &p2 = Roulette();
    
    m_sol[N_POP+j] = CrossOver(p1, p2, PC);
    m_sol[N_POP+j].Mutate(PM);
    
  }

  ///   UINT64 Roulette()
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  CSolution& Roulette()
  {
    double rnd = QuRandom::NextDouble();
    double val=0;

    for (int i=0; i < N_POP; i++) {
      val += m_sol[i].m_selProp;
      
      if (rnd < val)
        return m_sol[i];
    }
    
    return m_sol[N_POP-1];    // return last one if not found.
  }  

  



  
  ///   UINT64 CrossOverOnePoint(UINT64 p1, UINT64 p2, double Pc)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  CSolution& OnePointCrossOver(CSolution& p1, CSolution& p2, double Pc)
  {
    // If Above cross over probablity, return best fit parent
    if (QuRandom::NextDouble() > Pc) {
      // If both have same dominancde issue, randomly select; othewise take one not dominated.
      if (p1.m_dominated == p2.m_dominated) 
        return QuRandom::Truth() ? p1 : p2;
      else
        return p1.m_dominated ? p2 : p1;
    }
    
    int nFirst  = QuRandom::Next32(6) + 1;
    
    CSolution *c = new CSolution(p1);
    
    for (int i=nFirst; i<N; i++)
      c->m_x[i] = p2.m_x[i]; 
    
    return *c;
  }



  
  ///   CSolution& TwoPointCrossOver(CSolution& p1, CSolution& p2, double Pc)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  CSolution& TwoPointCrossOver(CSolution& p1, CSolution& p2, double Pc)
  {
    // If Above cross over probablity, return best fit parent
    if (QuRandom::NextDouble() > Pc) {
      // If both have same dominancde issue, randomly select; othewise take one not dominated.
      if (p1.m_dominated == p2.m_dominated) 
        return QuRandom::Truth() ? p1 : p2;
      else
        return p1.m_dominated ? p2 : p1;
    }
    
    int nFirst  = QuRandom::Next32(6) + 1;
    int nSecond  = QuRandom::Next32(6) + 1;
    
    if (nFirst > nSecond) {
      int tmp = nFirst;
      nFirst = nSecond;
      nSecond = tmp;
    }
    
    CSolution *c = new CSolution(p1);
    
    for (int i=nFirst; i<nSecond; i++)
      c->m_x[i] = p2.m_x[i];
    
    return *c;
  }

  ///   CSolution& TwoPointCrossOver(CSolution& p1, CSolution& p2, double Pc)
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  CSolution& UniformCrossOver(CSolution& p1, CSolution& p2, double Pc)
  {
    // If Above cross over probablity, return best fit parent
    if (QuRandom::NextDouble() > Pc) {
      // If both have same dominancde issue, randomly select; othewise take one not dominated.
      if (p1.m_dominated == p2.m_dominated) 
        return QuRandom::Truth() ? p1 : p2;
      else
        return p1.m_dominated ? p2 : p1;
    }

    CSolution *c = new CSolution(p1);
    
    for (int i=0; i<8; i++) {
      c->m_x[i] = QuRandom::Truth() ? p1.m_x[i] : p2.m_x[i];
    }
    return *c;
  }


};
