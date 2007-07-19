#!/usr/bin/env python
#
#  Fitting example  1
#
#   Chi2 Fit to a 2 Gaussian Distribution 
#
#  operation:
#
#   - create and fill a 1D histogram with a Gaussian distribution + background
#   - perform a chi2 gaussian fit to the central part of the histogram
#   - plot the histogram and the fitted function
#   - save result on XML store
#

from AidaProxy import * 

import random

# create histograms

c1 = Cloud1D("MyHist", "Signal + Background Distribution")


# fill histogram with two random gaussian points (70%) 

# peak (30%) 
for i in range(0,3000) :
   xval = 1.*random.gauss(0., 5.)
   c1.fill(xval,1.)

# backgorund  (70%)     
for i in range(0,7000) :
   xval = random.gauss( 0, 50 ) 
   c1.fill(xval,1.)


# print some Histogram information

print "Histogram " + c1.title()," is filled with ",c1.entries(),"entries"
print " Mean is ",c1.mean()
print " RMS  is ",c1.rms()


# convert cloud to histogram
h1 = Histogram1D("MyHist", "Signal + Background Distribution", 200, -100, 100.)
c1.fillHistogram(h1)

# fit the histogram with a gaussian function in central region
#            
# create the fitter
#


fitter = Fitter("Chi2","lcg_minuit")

# fit : set initial gaussian parameters

p=std.vector("double")()
p.push_back(0)
p.push_back(10)
p.push_back(100)


print "\n******************************************************"
print "\nPerform Chi2 Single Gaussian fit\n"
print "*******************************************************\n"

func=Function("G")
func.setParameters(p)


fitResult = fitter.fit(h1,func)
if (fitResult == None) :
   raise "Single Gaussian Chi2 fit failed"

# retrieve fit chi2
print " Fit completed - chi2 / ndf is ",fitResult.quality()," / ",fitResult.ndf() 


#retrieve fitted parameters and errors and print them

parNames = fitResult.fittedParameterNames()
par = fitResult.fittedParameters()
epar = fitResult.errors()

for i in range(0,len(par)):
   print parNames[i] + "    =   " + str(par[i]) + "   +/-  " + str(epar[i])



print "\n************************************************************"
print "\nPerform Chi2 Double Gaussian fit fixing the 2 mean values \n"
print "*************************************************************\n"


#  fit now with two gaussians (first fix means) 

func2 = Function("G+G")
p=std.vector("double")()
p.push_back(0)
p.push_back(10)
p.push_back(100)
p.push_back(0)
p.push_back(10)
p.push_back(100)

func2.setParameters(p)
# fix meand of functions 
p1 = fitter.fitParameterSettings(func2.parameterNames()[0]);
p2 = fitter.fitParameterSettings(func2.parameterNames()[3]);
p1.setFixed(1)
p2.setFixed(1)


fitResult = fitter.fit(h1,func2)
if (fitResult == None) :
   raise "Double Gaussian chi2 fit with fixed parameters failed"

# retrieve fit chi2
print " Fit completed - chi2 / ndf is ",fitResult.quality()," / ",fitResult.ndf() 


parNames = fitResult.fittedParameterNames()
par = fitResult.fittedParameters()
epar = fitResult.errors()

for i in range(0,len(par)):
   print parNames[i] + "    =   " + str(par[i]) + "   +/-  " + str(epar[i])



# now release parameters and refit

print "\n************************************************************"
print "\nPerform Chi2 Double Gaussian with all 6 parameters \n"
print "*************************************************************\n"

p1.reset()
p2.reset()

fitResult = fitter.fit(h1,func2)
if (fitResult == None) :
   raise "Double Gaussian chi2 fit failed"

# retrieve fit chi2
print " Fit completed - chi2 / ndf is ",fitResult.quality()," / ",fitResult.ndf() 


parNames = fitResult.fittedParameterNames()
par = fitResult.fittedParameters()
epar = fitResult.errors()

for i in range(0,len(par)):
   print parNames[i] + "    =   " + str(par[i]) + "   +/-  " + str(epar[i])



# perform now a binned likelihood fit

print "\n************************************************************"
print "\nPerform PoissonML  Double Gaussian fit \n"
print "*************************************************************\n"

fitter.setFitMethod("PoissonML")
func3 = Function(func2)
#func3 = func2

# use fitted function to keep previous parameter 
fitResult = fitter.fit(h1,func3)
if (fitResult == None) :
   raise "Binned Likelihood fit failed"

parNames = fitResult.fittedParameterNames()
par = fitResult.fittedParameters()

epar = fitResult.errors()
for i in range(0,len(par)):
   print parNames[i] + "    =   " + str(par[i]) + "   +/-  " + str(epar[i])


#plot with hippoDraw
#from hippoPlotter import *
#pl = HippoPlotter()
#pl.plot(c1)

print "Plotting in a ROOT canvas....." 
from rootPlotter2 import *
pl = RootPlotter()
pl.plot(h1)
# plot single gauss function
pl.plot(func,"S")
#plot double gauss from chi2 fit
pl.plot(func2,"S")
#plot double gauss from ML fit
pl.plot(func3,"S")

# plot with hippoPlotter

print "Plotting in a HippoDraw Canvas...." 
from hippoPlotter import *
pl2=HippoPlotter()
pl2.plot(h1)

pl2.plot(func,"S")
pl2.plot(func2,"S")
pl2.plot(func3,"S")
