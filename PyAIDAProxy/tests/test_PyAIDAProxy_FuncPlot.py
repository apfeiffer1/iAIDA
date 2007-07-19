#!/usr/bin/env python

from AidaProxy import *
# load my CPP function 

# define a python function

#fpy= lambda x: x**2 + x + 1
import math

fpy1= lambda x: math.sin(x)+math.cos(x)
fpy2= lambda x: math.sin(x)-math.cos(x)

f1 = UserFunction(fpy1,1,0)
f2 = UserFunction(fpy2,1,0)

from rootPlotter2 import * 
pl=RootPlotter()
#pl.canvas.Divide(1,2)
#pl.canvas.cd(1)
pl.plot(f1,"",0,10)
pl.plot(f2,"S",0,10)
#pl.canvas.cd(2)
#pl.plot(f2,"",0,10)

# hippodraw plotter

from hippoPlotter import * 
pl2 = HippoPlotter()

pl2.plot(f1,"",0,10)
pl2.plot(f2,"S",[0,10])

