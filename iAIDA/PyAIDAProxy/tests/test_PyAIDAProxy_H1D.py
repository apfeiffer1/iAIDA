#!/usr/bin/env python
#
# load Python binding to AIDA_Proxy
from AidaProxy import *

import random

h1=Histogram1D("myHist","Test Histogram1D",100,0,10)

# Filling the histogram with random data
for i in range(1000):                                            
    h1.fill( random.gauss( 5, 2 ) )

# Printing the contents of the histogram
xAxis = h1.axis();
print "X value   entries   Y value (height)"
for iBin in range( xAxis.bins() ):
    print "%d    %f      %d       %f" % ( iBin, h1.binMean( iBin ), h1.binEntries( iBin ), h1.binHeight( iBin ) )
# Printing some statistical information
print "Mean : %f" % h1.mean()                                    
print "RMS : %f" % h1.rms()


# save histo in an XML file 
store = Proxy_Store("testH1D.aida","XML",1)
store.write(h1)
store.close()

# read back the histogram

storeIn = Proxy_Store("testH1D.aida","XML",3)
names = storeIn.listObjectNames()
types = storeIn.listObjectTypes()
print "Objects in Store : "
for i in range(0,len(names)) :
    print "  ",types[i]," : " ,names[i]

hIn = storeIn.retrieveH1D("myHist")
print "Retrieved : ",hIn.name()
print "Mean : %f" % hIn.mean()                                    
print "RMS : %f" % hIn.rms()

