#ifndef __MetropolisSA__
#define __MetropolisSA__

#include "individual.h"
#include "random.h"

using namespace std;

class MetropolisSA {

private:
  Individual m_path;
  Individual m_best_path;
  double m_acceptance;

  double m_beta;

  Random * m_rnd;

  std::vector<std::vector<double>> m_distances; 
  /* This is the matrix containing the distances between the cities.
  distances[i][j] is the distance between the city i and the city j.
  the value of the distances depends on the problem studied*/

protected:

public:
  // constructors
  MetropolisSA(Random *rnd, int N);
  // destructor
  ~MetropolisSA() {;}
  // method
  double Cost();
  double Cost(Individual ind);
  double LowestCost();

  void SetDistances(std::vector<std::vector<double>> distances);
  void SetPath(std::vector<int> path);
  void SetBeta(double beta) {m_beta = beta;}

  void Step(); // this is the method which perform the metropolis step

  void Reset();

  std::vector<int> GetPath() const {return m_path.GetPath();}
  std::vector<int> GetBestPath() const {return m_best_path.GetPath();}
  double GetAcceptance() const {return m_acceptance;}

};

#endif // __MetropolisSA__