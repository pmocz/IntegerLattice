#include <algorithm>  // round()

#include "defines.h"
#include "init_conditions.h"
#include "Lattice.hpp"
#include "poisson1D.h"
#include "save.h"

using namespace std;


#ifdef METHOD_MEMSAVE

int accDt[NT][NX];       // acceleration integers
double rho[NX];          // density
double acc[NX];          // acceleration
double dt = TMAX / NT;   // timestep


/** Memory-Saving Integer Lattice Algorithm
 * (more computationally expensive than original algorithm,
 * but cheap on memory)
 */
void run_simulation() {
  
  int x_int, vx_int;
  double x_phys, vx_phys;

  save_mem(accDt, 0, dt);

  // Main Loop
  for ( int t = 0; t < NT; t++ )  {
    
    // get density
    for ( int x = 0; x < NX; x++ ) {
     rho[x] = 0.0;
     for ( int vx = 0; vx < NVX; vx++ ) {
       // trace back to initial condition
       x_int = x;
       vx_int = vx;
       for ( int tt = t - 1; tt > -1; tt-- ) {
         // undo drift
         vx_phys = -V + (1.0 * vx_int + 0.5) * DVX;
         x_int += (NX - (int) round( dt * vx_phys / DX ));
         x_int = x_int % NX;
         // undo kick
         vx_int -= accDt[tt][x_int];
       }
       x_phys  = -L + (1.0 * x_int + 0.5) * DX;
       vx_phys = -V + (1.0 * vx_int + 0.5) * DVX;
       
       rho[x] += get_init_f(x_phys,vx_phys);   // XXX make Lattice() class store accDt and recover f(x,v). acts like matrix lattice[x,v]
     }
     rho[x] *= DVX;
    }

    // get acceleration
    poisson1D(rho, acc);
    
    // store integer accelerations
    for ( int x = 0; x < NX; x++ )
      accDt[t][x] = (int) round( dt * acc[x] / DVX );
    
    // Save output as HDF5
    if(((t+1) % OUTPUTEVERY) == 0)
      save_mem(accDt, t+1, dt);
      
  }
  
}

#endif
