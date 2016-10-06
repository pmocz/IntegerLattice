EXEC        = IntegerLattice
CONFIG      = Config.sh
BUILD_DIR   = build
SRC_DIR     = src



CC          = mpicc
CPP         = mpicxx
OPTIMIZE    = -g -std=c++11 -fopenmp -funroll-loops -O3 -W -Wall -Wextra -Wfatal-errors -Wno-unknown-pragmas
LINKER      = mpicxx

#FFTW_LIB = -lfftw3
HDF5_INCL = -I/usr/include/hdf5/serial
HDF5_LIB  = -L/usr/lib/x86_64-linux-gnu/hdf5/serial -lhdf5
POISFFT_INCL = -I/home/pmocz/PoisFFT/src
POISFFT_LIB = -L/home/pmocz/PoisFFT/lib/gcc -lpoisfft -lm  -lfftw3 -lfftw3f -lfftw3_omp


############################################
# needed objects/headers

SUBDIRS = .
OBJS = main.o save.o poisson1D.o init_conditions.o
INCL = save.h poisson1D.h defines.h iterators.h init_conditions.h




############################################
# combine options

CFLAGS = $(OPTIMIZE) $(HDF5_INCL) $(POISFFT_INCL)
#$(GSL_INCL) $(FFTW_INCL) -I$(BUILD_DIR)

LIBS = $(HDF5_LIB) $(FFTW_LIB) $(POISFFT_LIB)
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

