

export SYSTYPE="odyssey-gcc"
source new-modules.sh
module purge
module load gcc/4.8.2-fasrc01
module load openmpi/1.10.1-fasrc01
module load gcc/4.8.2-fasrc01 openmpi/1.10.1-fasrc01 fftw/3.3.4-fasrc06
module load gsl/2.1-fasrc01
module load hdf5/1.8.16-fasrc01



make clean
make
