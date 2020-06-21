#include <iostream>
#include <fstream>
#include <string>

#include "random.h"
#include "functions.h"

#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
 
int main (int argc, char *argv[]){


   Random rnd;
   int seed[4];
   int p1, p2;

   /*questo deve esserci sempre, utile per il calcolo parallelo,per non avere più processi che usano lo stesso seme*/
   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   //============================
   //==== INtegral Calulation
   //============================
   int M = 1000000;      // Total number of throws
   int N = 100;         // Number of blocks
   int L = M/N;         // Number of throwns in each block

   vector<double> r;

   vector<double> ave;
   vector<double> var;
   vector<double> sum_prog;
   vector<double> err_prog;
   vector<double> sumvar_prog; //da graficare
   vector<double> errvar_prog; //da graficare

   //Setting the random generator

   ifstream input("seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;

   for(int i=0; i < M; i++){
      r.push_back(rnd.Rannyu());      
   }

   for(int i=0; i<N; i++){
      double s = 0.;
      double va = 0;
      for (int j = 0; j < L; j++){
         int k = j+i*L;
         s+=r[k];
         va+=(r[k]-0.5)*(r[k]-0.5);
      }
      ave.push_back(s/(double)L);
      var.push_back(va/(double)L);
   }

   vector<double> reduced_ave;
   vector<double> reduced_var;
   reduced_ave.push_back(ave[0]);
   reduced_var.push_back(var[0]);

   for (int i = 1; i < N; ++i){
      reduced_ave.push_back(ave[i]);
      sum_prog.push_back(average(reduced_ave));
      err_prog.push_back(meanStdDev(variance(reduced_ave),i));
      reduced_var.push_back(var[i]);
      sumvar_prog.push_back(average(reduced_var));
      errvar_prog.push_back(meanStdDev(variance(reduced_var),i));
   }

   print2Col(sum_prog, err_prog, "montecarlo.dat");
   print2Col(sumvar_prog, errvar_prog, "montecarlo_variance.dat");

   //============================
   //==== TEST X Squared
   //============================

   int n_intervals = 100;
   int tests = 100;
   int throws = M/tests;

   vector<double> X2(tests, 0.);
   
   for (int i = 0; i < tests; ++i){
      vector<int> observed(n_intervals, 0);
      for (int j = i*throws; j < (i+1)*throws; ++j)
         observed[(int) floor(r[j]*(double)n_intervals)] += 1;

      for (int j = 0; j < n_intervals; ++j)
         X2[i] += (double)(observed[j] - throws/n_intervals)*(double)(observed[j] - throws/n_intervals)/((double)(throws/n_intervals));
   }

   print1Col(X2, "chiquadro.dat");

   rnd.SaveSeed();
   return 0;
}