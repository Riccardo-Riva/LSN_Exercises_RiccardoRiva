#ifndef __PDF__
#define __PDF__

#include "probabilityDensity.h"
#include "linearVector.h"
#include <cmath>

// The following porbability densities aren't normalized

class PsiTrial: public ProbabilityDensity{
	public:
		PsiTrial(double mu, double sigma) {m_mu = mu; m_sigma = sigma;}

		virtual double Eval(std::vector<double> x) const {
			return pow(exp( -pow(x[0]-m_mu,2) / (2.*pow(m_sigma,2)) ) + exp( -pow(x[0]+m_mu,2) / (2.*pow(m_sigma,2)) ), 2);
		}

		double GetMu() {return m_mu; }
		double GetSigma() {return m_sigma; }

	private:
		double m_mu, m_sigma;
};

#endif //__PDF__
