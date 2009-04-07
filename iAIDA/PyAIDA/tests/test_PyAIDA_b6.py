#!/usr/bin/env python

from PyAida import *
import random

tree=tf.create()                                    # Creating a tree in memory
hf = af.createHistogramFactory(tree)                # Creating a histogram factory
ff = af.createFunctionFactory(tree)                 # Creating a function factory
fitf = af.createFitFactory()                        # Creating a fit factory

# Creating a histogram
h1 = hf.createHistogram1D("MyHisto", "Gaussian Distribution", 100, 0., 100.)

# fill histogram with random gaussian points
for i in range(0,10000) :
   xval = 1.*random.gauss(45., 10.)
   h1.fill(xval,1.)


# print some Histogram information
print "Histogram : "
print "  Title : %s" % h1.title()
print "  Entries : %d" % h1.entries()
print "  Mean : %f" % h1.mean()
print "  Rms : %f" % h1.rms()

# fit the histogram:  create first function and fitter
#                     need to create a function to set initial parameters
gaussFun = ff.createFunctionByName("Gaussian","G")

# set function initial parameters
gaussFun.setParameter("mean" , 50.)
gaussFun.setParameter("sigma", 10.)
gaussFun.setParameter("amp"  , 100.)

# create fitter (default is Chi2)
fitter = fitf.createFitter()

# fit
fitResult = fitter.fit( h1, gaussFun )

if (fitResult == None) :
   raise "Error : Fit failed" 

#
#alternative way to set initial fit parameters and fit
#
#p=[50,10,10]
#fitResult = fitter.fit(h1,"G",p)
            
# retrieve fit chi2
print "Fit result : chi2 / ndf : %f / %d " % (fitResult.quality(), fitResult.ndf())

#retrieve fitted parameters and errors and print them
parNames = fitResult.fittedParameterNames()
par = fitResult.fittedParameters()
epar = fitResult.errors()

for i in range(0,len(par)):
   print "%s = %f +/- %f" % ( parNames[i], par[i], epar[i] )

#add in the annotation of the histogram the fit result
annotation=h1.annotation()
for i in range(0,len(par)):
   annotation.addItem( parNames[i], "%f +/- %f" % ( par[i], epar[i] ) )

#Remove some existing annotation items
annotation.removeItem( "Overflow" )
annotation.removeItem( "Underflow" )
annotation.removeItem( "Extra Entries" )

#plot function and then histogram
#using root plotter 
from rootPlotter2 import *
pl = RootPlotter()


#pl.currentRegion().clear()
pl.plot(h1)
pl.plot(gaussFun,"S",0,100)
#pl.refresh()



del fitf
del ff
del hf
del tree
