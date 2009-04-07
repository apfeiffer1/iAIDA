#!/usr/bin/env python

from PyAida import * 
import random

tree=tf.create("tuple.hbook", "hbook", 0, 1, "lrec=2048")         # Creating a tree mapped to a new hbook file
tpf=af.createTupleFactory( tree )                                 # Creating a tuple factory associated to the tree

# description of the tuple
description = "int nTracks; double beamEnergy; bool goodTrigger;"
description += "Tuple{ double px } px;"
description += "Tuple{ double py } py;"
description += "Tuple{ double pz } pz;"
description += "Tuple{ double mass } mass"

# options defining the variable ranges (HBOOK CWN specific)
options = "nTracks[0,100] px(nTracks) py(nTracks) pz(nTracks) mass(nTracks)"

# creating the tuple
tuple = tpf.create( "100", "example tuple", description, options )

# filling the tuple
i_nTracks = tuple.findColumn( "nTracks" )
i_beamEnergy = tuple.findColumn( "beamEnergy" )
i_goodTrigger = tuple.findColumn( "goodTrigger" )
i_px = tuple.findColumn( "px" )
i_py = tuple.findColumn( "py" )
i_pz = tuple.findColumn( "pz" )
i_mass = tuple.findColumn( "mass" )

tracksPerEnergy = 0.5

for i in range(100) :
    beamEnergy = random.gauss( 90, 5 )
    numberOfTracks = int( tracksPerEnergy * beamEnergy + random.gauss( 0, 2 ) )
    if numberOfTracks < 0 :
        numberOfTracks = 0
    elif numberOfTracks > 100 :
        numberOfTracks = 100

    tuple.fill( i_nTracks, numberOfTracks )
    tuple.fill( i_beamEnergy, beamEnergy )
    gt = 1
    if ( random.gauss(0,1) < 0 ) :
        gt = 0
    
    tuple.fill( i_goodTrigger, gt )
    tpx = tuple.getTuple( i_px )
    tpy = tuple.getTuple( i_py )
    tpz = tuple.getTuple( i_pz )
    tmass = tuple.getTuple( i_mass )
    for iTrack in range(numberOfTracks) :
        tpx.fill( 0,  random.gauss( 0, 3 ) )
        tpx.addRow()
        tpy.fill( 0,  random.gauss( 0, 3 ) )
        tpy.addRow()
        tpz.fill( 0,  random.gauss( 0, 3 ) )
        tpz.addRow()
        tmass.fill( 0,  random.gauss( 1, 0.1 ) )
        tmass.addRow()
    
    tuple.addRow()

print "Filled the tuple with %d rows" % tuple.rows()
tree.commit()
tree.close()

del tpf
del tree
