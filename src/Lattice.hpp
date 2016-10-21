#ifndef LATTICE_HPP
#define LATTICE_HPP

#include <algorithm>  // round()

#include "defines.h"
#include "init_conditions.h"

/** @file Lattice.hpp
 * @brief memory efficient lattice class
 */
 
class Lattice {


 public:
 
 
  /** Constructor */
  Lattice() : accDt_() {
  }
  /** Default destructor */
  ~Lattice() = default;
  
  /** get distrib. func. at coordinate (x,vx) */
  double get_f(int x, int vx, int t) {
    double x_phys, vx_phys;
    
    // trace back to initial condition
    for ( int tt = t - 1; tt > -1; tt-- ) {
      // undo drift
      vx_phys = -V + (1.0 * vx + 0.5) * DVX;
      x += (NX - (int) round( DT * vx_phys / DX ));
      x = x % NX;
      // undo kick
      vx -= accDt_[tt][x];
    }
    x_phys  = -L + (1.0 * x + 0.5) * DX;
    vx_phys = -V + (1.0 * vx + 0.5) * DVX;
    
    return get_init_f(x_phys,vx_phys);

  }
  
  
  void store_accel(int t, double (&acc)[NX]) {
      for ( int x = 0; x < NX; x++ )
        accDt_[t][x] = (int) round( DT * acc[x] / DVX );
  }
  
 private:
  
   int accDt_[NT][NX];  // acceleration integers
 
};

#endif
