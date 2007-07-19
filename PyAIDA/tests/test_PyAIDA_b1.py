#!/usr/bin/env python

from PyAida import *
import random

tree=tf.create()                                                  # Creating a memory-mapped tree
hf=af.createHistogramFactory( tree )                              # Creating a histogram factory mapped to the tree
h1=hf.createHistogram1D("histo","Example histogram.", 50, 0, 50)  # Creating an 1-dimensional binned histogram
for i in range(1000):                                             # Filling the histogram with random data
    h1.fill( random.uniform( 0, 50 ) )

print "Mean : %f" % h1.mean()                                     # Printing some statistical information
print "RMS : %f" % h1.rms()
xAxis = h1.axis();                                                # Printing the contents of the histogram
print "X value   entries   Y value (height)"
for iBin in range( xAxis.bins() ):
    print "%f      %d       %f" % ( h1.binMean( iBin ), h1.binEntries( iBin ), h1.binHeight( iBin ) )

print
print "Current annotation items/values : "                        # Listing the current annotation items for the histogram
annotation = h1.annotation()
for annotationIndex in range( annotation.size() ):
    key = annotation.key( annotationIndex )
    value = annotation.value( annotationIndex )
    print "%s : %s" % ( key, value )

del hf
del tree
