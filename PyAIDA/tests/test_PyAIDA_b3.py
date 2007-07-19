#!/usr/bin/env python

from PyAida import *
import random

tree=tf.create()                                                 # Creating a memory-mapped tree
hf=af.createHistogramFactory( tree )                             # Creating a histogram factory mapped to the tree

print "Creating an 1-dimensional histogram"
h1=hf.createHistogram1D("histo","Example histogram.",50, 0, 50 )
print "Histogram title : %s" % h1.title()                        # Printing the histogram title
print "Tree contents after the creation of the 1st histogram."   # Printing the contents of the tree
#tree.ls()
print tree.pwd()
print tree.listObjectNames()
print tree.listObjectTypes()
tree.mkdir( "Dir_1" )                                            # Creating a new directory.
print "Tree contents :"
#tree.ls()
print tree.pwd()
print tree.listObjectNames()
print tree.listObjectTypes()
tree.cd( "Dir_1" );                                              # Entering the new directory and creating a 2-D histogram

print "Creating a 2-dimensional histogram"
h2=hf.createHistogram2D( "histo_2", "Example 2D histogram.", 50, 0, 50, 20, 0, 20 )
print "Histogram title : %s" % h2.title()                        # Printing the histogram title
for i in range( 1000 ):                                          # Filling the 2-D histogram with random data
    h2.fill( random.uniform( 0, 50 ), random.uniform( 0, 20 ) )

print "Bins in X : %i" % h2.xAxis().bins()                       # Retrieving statistical information for the 2-D histogram
print "Mean X : %f" % h2.meanX()
print "RMS X  : %f" % h2.rmsX()
print "Bins in Y : %i" % h2.yAxis().bins()
print "Mean Y : %f" % h2.meanY()
print "RMS Y  : %f" % h2.rmsY()

print "Creating a 3-dimensional histogram"
h3=hf.createHistogram3D( "histo_3", "Example 3D histogram.", 50, 0, 50, 20, 0, 20, 10, 0, 10 )
print "Histogram title : %s" % h3.title()
for i in range( 1000 ):                                          # Filling the 3-D histogram with random data
    h3.fill( random.uniform( 0, 50 ), random.uniform( 0, 20 ), random.uniform( 0, 10 ) )

print "Bins in X : %i" % h3.xAxis().bins()                       # Retrieving statistical information for the 3-D histogram
print "Mean X : %f" % h3.meanX()
print "RMS X  : %f" % h3.rmsX()
print "Bins in Y : %i" % h3.yAxis().bins()
print "Mean Y : %f" % h3.meanY()
print "RMS Y  : %f" % h3.rmsY()
print "Bins in Z : %i" % h3.zAxis().bins()
print "Mean Z : %f" % h3.meanZ()
print "RMS Z  : %f" % h3.rmsZ()
print "Tree contents in the directory %s" % tree.pwd()          # Printing the contents of the current working
#tree.ls()                                                       # directory in the tree
print tree.listObjectNames()
print tree.listObjectTypes()

print "Creating another directory"
tree.mkdir("/Dir_2")
tree.cd("/Dir_2")

print "Copying a histogram into the new directory"
tree.cp("../histo_1", "." )

print "Moving a histogram into the new directory"
tree.mv( "/Dir_1/histo_2", "." )

tree.cd( ".." )
print "Creating another directory"
tree.mkdir( "Dir_3" )
tree.cd( "Dir_3" )

print "Creating other histogram types"
hf.createCloud1D( "cloud_1", "Example 1-D cloud" )
hf.createCloud2D( "cloud_2", "Example 2-D cloud" )
hf.createCloud3D( "cloud_3", "Example 3-D cloud" )

hf.createProfile1D( "profile_1", "Example 1-D profile", 100, 0, 50 )
hf.createProfile2D( "profile_2", "Example 2-D profile", 100, 0, 50, 100, 0, 50 )

#print "Fetching a profile histogram from the tree"
#profile = tree.findP1D( "profile_1" )
#print "Profile histogram title : %s" % profile.title()

print "Final contents of the tree"

print tree.listObjectNames("/",1)
print tree.listObjectTypes("/",1)
#tree.ls( "/", 1 )

del hf
del tree
