#ifndef SAVE_H
#define SAVE_H

#include "Lattice.hpp"

void save(double (&dset_data)[NX][NVX], int time);
void save_mem(Lattice& lattice, int time);
void save_mem_parallel(Lattice& lattice, int time, int mpi_rank, int n_elem_per_proc);

#endif
