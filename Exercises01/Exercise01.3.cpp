#include <iostream>
#include <fstream>
#include <string>

#include "random.h"
#include "functions.h"

#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>

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

   int M = 100000000;
   int N = 100;
   int throws = M/N;

   double d = 1.;
   double l = 0.95;

   vector<double> pi;
   vector<double> pi_prog;
   vector<double> err_prog;

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

   vector<double> x;
   vector<double> ang;

   for (int i = 0; i < M; ++i){
      x.push_back(rnd.Rannyu(-d/2., d/2.));
      ang.push_back(rnd.Angle());
   }

   for (int i = 0; i < N; ++i){
      double hit = 0.;
      for (int j = 0; j < throws; ++j)
         if (0.>= x[i*throws + j] - 0.5*l*abs(sin(ang[i*throws + j])) and 0.<= x[i*throws + j] + 0.5*l*abs(sin(ang[i*throws + j])) )
            hit+=1.;
      pi.push_back(l*2*throws/(d*hit));
      pi_prog.push_back(average(pi));
      err_prog.push_back(meanStdDev(variance(pi),i));
   }

   print2Col(pi_prog, err_prog, "pi.dat");

   rnd.SaveSeed();
   return 0;
}