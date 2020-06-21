#include <iostream>
#include <fstream>
#include <string>

#include "random.h"
#include "individual.h"

#include <vector>
#include <string>

#include <cmath>

#include <ctime> // time measuring

#include"mpi.h" // MPICH library

using namespace std;
 
int main(int argc, char* argv[]){
   // Initializing the parallel computing
   int size, rank;
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   MPI_Comm_rank(MPI_COMM_WORLD, &rank);

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
      
      // Setting differne Primes numbers for each parallel process
      for (int i = 0; i < size; ++i)
         if (rank>=i)
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


   //######### Random Population ##########

   int number_of_cities = 32;
   int number_of_individuals = 60;

   std::vector<Individual> FristGen;

   ofstream out("first_generation" + to_string(rank+1) + ".dat");
   if (out.is_open()){
       out << number_of_cities << " " << number_of_individuals << endl;
   } else cerr << endl <<"PROBLEM: Unable to open first_generation.dat " << endl;

   for (int i = 0; i < number_of_individuals; ++i){
      Individual ind(number_of_cities);
      int number_of_swaps = 2*number_of_cities + 8*(int)floor(rnd->Rannyu()*(double)number_of_cities);

      for (int i = 0; i < number_of_swaps; ++i){
         int i1 = floor(rnd->Rannyu()*(double)(number_of_cities-1))+1;
         int i2 = floor(rnd->Rannyu()*(double)(number_of_cities-1))+1;
         ind.Swap(i1,i2);
      }   

      for (int i = 0; i < number_of_cities; ++i){
         out << ind.GetValue(i) << " ";
      }
      out << endl;
   }

   out.close();

   rnd->SaveSeed();
   delete rnd;

   /** Stop Time Measuring and Display **/
   t = clock() - t;
   cout << endl << "Time: " << t*1.0/CLOCKS_PER_SEC << " seconds" << endl;
   /*************************************/

   MPI_Finalize();

   return 0;
}