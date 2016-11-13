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

source new-modules.sh
module purge
module load gcc/4.8.2-fasrc01
module load openmpi/1.10.1-fasrc01
module load gcc/4.8.2-fasrc01 openmpi/1.10.1-fasrc01 fftw/3.3.4-fasrc06
module load gsl/2.1-fasrc01
module load hdf5/1.8.16-fasrc01

mpirun -np $SLURM_NTASKS ./IntegerLattice > output/OUTPUT.$SLURM_JOB_ID

