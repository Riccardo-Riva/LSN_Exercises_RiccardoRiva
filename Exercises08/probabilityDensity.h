#ifndef __ProbabilityDensity__
#define __ProbabilityDensity__

#include <vector>

class ProbabilityDensity{
	public:
		virtual double Eval(std::vector<double> x) const =0;
};

#endif //__ProbabilityDensity_h__
