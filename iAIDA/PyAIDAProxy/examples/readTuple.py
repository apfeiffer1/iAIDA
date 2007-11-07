#!/usr/bin/env python
#
#example to read tuple from a file 
#

import sys

if (len( sys.argv ) < 2) : raise "give fileName"
    
fileName = sys.argv[1]
ftypes=fileName.split(".")
type=ftypes[len(ftypes)-1]
if (type =="aida") : type="XML"
if (type =="hbk") : type = "hbook"
if (type =="root") : type = "root"


print "Reading file",fileName
print "Store type is",type

# test reading a root tuple
from AidaProxy import *


# open the store . 3 meand open read only mode
s = Proxy_Store(fileName,type,3)

names = s.listObjectNames()
types = s.listObjectTypes()
print "object Names ",names
print "object types ",types

tuple = s.retrieveTuple("1")

print "Read tuple : entries = ",tuple.rows()

# create histogram, evaluator and filter
h1 = Histogram1D("h1","pt",100,0,20)
eval=Evaluator("sqrt( px*px + py*py)")
cut = Filter("pz > 0")


# two possibility loop on all events or project
loop = 0
if (len( sys.argv ) > 2) : loop = sys.argv[2]

if loop :
    print "looping..."
    eval.initialize(tuple)
    cut.initialize(tuple)
    tuple.start()

    while tuple.next():
        if cut.accept():
            h1.fill(eval.evaluateDouble())

else :
    print 'projecting....'
    tuple.project(h1,eval,cut)

print h1.entries()
print h1.mean()
print h1.rms()


# plot in root
from rootPlotter2 import *
pl = RootPlotter()
pl.plot(h1)
