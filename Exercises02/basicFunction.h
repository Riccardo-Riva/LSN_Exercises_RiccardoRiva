#ifndef __BasicFunction__
#define __BasicFunction__

class BasicFunction{
	public:
		virtual ~BasicFunction() {;}
		virtual double Eval(double x) const =0;
};

#endif //__BasicFUnction__
