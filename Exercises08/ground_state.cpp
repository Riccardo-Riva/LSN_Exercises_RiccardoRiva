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

//== CONSTANTS =========
#define hbar 1.
#define boltzmann 1.
#define mass 1.
//======================

using namespace std;

//============ FUNCTIONS ================

// This is the observable whose average value we want to estimate
double LocalEnergy(double mu, double sigma, vector<double> v);

vector<double> EnergyEstimate(double mu, double sigma, Random *rnd);

double Error(double sum, double sum2, int n);

//=========================================
 
int main (int argc, char *argv[]){

   /** Start Time Measuring **/
   clock_t t;
   t = clock();
   /**************************/

   Random *rnd =new Random();
   int seed[4];
   int p1, p2;

   // // // // Setting the random generator // // // //
   
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

   // // // // // // // // // // // // // // // // // //

   double mu, sigma;

   ifstream parameters("minimum.dat");
   if (parameters.is_open()){
      string parameter;
      parameters >> parameter >> mu;
      parameters >> parameter >> sigma;
   } else cerr << "PROBLEM: Unable to open minimum.dat" << endl;
   parameters.close();

   vector<double> estimate = EnergyEstimate(mu, sigma, rnd);

   delete rnd;

   /** Stop Time Measuring and Display **/
   t = clock() - t;
   cout << endl << "Time: " << t*1.0/CLOCKS_PER_SEC << " seconds" << endl;
   /*************************************/

   return 0;
}

double LocalEnergy(double mu, double sigma, vector<double> v){ // This is the function we change if we want to study another wave function 
   double x = v[0]; 
   double V = pow(x,4) - 2.5*pow(x,2); //potential energy
   
   // wave function
   double psi = exp( -pow(x-mu,2) / (2.*pow(sigma,2)) ) + exp( -pow(x+mu,2) / (2.*pow(sigma,2))); 
   // second derivative of wave function
   double psi_second = psi*((pow(x,2)+pow(mu,2))/pow(sigma,2)-1)/pow(sigma,2) + 2*x*mu*(-exp( -pow(x-mu,2) / (2.*pow(sigma,2)) ) + exp( -pow(x+mu,2) / (2.*pow(sigma,2))))/pow(sigma,4);

   double H_psi = - hbar*hbar*psi_second/(2. * mass) + V*psi;

   return H_psi/psi;
}


vector<double> EnergyEstimate(double mu, double sigma, Random *rnd){
   vector<double> estimate(2, 0.); //estimate[0] contains the value, estimate[1] contains the uncertainty
   
   vector<double> x;
   x.push_back(mu); //starting point
   
   ProbabilityDensity * pdf;
   pdf = new PsiTrial(mu,sigma);
   
   Metropolis Metro(x, pdf, rnd);
   Metro.SetStep(2.8*sigma);

//==== Equilibration ========================

   double accept=0.;
      // vector<double> equilibration;
      int ntest = 1000;
      for (int i = 0; i < ntest; ++i){
         // M->Uniform_Step();
         Metro.Uniform_Step();
         // equilibration.push_back(LocalEnergy(mu, sigma, Metro.GetX()));
         // equilibration.push_back(Metro.GetR());

         if(Metro.GetAccept())
           accept += 1.;
      }
   //cout << endl << "Acceptance Rate: " << accept * 100. / ntest << endl;
   //-print1Col(equilibration, "equilibration.out");
//==========================================

   int nblocks = 40;
   int dimblocks = 5000;

   double ene_ave, ene_prog=0., ene_prog2=0.;
   vector<double> e, err_e; // they contains the value of E and the error as a function of the number of blocks
   vector<double> pos; // it contains the positions to fill an histogram


   for (int i = 0; i < nblocks; ++i){
      ene_ave = 0.;
      for (int j = 0; j < dimblocks; ++j){
         Metro.Uniform_Step();
         ene_ave += LocalEnergy(mu, sigma, Metro.GetX());
         pos.push_back(Metro.GetX()[0]);
      }
      ene_ave /= (double)dimblocks;
      ene_prog += ene_ave;
      ene_prog2 += ene_ave*ene_ave;
      
      e.push_back(ene_prog/(double)(i+1));
      err_e.push_back(Error(ene_prog, ene_prog2, i));
   }

   // Printing Energy and positions
   print2Col(e, err_e, "energy.out");
   print1Col(pos, "positions.out");

   estimate[0] = ene_prog/(double)(nblocks);
   estimate[1] = Error(ene_prog, ene_prog2, nblocks-1);

   return estimate;

   delete pdf;
}

double Error(double sum, double sum2, int n){
   if(n > 0)
      return sqrt((sum2/(double)(n+1) - pow(sum/(double)(n+1),2))/(double)n);
   else return 0.;
}
