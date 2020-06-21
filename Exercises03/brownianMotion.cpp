#include "brownianMotion.h"
#include <cmath>

using namespace std;

BrownianMotion :: BrownianMotion(double drift, double volatility, Random *rnd){
	m_rnd = rnd;
	m_drift = drift;
	m_volatility = volatility;
}

double BrownianMotion :: BW(double t){
	return m_rnd->Gauss(m_drift * t, m_volatility * m_volatility * t);
}

double BrownianMotion :: GBW(double S, double t){
	return S * exp(m_rnd->Gauss((m_drift - 0.5 * m_volatility * m_volatility) * t, m_volatility * sqrt(t)));
}