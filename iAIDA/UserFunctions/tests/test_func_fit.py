#!/usr/bin/env python


from AidaProxy import *
# load my CPP function 

# define a python exponetial function
import math 
#fpy= lambda x: x**2 + x + 1
def myExp(x,p) :
    return p[0]*math.exp(-x /p[1])


# ccreate CPP function

f = UserFunction(myExp,1,2)

vx=std.vector("double")()
par=std.vector("double")()
par.push_back(10)
par.push_back(3)
f.setParameters(par)

# try the function

for i in range(0,10) :
    vx.assign(1,i)
    print " x = ",vx[0], " f(x) = ",f.value(vx)


# test performance comparing with C++ 
import time
t1=time.clock()
for i in range(0,100000) :
    r=f.value(vx)

t2=time.clock()
print "Time using Python", (t2-t1)

f2 = Function("E")
par2=std.vector("double")()
par2.push_back(math.log(10))
par2.push_back(1/3)
f2.setParameters(par2)

t1=time.clock()
for i in range(0,100000) :
    r=f2.value(vx)

t2=time.clock()
print "Time using C++ function ",(t2-t1)


# test fitting
print "Test now fitting an histogram with my Python function"

import random
h1=Histogram1D("h1",100,0,10)

for i in range(0,1000) :
    h1.fill(random.expovariate(1.0/3))

print "Filled histogram with ",h1.entries()
print "Histogram max is",h1.maxBinHeight()

fitter=Fitter()
print "Fitting...."
result=fitter.fit(h1,f)
if (result == None) : raise "Error fitting"

print "fit completed ",result.quality(),result.ndf()
print "fit parameters",result.fittedParameters()
print "    errors =  ",result.errors()

