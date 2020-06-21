#include <iostream>
#include <fstream>
#include <string>

#include "random.h"
#include "population.h"
#include "linearVector.h"

#include <cmath>
#include <algorithm>
#include <vector>

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

   Population pop(rnd);
   vector<vector<double>> distances;
   int number_of_cities = 32;

   //=========================================
   //====== Cities on a Unit Circumference
   //=========================================

   vector<double> angles;

   ifstream Circ("cities_circumference.dat");
   if (Circ.is_open()){
      int city;
      double position;
      for (int i = 0; i < number_of_cities; ++i){
         Circ >> city >> position;
         angles.push_back(position);
      }
   }  else cerr << endl <<"PROBLEM: Unable to open file " << "cities_circumference.dat" << endl;
   Circ.close();

   cout << endl << "Travelling Salesman Problem on a Circumference:" << endl;
   cout << "Number of Cities: " << number_of_cities << endl;

   for (int i = 0; i < number_of_cities; ++i){
      vector<double> row(number_of_cities, 0.);
      distances.push_back(row);
   }

   for (int i = 0; i < number_of_cities; ++i){
      for (int j = i; j < number_of_cities; ++j){
         double distanceij = distanceOnCircumference(angles[i],angles[j]);
         distances[i][j]=distanceij;
         distances[j][i]=distanceij; 
      }
   }

   pop.SetDistances(distances);
   pop.ReadPopulationFromFile("first_generation.dat");

   ofstream avearges_circ("length_averages_circ.dat");
   if (!avearges_circ.is_open()){
      cerr << endl << "PROBLEM: Unable to open output file " << "length_averages_circ.dat" << endl;
   }

   for (int i = 0; i < 1000; ++i){
      pop.Generate();
      double sum =0.;
      for (int i = 0; i < (int)pop.GetSize()/2; ++i)
         sum+=pop.Cost(i);
      sum /= (double)((int)pop.GetSize()/2);

      avearges_circ << i+1 << " " << sum << endl;

      //cout << endl << "Shortest Path in Generation " << i+1 << ": " << pop.Cost(0);
      if (((i+1)%100)==0){
         cout << endl << "Generation " << i+1;
      }
   }
   cout << endl;
   
   avearges_circ.close();

   pop.PrintOnFile("final_generation_circumference.dat");

   vector<int> shortestPath(pop.GetPath(0));

   cout << endl << " Length of the best path: " << pop.Cost(0) << endl;

   ofstream BestPathOut("bestPath_circumference.out");
   if (BestPathOut.is_open()){
      for (int i = 0; i < (int)shortestPath.size(); ++i){
         BestPathOut<< shortestPath[i] << " " << angles[shortestPath[i]];
         BestPathOut << endl;
      }
      BestPathOut << endl;
   } else cerr << endl <<"PROBLEM: Unable to open output file " << "bestPath_circumference.out" << endl;
   BestPathOut.close();

   distances.clear();
   angles.clear();   

   //=========================================
   //====== Cities on a Plane
   //=========================================

   vector<vector<double>> points;

   ifstream Plane("cities_plane.dat");
   if (Plane.is_open()){
      int city;
      double x,y;
      for (int i = 0; i < 32; ++i){
         Plane >> city >> x >> y;
         vector<double> p;
         p.push_back(x);
         p.push_back(y);
         points.push_back(p);
      }
   }  else cerr << endl <<"PROBLEM: Unable to open file " << "cities_circumference.dat" << endl;
   Plane.close();

   cout << endl << "Travelling Salesman Problem on a Plane:" << endl;
   cout << "Number of Cities: " << number_of_cities << endl;

   for (int i = 0; i < number_of_cities; ++i){
      vector<double> row(number_of_cities, 0.);
      distances.push_back(row);
   }

   for (int i = 0; i < number_of_cities; ++i){
      for (int j = i; j < number_of_cities; ++j){
         double distanceij = distanceOnPlane(points[i],points[j]);
         distances[i][j]=distanceij;
         distances[j][i]=distanceij; 
      }
   }

   pop.SetDistances(distances);
   pop.ReadPopulationFromFile("first_generation.dat");

   ofstream avearges_plane("length_averages_plane.dat");
   if (!avearges_plane.is_open()){
      cerr << endl << "PROBLEM: Unable to open output file " << "length_averages_plane.dat" << endl;
   }

   for (int i = 0; i < 6000; ++i){
      pop.Generate();
      double sum =0.;
      for (int i = 0; i < (int)pop.GetSize()/2; ++i)
         sum+=pop.Cost(i);
      sum /= (double)((int)pop.GetSize()/2);

      avearges_plane << i+1 << " " << sum << endl;
      //cout << endl << "Shortest Path in Generation " << i+1 << ": " << pop.Cost(0);
      if (((i+1)%100)==0){
         cout << endl << "Generation " << i+1;
      }
   }
   cout << endl;

   avearges_plane.close();

   pop.PrintOnFile("final_generation_plane.dat");

   shortestPath = pop.GetPath(0);

   cout << endl << " Length of the best path: " << pop.Cost(0) << endl;

   BestPathOut.open("bestPath_plane.out");
   if (BestPathOut.is_open()){
      for (int i = 0; i < (int)shortestPath.size(); ++i){
         BestPathOut<< shortestPath[i] << " " << points[shortestPath[i]][0] << " " << points[shortestPath[i]][1];
         BestPathOut << endl;
      }
      BestPathOut << endl;
   } else cerr << endl <<"PROBLEM: Unable to open output file " << "bestPath_plane.out" << endl;
   BestPathOut.close();

   distances.clear();
   points.clear();   

   rnd->SaveSeed();
   delete rnd;

   /** Stop Time Measuring and Display **/
   t = clock() - t;
   cout << endl << "Time: " << t*1.0/CLOCKS_PER_SEC << " seconds" << endl;
   /*************************************/

   return 0;
}

double distanceOnCircumference(double theta1, double theta2){
   double alpha1 = theta1 - theta2;
   double alpha2 = theta2 - theta1;
   alpha1 -= floor(alpha1/M_PI/2.)*M_PI*2.;
   alpha2 -= floor(alpha2/M_PI/2.)*M_PI*2.;

   return min(alpha1, alpha2);
}

double distanceOnPlane(vector<double> p1, vector<double> p2){
   return sqrt((p1-p2)*(p1-p2));
}