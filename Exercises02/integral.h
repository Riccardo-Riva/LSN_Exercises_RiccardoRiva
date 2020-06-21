#ifndef __Integral__
#define __Integral__

#include "basicFunction.h"
#include "random.h"

class Integral{

	public:

		Integral(double a, double b, const BasicFunction* f, Random * rnd);
		~Integral() {delete m_rand; }
		
		double uniformMC(unsigned int N);
		double importanceMC_01(unsigned int N);
		
		
	private:
		double m_a, m_b;
		int m_sign;
		double m_integral;
		const BasicFunction* m_f;
		Random *m_rand;
		
};

#endif //__Integral__
