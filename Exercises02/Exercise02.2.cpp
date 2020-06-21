#include <iostream>
#include <fstream>
#include <string>

#include "randomWalk.h"
#include "random.h"
#include "library.h"

#include <vector>
#include <algorithm>
#include <cmath>

#include <ctime> // time measuring

using namespace std;
 
int main (int argc, char *argv[]){

   Random *rnd = new Random();
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


   /** Start Time Measuring **/
   clock_t t;
   t = clock();

   int M = 10000;
   int blocks = 100;
   int throws = M/blocks;
   int steps = 100;

   double a = 1.;

   RandomWalk rw(rnd);
   rw.SetA(a);

   //=========================================
   //======== Random Walk on Lattice 
   //=========================================
   vector<double> R;
   vector<double> err_R;
   vector<double> position(blocks, 0.);

   for (int i = 1; i < steps; ++i){
      for (int j = 0; j < blocks; ++j){
         vector<double> pos;
         for (int k = 0; k < throws; ++k){
            rw.LatticeRW(i+1);
            pos.push_back(rw.GetR());
            rw.Reset();
         }
         position[j]=average(pos);
      }
      R.push_back(average(position));
      err_R.push_back(meanStdDev(variance(position), blocks-1));
   }

   print2Col(R, err_R, "lattice.dat");

   //=========================================
   //======== Random Walk on Continuum
   //=========================================
   vector<double> Rc;
   vector<double> err_Rc;

   for (int i = 1; i < steps; ++i){
      for (int j = 0; j < blocks; ++j){
         vector<double> pos;
         for (int k = 0; k < throws; ++k){
            rw.ContinuumRW(i+1);
            pos.push_back(rw.GetR());
            rw.Reset();
         }
         position[j]=average(pos);
      }
      Rc.push_back(average(position));
      err_Rc.push_back(meanStdDev(variance(position), blocks-1));
   }

   print2Col(Rc, err_Rc, "continuum.dat");

   rnd->SaveSeed();
   delete rnd;

   /** Stop Time Measuring and Display **/
   t = clock() - t;
      cout << endl << "Time: " << t*1.0/CLOCKS_PER_SEC << " seconds" << endl;


   return 0;
}