#!/usr/bin/env python
#
# read a root tree using Pyroot and find muon track
#
#
import os

#from ROOT import TTree, TObjArray
#from ROOT import TCanvas, TPad, TFile, TPaveText

import pylcgdict
pylcgdict.loadDictionary('SealROOTDict')
g=pylcgdict.Namespace("")
TTree     = g.TTree
TObjArray     = g.TObjArray
TFile = g.TFile


# this works only on Unix
fileLoc = os.environ["PI"] + "/src/AnalysisServices/PyAIDAProxy/examples"
fileName = fileLoc + "/tree1.root"
print "reading from file",fileName

file=TFile(fileName)
tree=file.Get("t1")

# get all leaves 
leaves = tree.GetListOfLeaves()


# define dynamically a python class containing root Leaves objects
class PyListOfLeaves(dict) :
    pass

# create an istance
pyl = PyListOfLeaves()

# add leaves as attributes
for i in range(0,leaves.GetEntries() ) :
    leaf = leaves.At(i)
    name = leaf.GetName()
    # add dynamically attribute to my class 
    pyl.__setattr__(name,leaf)
  


# book histograms 
from AidaProxy import * 



# specify max events to keep in clouds 
hpx = Cloud1D("px","Px distribution",10001)
hpxpy = Cloud2D("pxpy","Px vs Py Distribution",10001)


nev = tree.GetEntries()
for iev in range(0,nev) :
    tree.GetEntry(iev)
    # get values from the tree using Python class pyl which contains leaves
    # objects 
    px = pyl.px.GetValue()
    py = pyl.py.GetValue()
    if (iev < 10) : print px,py
    
    hpx.fill(px)
    hpxpy.fill(px,py)


# save histo in an xml file
store = Proxy_Store("histoFromTree1.aida","XML",1)
print "Entries for Histogram",hpx.name(),"=",hpx.entries()
store.write(hpx)
store.write(hpxpy)

store.close()

# plot histograms

from hippoPlotter import *
plotter = HippoPlotter()
plotter.plot(hpx)                    
plotter.plot(hpxpy)                    
                                    
