#!/usr/bin/env python

from AidaProxy import *

# define a python function

#fpy= lambda x: x**2 + x + 1
fpy= lambda x: x**3 + x**2 + x + 1

#def fpy(x) :
#    y= x**3 + x**2 + x + 1
#    print x,y
#    return y

# ccreate CPP function

f = UserFunction(fpy,1,0)

vx=std.vector("double")()

for i in range(0,10) :
    vx.assign(1,i)
    print " x = ",vx[0], " f(x) = ",f.value(vx)

f2 = Function("P3")


import pylcgdict
#use SealRootDict from pylcgdict
pylcgdict.loadDictionary('SealROOTDict')
g=pylcgdict.Namespace("")
TCanvas  = g.TCanvas
TH1D     = g.TH1D
TH2D     = g.TH2D
TProfile     = g.TProfile
TF1     = g.TF1

# create root application (needs to be global) 
#gApplication = g.ROOTApplication('ROOTApp')
# root globals (not sure if needed) 
#TGlobals = g.TGlobals
#gSystem = TGlobals.System()
#gROOT = TGlobals.ROOT();

# function converter
try : 
  pylcgdict.loadDictionary("AIDA_RootConverterDict")
  FunctionConverter = g.pi.AIDA_RootConverter.FunctionConverter
  theFunctionConverter = FunctionConverter()
except :
  print "Warning : cannot load AIDA_RootConverter dictionary " 
  pass


rootFunc2 = theFunctionConverter.toRoot(f2,"flmFunc",0,10)
for i in range(0,10):
    x=float(i)
    y2=rootFunc2.Eval(x)
    print "x =",x,"f2=",y2

rootFunc1 = theFunctionConverter.toRoot(f,"pyFunc",0,10)
for i in range(0,10):
    x=float(i)
    y1=rootFunc1.Eval(x)
    print "x =",x,"f1=",y1

