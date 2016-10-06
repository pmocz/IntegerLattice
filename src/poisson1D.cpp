#include <fftw3.h>
#include <math.h>

#include "defines.h"
#include "poisfft.h"

#define REAL 0
#define IMAG 1


/** get the acceletaion  -- uses PoisFFT package
 *  k = 2*pi*(-Nx/2:Nx/2-1)'
 *  phihat = -fftshift(fft(4*pi*(rho-rhobar)))./ ( k.^2  + (k==0));
 *  phi = ifft(ifftshift(phihat));
 *  a = -B*phi;
**/
void poisson1D(const double (&rho)[NX], double (&acc)[NX]) {

  // domain dimensions
  const double Ls[DIM] = {2.0*L};

  // gridpoint numbers
  const int ns[DIM] = {NX};
  
  //boundary conditions
  const int BCs[2*DIM] = {POISFFT_PERIODIC, POISFFT_PERIODIC};
  

  // allocate the arrays contiguously, you can use any other class
  // from which you can get a pointer to contiguous buffer
  double *phi = new double[ns[0]];
  double *RHS = new double[ns[0]];

  // set the right-hand side
  for(int x = 0; x < NX; x++) RHS[x] = 4.0*M_PI*(rho[x] - 1.0);

  // create solver object, 3 dimensions, double precision
  PoisFFT::Solver<DIM, double> S(ns, Ls, BCs);
  
  //run the solver, can be run many times for different right-hand sides
  S.execute(phi, RHS);
  
  // calculate acceleration (2nd-order central difference)
  acc[0] = -( -3.0*phi[0] + 4.0*phi[1] - phi[2] ) / (2.0*DX);
  acc[NX-1] = -( 3.0*phi[NX-1] - 4.0*phi[NX-2] + phi[NX-3] ) / (2.0*DX);
  for(int x = 1; x < NX-1; x++)
    acc[x] = -( phi[x + 1] - phi[x - 1] ) / (2.0*DX);
  
  // free memory
  delete[] RHS;
  delete[] phi;
}	
