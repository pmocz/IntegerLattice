#ifndef ITERATORS_H
#define ITERATORS_H

#include <algorithm>
#include <iterator>

#include "defines.h"

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

row_iterator row_begin(double (* lattice)[NVX], int row) {  return row_iterator(&(lattice[row][0]));          }
row_iterator row_end(double (* lattice)[NVX], int row)   {  return row_iterator(&(lattice[row][0]) + NVX); }
row_iterator row_entry(double (* lattice)[NVX], int row, int col) {  return row_iterator(&(lattice[row][0]) + col); }



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

col_iterator col_begin(double (* lattice)[NVX], int col) {  return col_iterator(&(lattice[0][col]));          }
col_iterator col_end(double (* lattice)[NVX], int col)   {  return col_iterator(&(lattice[0][col]) + NVX*NX); }
col_iterator col_entry(double (* lattice)[NVX], int col, int row) {  return col_iterator(&(lattice[0][col]) + NVX*row); }

#endif
