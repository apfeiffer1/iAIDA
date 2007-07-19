#!/usr/bin/env python

# test memory tree

from PyAida import *
tree=tf.create()
hf=af.createHistogramFactory(tree)
ff=af.createFunctionFactory(tree)
df=af.createDataPointSetFactory(tree)
tpf=af.createTupleFactory(tree)

c1=hf.createCloud1D("c1","My cloud 1d")
c2=hf.createCloud2D("c2","My cloud 2d")
c3=hf.createCloud3D("c3","My cloud 3d")
h1=hf.createHistogram1D("h1","My histo 1d",100,0,100)
h2=hf.createHistogram2D("h2","My histo 2d",50,0,100,50,0,100)
h3=hf.createHistogram3D("h3","My histo 3d",10,0,100,10,0,100,10,0,100)
p1=hf.createProfile1D("p1","My profile 1d",100,0,100)
p2=hf.createProfile2D("p2","My profile 2d",50,0,100,50,0,100)
func = ff.createFunctionByName("f","G")
dps = df.create("dps","My DataPointSet",2)
tuple = tpf.create("tuple","My tuple","double x, double y")

names=tree.listObjectNames()
types=tree.listObjectTypes()
print "Tree Contents "
for i in range(0,names.size()):
    print names[i],types[i]

    
# test now casting
print "\nTest casting..."
cast=g.CastConverter()

ih1=cast.toH1D(tree.find("h1"))
print ih1.title(),ih1.entries(),ih1.mean()

ih2=cast.toH2D(tree.find("h2"))
print ih2.title(),ih2.meanX()

ih3=cast.toH3D(tree.find("h3"))
print ih3.title(),ih3.meanZ()

ic1=cast.toC1D(tree.find("c1"))
print ic1.title(),ic1.lowerEdge()

ic2=cast.toC2D(tree.find("c2"))
print ic2.title(),ic2.lowerEdgeX()

ic3=cast.toC3D(tree.find("c3"))
print ic3.title(),ic3.lowerEdgeZ()

ip1=cast.toP1D(tree.find("p1"))
print ip1.title(),ip1.binMean(0)

ip2=cast.toP2D(tree.find("p2"))
print ip2.title(),ip2.binMeanY(0,0)

ifun = cast.toFunction(tree.find("f"))
print ifun.title(),ifun.numberOfParameters()

idps = cast.toDPS(tree.find("dps"))
print idps.title(),idps.size()

ituple = cast.toTuple(tree.find("tuple"))
print ituple.title(),ituple.columns()




