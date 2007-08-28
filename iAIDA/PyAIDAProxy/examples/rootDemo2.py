#!/usr/bin/env python

from AidaProxy import * 

import random, math


#
#  create 1D and 2D histograms and plot them using PyLCGDict
#
#

h1 = Histogram1D("h1", "Gaussian Distribution",100,-10,10.)
h2 = Histogram1D("h2", "Exponential Distribution",100,0.,10.)
h3 = Histogram1D("h3", "Log Normal Distribution",100,0.,10.)
h4 = Histogram1D("h4", "Gamma Distribution",100,0,10.)
h5 = Histogram2D("h5", "2D Gaussian Distribution",50,-3,3.,50,-3,3.)
h6 = Histogram2D("h6", " r vs x",50,-3,3.,50,0,5)
for i in range(0,5000) :
   r = h1.fill( random.gauss(0, 3. ) )
   r = h2.fill( random.expovariate(1.) )
   r = h3.fill( random.lognormvariate(0,1) )
   r = h4.fill( random.gammavariate(2,1) )
   x = random.gauss(0,1)
   y = random.gauss(0,1)
   z = math.sqrt(x*x + y*y) 
   r = h5.fill(x,y)
   r = h6.fill(x,z)


print "histogram filled with ",h1.entries(),"entries"
print "plotting now in a root canvas......"

from rootPlotter2 import *
rp = RootPlotter("AIDA to Root example",200,10,700,800)
# set two canvas zones and options
rp.canvas.Divide(2,3)
rp.canvas.cd(1)
rp.plot(h1)
rp.canvas.cd(2)
rp.plot(h2)
rp.canvas.cd(3)
rp.plot(h3)
rp.canvas.cd(4)
rp.plot(h4)
rp.canvas.cd(5)
rp.plot(h5)
rp.canvas.cd(6)
rp.plot(h6)
