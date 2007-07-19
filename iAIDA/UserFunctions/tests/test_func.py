#!/usr/bin/env python


from AidaProxy import *
# load my CPP function 

# define a python function

#fpy= lambda x: x**2 + x + 1
fpy= lambda x: x**3 + x**2 + x + 1


# ccreate CPP function

f = UserFunction(fpy,1,0)

vx=std.vector("double")()

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

f2 = Function("P3")
t1=time.clock()
for i in range(0,100000) :
    r=f2.value(vx)

t2=time.clock()
print "Time using C++ function ",(t2-t1)
