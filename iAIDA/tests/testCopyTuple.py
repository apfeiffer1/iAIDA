#!/usr/bin/env python

#test creating a copying tuple

from PyAida import *

import random
# create memory tree
tree = tf.create()
tpf=af.createTupleFactory( tree )

# description of the tuple
description = "int nTracks; double beamEnergy; bool goodTrigger;"
description += "Tuple{ double px, py, pz, mass } track;"
description += "string eventType"

tuple = tpf.create( "myTuple", "example XML tuple", description)
# filling the tuple
i_nTracks = tuple.findColumn( "nTracks" )
i_beamEnergy = tuple.findColumn( "beamEnergy" )
i_goodTrigger = tuple.findColumn( "goodTrigger" )
i_track = tuple.findColumn( "track" )
i_type = tuple.findColumn( "eventType" )


tracksPerEnergy = 0.5

for i in range(100) :
    beamEnergy = random.gauss( 20, 5 )
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
    
    t = tuple.getTuple( i_track )
    for iTrack in range(numberOfTracks) :
        # fill track tupla
        t.fill( 0,  random.gauss( 0, 3 ) )
        t.fill( 1,  random.gauss( 0, 3 ) )
        t.fill( 2,  random.gauss( 0, 3 ) )
        t.fill( 3,  random.gauss( 1, 0.1 ) )
        t.addRow()
    

    s = "Event Number = " + str(i) 
    tuple.fill( i_type,s)
    
    tuple.addRow()

print "Filled the tuple with %d rows" % tuple.rows()

# copy the tuple using a dummy filter

dummy_filter = tpf.createFilter("1")
tupleCopy = tpf.createFiltered("myTupleCopy",tuple,dummy_filter)
tree.ls()

print "Rows  of copy ",tupleCopy.rows()
