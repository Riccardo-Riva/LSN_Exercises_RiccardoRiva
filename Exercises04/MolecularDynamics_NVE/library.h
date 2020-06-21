#include <vector>

//===============================
//=== average - variance
//===============================

double average(const std::vector<double>&);
double variance(const std::vector<double>&);
double meanStdDev(const double&, const int&);
void print1Col(const std::vector<double>&, const std::string&);
void print2Col(const std::vector<double>&, const std::vector<double>&, const std::string&); //stampa 2 vector divisi in colonne
void printCol(const std::vector<std::vector<double>>&, const std::string&); //stampa un vector di vector, utile per stampare più colonne