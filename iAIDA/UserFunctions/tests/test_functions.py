#!/usr/bin/env python


from AidaProxy import *
# load my CPP function 

# define various python function

fp1 = lambda x: x**2 - 1
fp2 = lambda x: x[0] * x[1] + 1 
fp3 = lambda x: x[0] + x[1] + x[2]

# ccreate CPP function with zero params

f1 = UserFunction(fp1,1,0)
f2 = UserFunction(fp2,2,0)
f3 = UserFunction(fp3,3,0)

vx=std.vector("double")()

for i in range(0,5) :
    vx.assign(1,i)
    print " x = ",vx[0], " f1(x) = ",f1.value(vx)

for i in range(0,5) :
    vx.assign(2,i)
    print " x = ",vx[0]," y = ",vx[1], " f2(x,y) = ",f2.value(vx)

for i in range(0,5) :
    vx.assign(3,i)
    print " x = ",vx[0]," y = ",vx[1], " 2 = ",vx[2], " f2(x,y,z) = ",f3.value(vx)

# test functions with params
print "\nTest Functions with parameters \n"


fp11 = lambda x,p: x**2 + p[0]
fp12 = lambda x,p: p[0]*x**2 + p[1]
fp21 = lambda x,p: x[0] * x[1] + p[0]
fp33 = lambda x,p: p[0]*x[0] + p[1]*x[1] + p[2]*x[2]



f11 = UserFunction(fp11,1,1)
f12 = UserFunction(fp12,1,2)
f21 = UserFunction(fp21,2,1)
f33 = UserFunction(fp33,3,3)

vx=std.vector("double")()
par=std.vector("double")()

vx.assign(1,2)
par.assign(1,5)
f11.setParameters(par)
print " x = ",vx, " par = ",f11.parameters()," f( ) = ",f11.value(vx)

vx.assign(1,3)
par.assign(2,2)
f12.setParameters(par)
print " x = ",vx, " par = ",f12.parameters()," f( ) = ",f12.value(vx)

vx.assign(2,2)
par.assign(1,5)
f21.setParameters(par)
print " x = ",vx, " par = ",f21.parameters()," f( ) = ",f21.value(vx)

vx.assign(3,2)
par.assign(3,3)
f33.setParameters(par)
print " x = ",vx, " par = ",f33.parameters()," f( ) = ",f33.value(vx)



