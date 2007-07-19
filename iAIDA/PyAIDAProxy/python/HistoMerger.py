
import os, sys, string

# load Python binding to AIDA_Proxy
from AidaProxy import *

# ================================================================================

class BaseHistoMerger :

    # --------------------------------------------------------------------------------

    def __init__(self) :

        self._typeList    = ["IHistogram1D", "IHistogram2D", "IHistogram3D",
                             "ICloud1D"    , "ICloud2D"    , "ICloud3D"    ,
                             "IProfile1D"  , "IProfile2D"  ]

        self._outNameList = {}
        self._outTypeList = {}

        self._verbose = 1

        # This is needed to get the correct entries ... if we rely on the
        # "default" implementation, the bin-by-bin entries are not available
        # and are calculated/estimated from binHeight/binError values, which
        # are not correct when filled with weights ... 
        Proxy_Selector.setHistogramType("AIDA_Histogram_Native")

        return

    # --------------------------------------------------------------------------------

    def setVerbosity (self, verbosity) :
        self._verbose = verbosity
        return

    # --------------------------------------------------------------------------------

    def writeOutFile (self) :

        if (self._verbose > 0) : print "going to store to file ", self._outFileName, " of type ", self._outFileType

        if (self._verbose > 1) :
            print "list of histograms to be stored: \n", self._outNameList.keys()

        outStore = Proxy_Store(self._outFileName, self._outFileType, 1)


        for histName in self._outNameList.keys() :
            # no clouds, 3D histos and 2D profiles for hbook
            if ( (self._outTypeList[histName] in ["ICloud1D"    , "ICloud2D"    , "ICloud3D"    ,
                                                  "IHistogram3D", "IProfile2D"  ]) and
                 (self._outFileType == "hbook") ) : continue
            
            # no clouds for root
            if ( (self._outTypeList[histName] in ["ICloud1D"    , "ICloud2D"    , "ICloud3D"]) and
                 (self._outFileType == "root") ) : continue
            
            if (self._verbose > 1) :
                print "storing \n", self._outNameList[histName], " of type ", self._outTypeList[histName]

            outStore.write(self._outNameList[histName])

        outStore.close()

        return

    # --------------------------------------------------------------------------------

    def setOutFile(self, outFileName, outFileType) :
        self._outFileName = outFileName
        self._outFileType = outFileType
        return

    # --------------------------------------------------------------------------------

    def action(self, histo, name, type) :

        # ignore non-histograms for now
        if ( not self.checkType(type) ) :
            if (self._verbose > 10) : print "ignoring ", name, " of type ", type
            return
            
        # default action ...
        self.sumOrCreate(histo, name, type)

        return

    # --------------------------------------------------------------------------------

    def checkType(self, type) :
        # ignore non-histograms for now
        if (type not in self._typeList) :
            return False

        return True
            
    # --------------------------------------------------------------------------------

    def sumOrCreate(self, histo, name, type) :
        # if this histogram is not yet in the list, simply add it to the list
        # otherwise add this histogram to the one with the same name in the list
        if ( self._outNameList.has_key(name) ) :
            if (self._verbose > 10) : print " adding ", name, " of type ", type
            self._outNameList[name].add( histo )
        else:
            if (self._verbose > 10) : print "creating ", name, " of type ", type
            self._outNameList[name] = histo
            self._outTypeList[name] = type

        return

# ================================================================================

class HistoMerger(BaseHistoMerger) :

    # --------------------------------------------------------------------------------

    def __init__(self) :
        self._outFileName = None
        self._outFileType = None

        BaseHistoMerger.__init__(self)
        
        return

    # --------------------------------------------------------------------------------

    def add(self, fileName, fileType) :

        inStore = Proxy_Store(fileName, fileType)

        nameList = inStore.listObjectNames()
        typeList = inStore.listObjectTypes()

        cmdIn  = {"IHistogram1D" : inStore.retrieveH1D,
                  "IHistogram2D" : inStore.retrieveH2D,
                  "IHistogram3D" : inStore.retrieveH3D,
                  "ICloud1D"     : inStore.retrieveC1D,
                  "ICloud2D"     : inStore.retrieveC2D,
                  "ICloud3D"     : inStore.retrieveC3D,
                  "IProfile1D"   : inStore.retrieveP1D,
                  "IProfile2D"   : inStore.retrieveP2D }

        for i in range(len(nameList)) :
            name = nameList[i]
            type = typeList[i]

            # do whatever needs to be done with this histogram
            # factorized that out, as derived classes may just
            # want to overwrite this method ...
            self.action(cmdIn[type](name), name, type)
            
        inStore.close()

        return

    # --------------------------------------------------------------------------------

    def mergeFile(self, fileName, fileType) :

        if (self._verbose > 0) : print "going to add ", fileName, " of type ", fileType

        try:
            self.add(fileName, fileType)
        except:
            raise

        return

# ================================================================================

