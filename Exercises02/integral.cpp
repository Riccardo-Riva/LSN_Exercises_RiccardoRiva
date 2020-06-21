#include "integral.h"
#include <algorithm>

Integral::Integral(double a, double b, const BasicFunction * f, Random * rnd){
	m_f = f;
	m_a = std::min(a,b);
	m_b = std::max(a,b);
	if(a<b) m_sign =1;
	else m_sign =-1;
	m_rand = rnd;
}

double Integral::uniformMC(unsigned int N){
	m_integral =0.;
	for(int i = 0; i<N; i++)
		m_integral += m_f->Eval(m_rand->Rannyu(m_a, m_b));
		
	m_integral = m_integral*(m_b-m_a)/(double)N;

	return m_integral;
}

double Integral::importanceMC_01(unsigned int N){
	m_integral =0.;
	for(int i = 0; i<N; i++)
		m_integral += m_f->Eval(m_rand->Linear(m_a, m_b));
		
	m_integral = m_integral*(m_b-m_a)/(double)N;

	return m_integral;
}