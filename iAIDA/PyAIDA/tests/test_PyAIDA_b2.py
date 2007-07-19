#!/usr/bin/env python

from PyAida import *
import random


tree=tf.create()                                                 # Creating a memory-mapped tree
hf=af.createHistogramFactory( tree )                             # Creating a histogram factory mapped to the tree
h1=hf.createHistogram1D("histo","Example histogram.",50, 0, 50)  # Creating an 1-dimensional binned histogram
for i in range(1000):                                            # Filling the histogram with random data
    h1.fill( random.uniform( 0, 50 ) )

# plot the histogram using the rootplotter
from rootPlotter2 import *
pl = RootPlotter()
pl.plot( h1)                                                    # Plotting the histogram

del hf
del tree
