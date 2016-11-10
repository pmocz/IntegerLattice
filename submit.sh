#!/bin/sh
#SBATCH -p hernquist
#SBATCH -J dCTRef 
#SBATCH -n 64
#SBATCH --ntasks-per-node=64
#SBATCH -o output/OUTPUT.lsf
#SBATCH -e output/ERROR.lsf
#SBATCH --exclusive
#SBATCH --mail-user=pmocz@cfa.harvard.edu
#SBATCH --mail-type=BEGIN
#SBATCH --mail-type=END
#SBATCH --mail-type=FAIL
#SBATCH --mem-per-cpu=2400
#SBATCH -t 0-02:00           # Runtime in D-HH:MM

module purge

module load hpc/intel-ipp-7.1.0.079
module load hpc/intel-mkl-11.0.0.079
module load hpc/intel-mpi-4.1.0.024
module load hpc/gmp-5.1.1
module load hpc/hdf5-1.8.9_intel-13.0.079
module load hpc/fftw-2.1.5_impi-4.1.0.024_intel-13.0.079
module load centos6/hwloc-1.7.2 
module load hpc/fftw-3.3.2_openmpi-1.6.2_intel-13.0.079

mpirun -np $SLURM_NTASKS ./IntegerLattice > output/OUTPUT.$SLURM_JOB_ID

