#include <algorithm>  // round()

#include "defines.h"
#include "init_conditions.h"
#include "Lattice.hpp"
#include "poisson1D.h"
#include "save.h"

using namespace std;

#ifdef METHOD_MEMSAVE

/** Memory-Saving Integer Lattice Algorithm
 * (more computationally expensive than original algorithm,
 * but cheap on memory)
 */
void run_simulation() {
  
  Lattice lattice;
  double rho[NX];          // density
  double acc[NX];          // acceleration

  save_mem(lattice, 0);

  // Main Loop
  for ( int t = 0; t < NT; t++ )  {
    
    // get density
    for ( int x = 0; x < NX; x++ ) {
     rho[x] = 0.0;
     for ( int vx = 0; vx < NVX; vx++ ) {
       rho[x] += lattice.get_f(x, vx, t);    // this is where the bulk focus of optimization must go
     }
     rho[x] *= DVX;
    }

    // get acceleration
    poisson1D(rho, acc);
    
    // store integer accelerations
    lattice.store_accel(t, acc);
    
    // Save output as HDF5
    if(((t+1) % OUTPUTEVERY) == 0)
      save_mem(lattice, t+1);
      
  }
  
}

#endif
