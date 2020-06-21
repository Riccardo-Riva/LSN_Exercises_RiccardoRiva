#include <iostream>
#include <fstream>

#include <cstdlib>
#include <cmath>
#include <iomanip>

#include <string>
#include <vector>

using namespace std;

//================================================
//=== average - variance - standard deviation
//================================================

double average(const vector<double>& v){
	double average=0;
	if(v.size()==0) return average;

	for (int i = 0; i < v.size(); ++i)
		average = static_cast<double>(i)/static_cast<double>(i+1) *average + static_cast<double>(v[i])/static_cast<double>(i+1);

	return average;
}

double variance(const vector<double>& v){
	double var=0;
	if(v.size()==0) return var;

	double oldaverage, average = 0;
	for (int i = 0; i < v.size(); ++i){
		oldaverage = average;
		average = static_cast<double>(i)/static_cast<double>(i+1) *average + 1./static_cast<double>(i+1) * static_cast<double>(v[i]);
		var = 1./static_cast<double>(i+1) *(static_cast<double>(i) * var + static_cast<double>(i)*pow(oldaverage,2) + pow(static_cast<double>(v[i]),2) - static_cast<double>(i+1)*pow(average,2));
	}
	return var;
}

double meanStdDev(const double& var, const int& n){
	if (n>0)
		return sqrt(var/(double)n);
	else return 0.;
}

//================================================
//====== File Output
//================================================

void print1Col(const vector<double>& v, const string& filename){
	ofstream out(filename);
	if (out.is_open()){
		out << scientific << setprecision(9);
   		for(int i=0; i<v.size(); ++i)
      		out << v[i] << endl;
		out.close();
   	} 
   	else cerr << "PROBLEM: Unable to open " + filename << endl;
}

void print2Col(const vector<double>& v, const vector<double>& w, const string& filename){
	if(v.size() != w.size())
		cerr << endl << "Error: unable to print columns. Columns have different number of rows" << endl;
	else{
		ofstream out(filename);
		if (out.is_open()){
			out << scientific << setprecision(9);
   			for(int i=0; i<v.size(); ++i)
      			out << v[i] << " " << w[i] << endl;
			out.close();
   		} 
   		else cerr << "PROBLEM: Unable to open " + filename << endl;
	}
}

void printCol(const vector<vector<double>>& col, const string& filename){
	for (int i = 1; i < col.size(); ++i){
		if (col[i].size()!=col[0].size()){
			cerr << endl << "Error in printing on file " << filename << ": the sizes of two columns are different" << endl;
			return; 
		}
	}
	ofstream out(filename);
	if (out.is_open()){
		out << scientific << setprecision(9);
		for(int i=0; i<col[0].size(); ++i){
	  		for(int j = 0; j<col.size(); ++j)
				out << col[j][i] << " ";
			out << endl;
		}
		out.close();
	} 
   	else cerr << "PROBLEM: Unable to open " + filename << endl;
}
