#ifndef __BaseFunction__
#define __BaseFunction__

#include <vector>

class BaseFunction{
	public:
		virtual double Eval(std::vector<double> x) const =0;
};

#endif //__BaseFunction__