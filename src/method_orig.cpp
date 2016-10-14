#include <algorithm>  // round()

#include "defines.h"
#include "init_conditions.h"
#include "iterators.h"
#include "poisson1D.h"
#include "save.h"

using namespace std;


#ifndef METHOD_MEMSAVE

double lattice[NX][NVX]; // lattice (distribution function f)
double rho[NX];          // density
double acc[NX];          // acceleration
double dt = TMAX / NT;   // timestep
  

/** Original Integer Lattice Algorithm
 */
void run_simulation() {
  
  int shift;
  double x_phys, vx_phys;
  
  // Set the initial conditions
  for ( int x = 0; x < NX; x++ ) 
    for ( int vx = 0; vx < NVX; vx++ ) {
      
      x_phys  = -L + (1.0 * x + 0.5) * DX;
      vx_phys = -V + (1.0 * vx + 0.5) * DVX;

      lattice[x][vx] = get_init_f(x_phys,vx_phys);
    }
  save(lattice, 0);


  // Main Loop
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
  
}

#endif
