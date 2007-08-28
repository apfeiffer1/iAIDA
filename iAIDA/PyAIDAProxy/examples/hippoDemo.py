#!/usr/bin/env python

from AidaProxy import * 

import random

#
#
# create 1D and 2D cloud and plot them in hippoDraw
#
#

c1 = Cloud1D("MyCloud", "Log Normal Distribution")
c2 = Cloud2D("MyCloud2", "Gaussian Distribution")


# fill histograms with random gaussian points

for i in range(0,5000) :
   c1.fill(random.lognormvariate(0,1))
   xval = 1.*random.gauss(45., 10.)
   yval = 1.*random.gauss(60., 20.)
   c2.fill(xval,yval,1.)


print "Histogram filled with ",c1.entries()
print c1.title()," mean = ",c1.mean()," rms = ",c1.rms()

# use hippoplotter


from hippoPlotter import *

# create HippoPLotter (needs to be global to avoid it being destroyed at the
#  end of the script
global plHD
plHD=HippoPlotter() 

plHD.plot(c1,'Histogram',['x','Entries/bin']) # with labels

plHD.plot(c2,'Scatter Plot',['x','y'])

plHD.plot(c2,'Color Plot',['x','y'])



#  input Y data points  
x = std.vector("double")()
ex = std.vector("double")()
y = std.vector("double")()
ey = std.vector("double")()
nPoints = 10
for  i in range(0,nPoints) :
    x.push_back(i)
    y.push_back( random.uniform(0,10))
    ey.push_back( 1.)
    ex.push_back(0.5)

# fill now the data point set


# create an X-Y dps from a X, Y and error vectors

dps2=DataPointSet("my_dps2","Example of x-y data set",x,y,ex,ey)


plHD.plot(dps2,'XY Plot',['x','y','ex','ey'])
#wait()
#plHD.plot(dps2,'Y Plot',['y','ey'])
#wait()
#plHD.plot(dps2,'Strip Chart')
#wait()


# plot histograms from clouds 
h1 = Histogram1D("h1", "Log Normal Distribution (Binned Histogram)", 100,0, 10.)
h2 = Histogram2D("h2", "Gaussian Distribution (Binned Histogram)", 50, 0, 100., 50, 0, 150)
c1.fillHistogram(h1)
c2.fillHistogram(h2)

plHD.plot(h1)
plHD.plot(h2)
