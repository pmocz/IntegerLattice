#ifndef SAVE_H
#define SAVE_H

#include "Lattice.hpp"

void save(double (&dset_data)[NX][NVX], int time);
void save_mem(Lattice& lattice, int time);

#endif
