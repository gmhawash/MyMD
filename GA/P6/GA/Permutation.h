#pragma once

using namespace System;
using namespace System::IO;
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <math.h>
#include <limits>

using namespace std;

#define N 10
#define N_GEN 400
#define N_POP 100
#define PM 0.45
#define PC 0.6

#define CROSS_OVER CrossOverOnePoint
//#define CROSS_OVER CrossOverTwoPoint

  static int flow[N][N] = {{},
                 {5},
                 {2,3},
                 {4,0,0},
                 {1,2,0,5},
                 {0,2,0,2,10},
                 {6,1,0,5,3,1},
                 {0,0,2,3,1,1,1},
                 {1,1,3,2,3,5,8,2},
                 {6,0,0,2,7,3,1,1,2}
                 };
                 
  static int dist[N][N] = {{},
                 {1},
                 {2,1},
                 {3,2,1},
                 {1,2,3,4},
                 {2,1,2,3,1},
                 {3,2,1,2,2,3},
                 {4,5,2,7,3,3,4},
                 {6,4,3,1,4,4,2,1},
                 {7,3,1,3,2,2,1,2,2}
                 };
   
static    int facility[N]={0,1,2,3,4,5,6,7,8,9};   // index represnt location where facility is in
int solution[N];

int  pop[2*N_POP][N];

ref class CPermutation
{
  
public:
  Random m_rnd;  
  double m_ParentTotalFitness;
  
  CPermutation(void)
  {
    // Mirror the matrices
    for (int i=1; i<N; i++) 
      for (int j=0; j<i; j++) {
        flow[j][i] = flow[i][j];
        dist[j][i] = dist[i][j];
    }
  };

  int Cost(int *fac)
  {
    // k,p represent the locations where the facility is located in
    int cost = 0;
    for (int k=0; k<N-1; k++)
      for (int p=k+1; p<N; p++) {
        //COUT << k << "-" << p << ", flow: " << fac[k] << "-" << fac[p] << ": " ;
        //COUT << dist[k][p] << " x " << flow[fac[k]][fac[p]] << "=" << dist[k][p] * flow[fac[k]][fac[p]];
        cost += dist[k][p] * flow[fac[k]][fac[p]];
        // COUT << " Total: " << cost << "\n";
      } 
     return cost;  
  }
  
  int Cost()
  {
    return Cost(facility);
  }
  
  void SolveWithPermutation()
  {
    int cost =  numeric_limits<int>::max();
    ofstream f("output.txt", ios::out);

    int index=0;
    do {
      int c = Cost();
      
      if (c <= cost) {
        cost = c;
        COUT << c << "\r";
        f << c << ": ";
        for (int i=0; i<N-1; i++)
          f << facility[i] << ", ";
         
        f << "\n";
        copy(facility, facility+N, solution); 
      }
    } while(next_permutation(facility, facility+N));
    
    f.close();
  } 
  
  
  void Init()
  {
    // allocate population with N elements for each individual, copied
    // from the original unique set (facility) and randomly suffle them..
    //
    for (int i=0; i<N_POP; i++) {
      copy(facility, facility+N, pop[i]);
      random_shuffle(pop[i], pop[i] + N);
    }
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
    for (int i=0; i<20; i++)
      DoGeneration(i);
  }
  
  #define ELITE 1
  
  void DoGeneration(int gen)
  {
    char szTmp[20];
    
    sprintf_s(szTmp, "OutputGA %d.txt", gen);
    
    ofstream f( szTmp, ios::out);

    // initialize population
    Init();
    
    for (int i=0; i<N_GEN; i++) {
      m_ParentTotalFitness = TotalFitness();
      // Keep most elite parent
      int *p = BestFit();
      
      #if ELITE > 0
      copy(p, p + N, pop[N_POP]); 
      #endif
      
      COUT << "Gen: " << i << ": " << Cost(p) << "\n";
      f << Cost(p) << "\n";
      //for (int i=0; i<N-1; i++)
      //  f << p[i] << ", ";
      // 
      //f << "\n";

      #if ELITE == 0
        Breed(0);
      #endif
      
      for (int j=1; j<N_POP; j++) {
        Breed(j);
      }
      
      // terminate parents

      for (int j=0; j<N_POP; j++) {
        copy (pop[j + N_POP], pop[j+N_POP+1], pop[j]);
      }
    }
    f.close();
    
  }
  
  ///   double TotalFitness()
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  double TotalFitness()
  {
    double total=0;
    for (int i=0; i< N_POP; i++)
      total += Cost(pop[i]);
  
    return total;
  }
  
  
  
    int *Roulette()
  {
    double rnd = m_rnd.NextDouble();
    double val=0;
    
    for (int i=0; i < N_POP; i++) {
      val += Cost(pop[i])/m_ParentTotalFitness;
      if (rnd < val)
        return pop[i];
    }
    
    return pop[N_POP-1];
  }  

  
  ///   int *BestFit()
  ///
  ///
  /// Inputs:
  ///
  /// Outputs:
  ///
  int *BestFit()
  {
    int minCost =  numeric_limits<int>::max();
    int *p;
    
    for (int i=0; i<N_POP; i++) {
      int cost = Cost(pop[i]);
      if (minCost > cost) {
        minCost = cost;
        p = pop[i];
      }
    }
    
    return p;
  }
  
  bool exists(int item, int *p, int n)
  {
    for (int i=0; i<n; i++)
      if (p[i] == item) 
        return true;
  
    return false;
  }

  void Copy(int dst[], int src[], int nCount)
  {
    for (int i=0; i<nCount;i++)
      dst[i] = src[i];
    
  }
  
  int *CrossOverOnePoint(int *p1, int *p2, double Pc)
  {
    // If Above cross over probablity, return best fit parent
    if (Pc > m_rnd.NextDouble())
      return Cost(p1) < Cost(p2) ? p1 : p2;
    
    int nFirst  = m_rnd.Next(8)+1;
    
    // copy p1 to solution
    copy(p1, p1+N, solution);
    int nCount = N - nFirst;
    
    int s1[N];
    int j=0;
    // Create a set of elements in p1[nFirst..nSecond] but not in p2[nFirst..nSecond]
    for (int i=nFirst; i<N; i++) {
      if (!exists(p1[i], p2+nFirst, nCount))
        s1[j++] = p1[i];
    }
    
    Copy (&solution[nFirst], &p2[nFirst], nCount);
    
    for (int i=nFirst, j=0; i<N; i++) {
      if (!exists(solution[i], p1+nFirst, nCount))
        solution[i] = s1[j++];
    }

    return solution;
  }
  

  int *CrossOverTwoPoint(int *p1, int *p2, double Pc)
  {
    // If Above cross over probablity, return best fit parent
    if (Pc > m_rnd.NextDouble())
      return Cost(p1) < Cost(p2) ? p1 : p2;
    
    int nFirst  = m_rnd.Next(7)+1;
    int nSecond = m_rnd.Next(7)+1;
    
    if (nFirst > nSecond)
      swap(nFirst, nSecond);
    
    // copy p1 to solution
    copy(p1, p1+N, solution);
    int nCount = nSecond - nFirst+1;
    
    int s1[N];
    int j=0;
    // Create a set of elements in p1[nFirst..nSecond] but not in p2[nFirst..nSecond]
    for (int i=nFirst; i<=nSecond; i++) {
      if (!exists(p1[i], p2+nFirst, nCount))
        s1[j++] = p1[i];
    }
    
    Copy (&solution[nFirst], &p2[nFirst], nCount);
    
    for (int i=nFirst, j=0; i<=nSecond; i++) {
      if (!exists(solution[i], p1+nFirst, nCount))
        solution[i] = s1[j++];
    }

    return solution;
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
    int * p1 = Roulette();
    int * p2 = Roulette();
    int * p = CROSS_OVER(p1, p2, PC);
    copy(p, p + N, pop[N_POP + j]);
    Mutate(pop[N_POP + j]);
  }
  
  void Mutate(int indv[])
  {
    if (m_rnd.NextDouble() < PM) {
      // Swap 
      int one = m_rnd.Next(N);
      int two = m_rnd.Next(N);
      int tmp = indv[one];
      indv[one] = indv[two];
      indv[two] = tmp;
    }
  }
};
