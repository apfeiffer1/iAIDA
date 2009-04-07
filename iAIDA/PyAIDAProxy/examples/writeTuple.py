#!/usr/bin/env python

#creating a tuple of user defined name and type
#
#
import sys

if (len( sys.argv ) < 3) : raise "give fileName and number of events "
    
fileName = sys.argv[1]
ftypes=fileName.split(".")
type=ftypes[len(ftypes)-1]
if (type =="aida") : type="XML"
if (type =="hbk") : type = "hbook"

size = int(sys.argv[2])

print "Creating ",fileName
print "type is ",type
print "Size:", size

from AidaProxy import *

description = "float px; float py; float pz; float mass"

tuple = Tuple("1","Example Tuple",description)

# fill the tuple

# cache first indeces
i_px = tuple.findColumn( "px" )
i_py = tuple.findColumn( "py" )
i_pz = tuple.findColumn( "pz" )
i_mass = tuple.findColumn( "mass" )


import random

for i in range(0,size) :
    tuple.fill(i_px, random.gauss(0,3) )
    tuple.fill(i_py, random.gauss(0,3) )
    tuple.fill(i_pz, random.gauss(0,5) )
    tuple.fill(i_mass, random.gauss(1,0.1) )
    tuple.addRow()

print "Tuple filled with rows = ",tuple.rows()

#store now the tuple. 1 means recreate the file if already exists

s = Proxy_Store(fileName,type,1)
s.write(tuple)
s.close()
