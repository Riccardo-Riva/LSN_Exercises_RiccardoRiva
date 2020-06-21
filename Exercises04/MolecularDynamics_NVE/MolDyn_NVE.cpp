/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
#include <stdlib.h>     // srand, rand: to generate random number
#include <iostream>     // cin, cout: Standard Input/Output Streams Library
#include <fstream>      // Stream class to both read and write from/to files.
#include <cmath>        // rint, pow
#include <vector>
#include "MolDyn_NVE.h"

#include "library.h" // for average and stddev of std::vector

#include <ctime> // time measuring

#define KB 1.38064852e-23 // Boltzmann Constant

using namespace std;

int main(){

  /** Start Time Measuring **/
  clock_t t;
  t = clock();
  /**************************/

  // DATA BLOCKING //
  vector<double> ave_temp, ave_epot, ave_ekin, ave_etot, ave_pres;
  vector<double> ave_prog_temp, ave_prog_epot, ave_prog_ekin, ave_prog_etot, ave_prog_pres;
  vector<double> err_temp, err_epot, err_ekin, err_etot, err_pres;
  
  /**** computing g(r) *********/
  vector<double> g(100, 0.);
  vector<double> err_g(100, 0.);

  double sum_temp=0., sum_pot=0., sum_kin=0., sum_etot=0., sum_pres=0.;

  Input();             //Inizialization

  int nconf = 1;
  int imes = 0;
  int iblocks = 1;

  for(int istep=1; istep <= nstep; ++istep){
    Move();           //Move particles with Verlet algorithm
    if(istep%iprint == 0) cout << "Number of time-steps: " << istep << endl;
    
    if(istep%10 == 0){
      Measure();     //Properties measurement
      imes++; 
      sum_temp += stima_temp;
      sum_etot += stima_etot;
      sum_kin += stima_kin;
      sum_pot += stima_pot;
      sum_pres += stima_pres;

      ConfXYZ(nconf);       //Write actual configuration in XYZ format //Commented to avoid "filesystem full"! 
      nconf += 1;
    }
    if(istep%dimblocks == 0){
       // DATA BLOCKING

      ave_temp.push_back(sum_temp/(double)(imes));
      ave_ekin.push_back(sum_kin/(double)(imes));
      ave_epot.push_back(sum_pot/(double)(imes));
      ave_etot.push_back(sum_etot/(double)(imes));
      ave_pres.push_back(sum_pres/(double)(imes));

      ave_prog_temp.push_back(average(ave_temp));
      ave_prog_ekin.push_back(average(ave_ekin));
      ave_prog_epot.push_back(average(ave_epot));
      ave_prog_etot.push_back(average(ave_etot));
      ave_prog_pres.push_back(average(ave_pres));

      err_temp.push_back(meanStdDev(variance(ave_temp), iblocks));
      err_ekin.push_back(meanStdDev(variance(ave_ekin), iblocks));
      err_etot.push_back(meanStdDev(variance(ave_etot), iblocks));
      err_epot.push_back(meanStdDev(variance(ave_epot), iblocks));
      err_pres.push_back(meanStdDev(variance(ave_pres), iblocks));


      /********** computing g(r) - exercitation 07**********/
      for (int i = 0; i < nbins; ++i){
        double dr3 = pow((double)(i+1)*bin_size,3) - pow((double)(i)*bin_size,3);
        double gdir = bins[i] / dimblocks / (rho * (double)npart * 4. * M_PI * dr3 / 3.);
        glob_av[i] += gdir;
        glob_av2[i] += gdir*gdir;
        err_gofr = sqrt(glob_av2[i]/(double)iblocks - pow(glob_av[i]/(double)iblocks, 2));
        g[i] = glob_av[i]/(double)iblocks;
        err_g[i] = err_gofr;
      }
      /*****************************************************/

      iblocks++;
      imes = 0;

      for(int i=0; i<nbins; i++) bins[i]=0.;
      sum_temp=0.; 
      sum_pot=0.; 
      sum_kin=0.;
      sum_etot=0.;
      sum_pres=0.;
    }
  }
  ConfFinal();         //Write final configuration to restart

  print2Col(ave_prog_temp, err_temp, "ave_temp.out");
  print2Col(ave_prog_epot, err_epot, "ave_epot.out");
  print2Col(ave_prog_ekin, err_ekin, "ave_ekin.out");
  print2Col(ave_prog_etot, err_etot, "ave_etot.out");
  print2Col(ave_prog_pres, err_pres, "ave_pres.out");

/****** Printing g(r) - Exercitation 7 *******/
  print2Col(g, err_g, "output.gaveMD.0"); 

  /** Stop Time Measuring and Display **/
  t = clock() - t;
  cout << endl << "Time: " << t*1.0/CLOCKS_PER_SEC << " seconds" << endl;
  /*************************************/

  return 0;
}


void Input(void){ //Prepare all stuff for the simulation
  ifstream ReadInput,ReadConf;
  double ep, ek, pr, et, vir;

  cout << "Classic Lennard-Jones fluid        " << endl;
  cout << "Molecular dynamics simulation in NVE ensemble  " << endl << endl;
  cout << "Interatomic potential v(r) = 4 * [(1/r)^12 - (1/r)^6]" << endl << endl;
  cout << "The program uses Lennard-Jones units " << endl;

  seed = 1;    //Set seed for random numbers
  srand(seed); //Initialize random number generator
  
  ReadInput.open("input.dat"); //Read input

  ReadInput >> reset;

  ReadInput >> temp;

  ReadInput >> npart;
  cout << "Number of particles = " << npart << endl;

  ReadInput >> rho;
  cout << "Density of particles = " << rho << endl;
  vol = (double)npart/rho;
  cout << "Volume of the simulation box = " << vol << endl;
  box = pow(vol,1.0/3.0);
  cout << "Edge of the simulation box = " << box << endl;

  ReadInput >> rcut;
  ReadInput >> delta;
  ReadInput >> nstep;
  ReadInput >> iprint;

  dimblocks = nstep/nblocks;

  cout << "The program integrates Newton equations with the Verlet method " << endl << endl;
  cout << "Time step = " << delta << endl;
  cout << "Number of steps = " << nstep << endl << endl;
  ReadInput.close();

//Prepare array for measurements
  iv = 0; //Potential energy
  ik = 1; //Kinetic energy
  ie = 2; //Total energy
  it = 3; //Temperature
  n_props = 4; //Number of observables

// Computing of g(r)
  igofr = 2;
  nbins = 100;
  bin_size = (box/2.0)/(double)nbins;
  for(int i=0; i<nbins; i++) bins[i]=0.;

//Read initial configuration
  cout << "Read initial configuration from file config.0 " << endl << endl;
  ReadConf.open("config.0");
  for (int i=0; i<npart; ++i){
    ReadConf >> x[i] >> y[i] >> z[i];
    x[i] = x[i] * box;
    y[i] = y[i] * box;
    z[i] = z[i] * box;
  }
  ReadConf.close();

  if(reset == true){ /// only the actual configuration as input ///
//Prepare initial velocities
    cout << "Prepare random velocities with center of mass velocity equal to zero " << endl << endl;
    double sumv[3] = {0.0, 0.0, 0.0};
    for (int i=0; i<npart; ++i){
      vx[i] = rand()/double(RAND_MAX) - 0.5;
      vy[i] = rand()/double(RAND_MAX) - 0.5;
      vz[i] = rand()/double(RAND_MAX) - 0.5;

      sumv[0] += vx[i];
      sumv[1] += vy[i];
      sumv[2] += vz[i];
    }
    for (int idim=0; idim<3; ++idim) sumv[idim] /= (double)npart;
    double sumv2 = 0.0, fs;
    for (int i=0; i<npart; ++i){
      vx[i] = vx[i] - sumv[0];
      vy[i] = vy[i] - sumv[1];
      vz[i] = vz[i] - sumv[2];

      sumv2 += vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i];
    }
    sumv2 /= (double)npart;

    fs = sqrt(3 * temp / sumv2);   // fs = velocity scale factor 
    for (int i=0; i<npart; ++i){
      vx[i] *= fs;
      vy[i] *= fs;
      vz[i] *= fs;

      xold[i] = Pbc(x[i] - vx[i] * delta);
      yold[i] = Pbc(y[i] - vy[i] * delta);
      zold[i] = Pbc(z[i] - vz[i] * delta);
    }
  }
  
  else{ /// also the old configuration as input  ///
    cout << "Read previous configuration from file old.0 " << endl << endl;
    ReadConf.open("old.0");
    for (int i=0; i<npart; ++i){
      ReadConf >> xold[i] >> yold[i] >> zold[i];
      xold[i] = xold[i] * box;
      yold[i] = yold[i] * box;
      zold[i] = zold[i] * box;
    }
    ReadConf.close(); 

    /////  FRIST STEP /////
    double xnew, ynew, znew, fx[m_part], fy[m_part], fz[m_part];

    for(int i=0; i<npart; ++i){ //Force acting on particle i
      fx[i] = Force(i,0);
      fy[i] = Force(i,1);
      fz[i] = Force(i,2);
    }

    for(int i=0; i<npart; ++i){ //Verlet integration scheme

      xnew = Pbc( 2.0 * x[i] - xold[i] + fx[i] * pow(delta,2) );
      ynew = Pbc( 2.0 * y[i] - yold[i] + fy[i] * pow(delta,2) );
      znew = Pbc( 2.0 * z[i] - zold[i] + fz[i] * pow(delta,2) );

      xold[i] = x[i];
      yold[i] = y[i];
      zold[i] = z[i];

      x[i] = xnew;
      y[i] = ynew;
      z[i] = znew;
    }

    double sumv[3] = {0.0, 0.0, 0.0};
    // Computing the velocities at time t + dt/2
    for (int i=0; i<npart; ++i){
      vx[i] = (x[i]-xold[i])/delta;
      vy[i] = (y[i]-yold[i])/delta;
      vz[i] = (z[i]-zold[i])/delta;

      sumv[0] += vx[i];
      sumv[1] += vy[i];
      sumv[2] += vz[i];
    }

    for (int idim=0; idim<3; ++idim) sumv[idim] /= (double)npart;
    double sumv2 = 0.0, fs;
    for (int i=0; i<npart; ++i){
      vx[i] = vx[i] - sumv[0];
      vy[i] = vy[i] - sumv[1];
      vz[i] = vz[i] - sumv[2];
    } // Total Linear Momentum = 0

  // Scaling the Velocities
    for (int i=0; i<npart; ++i) sumv2 += vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i];
    sumv2 /= (double)npart;

    fs = sqrt(3 * temp / sumv2);   // fs = velocity scale factor 
    for (int i=0; i<npart; ++i){
      vx[i] *= fs;
      vy[i] *= fs;
      vz[i] *= fs;

      xold[i] = Pbc(x[i] - vx[i] * delta);
      yold[i] = Pbc(y[i] - vy[i] * delta);
      zold[i] = Pbc(z[i] - vz[i] * delta);
    }
  } 
  return;
}


void Move(void){ //Move particles with Verlet algorithm
  double xnew, ynew, znew, fx[m_part], fy[m_part], fz[m_part];

  for(int i=0; i<npart; ++i){ //Force acting on particle i
    fx[i] = Force(i,0);
    fy[i] = Force(i,1);
    fz[i] = Force(i,2);
  }

  for(int i=0; i<npart; ++i){ //Verlet integration scheme

    xnew = Pbc( 2.0 * x[i] - xold[i] + fx[i] * pow(delta,2) );
    ynew = Pbc( 2.0 * y[i] - yold[i] + fy[i] * pow(delta,2) );
    znew = Pbc( 2.0 * z[i] - zold[i] + fz[i] * pow(delta,2) );

    vx[i] = Pbc(xnew - xold[i])/(2.0 * delta);
    vy[i] = Pbc(ynew - yold[i])/(2.0 * delta);
    vz[i] = Pbc(znew - zold[i])/(2.0 * delta);

    xold[i] = x[i];
    yold[i] = y[i];
    zold[i] = z[i];

    x[i] = xnew;
    y[i] = ynew;
    z[i] = znew;
  }
  return;
}

double Force(int ip, int idir){ //Compute forces as -Grad_ip V(r)
  double f=0.0;
  double dvec[3], dr;

  for (int i=0; i<npart; ++i){
    if(i != ip){
      dvec[0] = Pbc( x[ip] - x[i] );  // distance ip-i in pbc
      dvec[1] = Pbc( y[ip] - y[i] );
      dvec[2] = Pbc( z[ip] - z[i] );

      dr = dvec[0]*dvec[0] + dvec[1]*dvec[1] + dvec[2]*dvec[2];
      dr = sqrt(dr);

      if(dr < rcut){
        f += dvec[idir] * (48.0/pow(dr,14) - 24.0/pow(dr,8)); // -Grad_ip V(r)
      }
    }
  }
  
  return f;
}

void Measure(){ //Properties measurement
  int bin;
  double v, w, t, vij, wij;
  double dx, dy, dz, dr;
  ofstream Epot, Ekin, Etot, Temp, Pres;

  Epot.open("output_epot.dat",ios::app);
  Ekin.open("output_ekin.dat",ios::app);
  Temp.open("output_temp.dat",ios::app);
  Etot.open("output_etot.dat",ios::app);
  Pres.open("output_pres.dat",ios::app);

  w = 0.0; //reset observables
  v = 0.0; //reset observables
  t = 0.0;

//cycle over pairs of particles
  for (int i=0; i<npart-1; ++i){
    for (int j=i+1; j<npart; ++j){

     dx = Pbc( xold[i] - xold[j] );
     dy = Pbc( yold[i] - yold[j] );
     dz = Pbc( zold[i] - zold[j] );

     dr = dx*dx + dy*dy + dz*dz;
     dr = sqrt(dr);

     //update of the histogram of g(r)
     if (dr < box/2.)
      bins[(int) floor((double)nbins * (2. * dr / box))] += 2.;

     if(dr < rcut){
       vij = 4.0/pow(dr,12) - 4.0/pow(dr,6);
       wij = 48.0/pow(dr,12) - 24.0/pow(dr,6);

//Potential energy
       v += vij;
       w += wij;
     }
    }          
  }

  virial.push_back(w);

//Kinetic energy
  for (int i=0; i<npart; ++i) t += 0.5 * (vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i]);
   
    stima_pot = v/(double)npart; //Potential energy per particle
    stima_kin = t/(double)npart; //Kinetic energy per particle
    stima_temp = (2.0 / 3.0) * t/(double)npart; //Temperature
    stima_etot = (t+v)/(double)npart; //Total energy per particle
    stima_pres = rho * KB * stima_temp + average(virial)/(3.0*pow(box,3));

    Epot << stima_pot  << endl;
    Ekin << stima_kin  << endl;
    Temp << stima_temp << endl;
    Etot << stima_etot << endl;
    Pres << stima_pres << endl;

    Epot.close();
    Ekin.close();
    Temp.close();
    Etot.close();
    Pres.close();

    return;
}


void ConfFinal(void){ //Write the actual and previous configuration 
  ofstream WriteConf, WriteOldConf;  

  cout << "Print final configuration to file config.final " << endl;
  cout << "Print second-last configuration to file old.final " << endl << endl;

  WriteConf.open("config.final");
  WriteOldConf.open("old.final");
  for (int i=0; i<npart; ++i){
    WriteConf << x[i]/box << "   " <<  y[i]/box << "   " << z[i]/box << endl;
    WriteOldConf << xold[i]/box << "   " <<  yold[i]/box << "   " << zold[i]/box << endl;
  }
  WriteConf.close();
  WriteOldConf.close();

  return;
}

void ConfXYZ(int nconf){ //Write configuration in .xyz format
  ofstream WriteXYZ;

  WriteXYZ.open("frames/config_" + to_string(nconf) + ".xyz");
  WriteXYZ << npart << endl;
  WriteXYZ << "This is only a comment!" << endl;
  for (int i=0; i<npart; ++i){
    WriteXYZ << "LJ  " << Pbc(x[i]) << "   " <<  Pbc(y[i]) << "   " << Pbc(z[i]) << endl;
  }
  WriteXYZ.close();
}

double Pbc(double r){  //Algorithm for periodic boundary conditions with side L=box
    return r - box * rint(r/box);
}
/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
