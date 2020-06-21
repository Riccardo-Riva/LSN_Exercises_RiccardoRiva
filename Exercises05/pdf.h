#ifndef __PDF__
#define __PDF__

#include "probabilityDensity.h"
#include "linearVector.h"
#include <cmath>

// The following porbability densities aren't normalized

class Psi100: public ProbabilityDensity{
	public:
		virtual double Eval(std::vector<double> x) const {return exp(-2*sqrt(x*x));}
};

class Psi210: public ProbabilityDensity{
	public:
		virtual double Eval(std::vector<double> x) const {
			double r = sqrt(x*x);
			if(r!=0.)
				return exp(-r) * pow(x[2],2); // this is r^2 * e^(-r) * (z / r)^2 = e^(-r) * z^2   
			else
				return 0.;
		}
};


#endif //__PDF__
