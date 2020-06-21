#include <fstream>

#include <vector>
#include <algorithm>

#include <cmath>

#include "individual.h"

using namespace std;

Individual::Individual(int N){
	for (int i = 0; i < N; ++i)
		m_path.push_back(i);
}

Individual::Individual(const Individual& ind){
	m_path = ind.m_path;
}

Individual& Individual::operator=(const Individual& ind){
	m_path = ind.m_path;
	return *this; 
}

void Individual::Bond(){
	while(m_path[0]!=0){
		int appo=m_path[0];
		m_path.erase(m_path.begin());
		m_path.push_back(appo);
	}
}

void Individual::ChangeValue(int j, int val){
	m_path[j] = val;
}

void Individual::Swap(int i, int j){
	swap(m_path[i],m_path[j]);
	Bond();
}

void Individual::Cycle(int i, int m, int N){
	int index = indexPBC(i,m_path.size());
	if(m<1)
		m=1;
	int appo[m];
	N = N % m;
	for (int j = 0; j < m; ++j)
		appo[j] = m_path[indexPBC(index+j,m_path.size())];
	for (int j = 0; j < m; ++j)
		m_path[indexPBC(index+j,m_path.size())] = appo[indexPBC(j-N,m)];
	Bond();
}

void Individual::SwapBlocks(int i, int m){
	int index = indexPBC(i,m_path.size());
	int size = m%((int)m_path.size()/2);
	for (int j = 0; j < size; ++j){
		swap(m_path[indexPBC(index+j,m_path.size())],m_path[indexPBC(index+m+j,m_path.size())]);
	}
	Bond();
}

void Individual::Invert(int i, int m){
	int appo[m];
	for (int j = 0; j < m; ++j)
		appo[m-1-j] = m_path[indexPBC(i+j,m_path.size())];
	for (int j = 0; j < m; ++j)
		m_path[indexPBC(i+j, m_path.size())] = appo[j];
	Bond();
}

bool Individual::Individual::Check(){
	bool bonds = true;
	if (m_path[0]!=0) bonds = false;
	for (int i = 0; i < (int)m_path.size(); ++i){
		bool is_in_vector = false;
		for (int j = 0; j < (int)m_path.size(); ++j){
			if(m_path[j]==i) is_in_vector = true;
		}
		bonds = is_in_vector;
	}
	return bonds;
}

//=============//

int indexPBC(int i,int imax){
  	int iPBC=i;
  	if(iPBC<0 or iPBC >= imax)
    	iPBC =  iPBC - (int)floor((double)iPBC / (double)imax) * imax;
    return iPBC;
}