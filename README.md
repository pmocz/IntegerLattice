# Integer Lattice

Philip Mocz (2016)

Harvard University

(code in development)


# Libraries

Needs the PoisFFT package

https://github.com/LadaF/PoisFFT

You can install the library locally and add it to your path.
E.g. Add the following to your .bashrc file:
```
export LD_LIBRARY_PATH=/home/pmocz/PoisFFT/lib/gcc:$LD_LIBRARY_PATH
```
To install this library, in your directory for libraries, you can do 
```
git clone https://github.com/LadaF/PoisFFT.git
cd PoisFFT/
cd src/
scons
```

Also needs hdf5, fftw3

You will have to modify the Makefile to point to your library installations

# How to run

recompile and run:
```
make clean; make; ./IntegerLattice 
```

to plot snapshots (e.g. snaps 0, 10, 20, 30), 
in the analysis/ folder, run the python script:
```
python plot.py 0 10 20 30
```




# Useful references

example FFTW 
https://github.com/undees/fftw-example/blob/master/fftw_example.c

3d array 
http://www.cplusplus.com/forum/articles/7459/
