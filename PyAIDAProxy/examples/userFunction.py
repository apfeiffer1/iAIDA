#!/usr/bin/env python


from AidaProxy import *

# define a python exponetial function
import math 


#define my  function :
# a log normal function
def myFunc(x,p) :
    z = math.log(x) - p[0] 
    return p[2]*math.exp(- ( z**2 )/ (2.0*p[1]**2 ) )


# create corrsponding AIDA function

f = UserFunction(myFunc,1,3)

## # set parameters 
## vx=std.vector("double")()
## par=std.vector("double")()
## par.push_back(0)
## par.push_back(2)
## par.push_back(10)
## f.setParameters(par)
f.setParams(0,2,10)

#test function
## vx.assign(1,1)
## print "f (1) = ",f.value(vx)
## vx.assign(1,10)
## print "f (10) = ",f.value(vx)

print "f (1) = ",f.eval(1)
print "f (10) = ",f.eval(10)


# plot the  function in range (0,10)

from rootPlotter2 import *
pl=RootPlotter()
pl.plot(f,"",0,10)


# test fitting
print "Test now fitting an histogram with my Python function"

import random
h1=Histogram1D("h1",100,0,10)

for i in range(0,1000) :
    h1.fill(random.lognormvariate(0.1,1) )

print "Filled histogram with ",h1.entries()
print "Histogram max is",h1.maxBinHeight()

fitter=Fitter("Chi2","migrad")
print "Fitting...."
result=fitter.fit(h1,f)
if not result : raise "Error fitting"

print "fit completed ",result.quality(),result.ndf()
print "fit parameters",result.fittedParameters()
print "    errors =  ",result.errors()

pl.plot(h1)
f.setParameters(result.fittedParameters())
pl.plot(f,"S")

