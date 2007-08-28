#!/usr/bin/env python

# test memory tree

from PyAida import *
tree=tf.create()
hf=af.createHistogramFactory(tree)


# test remove
iret=tree.rm("")
print iret

c1=hf.createCloud1D("c1")
print tree.listObjectNames(),tree.listObjectTypes()

# test remove again
# test remove
iret=tree.rm("xxx")
print iret

print "try removing empty object"
iret=tree.rm("")
print iret

print "try removing cloud"
iret=tree.rm("c1")
print iret





