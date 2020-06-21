#include <iostream>
#include <fstream>
#include <string>

#include "random.h"
#include "population.h"
#include "linearVector.h"

#include <cmath>
#include <algorithm>
#include <vector>
#include <string>

#include <ctime> // time measuring

#include"mpi.h" // MPICH library

using namespace std;

double distanceOnCircumference(double theta1, double theta2);
double distanceOnPlane(vector<double> p1, vector<double> p2);
 
int main (int argc, char *argv[]){
   // Initializing the parallel computing
   int size, rank;
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   vector<MPI_Status> stat;
   for (int i = 0; i < size; ++i){
      MPI_Status stat_i;
      stat.push_back(stat_i);
   }

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
      
      // Setting differne Primes numbers for each parallel process
      for (int i = 0; i < size; ++i)
         if (rank>=i)
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

   int N_migr = 250;

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
   pop.ReadPopulationFromFile("first_generation" + to_string(rank+1) + ".dat");

   for (int i = 0; i < 3100; ++i){
      pop.Generate();

      if (i%N_migr==0){//************* MIGRATION *******************//
         
         vector<int> BestPath = pop.GetPath(0); // the best path of each continent
         
         vector<int> itag; // this vector contains the tags for each exchange of path between the continents
         
         for (int j = 0; j < size; ++j)
            itag.push_back(j+1);
            
         vector<int*> imesg;
         
         for (int j = 0; j < size; ++j){
            int* imesg_j = new int[number_of_cities];
            for (int k = 0; k < number_of_cities; ++k)
               imesg_j[k] = BestPath[k];
            imesg.push_back(imesg_j);
         }

         // Every continent send its best path to the next continent
         MPI_Send(imesg[rank], number_of_cities, MPI_INTEGER, indexPBC(rank+1,size), itag[rank], MPI_COMM_WORLD);
         MPI_Recv(imesg[indexPBC(rank-1,size)], number_of_cities, MPI_INTEGER, indexPBC(rank-1,size), itag[indexPBC(rank-1,size)], MPI_COMM_WORLD, &stat[rank]);
         
         for (int j = 0; j < number_of_cities; ++j)
            BestPath[j] = imesg[indexPBC(rank-1,size)][j];      

         pop.ChangePath(0, BestPath);
      }



      if (((i+1)%100)==0){
         //cout << endl << "Shortest Path in Generation " << i+1 << ": " << pop.Cost(0);
         //cout << endl << "Generation " << i+1;
      }
   }
   cout << endl;

   pop.PrintOnFile("final_generation" + to_string(rank+1) + ".dat");

   cout << endl << " Length of the best path for process " << rank +1 << ": " << pop.Cost(0) << endl;

   distances.clear();
   points.clear();   

   rnd->SaveSeed();
   delete rnd;

   /** Stop Time Measuring and Display **/
   t = clock() - t;
   cout << endl << "Time: " << t*1.0/CLOCKS_PER_SEC << " seconds" << endl;
   /*************************************/

   MPI_Finalize();
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