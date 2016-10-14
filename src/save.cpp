#include <algorithm>  // round()
#include <stdio.h>
#include <string>

#include "hdf5.h"
#include "defines.h"
#include "init_conditions.h"

double buffer[NVX]; // data buffer for memsave version of code

/** Save the lattice */
void save(double (&dset_data)[NX][NVX], int time) {
  std::string st = std::to_string(time);
  std::string str = "output/snap" + st + ".h5";
  const char * filename = str.c_str();
  
  hid_t       file_id, dataset_id, dataspace_id;  /* identifiers */
  hsize_t     dims[2];
  //herr_t      status;
  
  /* Create a new file using default properties. */
  file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  
  /* Create the data space for the dataset. */
  dims[0] = NX; 
  dims[1] = NVX; 
  dataspace_id = H5Screate_simple(2, dims, NULL);
  
  /* Create the dataset. */
  dataset_id = H5Dcreate2(file_id, "/f", H5T_NATIVE_DOUBLE, dataspace_id, 
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  
  /* Write the data to the dataset. */
  H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, H5S_ALL, H5S_ALL, H5P_DEFAULT, 
                         dset_data);
  
  /* End access to the dataset and release resources used by it. */
  H5Dclose(dataset_id); // status = 
  
  /* Terminate access to the data space. */ 
  H5Sclose(dataspace_id); // status = 
  
  /* Close the file. */
  H5Fclose(file_id); // status = 
}	




/** Receover the lattice line-by-line and save it */
void save_mem(int (&accDt)[NT][NX], int time, double dt) {
  int x_int, vx_int;
  double x_phys, vx_phys;
  
  std::string st = std::to_string(time);
  std::string str = "output/snap" + st + ".h5";
  const char * filename = str.c_str();
  
  hid_t       file_id, dataset_id, dataspace_id;  /* identifiers */
  hsize_t     dims[2], start[2], count[2];
  
  /* Create a new file using default properties. */
  file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  
  /* Create the data space for the dataset. */
  dims[0] = NX; 
  dims[1] = NVX;  
  dataspace_id = H5Screate_simple(2, dims, NULL);
  
  /* Create the dataset. */
  dataset_id = H5Dcreate2(file_id, "/f", H5T_NATIVE_DOUBLE, dataspace_id, 
                         H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  
  /* Terminate access to the data space. */ 
  H5Sclose(dataspace_id);
  
  /* Create buffer */
  dims[0] = 1;
  dims[1] = NVX;
  hid_t mem_space = H5Screate_simple(2, dims, NULL);

  /* Write the data to the dataset. */
  // Append row-by-row. (Recover f)
  for ( int x = 0; x < NX; x++ ) {
    // recover row of f to buffer
    for ( int vx = 0; vx < NVX; vx++ ) {
      // trace back to initial condition
      x_int = x;
      vx_int = vx;
      for ( int tt = time - 1; tt > -1; tt-- ) {
        // undo drift
        vx_phys = -V + (1.0 * vx_int + 0.5) * DVX;
         x_int += (NX - (int) round( dt * vx_phys / DX ));
         x_int = x_int % NX;
        // undo kick
        vx_int -= accDt[tt][x_int];
      }
      x_phys  = -L + (1.0 * x_int + 0.5) * DX;
      vx_phys = -V + (1.0 * vx_int + 0.5) * DVX;
      
      buffer[vx] = get_init_f(x_phys,vx_phys);  // XXX finish Lattice.hpp class and use that instead to avoid repeating code
    }
    
    // append buffer to datafile
    
    // Select hyperslab on file dataset
    dataspace_id = H5Dget_space(dataset_id);
    start[0] = x;
    start[1] = 0;
    count[0] = 1;
    count[1] = NVX;
    H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, start, NULL, count, NULL);
    
    H5Dwrite(dataset_id, H5T_NATIVE_DOUBLE, mem_space, dataspace_id, H5P_DEFAULT, buffer);
    
    H5Sclose(dataspace_id);
  }
  
  
  H5Sclose(mem_space);
  
  /* End access to the dataset and release resources used by it. */
  H5Dclose(dataset_id);
  
  /* Close the file. */
  H5Fclose(file_id);
}
