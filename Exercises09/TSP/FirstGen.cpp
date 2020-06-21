#include <iostream>
#include <fstream>
#include <string>

#include "random.h"
#include "individual.h"

#include <vector>

#include <cmath>

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


   //######### Random Population ##########

   int number_of_cities = 32;
   int number_of_individuals = 100;

   std::vector<Individual> FristGen;

   ofstream out("first_generation.dat");
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

   return 0;
}