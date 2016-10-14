#ifndef SAVE_H
#define SAVE_H

void save(double (&dset_data)[NX][NVX], int time);
void save_mem(int (&accDt)[NT][NX], int time, double dt);

#endif
