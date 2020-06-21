#include "linearVector.h"
#include "randomWalk.h" 
#include <cmath>

RandomWalk :: RandomWalk(Random *rnd){
	vector<double> v(3,0.);
	m_v = v;
	m_rand = rnd;
	m_a = 1.;
}
void RandomWalk :: Reset(){
	vector<double> v(m_v.size(), 0.);
 	m_v = v;
}
double RandomWalk :: GetR(){
	double r=0.;
	for (int i = 0; i < m_v.size(); ++i)
		r+=m_v[i]*m_v[i];
	return sqrt(r);
}

void RandomWalk :: LatticeRW(int N){
	for (int i = 0; i < N; ++i){
		int rand = (int) floor(m_rand->Rannyu()*6);
		switch(rand){
		case 0:
			m_v[0]+=m_a;
			break;
		case 1:
			m_v[0]-=m_a;
			break;
		case 2:
			m_v[1]+=m_a;
			break;
		case 3:
			m_v[1]-=m_a;
			break;
		case 4:
			m_v[2]+=m_a;
			break;
		case 5:
			m_v[2]-=m_a;
			break;
		}		
	}
}
void RandomWalk :: ContinuumRW(int N){
	for(int i=0.; i < N; ++i){
		double theta = m_rand->Theta();
		double phi = m_rand->Phi();

		vector<double> v;

		v.push_back(sin(theta)*cos(phi));
		v.push_back(sin(theta)*sin(phi));
		v.push_back(cos(theta));

		m_v = m_v + v;
	}
}