#include <iostream>
#include <fstream>

#include <cmath>

#include <vector>
#include <string>

#include "metropolisSA.h"

using namespace std;

MetropolisSA::MetropolisSA(Random *rnd, int N){
  m_rnd = rnd;
  Individual ind(N);
  m_path = ind;
  m_best_path = ind;
  m_beta = 1.;
}

double MetropolisSA::Cost(){
  double sum=0.;
  for (int i = 0; i < (int)m_path.GetSize(); ++i){
    int i_previous = indexPBC(i,m_path.GetSize());
    int i_next = indexPBC(i+1,m_path.GetSize());
    sum += m_distances[m_path.GetValue(i_previous)][m_path.GetValue(i_next)];
  }
  return sum;
}

double MetropolisSA::Cost(Individual ind){
  double sum=0.;
  for (int i = 0; i < (int)ind.GetSize(); ++i){
    int i_previous = indexPBC(i,ind.GetSize());
    int i_next = indexPBC(i+1,ind.GetSize());
    sum += m_distances[ind.GetValue(i_previous)][ind.GetValue(i_next)];
  }
  return sum;
}

double MetropolisSA::LowestCost(){
  double sum=0.;
  for (int i = 0; i < (int)m_best_path.GetSize(); ++i){
    int i_previous = indexPBC(i,m_best_path.GetSize());
    int i_next = indexPBC(i+1,m_best_path.GetSize());
    sum += m_distances[m_best_path.GetValue(i_previous)][m_best_path.GetValue(i_next)];
  }
  return sum;
}

void MetropolisSA::Step(){
  m_acceptance = 0.;
  double p=0.;

  Individual next_path = m_path;

//Mutation 1
  for(int i=0; i<3; ++i){
    int i1 = floor(m_rnd->Rannyu()*(double)next_path.GetSize());
    int i2 = floor(m_rnd->Rannyu()*(double)next_path.GetSize());
    while(i2==i1) i2 = floor(m_rnd->Rannyu()*(double)next_path.GetSize());
    next_path.Swap(i1,i2);
  }

  p = exp(-m_beta *(Cost(next_path) - Cost()));
  if (m_rnd->Rannyu() < p){
    m_path = next_path;
    m_acceptance += 1.;
    if (Cost()<LowestCost())
      m_best_path = m_path;
  }
  next_path = m_path;

// Mutation 2
  int index = floor(m_rnd->Rannyu()*(double)next_path.GetSize());
  int m = floor(m_rnd->Rannyu()*(double)(next_path.GetSize()-1)+1);
  int N = floor(m_rnd->Rannyu()*(double)(m-1))+1;
  next_path.Cycle(index,m,N); 

  p = exp(-m_beta *(Cost(next_path) - Cost()));
  if (m_rnd->Rannyu() < p){
    m_path = next_path;
    m_acceptance += 1.;
    if (Cost()<LowestCost())
      m_best_path = m_path;
  }
  next_path = m_path;

// Mutation 3
  index = floor(m_rnd->Rannyu()*(double)(next_path.GetSize()));
  m = floor(m_rnd->Rannyu()*(double)(next_path.GetSize()/2)+1.);
  next_path.SwapBlocks(index, m); 

  p = exp(-m_beta *(Cost(next_path) - Cost()));
  if (m_rnd->Rannyu() < p){
    m_path = next_path;
    m_acceptance += 1.;
    if (Cost()<LowestCost())
      m_best_path = m_path;
  }
  next_path = m_path;

// Mutation 4
  index = floor(m_rnd->Rannyu()*(double)next_path.GetSize());
  m = floor(m_rnd->Rannyu()*(double)(next_path.GetSize()-2))+2;
  next_path.Invert(index, m);  

  p = exp(-m_beta *(Cost(next_path) - Cost()));
  if (m_rnd->Rannyu() < p){
    m_path = next_path;
    m_acceptance += 1.;
    if (Cost()<LowestCost())
      m_best_path = m_path;
  }

// Mutation 5
  int i1 = floor(m_rnd->Rannyu()*(double)next_path.GetSize());
  int i2 = floor(m_rnd->Rannyu()*(double)next_path.GetSize());
  int i3 = floor(m_rnd->Rannyu()*(double)next_path.GetSize());
  while(i2==i1) i2 = floor(m_rnd->Rannyu()*(double)next_path.GetSize());
  while(i3==i1 or i3==i1) i3 = floor(m_rnd->Rannyu()*(double)next_path.GetSize());
  next_path.Swap(i1,i2);
  next_path.Swap(i2,i3);
  next_path.Swap(i3,i1);

  p = exp(-m_beta *(Cost(next_path) - Cost()));
  if (m_rnd->Rannyu() < p){
    m_path = next_path;
    m_acceptance += 1.;
    if (Cost()<LowestCost())
      m_best_path = m_path;
  }

}

void MetropolisSA::Reset(){
  Individual ind(m_path.GetSize());
  m_path = ind;
  m_best_path = ind;
}

void MetropolisSA::SetDistances(std::vector<std::vector<double>> distances){
  m_distances = distances;
}

void MetropolisSA::SetPath(std::vector<int> path){
  Individual ind(path.size());
  for (int i = 0; i < (int)path.size(); ++i)
    ind.ChangeValue(i, path[i]);
  if (ind.Check()){
    m_path = ind;
    if (Cost()<LowestCost())
      m_best_path = ind;  
  }
  else {
    cerr << endl << "ERROR: the Individual does not satisfy the bonds" << endl;
  }
}