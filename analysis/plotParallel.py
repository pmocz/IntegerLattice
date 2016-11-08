#!/usr/bin/python

import os.path
import sys
import numpy as np
import matplotlib.pyplot as plt
import h5py

"""
Plot the output snapshots
usage example: python plotParallel.py 0 10 20 30
"""
def main(argv):
  
  for snap in argv:
    hfile = h5py.File('../output/snap' + snap + '.0.h5','r')
    f = hfile["/f"][:]
    hfile.close()
    p = 1
    while(os.path.exists('../output/snap' + snap + '.' + str(p) + '.h5')):
      hfile = h5py.File('../output/snap' + snap + '.' + str(p) + '.h5','r')
      f = np.vstack((f, hfile["/f"][:]))
      hfile.close()
      p += 1
    
    fig = plt.imshow(f.T, cmap="spectral", interpolation="none")
    plt.gca().invert_yaxis()
    plt.axis('off')
    fig.axes.get_xaxis().set_visible(False)
    fig.axes.get_yaxis().set_visible(False)
    
   
    plt.savefig('snap' + snap + '.pdf', aspect = 'normal', bbox_inches='tight', pad_inches = 0)
    plt.close()


if __name__ == "__main__":
  main(sys.argv[1:])
