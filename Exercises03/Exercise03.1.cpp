#include <iostream>
#include <fstream>
#include <string>

#include "random.h"
#include "library.h"
#include "brownianMotion.h"

#include <cmath>
#include <vector>
#include<algorithm>

#include <ctime> // time measuring

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

	/** Start Time Measuring **/
   clock_t t;
   t = clock();

   int M = 500000;
   int N = 200;
   int throws = M/N;

   double S0 = 100.;
   double T = 1.;
   double K = 100.;
   double r = 0.1;
   double sigma = 0.25;

	BrownianMotion gbw(r, sigma, rnd);

   //======================================
   //====== Direct Sample
   //======================================

   /** CALL **/

   vector<double> direct_call;
   vector<double> direct_call_prog;
   vector<double> err_direct_call_prog;

	for (int i = 0; i < N; ++i){
		double sum = 0.;
		for (int j = 0; j < throws; ++j){
			sum += exp(-r*T) * max(0., gbw.GBW(S0, T) - K);
		}
		sum = sum/throws;
		direct_call.push_back(sum);
    	direct_call_prog.push_back(average(direct_call));
    	err_direct_call_prog.push_back(meanStdDev(variance(direct_call),i));
	}

	print2Col(direct_call_prog, err_direct_call_prog, "direct_call.dat");


   /** PUT **/

   vector<double> direct_put;
   vector<double> direct_put_prog;
   vector<double> err_direct_put_prog;

	for (int i = 0; i < N; ++i){
		double sum = 0.;
		for (int j = 0; j < throws; ++j){
			sum += exp(-r*T) * max(0., K - gbw.GBW(S0, T));
		}
		sum = sum/throws;
		direct_put.push_back(sum);
    	direct_put_prog.push_back(average(direct_put));
    	err_direct_put_prog.push_back(meanStdDev(variance(direct_put),i));
	}

	print2Col(direct_put_prog, err_direct_put_prog, "direct_put.dat");

	//======================================
	//====== Discrete Sample
	//======================================

	int n_intervals = 100;

   /** CALL **/

   vector<double> discrete_call;
   vector<double> discrete_call_prog;
   vector<double> err_discrete_call_prog;

	for (int i = 0; i < N; ++i){
		double sum = 0.;
		for (int j = 0; j < throws; ++j){
			double S = S0;
			for(int k = 0; k < n_intervals; ++k)
				S = gbw.GBW(S, T / (double) n_intervals);
			sum += exp(-r*T) * max(0., S - K);
		}
		sum = sum/throws;
		discrete_call.push_back(sum);
    	discrete_call_prog.push_back(average(discrete_call));
    	err_discrete_call_prog.push_back(meanStdDev(variance(discrete_call),i));
	}

	print2Col(discrete_call_prog, err_discrete_call_prog, "discrete_call.dat");

	/** PUT **/

   vector<double> discrete_put;
   vector<double> discrete_put_prog;
   vector<double> err_discrete_put_prog;

	for (int i = 0; i < N; ++i){
		double sum = 0.;
		for (int j = 0; j < throws; ++j){
			double S = S0;
			for(int k=0; k< n_intervals; ++k)
				S = gbw.GBW(S, T / (double) n_intervals);
			sum += exp(-r*T) * max(0., K - S);
		}
		sum = sum/throws;
		discrete_put.push_back(sum);
    	discrete_put_prog.push_back(average(discrete_put));
    	err_discrete_put_prog.push_back(meanStdDev(variance(discrete_put),i));
	}

	print2Col(discrete_put_prog, err_discrete_put_prog, "discrete_put.dat");

	/** Stop Time Measuring and Display **/
	t = clock() - t;
	cout << endl << "Time: " << t*1.0/CLOCKS_PER_SEC << " seconds" << endl;


	rnd->SaveSeed();
	delete rnd;

	return 0;
}