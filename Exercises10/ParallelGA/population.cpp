#include <iostream>
#include <fstream>

#include <cmath>

#include <vector>
#include <string>

#include "population.h"

using namespace std;

Population::Population(Random *rnd){
  m_rnd = rnd;
}

double Population::Cost(int j){
  j = sortedIndex[indexPBC(j,m_population.size())];
  double sum=0.;
  for (int i = 0; i < (int)m_population[j].GetSize(); ++i){
    int i_previous;
    i_previous = indexPBC(i,m_population[j].GetSize());
    int i_next = indexPBC(i+1,m_population[j].GetSize());
    sum += m_distances[m_population[j].GetValue(i_previous)][m_population[j].GetValue(i_next)];
  }
  /*   
  for (int i = 0; i < m_population.size(); ++i)
    sum += pow(m_distances[m_population[j][i]][m_population[j][i+1]], 2);
  */
  return sum;
}

void Population::ChangePath(int i, std::vector<int> newPath){
  Individual ind(newPath);
  m_population[sortedIndex[i]] = ind;
}

void Population::Generate(){
  
  vector<Individual> nextGeneration;

  double p_swap, p_cycle, p_invert, p_swapBlocks, p_tripleSwap, p_crossover;

  /*** Probabilities of mutation ***/

  p_swap = 0.01;
  p_cycle = 0.01;
  p_invert = 0.01;
  p_swapBlocks = 0.01;
  p_tripleSwap = 0.01;
  p_crossover = 0.85;   

  /*********************************/

  for (int i = 0; i < (int)m_population.size()/2; ++i){
    double a;
    a = m_rnd->Rannyu();
    int selection_index = (int)floor(pow(a,1.5)*m_population.size());
    Individual ind1(m_population[sortedIndex[selection_index]]);
    a = m_rnd->Rannyu();
    selection_index = (int)floor(pow(a,1.5)*m_population.size());
    Individual ind2(m_population[sortedIndex[selection_index]]);

// Crossover

    if (m_rnd->Rannyu() < p_crossover){
      int cut = (int)floor(m_rnd->Rannyu()*(ind1.GetSize()));
      vector<int> appo1;
      vector<int> appo2;
      for (int i = cut; i < ind1.GetSize(); ++i){
        appo1.push_back(ind1.GetValue(i));
        appo2.push_back(ind2.GetValue(i));
      }

      int index1=0;
      int index2=0;
      vector<int> new_appo1;
      vector<int> new_appo2;

      for (int i = 0; i < ind2.GetSize(); ++i){
        for (int j = 0; j < (int)appo1.size(); ++j){
          if(appo1[j]==ind2.GetValue(i)){
            new_appo1.push_back(appo1[j]);
            index1++;
          }
          if(appo2[j]==ind1.GetValue(i)){
            new_appo2.push_back(appo2[j]);
            index2++;
          }
        }
      }
      for (int i = cut; i < ind1.GetSize(); ++i){
        ind1.ChangeValue(i,new_appo1[i-cut]);
        ind2.ChangeValue(i,new_appo2[i-cut]);
      }
    }

    if (!(ind1.Check() or ind2.Check())){
      cerr << endl << "ERROR: the individual does not satisfy the bonds" << endl;
    }

// Mutation 1

    if (m_rnd->Rannyu() < p_swap){
      int i1 = floor(m_rnd->Rannyu()*(double)ind1.GetSize());
      int i2 = floor(m_rnd->Rannyu()*(double)ind1.GetSize());
      while(i2==i1) i2 = floor(m_rnd->Rannyu()*(double)ind1.GetSize());
      ind1.Swap(i1,i2);
    }
    if (m_rnd->Rannyu() < p_swap){
      int i1 = floor(m_rnd->Rannyu()*(double)ind2.GetSize());
      int i2 = floor(m_rnd->Rannyu()*(double)ind2.GetSize());
      while(i2==i1) i2 = floor(m_rnd->Rannyu()*(double)ind1.GetSize());
      ind2.Swap(i1,i2);
    }
    

// Mutation 2
    if (m_rnd->Rannyu() < p_cycle){
      int index = floor(m_rnd->Rannyu()*(double)ind1.GetSize());
      int m = floor(m_rnd->Rannyu()*(double)(ind1.GetSize()-1)+1);
      //int m = floor(m_rnd->Rannyu()*8. + 2.);
      int N = floor(m_rnd->Rannyu()*(double)(m-2))+1;
      ind1.Cycle(index,m,N);  
    }
    if (m_rnd->Rannyu() < p_cycle){
      int index = floor(m_rnd->Rannyu()*(double)ind2.GetSize());
      int m = floor(m_rnd->Rannyu()*(double)(ind1.GetSize()-1)+1);
      //int m = floor(m_rnd->Rannyu()*8. + 2.);
      int N = floor(m_rnd->Rannyu()*(double)(m-2))+1;
      ind2.Cycle(index,m,N);  
    }

// Mutation 3
    if (m_rnd->Rannyu() < p_invert){
      int index = floor(m_rnd->Rannyu()*(double)ind1.GetSize());
      int m = floor(m_rnd->Rannyu()*(double)(ind1.GetSize()-1)+1);
      //int m = floor(m_rnd->Rannyu()*8. + 2.);
      ind1.Invert(index, m);  
    }
    if (m_rnd->Rannyu() < p_invert){
      int index = floor(m_rnd->Rannyu()*(double)(ind2.GetSize()-1))+1;
      int m = floor(m_rnd->Rannyu()*(double)(ind1.GetSize()-1)+1);
      //int m = floor(m_rnd->Rannyu()*8. + 2.);
      ind2.Invert(index, m);  
    }

// Mutation 4
    if (m_rnd->Rannyu() < p_swapBlocks){
      int index = floor(m_rnd->Rannyu()*(double)(ind1.GetSize()));
      int m = floor(m_rnd->Rannyu()*(double)(ind1.GetSize()/2)+1.);
      ind1.SwapBlocks(index, m);  
    }
    if (m_rnd->Rannyu() < p_swapBlocks){
      int index = floor(m_rnd->Rannyu()*(double)(ind1.GetSize()));
      int m = floor(m_rnd->Rannyu()*(double)(ind1.GetSize()/2)+1.);
      ind2.SwapBlocks(index, m);    
    }

// Mutation 5

    if (m_rnd->Rannyu() < p_tripleSwap){
      int i1 = floor(m_rnd->Rannyu()*(double)ind1.GetSize());
      int i2 = floor(m_rnd->Rannyu()*(double)ind1.GetSize());
      int i3 = floor(m_rnd->Rannyu()*(double)ind1.GetSize());
      while(i2==i1) i2 = floor(m_rnd->Rannyu()*(double)ind1.GetSize());
      while(i3==i1 or i3==i1) i3 = floor(m_rnd->Rannyu()*(double)ind1.GetSize());
      ind1.Swap(i1,i2);
      ind1.Swap(i2,i3);
      ind1.Swap(i3,i1);
    }
    if (m_rnd->Rannyu() < p_tripleSwap){
      int i1 = floor(m_rnd->Rannyu()*(double)ind2.GetSize());
      int i2 = floor(m_rnd->Rannyu()*(double)ind2.GetSize());
      int i3 = floor(m_rnd->Rannyu()*(double)ind2.GetSize());
      while(i2==i1) i2 = floor(m_rnd->Rannyu()*(double)ind1.GetSize());
      while(i3==i1 or i3==i1) i3 = floor(m_rnd->Rannyu()*(double)ind1.GetSize());
      ind2.Swap(i1,i2);
      ind2.Swap(i2,i3);
      ind2.Swap(i3,i1);

    }

    nextGeneration.push_back(ind1);
    nextGeneration.push_back(ind2);
  }

  m_population = nextGeneration;
  
  Sorting();
}

void Population::SetDistances(std::vector<std::vector<double>> distances){
  m_distances = distances;
}

void Population::ReadPopulationFromFile(std::string namefile){
  int number_of_cities;
  int number_of_individuals;

  m_population.clear();

  ifstream pop(namefile);
  if (pop.is_open()){
    pop >> number_of_cities >> number_of_individuals;
    for (int i = 0; i < number_of_individuals; ++i){
      Individual ind(number_of_cities);
      for (int j = 0; j < number_of_cities; ++j){
        int val;
        pop >> val;
        ind.ChangeValue(j, val);
      }
      if(!ind.Check())
        cerr << endl << "PROBLEM: The individual at line " 
              << i+2 << " in file " << namefile << " does not fulfil the bonds" << endl;
      m_population.push_back(ind);
    }
  } else cerr << "PROBLEM: Unable to open " << namefile << endl;
  pop.close();

  for (int i = 0; i < number_of_cities; ++i){
    sortedIndex.push_back(i);
  }

  Sorting();
}

void Population::PrintOnFile(std::string filename){
  ofstream out(filename);
  if (out.is_open()){
    out << m_population[0].GetSize() << "\t" << m_population.size() << endl;
    for (int i = 0; i < (int)m_population.size(); ++i){
      for (int j = 0; j < m_population[0].GetSize(); ++j){
        out << m_population[i].GetValue(j) << " ";
      }
      out << endl;
    } 
  } else cerr << endl <<"PROBLEM: Unable to open file " << filename << endl;
  out.close();
}

void Population::Sorting(){
  sortedIndex.clear();
  for (int i = 0; i < (int)m_population.size(); ++i)
    sortedIndex.push_back(i);

  mergeSort(0,m_population.size()-1);
}


//************ PRIVATE ***************//

void Population::mergeSort(int low, int high){
  if(low < high){
    int m = (high+low)/2;
    mergeSort(low,m);
    mergeSort(m+1,high);
    
    merge(low,m,high);
  }
}

void Population::merge(int low, int mid, int high){

  int i, h, j;

  vector<int> appo;     //vettore di appoggio
                        //in questo vettore ricopieremo
                        //in ordine gli elementi

  i=0; // Indichera` la prima posizione libera del vettore appo
  h=low; //Dove inizia il primo sottovettore
  j=mid+1; //Dove inizia il secondo sottovettore.
    //Osservazione: in ogni suddivisione mergesort produce due
    //sottovettori senza soluzione di continuita`.

  while ((h <= mid) and (j<=high)) 
  {
    if(Cost(h) <= Cost(j))
    {
        appo.push_back(sortedIndex[h]);  //Ricopia l'elemento piu` piccolo
                            //dal sottovettore di sx
        h++;            //Incrementa indice vettore sx
    }
    else 
    {
      appo.push_back(sortedIndex[j]); //Ricopia da dx
      j++;            //Incrementa indice dx
    }
    i++; //Incrementa indice vettore appo
  }
    //Qui finsce il while. A questo punto se uno dei due sottovettori
    //non e` vuoto, per costruzione conterra` elementi >= di tutti
    //quelli gia` ricopiati in appo.
    //Riverso quindi in appo gli elementi residui senza fare controlli
    // sui valori

  if(h > mid) 
  { //Vuol dire che il sottovettore di sx e` vuoto

    for (int k = j; k <= high; k++) 
    {
      appo.push_back(sortedIndex[k]);
      i++;
    }
  }
  else 
  { //E` il sottovettore di dx ad essere vuoto
    for (int k = h; k <= mid; k++) 
    {
      appo.push_back(sortedIndex[k]);
      i++;
    }
  }

    //Qui il vettore appo contiene i due sottovettori "fusi"
    //in modo ordinato.
    //Ricopio la sequenza ordinata nel vettore originale.

  for(int k=low; k<=high; k++)
    sortedIndex[k]=appo[k-low];   //Ricordate che il vettore di
                            //appoggio ha indici a partire da 0
}