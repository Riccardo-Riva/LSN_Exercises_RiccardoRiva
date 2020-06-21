#include <iostream>
#include <fstream>
#include <string>

#include "random.h"
#include "functions.h"

#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;
 
int main (int argc, char *argv[]){


   Random rnd;
   int seed[4];
   int p1, p2;

   /*questo deve esserci sempre, utile per il calcolo parallelo,per non avere più processi che usano lo stesso seme*/
   ifstream Primes("Primes");
   if (Primes.is_open()){
      Primes >> p1 >> p2 ;
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   int M = 1000000;
   int num = 10000;
   int N[4] = {1, 2, 10, 100};

   //Setting the random generator

   ifstream input("seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;

   //============================
   //==== STANDARD DICE
   //============================

   vector<int> std_dice;

   // DICE THOWS
   for(int i=0; i < M; i++)
      std_dice.push_back(floor(rnd.Rannyu()*6.)+1);

   vector<vector<double>> tests_std(4);

   for (int i = 0; i < 4; ++i)
      for (int j = 0; j < num; ++j){
         tests_std[i].push_back(0.);
         for (int k = 0; k < N[i]; k++)
            tests_std[i][j]+=(double)std_dice[N[i]*j + k];
         tests_std[i][j]/=(double)N[i];
      }

   printCol(tests_std, "standard_dice.dat");      

   /*
   vector<vector<double>> tests(4);

   for (int i = 0; i < 4; ++i){
      for (int j = N[i]; j <= 6*N[i]; ++j)
         tests[i].push_back(0.);
      for (int j = 0; j < num ; ++j){
         int s=0;
         for (int k = 0; k < N[i]; k++){
            s+=dice[N[i]*j + k];
         }
         tests[i][s-N[i]]++;
      }
   }
   

   string s1 = "standard_dice";
   string s2 = ".dat";
   for (int i = 0; i < 4; ++i){
      print1Col(tests[i], s1 + to_string(N[i]) + s2);
   }
   */

   //============================
   //==== LORENTIAN DICE
   //============================

   vector<double> lor_dice;

   // DICE THOWS
   for(int i=0; i < M; i++)
      lor_dice.push_back(rnd.Lorentz(0., 1.));     

   vector<vector<double>> tests_lor(4);

   for (int i = 0; i < 4; ++i)
      for (int j = 0; j < num; ++j){
         tests_lor[i].push_back(0.);
         for (int k = 0; k < N[i]; k++)
            tests_lor[i][j]+=lor_dice[N[i]*j + k];
         tests_lor[i][j]/=(double)N[i];
      }

   printCol(tests_lor, "lorentian_dice.dat");

   //============================
   //==== EXPONENTIAL DICE
   //============================

   vector<double> exp_dice;

   // DICE THOWS
   for(int i=0; i < M; i++)
      exp_dice.push_back(rnd.Exp(1.));

   vector<vector<double>> tests_exp(4);

   for (int i = 0; i < 4; ++i)
      for (int j = 0; j < num; ++j){
         tests_exp[i].push_back(0.);
         for (int k = 0; k < N[i]; k++)
            tests_exp[i][j]+=exp_dice[N[i]*j + k];
         tests_exp[i][j]/=(double)N[i];
      }

   printCol(tests_exp, "exponential_dice.dat");

   rnd.SaveSeed();
   return 0;
}