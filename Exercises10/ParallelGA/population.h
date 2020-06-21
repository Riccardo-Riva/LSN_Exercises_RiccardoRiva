#ifndef __Population__
#define __Population__

#include "individual.h"
#include "random.h"

using namespace std;

class Population {

private:
  std::vector<Individual> m_population;
  Random * m_rnd;

  std::vector<std::vector<double>> m_distances; 
  /* This is the matrix containing the distances between the cities.
  distances[i][j] is the distance between the city i and the city j.
  the value of the distances depends on the problem studied*/

  std::vector<int> sortedIndex; // this vector contains the indexes of the population
  								// from the cheapest to the most expensive, i.e.
  								// sortedIndex[0] is the index of the individual 
  								// of the population with minumum cost() value

  void mergeSort(int low, int high);
  void merge(int low, int mid, int high);

protected:

public:
  // constructors
  Population(Random *rnd);
  // destructor
  ~Population() {;}
  // method
  double Cost(int j); 	// method to evaluate the cost function 
  						//over the j-th shortest individual of the poulation
  void ChangePath(int i, std::vector<int> newPath); 

  void Generate(); 	// this is the main methodof the class, 
  					// it generates the next generation

  void SetDistances(std::vector<std::vector<double>> distances);
  void ReadPopulationFromFile(std::string namefile); 
  void PrintOnFile(std::string);
  void Sorting(); /* this method order the population with respect to 
  							the value of the cost function of each individual.
  							The method uses the Merge Sort Algorithm */
  int GetSize() {return m_population.size();}
  std::vector<int> GetPath(int i) {return m_population[sortedIndex[i]].GetPath();}
                /* Return the i-th shortest path;*/
};

#endif // __Population__