#include <stdio.h>
#include <string>

#include "hdf5.h"
#include "defines.h"


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
