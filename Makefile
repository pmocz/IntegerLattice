EXEC        = IntegerLattice
CONFIG      = Config.sh
BUILD_DIR   = build
SRC_DIR     = src



CC          = mpicc
CPP         = mpicxx
OPTIMIZE    = -g -std=c++11 -fopenmp -funroll-loops -O3 -W -Wall -Wextra -Wfatal-errors -Wno-unknown-pragmas
LINKER      = mpicxx

#FFTW_LIB = -lfftw3
ifeq ($(SYSTYPE),"odyssey-gcc")
  HDF5_INCL = -I/usr/include/hdf5/openmpi
  HDF5_LIB  = -L/usr/lib/x86_64-linux-gnu/hdf5/openmpi -lhdf5
  POISFFT_INCL = -I/home/pmocz/PoisFFT/src
  POISFFT_LIB = -L/home/pmocz/PoisFFT/lib/gcc -lpoisfft -lm  -lfftw3 -lfftw3f -lfftw3_omp
else
  HDF5_INCL = 
  HDF5_LIB  = -lhdf5
  POISFFT_INCL = -I/n/home02/pmocz/C/PoisFFT/src
  POISFFT_LIB = -L/n/home02/pmocz/C/PoisFFT/lib/gcc -lpoisfft -lm  -lfftw3 -lfftw3f -lfftw3_omp
endif

############################################
# needed objects/headers

SUBDIRS = .
OBJS = main.o save.o poisson1D.o init_conditions.o method_orig.o method_memsave.o
INCL = save.h poisson1D.h defines.h iterators.h init_conditions.h method.h




############################################
# combine options

CFLAGS = $(OPTIMIZE) $(HDF5_INCL) $(POISFFT_INCL)
#$(GSL_INCL) $(FFTW_INCL) -I$(BUILD_DIR)

LIBS = $(HDF5_LIB) $(POISFFT_LIB)
# $(GSL_LIBS)



SUBDIRS := $(addprefix $(BUILD_DIR)/,$(SUBDIRS))
OBJS := $(addprefix $(BUILD_DIR)/,$(OBJS))
INCL := $(addprefix $(SRC_DIR)/,$(INCL))



############################################
# create subdirs

RESULT := $(shell mkdir -p $(SUBDIRS)  )



############################################
# build rules

all: $(EXEC)

$(EXEC): $(OBJS)
	$(LINKER) $(OPTIMIZE) $(OBJS) $(LIBS) -o $(EXEC)

clean:
	rm -f $(OBJS) $(EXEC)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCL) $(MAKEFILES)
	$(CPP) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c $(INCL) $(MAKEFILES)
	$(CC) $(CFLAGS) -c $< -o $@

