#ifndef __Individual__
#define __Individual__

#include <vector>

using namespace std;

class Individual {

private:
  vector<int> m_path;  // This is the 1D vector containing the path folloed by the salesman

protected:

public:
  // constructors
  Individual() {;}
  Individual(int N);  // The constarctor create the vector m_path of dimension N filled with theordered numbers from 0 to N-1
  Individual(const Individual& ind);
  // destructor
  ~Individual() {;}

  Individual& operator=(const Individual& ind);
  // methods
  void Bond(); //this method cycles all the path until the lement 0 is in the first place
  void ChangeValue(int j, int val); /* Change the value of and element of the vector.
                                        N.B.: after using this method you should Check 
                                        if the individual satisfy the bonds */
  void SetPath(std::vector<int> path) {m_path = path;}

  std::vector<int> GetPath() const {return m_path;}  
  int GetValue(int i) {return m_path[i];}
  int GetSize() {return m_path.size();}

  bool Check(); //  Method to check if the individual satisfies the bonds

  // Mutation Methods
  void Swap(int i, int j);  //  method to swap a pair of cities
  void Cycle(int i, int m, int N); // method for a +N sfift of the cities from i to i+m-1 
  void Invert(int i, int m); // method to invert the siquence of cities from i to i+m-1
  void SwapBlocks(int i, int m);
};

#endif // __Individual__

//=============//

int indexPBC(int i,int imax);

