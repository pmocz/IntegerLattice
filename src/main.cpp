
#include <ctime>
#include <iostream>
#include <math.h>

#include "defines.h"
#include "iterators.h"
#include "poisson1D.h"
#include "save.h"

using namespace std;



double lattice[NX][NVX]; // lattice (distribution function f)
double rho[NX];          // density
double acc[NX];          // acceleration
double dt = TMAX / NT;   // timestep





/** set the initial condition */
double get_init_f(double x, double vx) {
  double f, sigma, A, k;
  sigma = 0.5 / sqrt( 4.0 * M_PI );
  A = 0.01;
  k = 2.0 * 2.0 * M_PI / ( 2.0 * L );
  f = exp( -vx * vx / 2.0 / sigma / sigma );
  f *= ( 1.0 + A * cos( k * x / (2.0 * L) ) );
  f /= sqrt( 2.0 * M_PI * sigma * sigma );
  return f;
}




/** Main Function */
int main() {
  int shift;
  double x_phys, vx_phys;
  clock_t clock_begin, clock_end;
  double elapsed_secs;
  

  // Set the initial conditions
  for ( int x = 0; x < NX; x++ ) 
    for ( int vx = 0; vx < NVX; vx++ ) {
      
      x_phys  = -L + (1.0 * x + 0.5) * DX;
      vx_phys = -V + (1.0 * vx + 0.5) * DVX;

      lattice[x][vx] = get_init_f(x_phys,vx_phys);
    }
  save(lattice, 0);


  // Main Loop
  clock_begin = clock();
  for ( int t = 0; t < NT; t++ )  {
    
    // get density
    for ( int x = 0; x < NX; x++ ) {
     rho[x] = 0.0;
     for ( int vx = 0; vx < NVX; vx++ )   rho[x] += lattice[x][vx];
     rho[x] *= DVX;
    }
    
    // get acceleration
    poisson1D(rho, acc);
    
    // kick
    for ( int x = 0; x < NX; x++ ) {
      shift = ( NVX - (int) round( dt * acc[x] / DVX ) ) % NVX;
      rotate( row_begin(lattice,x), row_entry(lattice,x,shift), row_end(lattice,x) );
    }
    
    // drift
    for ( int vx = 0; vx < NVX; vx++ ) {
      vx_phys = -V + (1.0 * vx + 0.5) * DVX;
      shift = ( NX - (int) round( dt * vx_phys / DX ) ) % NX;
      rotate( col_begin(lattice,vx), col_entry(lattice,vx,shift), col_end(lattice,vx) );
    }
    
    // Save output as HDF5
    if(((t+1) % OUTPUTEVERY) == 0)
      save(lattice, t+1);
      
  }
  clock_end = clock();
  
  elapsed_secs = double(clock_end - clock_begin) / CLOCKS_PER_SEC;
  
  cout << "Simulation done! Took " << elapsed_secs << " seconds\n";
  
}
