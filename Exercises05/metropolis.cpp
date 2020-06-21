#include "metropolis.h"

#include <vector>
#include <algorithm>
#include <cmath>

#include "linearVector.h"

Metropolis::Metropolis(std::vector<double> x, ProbabilityDensity * pdf, Random * rnd) {
	m_x = x;
	m_dim = x.size();
	m_pdf = pdf;
	m_rnd = rnd;
	m_accept = true;
	m_step = 1.;
	m_sigma = 1.;
}

void Metropolis::Uniform_Step(){ 
	vector<double> y = m_x;
	double a;
	for (int i = 0; i < m_dim; ++i)
		m_x[i] = m_rnd->Rannyu(y[i] - m_step, y[i] + m_step);
	
	a = min(1. , m_pdf->Eval(m_x) / m_pdf->Eval(y)); 

	m_accept = (m_rnd->Rannyu() < a);
	if(!m_accept)
		m_x = y; //remains in the old position
}

void Metropolis::Gaussian_Step(){ 
	vector<double> y = m_x;
	double a;

	for (int i = 0; i < m_dim; ++i)
		m_x[i] = m_rnd->Gauss(y[i], m_sigma);
	
	a = min(1. , m_pdf->Eval(m_x) / m_pdf->Eval(y)); 

	m_accept = (m_rnd->Rannyu() < a);
	if(!m_accept)
		m_x = y; //remains in the old position
}

vector<double> Metropolis::GetX(){
	return m_x;
}

double Metropolis::GetR(){
	return sqrt(m_x * m_x);
}

bool Metropolis::GetAccept(){
	return m_accept;
}

void Metropolis::SetX(std::vector<double> x){
	m_x = x;
}

void Metropolis::SetStep(double step){
	m_step = step;
}

void Metropolis::SetSigma(double sigma){
	m_sigma = sigma;
}