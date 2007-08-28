#!/usr/bin/env python

#test creating a root tuple

from PyAida import *
tree=tf.create("tuple.aida", "XML", 0, 1)   
tpf=af.createTupleFactory( tree )                           

description = "float px; float py; float pz; float mass"

tuple = tpf.create("t1","example tuple",description)

# fill the tuple

# cache first indeces
i_px = tuple.findColumn( "px" )
i_py = tuple.findColumn( "py" )
i_pz = tuple.findColumn( "pz" )
i_mass = tuple.findColumn( "mass" )



import random

for i in range(0,10) :
    px =  random.gauss(0,3)
    if ( i < 2 or i > 7) : print px,i
    r=tuple.fill(i_px, px )
    r=tuple.fill(i_py, random.gauss(0,3) )
    r=tuple.fill(i_pz, random.gauss(0,5) )
    r=tuple.fill(i_mass, random.gauss(1,0.1) )
    r=tuple.addRow()



print "Tuple filled with rows = ",tuple.rows()

tree.commit()
tree.close()
