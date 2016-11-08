#include <algorithm>  // round()
#include <mpi.h>

#include "defines.h"
#include "init_conditions.h"
#include "Lattice.hpp"
#include "poisson1D.h"
#include "save.h"
#include "hdf5.h"

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

  MPI_Init(NULL, NULL);

  int mpi_rank, mpi_size;
  MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
  
  int n_elem_per_proc = NX / mpi_size;
  
  double rho_buffer[n_elem_per_proc];

  save_mem_parallel(lattice, 0, mpi_rank, n_elem_per_proc);

  // Main Loop
  for ( int t = 0; t < NT; t++ )  {
    
    // get density -- scatter allgather
    for ( int x = 0; x < NX; x++ ) 
      rho[x] = 0.0;
    
    MPI_Scatter(rho, n_elem_per_proc, MPI_DOUBLE, rho_buffer,
              n_elem_per_proc, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    for ( int x = 0; x < n_elem_per_proc; x++ ) {
     for ( int vx = 0; vx < NVX; vx++ ) {
       rho_buffer[x] += lattice.get_f(x + mpi_rank * n_elem_per_proc, vx, t);    // this is where the bulk focus of optimization must go
     }
     rho_buffer[x] *= DVX;
    }
    
    MPI_Allgather(&rho_buffer, n_elem_per_proc, MPI_DOUBLE, rho, n_elem_per_proc, MPI_DOUBLE, MPI_COMM_WORLD);

    // get acceleration
    poisson1D(rho, acc);
    
    // store integer accelerations
    lattice.store_accel(t, acc);
    
    // Save output as HDF5
    if(((t+1) % OUTPUTEVERY) == 0)
      save_mem_parallel(lattice, t+1, mpi_rank, n_elem_per_proc);
      
  }
  
  MPI_Finalize();
  
}

#endif
