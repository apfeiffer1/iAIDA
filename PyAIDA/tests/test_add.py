#!/usr/bin/env python
#
# test histogram additions
#

from PyAida import *

tree1=tf.create("to1.aida","xml",1,0,"")
tree0=tf.create("to0.aida","xml",1,0,"")
tree=tf.create("sum.aida","xml",0,1,"")

cast=g.CastConverter()
h1=cast.toH1D(tree1.find('20'))
h0=cast.toH1D(tree0.find('20'))
xAxis1 = h1.axis()
xAxis = h0.axis()

print h1.axis().bins(), h1.axis().lowerEdge(), h1.axis().upperEdge()
print h0.axis().bins(), h0.axis().lowerEdge(), h0.axis().upperEdge()

h0.fill(0)
h0.fill(0.5)
h1.fill(.9 )

title = h1.title()
xBins = h1.axis().bins()
xLowEdge = h1.axis().lowerEdge()
xHighEdge = h1.axis().upperEdge()

print "taskout1.aida"
for iBin in range( xAxis1.bins() ):
    if h1.binHeight(iBin) != 0.:
        print "%f      %d       %f" % ( h1.binMean( iBin ), h1.binEntries( iBin ), h1.binHeight( iBin ) )

print "taskout0.aida"
for iBin in range( xAxis.bins() ):
    if h0.binHeight(iBin) != 0.:
        print "%f      %d       %f" % ( h0.binMean( iBin ), h0.binEntries( iBin ), h0.binHeight( iBin ) )


wf=af.createHistogramFactory( tree )

print h1.axis().lowerEdge(), h0.axis().lowerEdge()
result1 = wf.add('20', h1, h0)
print result1.entries()

result = wf.createHistogram1D("20",title, xBins, xLowEdge, xHighEdge)
ret = result.add(h1)
print "adding h1:", ret
ret = result.add(h0)
print "adding h0:", ret

print "sum.aida, entries", result.entries()
for iBin in range( xAxis.bins() ):
    if result.binHeight(iBin) != 0.:
        print "%f      %d       %f" % ( result.binMean( iBin ), result.binEntries( iBin ), result.binHeight( iBin ) )


#pl.plot(h0)
#pl.plot(h1)
tree.commit()
tree.close()
