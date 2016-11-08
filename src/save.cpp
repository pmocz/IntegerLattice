#include <stdio.h>
#include <string>
#include <mpi.h>

#include "defines.h"
#include "hdf5.h"
#include "Lattice.hpp"

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






/** Recover the lattice line-by-line and save it */
void save_mem(Lattice & lattice, int time) {
 
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
    for ( int vx = 0; vx < NVX; vx++ )
      buffer[vx] = lattice.get_f(x, vx, time);
    
    // append buffer to datafile:
    
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
  
  /* Close buffer space */
  H5Sclose(mem_space);
  
  /* End access to the dataset and release resources used by it. */
  H5Dclose(dataset_id);
  
  /* Close the file. */
  H5Fclose(file_id);
}




/** Recover the lattice line-by-line and save it */
void save_mem_parallel(Lattice& lattice, int time, int mpi_rank, int n_elem_per_proc) {
 
  std::string st = std::to_string(time);
  std::string stmpi = std::to_string(mpi_rank);
  std::string str = "output/snap" + st + "." + stmpi + ".h5";
  const char * filename = str.c_str();
  
  hid_t       file_id, dataset_id, dataspace_id;  /* identifiers */
  hsize_t     dims[2], start[2], count[2];

  /* Set up file access property list with parallel I/O access */
  //plist_id = H5Pcreate(H5P_FILE_ACCESS);
  //H5Pset_fapl_mpio(plist_id, MPI_COMM_WORLD, MPI_INFO_NULL);
  
  /* Create a new file using default properties. */
  file_id = H5Fcreate(filename, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
  
  /* Create the data space for the dataset. */
  dims[0] = n_elem_per_proc; 
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
  for ( int x = 0; x < n_elem_per_proc; x++ ) {
    // recover row of f to buffer
    for ( int vx = 0; vx < NVX; vx++ )
      buffer[vx] = lattice.get_f(x + mpi_rank * n_elem_per_proc, vx, time);
    
    // append buffer to datafile:
    
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
  
  /* Close buffer space */
  H5Sclose(mem_space);
  
  /* End access to the dataset and release resources used by it. */
  H5Dclose(dataset_id);
  
  /* Close the property list*/
  //H5Pclose(plist_id);
  
  /* Close the file. */
  H5Fclose(file_id);
}
