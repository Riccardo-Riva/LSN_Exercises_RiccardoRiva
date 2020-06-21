#ifndef __BrownianMotion__
#define __BrownianMotion__

#include "random.h"

class BrownianMotion {

private:
  double m_drift, m_volatility;
  Random *m_rnd;

protected:

public:
  // constructors
  BrownianMotion(double drift, double volatility, Random *rnd);
  // destructor
  ~BrownianMotion() {;}
  // methods
  void SetMu(double drift) {m_drift = drift; }
  void SetSigma(double volatility) {m_volatility = volatility; }

  double BW(double t);
  double GBW(double S, double t);
};

#endif // __BrownianMotion__