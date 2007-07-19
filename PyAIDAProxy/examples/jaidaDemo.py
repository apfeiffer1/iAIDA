#!/usr/bin/env python

from AidaProxy import *

Plotter = pi_aida.Plotter

print "Test plotting in JAIDA"

import random, math
h1=Histogram1D("h1","Example histogram 1D",100,0,10)
h2=Histogram2D("h2","Example histogram 2D",100,0,10,100,0,10)
c1=Cloud1D("c1","Example cloud 1D")
c2=Cloud2D("c2","Example cloud 2D")
p1=Profile1D("p1","Example profile",50,0,2)
p2=Profile2D("p2","Example profile 2D",50,0,2,50,0,10)

for i in range(0,10000) :
    h1.fill(random.gauss(5,2))
    h2.fill(random.gauss(5,2),random.gauss(4,1))
    c1.fill(random.gauss(5,2))
    c2.fill(random.gauss(5,2),random.gauss(4,1))
    p1.fill(random.expovariate(1), random.uniform(5,15))
    p2.fill(random.expovariate(1), random.gauss(5,3), random.uniform(5,15))

pl=Plotter()
pl.createRegions(3,3)
print "plotting... ",h1
pl.region(0).plot(h1)
pl.show()
print "plotting.. ",h2
pl.region(1).plot(h2)
pl.show()
print "plotting.. ",c1
pl.region(2).plot(c1)
pl.show()
print "plotting.. ",c2
pl.region(3).plot(c2)
pl.show()
print "plotting.. ",p1
pl.region(4).plot(p1)
pl.show()
print "plotting.. ",p2
pl.region(5).plot(p2)
pl.show()

print " try plotting now histogram and fitted function"

fitter=Fitter()
f=Function("G")
p=std.vector("double")()
p.push_back(h1.mean())
p.push_back(h1.rms())
p.push_back(h1.maxBinHeight())
f.setParameters(p)

r=fitter.fit(h1,f)
if (r == None) : raise "Error fitting histogram"

ff=r.fittedFunction()

#cannot plot same  histo again (need copying) 
#pl.region(6).plot(h1)
h1_copy = Histogram1D(h1)
print "plotting ",h1_copy
pl.region(6).plot(h1_copy)
pl.region(6).plot(ff)
pl.show()

print "Plotting DPS"

x=std.vector("double")()
y=std.vector("double")()
ex = std.vector("double")()
ey =std.vector("double")()

for i in range(0,p1.axis().bins()) :
    x.push_back(p1.binMean(i))
    ex.push_back(p1.axis().binWidth(i))
    y.push_back(p1.binHeight(i))
    ey.push_back(p1.binError(i))
    
dps=DataPointSet("dps","Example XY DPS",x, y, ex, ey)

print "plotting DPS",dps
pl.region(7).plot(dps)
pl.show()

x=std.vector("double")()
ex1=std.vector("double")()
ex2=std.vector("double")()
for i in range(0,10) :
    x.push_back(random.uniform(5,15))
    ex1.push_back(math.sqrt(x.back() ) )
    ex2.push_back(2*ex1.back())

# Error along x are not plotted...
#dps2=DataPointSet(0,"dps","Example Y DPS",x, ex1,ex2)    
dps2=DataPointSet(1,"dps","Example Y DPS",x, ex1,ex2)    
print "plotting DPS X..",dps2
pl.region(8).plot(dps2)
pl.show()
                 
