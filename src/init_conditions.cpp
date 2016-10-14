#include <math.h>

#include "defines.h"

// use inline version in init_conditions.h

///** set the initial condition */
//double get_init_f(double x, double vx) {
//  double f, sigma, A, k;
//  sigma = 0.5 / sqrt( 4.0 * M_PI );
//  A = 0.01;
//  k = 2.0 * 2.0 * M_PI / ( 2.0 * L );
//  f = exp( -vx * vx / 2.0 / sigma / sigma );
//  f *= ( 1.0 + A * cos( k * x / (2.0 * L) ) );
//  f /= sqrt( 2.0 * M_PI * sigma * sigma );
//  return f;
//}

