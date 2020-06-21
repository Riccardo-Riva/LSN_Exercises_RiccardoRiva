#ifndef __Metropolis__
#define __Metropolis__

#include <vector>
#include "random.h"
#include "probabilityDensity.h"

class Metropolis {

private:
  int m_dim; // the class allows to chose de dimension of the cartesian space
  std::vector<double> m_x; // this is the position after every metropolis step 
  ProbabilityDensity * m_pdf; // by means of inheritance, the class permits to choose different PDFs
  Random * m_rnd; // the random number generator declared in the main
  bool m_accept; // the value is True if the last metropolis step was accepted, otherwise it is false
  double m_step; // half-width of the uniform transition matrix
  double m_sigma; // standard deviation of the gaussian transition probability

protected:

public:
  // constructors
  Metropolis(std::vector<double> x, ProbabilityDensity * pdf, Random * rnd);
  // destructor
  ~Metropolis() {;}
  // methods
  void Uniform_Step(); // Metropolis Algorithm with Uniform Transition PDF
  void Gaussian_Step(); // Metropolis Algorithm with Gaussian transition probability
  std::vector<double> GetX(); // Get the value of the distance from the origin
  double GetR(); // Get the value of the distance from the origin
  bool GetAccept(); // returns the value of m_accept
  void SetX(std::vector<double> x); // Change actual position
  void SetStep(double step);
  void SetSigma(double sigma);


};

#endif // __Metropolis__