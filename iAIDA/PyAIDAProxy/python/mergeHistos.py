#!/usr/bin/env python

import os, sys, string

from HistoMerger import HistoMerger

# ================================================================================

def merge(outFileName, fileListIn) :

    typeMap = { ".hbook" : "hbook",
                ".hbk"   : "hbook",
                ".aida"  : "XML"  ,
                ".root"  : "root" }

    merger = HistoMerger()

    merger.setVerbosity(2)
    
    typeOut = typeMap[outFileName[string.rfind(outFileName,"."):]]

    merger.setOutFile(outFileName, typeOut)

    for fileName in fileListIn :
        extension = fileName[string.rfind(fileName,"."):]
        merger.mergeFile(fileName, typeMap[extension])

    merger.writeOutFile()

    return

# ================================================================================

if (__name__ == "__main__" ) :
    
    if (len(sys.argv) < 3) :
        print "usage: ", sys.argv[0]," <outFile> <inFile> [<inFile> ...] "
        sys.exit(-1)
        
    merge(sys.argv[1], sys.argv[2:])
    
