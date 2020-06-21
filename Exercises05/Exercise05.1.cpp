#include <iostream>
#include <fstream>
#include <string>

#include "random.h"
#include "library.h"

#include "metropolis.h"
#include "probabilityDensity.h"
#include "pdf.h"

#include <vector>

#include <ctime> // time measuring

using namespace std;
 
int main (int argc, char *argv[]){

   /** Start Time Measuring **/
   clock_t t;
   t = clock();
   /**************************/

   Random *rnd =new Random();
   int seed[4];
   int p1, p2;


    // // Setting the random generator // // // //
   
   ifstream Primes("Primes"); /*this part is useful for parallel prgramming*/
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

  

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

   // // // // // // // // // // // // // // // //

   vector<double> x;  
   ProbabilityDensity * PDF;
   Metropolis * M;

   /*
   ifstream inputPosition("start.dat");
   if (inputPosition.is_open()){
      double pos = 0.;
      for (int i = 0; i < 3; ++i){
         inputPosition >> pos;
         x.push_back(pos);
      }
   } else cerr << "PROBLEM: Unable to open start.dat" << endl;
   inputPosition.close();
   */

   int steps = 1000000; // 10^6
   int nblocks = 100;
   int dimblocks = steps / nblocks;

   vector<double> ave_r, prog_r, err_r;

   vector<double> equilibration_r;
   int ntest = 100;

   //=======================================================
   //========= Hydrogen Atom - (n,l,m) = (1,0,0) 
   //=======================================================

   x.push_back(1.5); // setting initial position
   x.push_back(0.);
   x.push_back(0.);

   vector<vector<double>> pos; // it contains all the simulated positions
   vector<double> r; // it contains the distance from the origin of the simulated positions

   PDF = new Psi100();
   M = new Metropolis(x, PDF, rnd);
   M->SetStep(1.2);
   M->SetSigma(0.7);
   
   /********** Equilibration ***********/

   ///// The commented parts are useful to choose the correct step /////
   double accept1;

   ntest = 150;

   for (int i = 0; i < ntest; ++i){
      // M->Uniform_Step();
      M->Gaussian_Step();
      equilibration_r.push_back(M->GetR());
     
      if(M->GetAccept())
        accept1 += 1.;
   }

   cout << endl << "The percentage of accepted Metropolis step for " << ntest << " steps is " << accept1 * 100. / ntest << endl;

   print1Col(equilibration_r, "equilibration_test_100.out");


   //****** Simulation ***********//

   for (int i = 0; i < nblocks; ++i){
      ave_r.push_back(0.);
      for (int j = 0; j < dimblocks; ++j){
         // M->Uniform_Step();
         M->Uniform_Step();
         ave_r[i] += M->GetR();
         if(j%1000==0)   
            pos.push_back(M->GetX());
         r.push_back(M->GetR());
      }
      ave_r[i] /= (double) dimblocks;
      prog_r.push_back(average(ave_r));
      err_r.push_back(meanStdDev(variance(ave_r),i));
   }

   print2Col(prog_r, err_r, "psi100.out");
   printCol(pos, "positions_100.out"); 
   print1Col(r, "radius_100.out");

   delete PDF;
   delete M; 

   pos.clear();
   r.clear();
   x.clear();
   ave_r.clear();
   prog_r.clear();
   err_r.clear();
   equilibration_r.clear();

   //=======================================================
   //========= Hydrogen Atom - (n,l,m) = (2,1,0) 
   //=======================================================

   x.push_back(5.0); // setting iitial position
   x.push_back(0.);
   x.push_back(0.);

   PDF = new Psi210();
   M = new Metropolis(x, PDF, rnd);
   M->SetStep(3.);
   M->SetSigma(1.8);
   
   /********** Equilibration ***********/

   ///// The commented parts are useful to choose the correct step /////
   double accept2; 

   ntest = 150;

   for (int i = 0; i < ntest; ++i){
      // M->Uniform_Step();
      M->Uniform_Step();
      equilibration_r.push_back(M->GetR());
     
      if(M->GetAccept())
         accept2 += 1.;
   }

   cout << endl << "The percentage of accepted Metropolis step for " << ntest << " steps is " << accept2 * 100. / ntest << endl;

   print1Col(equilibration_r, "equilibration_test_210.out");


   //****** Simulation ***********//

   for (int i = 0; i < nblocks; ++i){
      ave_r.push_back(0.);
      for (int j = 0; j < dimblocks; ++j){
         // M->Uniform_Step();
         M->Gaussian_Step();
         ave_r[i] += M->GetR();
         if(j%1000==0)
            pos.push_back(M->GetX());
         r.push_back(M->GetR());
      }
      ave_r[i] /= (double) dimblocks;
      prog_r.push_back(average(ave_r));
      err_r.push_back(meanStdDev(variance(ave_r),i));
   }


   print2Col(prog_r, err_r, "psi210.out");
   printCol(pos, "positions_210.out");
   print1Col(r, "radius_210.out");

   delete PDF;
   delete M;  

   pos.clear();
   r.clear();
   x.clear();
   ave_r.clear();
   prog_r.clear();
   err_r.clear();
   equilibration_r.clear(); 

   rnd->SaveSeed(); // Output file: seed.out

   delete rnd;

   /** Stop Time Measuring and Display **/
   t = clock() - t;
   cout << endl << "Time: " << t*1.0/CLOCKS_PER_SEC << " seconds" << endl;
   /*************************************/

   return 0;
}