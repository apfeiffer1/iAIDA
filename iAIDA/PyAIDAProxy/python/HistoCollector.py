
import os, sys, string

# load Python binding to AIDA_Proxy
from AidaProxy import *

from HistoMerger import BaseHistoMerger, HistoMerger

# ================================================================================

class HistoCollector(HistoMerger) :
    def __init__(self) :
        HistoMerger.__init__(self)
        return
    def addHisto(self, histo, type) :
        self.action(histo, histo.name(), type)
        return

# ================================================================================

class HistoSender :
    def __init__(self) :
        self.h1=Histogram1D("myHist","Test Histogram1D",100,0,10)
        return
    
    def getHisto (self) :
        import random
        # Filling the histogram with random data
        for i in range(1000):                                            
            self.h1.fill( random.gauss( 5, 2 ) )

        return (self.h1, "IHistogram1D")

# ================================================================================

def main() :
    collector = HistoCollector()
    collector.setVerbosity(2)
    collector.setOutFile("tstColl.aida", "xml")

    sender = HistoSender()

    count = 0
    loop = 0
    while (loop < 3) :
        count += 1
        # check if histos are ready
        (histo, type) = sender.getHisto()
        collector.addHisto(histo, type)
        if (count == 10) :
            count = 0
            loop += 1
            collector.writeOutFile()
            print "file updated"

if __name__ == "__main__" :
    main()
    
