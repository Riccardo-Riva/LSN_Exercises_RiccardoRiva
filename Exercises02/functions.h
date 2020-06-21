#ifndef __Functions__
#define __Functions__

#include "basicFunction.h"
#include <cmath>

class Parabola: public BasicFunction{

	public:
		Parabola();
		Parabola(double a, double b, double c);
		virtual ~Parabola();
	
		virtual double Eval(double x) const {return m_a*x*x + m_b*x +m_c; }
	
		void SetA(double a) {m_a = a; }
		double GetA() const {return m_a; }
		void SetB(double b) {m_b = b; }
		double GetB() const {return m_b; }
		void SetC(double c) {m_c =c; }
		double GetC() const {return m_c; }	
		
		double GetXVertex() const {return -m_b/(2*m_a); }
		double GetYVertex() const {return Eval(GetXVertex()); }
		
	private:
		double m_a, m_b, m_c;
};

class Seno: public BasicFunction{
	public:
		virtual ~Seno() {;}
		virtual double Eval(double x) const {return sin(x); }
};

class Coseno: public BasicFunction{
	public:
		virtual ~Coseno() {;}
		virtual double Eval(double x) const {return cos(x); }
};   

class Func_01: public BasicFunction{
	public:
		virtual~Func_01() {;}
		virtual double Eval(double x) const {return M_PI*cos(M_PI*x/2.)/2.; }
};

class Func_02: public BasicFunction{
	public:
		virtual ~Func_02() {;}
		virtual double Eval(double x) const {return M_PI*cos(M_PI*x/2.)/(4*(1-x)); }
};
#endif //__Functions__
