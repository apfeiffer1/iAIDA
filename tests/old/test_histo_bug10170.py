#!/usr/bin/env python
#
# test to check numer of underflow/overflow in multidimensional histograms
# problem reported in bug 10170
#
#
from AidaProxy import *

p=Proxy_Selector()
#p.setHistogramType("AIDA_Histogram_Native")
 
h1 = Histogram2D("h1","test Histo2D",10,0.,10.,10,0.,10.)

import random

ntot = 40000
for i in range(0,ntot) :
    x = random.uniform(-5,15)
    y = random.uniform(-5,15)
    h1.fill(x,y)


print "Entries ",h1.entries()
print "All Entries",h1.allEntries()
print "Extra Entries ",h1.extraEntries()

print " table of entries "


n00 = h1.binEntries(-2,-1)
n02 = h1.binEntries(-1,-1)
n20 = h1.binEntries(-2,-2)
n22 = h1.binEntries(-1,-2)
n01 = h1.binEntriesY(-1) - n00 - n02
n10 = h1.binEntriesX(-2) - n00 - n20
n11 = h1.entries()
n12 = h1.binEntriesX(-1) - n02 - n22
n21 = h1.binEntriesY(-2) - n20 - n22


print n00,n01,n02
print n10,n11,n12
print n20,n21,n22



print "\ncheck total entries"
tot = n00 + n01 + n02 + n10 + n11 + n12 + n20 + n21 + n22
print tot
if (tot != ntot ) :  
    raise "test failed - calculates total entries ",tot
if (h1.entries() + h1.extraEntries() != ntot ) :  
    raise "test failed - allEntries ",h1.allEntries(),h1.extraEntries()


print "2D histos OK "

#s=Proxy_Store("hist2D.root","root",1)
#s.write(h1)
#s.close()

#3D histo testing

print "\n\nTest now 3D histos ..."

h1 = Histogram3D("h3","test Histo3D",10,0.,10.,10,0.,10.,5,0.,10.)


ntot = 80000
for i in range(0,ntot) :
    x = random.uniform(-5,15)
    y = random.uniform(-5,15)
    z = random.uniform(-5,15)
    h1.fill(x,y,z)


print "Entries ",h1.entries()
print "All Entries",h1.allEntries()
print "Extra Entries ",h1.extraEntries()


if (h1.entries() + h1.extraEntries() != ntot ) :  
    raise "test failed - allEntries ",h1.allEntries(),h1.extraEntries()


print "3D histos OK "

