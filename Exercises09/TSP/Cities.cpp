#include <iostream>
#include <fstream>
#include <string>

#include "random.h"
#include "linearVector.h"

#include <vector>

#include <cmath>

#include <ctime> // time measuring


using namespace std;

double distanceOnCircumference(double theta1, double theta2);
double distanceOnPlane(vector<double> p1, vector<double> p2);
 
int main (int argc, char *argv[]){

   /** Start Time Measuring **/
   clock_t t;
   t = clock();
   /**************************/

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

   int number_of_cities = 32;

   //=========================================
   //====== Cities on a Unit Circumference
   //=========================================

   vector<double> cities;
   for (int i = 0; i < number_of_cities; ++i){
      cities.push_back(rnd->Rannyu(0.,2.*M_PI));
   }

   ofstream CitiesOut("cities_circumference.dat");
   if (CitiesOut.is_open()){
      for (int i = 0; i < (int)cities.size(); ++i){
         CitiesOut << i << " " << cities[i];
         CitiesOut << endl;
      }
   } else cerr << endl <<"PROBLEM: Unable to open output file " << "cities_circumference.dat" << endl;
   CitiesOut.close();

   //=========================================
   //====== Cities on a Plane
   //=========================================

   vector<double> x;
   vector<double> y;
   for (int i = 0; i < number_of_cities; ++i){
      x.push_back(rnd->Rannyu());
      y.push_back(rnd->Rannyu());
   }

   CitiesOut.open("cities_plane.dat");
   if (CitiesOut.is_open()){
      for (int i = 0; i < (int)cities.size(); ++i){
         CitiesOut << i << " " << x[i] << " " << y[i];
         CitiesOut << endl;
      }
   } else cerr << endl <<"PROBLEM: Unable to open output file " << "cities_plane.dat" << endl;
   CitiesOut.close();

   rnd->SaveSeed();
   delete rnd;

   /** Stop Time Measuring and Display **/
   t = clock() - t;
   cout << endl << "Time: " << t*1.0/CLOCKS_PER_SEC << " seconds" << endl;
   /*************************************/

   return 0;
}