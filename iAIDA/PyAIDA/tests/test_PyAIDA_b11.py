#!/usr/bin/env python

from PyAida import * 
import random


tree=tf.create("dps.aida", "xml", 0,1)                           # Creating a tree mapped to a new XML file
df=af.createDataPointSetFactory( tree )                          # Creating a data point set factory mapped to the tree
dpsa=df.create("my_dps","Example of x-y data set",2)             # Creating an empty 2-dimensional data point set

#  input data 
nPoints = 10
x = []
y = []
ex = []
ey = []
for  i in range(0,nPoints) :
    x.append(i)
    y.append(random.uniform(0,10))
    ex.append(0.5)
    ey.append(1)

# fill now the data point set

for i in range(0,nPoints) :
    dpsa.addPoint()                                              # add an empty point to the set
    point = dpsa.point(i)                                            # retrieve the added point
    mX = point.coordinate(0)                                     # retrieve first dimension measurement (X)
    mX.setValue(x[i])                                            # set x values 
    mX.setErrorPlus( ex[i] )                                     # set x errors (not necessary if errors = 0)  
    mX.setErrorMinus( ex[i] ) 

    mY = point.coordinate(1)                                     # retrieve second dimension measurement (Y)
    mY.setValue(y[i])                                            # set y values 
    mY.setErrorPlus( ey[i] )                                     # set y errors (not necessary if errors = 0)  
    mY.setErrorMinus( ey[i] ) 


print  "DataPointSet :       ", dpsa.title() 
print "Number of points :   " , dpsa.size()
print "Dimension :          " , dpsa.dimension()    


tree.commit()                                                    # Flushing the histograms into the file
tree.close()                                                     # Explicitly closing the tree

del df
del tree
