#include <iostream>
#include <fstream>
#include <string>

#include "random.h"
#include "library.h"

#include "basicFunction.h"
#include "functions.h"
#include "integral.h"

#include <vector>

using namespace std;
 
int main (int argc, char *argv[]){

   Random *rnd =new Random();
   int seed[4];
   int p1, p2;

   /*questo deve esserci sempre, utile per il calcolo 
   parallelo,per non avere più processi che usano lo stesso seme*/

   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   //Setting the random generator

   ifstream input("seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd->SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;

   int M = 1000000;
   int N = 100;
   int throws = M/N;

   BasicFunction *f = new Func_01();
   Integral *I = new Integral(0., 1., f, rnd);

   vector<double> integral;
   vector<double> integral_prog;
   vector<double> err_prog;

   integral.push_back(I->uniformMC(throws));
   for (int i = 1; i < N; ++i){
      integral.push_back(I->uniformMC(throws));
      integral_prog.push_back(average(integral));
      err_prog.push_back(meanStdDev(variance(integral),i));
   }

   print2Col(integral_prog, err_prog, "uniform_sampling.dat");

   delete I;
   delete f;

   f = new Func_02();
   I = new Integral(0., 1., f, rnd);

   vector<double> integral2;
   vector<double> integral2_prog;
   vector<double> err2_prog;

   integral2.push_back(I->importanceMC_01(throws));
   for (int i = 1; i < N; ++i){
      integral2.push_back(I->importanceMC_01(throws));
      integral2_prog.push_back(average(integral2));
      err2_prog.push_back(meanStdDev(variance(integral2),i));
   }

   print2Col(integral2_prog, err2_prog, "importance_sampling.dat");

   rnd->SaveSeed();

   delete I;
   delete f;
   delete rnd;

   return 0;
}