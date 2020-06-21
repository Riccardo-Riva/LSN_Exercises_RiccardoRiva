#ifndef __RandomWalk__
#define __RandomWalk__

#include "random.h"
#include <vector>

using namespace std;

class RandomWalk {

private:
  Random *m_rand;
  vector<double> m_v;
  double m_a; //this is the lattice spacing for the randomwalk on a lattice and the legnth of the step for the random wolk in a continuum;

protected:

public:
  // constructors
  RandomWalk(Random *rnd);
  // destructor
  ~RandomWalk() {;}
  // methods

  void SetA(double a) {m_a = a;}
  void Reset();
  double GetR();
  void LatticeRW(int N);
  void ContinuumRW(int N);
};

#endif // __RandomWalk__