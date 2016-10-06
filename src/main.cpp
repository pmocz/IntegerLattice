#include <algorithm>
#include <ctime>
#include <iostream>
#include <iterator>

#include <math.h>

#include "defines.h"
#include "poisson1D.h"
#include "save.h"

double lattice[NX][NVX];
double rho[NX];
double acc[NX];
double dt = 1.0 * TMAX / NT;

using namespace std;



/** Row iterator*/
struct row_iterator : public iterator<forward_iterator_tag, double> {
 private:
  double* row_ptr_;
 public:
  row_iterator(double* row) : row_ptr_(row) {}
  row_iterator& operator++() { row_ptr_++; return *this;}
  row_iterator operator++(int) { row_iterator tmp(*this); operator++(); return tmp; }
  bool operator==(const row_iterator& rhs) { return row_ptr_ == rhs.row_ptr_; }
  bool operator!=(const row_iterator& rhs) { return row_ptr_ != rhs.row_ptr_; }
  double& operator*() {return *row_ptr_;} 
};

row_iterator row_begin(int row) {  return row_iterator(&(lattice[row][0]));          }
row_iterator row_end(int row)   {  return row_iterator(&(lattice[row][0]) + NVX); }
row_iterator row_entry(int row, int col) {  return row_iterator(&(lattice[row][0]) + col); }




/** Column iterator*/
struct col_iterator : public iterator<forward_iterator_tag, double> {
 private:
  double* col_ptr_;
 public:
  col_iterator(double* col) : col_ptr_(col) {}
  col_iterator& operator++() { col_ptr_+=NVX; return *this;}
  col_iterator operator++(int) { col_iterator tmp(*this); operator++(); return tmp; }
  bool operator==(const col_iterator& rhs) { return col_ptr_ == rhs.col_ptr_; }
  bool operator!=(const col_iterator& rhs) { return col_ptr_ != rhs.col_ptr_; }
  double& operator*() {return *col_ptr_;} 
};

col_iterator col_begin(int col) {  return col_iterator(&(lattice[0][col]));          }
col_iterator col_end(int col)   {  return col_iterator(&(lattice[0][col]) + NVX*NX); }
col_iterator col_entry(int col, int row) {  return col_iterator(&(lattice[0][col]) + NVX*row); }




/** set the initial condition */
double get_init_f(double x, double vx) {
  double f, sigma, A, k;
  
  sigma = 0.5 / sqrt(4.0*M_PI);
  A = 0.01;
  k = 2.0 * 2*M_PI / (2.0*L);
  f = exp( -vx*vx/2.0/sigma/sigma );
  f *= ( 1.0 + A*cos(k*x/(2.0*L)) );
  f /= sqrt( 2.0*M_PI*sigma*sigma );
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
      
      x_phys  = -L + (1.0*x + 0.5) * DX;
      vx_phys = -V + (1.0*vx + 0.5) * DVX;

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
      rotate( row_begin(x), row_entry(x,shift), row_end(x) );
    }
    
    // drift
    for ( int vx = 0; vx < NVX; vx++ ) {
      vx_phys = -V + (1.0*vx + 0.5) * DVX;
      shift = ( NX - (int) round( dt * vx_phys / DX ) ) % NX;
      rotate( col_begin(vx), col_entry(vx,shift), col_end(vx) );
    }
    
    // Save output as HDF5
    if(((t+1) % OUTPUTEVERY) == 0)
      save(lattice, t+1);
      
  }
  clock_end = clock();
  
  elapsed_secs = double(clock_end - clock_begin) / CLOCKS_PER_SEC;
  
  cout << "Simulation done! Took " << elapsed_secs << " seconds\n";
  
}
