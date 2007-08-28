# plotter module to interface AIDA (Lizard) to HippoDraw
#
# set up the path

import os,sys
if 'SEAL' in os.environ : 
  seal = os.environ['SEAL']
  if 'SCRAM_ARCH' in os.environ : arch = os.environ['SCRAM_ARCH']
  else                          : arch = 'rh73_gcc32'
  sys.path.append(seal+'/'+ arch+'/lib')
  sys.path.append(seal+'/seal/Scripting/PyROOT/src')


from  ROOT import TCanvas, TH1D, TH2D, TProfile

import math

# plotter class 

class RootPlotter :
    def __init__(self,title="Root Canvas",x1=200,y1=10,x2=700,y2=510):
        # create canvas 
        self.canvas = TCanvas('canvas',title,x1,y1,x2,y2)
        # dictionary with instances of display objects
        self.d = {}
        self.namecount = 0
        self.nameMap = {}


    # get histogram name 
    def getName(self, obj) : 
        # check if python object has name
        if (dir(obj).__contains__("name") ) :
          name = obj.name()
          # is not unique so add space in fron
        # in case of Swig bindings I have this
        else :
          if (dir(obj).__contains__("this") ) :
            name = obj.this
          else :
            name = obj.title() # not unique

        return name


    def getUniqueName(self, name) : 
        # keep name in a maps of name counter 
        if (self.nameMap.has_key(name) ) :
          self.nameMap[name] += 1
          name += "_" + str(self.nameMap[name])
        else :
          self.nameMap[name] = 0 
        return name


    
    def savePlot(self, obj , plot) :


        # get name
        name = self.getName(obj)
        if (self.d.has_key(name) ) :
          self.d[name].append(plot)
        else : 
          self.d[name] = [plot]


        # plotting  
    def plot( self, aidaObj, error = 0 ) :
        type = aidaObj.__class__.__name__
        # can be IHistogram1D or Histogram1D
        # select function according to type
        
        
        if (type.find("Histogram1D") >= 0 ) :
            self.plotH1D(aidaObj, error )
            return
        if (type.find("Histogram2D") >=0 ) :
            self.plotH2D(aidaObj, error)
            return
        if (type.find("Profile1D") >=0 ) :
            self.plotP1D(aidaObj, error)
            return
        
        print "Plotting for " + type + " is not yet implemented " 

#
#   Histo 1D plotting
#
    def plotH1D(self, h1, error) : 
        # create Root histograms and copy over
        #
        name = self.getName(h1)
        # make unique
        name = self.getUniqueName(name)
        hist = TH1D(name,h1.title(),h1.axis().bins(),h1.axis().lowerEdge(), h1.axis().upperEdge() )
        nbin = h1.axis().bins()
        for i in range(0,nbin ) :
            hist.SetBinContent(i+1,h1.binHeight(i))
            if (error) :
              hist.SetBinError(i+1,h1.binError(i))

        hist.SetEntries(h1.allEntries() )
        hist.Draw()
        # keep root object in memory 
        self.savePlot(h1,hist)

#
#   Histo 2D plotting
#
    def plotH2D(self, h, error) : 
        # create Root histograms and copy over
        #
        name = self.getName(h)
        # make unique
        name = self.getUniqueName(name)
        hist = TH2D(name,h.title(),h.xAxis().bins(),h.xAxis().lowerEdge(), h.xAxis().upperEdge(), h.yAxis().bins(),h.yAxis().lowerEdge(), h.yAxis().upperEdge() )
        for i in range(0,h.xAxis().bins() ) :
          for j in range(0,h.yAxis().bins() ) :
            hist.SetBinContent(i+1,j+1,h.binHeight(i,j))
            if (error) : 
              hist.SetBinError(i+1,j+1,h.binError(i,j))


        hist.SetEntries(h.allEntries() )
        hist.Draw()
        
        # keep root object in memory 
        self.savePlot(h,hist)


#
#   Profile 1D plotting
#
    def plotP1D(self, h, error) : 
        # create Root histograms and copy over
        #
        name = self.getName(h)
        # make unique
        name = self.getUniqueName(name)
        hist = TProfile(name,h.title(),h.axis().bins(),h.axis().lowerEdge(), h.axis().upperEdge() )
        nbin = h.axis().bins()
        for i in range(0,nbin ) :
            sumwyBin = h.binHeight(i)*h.binEntries(i)
            sumwy2Bin = ( h.binRms(i)*h.binRms(i) + h.binHeight(i)*h.binHeight(i) )*h.binEntries(i)
            hist.SetBinContent(i+1, sumwyBin ); 
            hist.SetBinError(i+1, math.sqrt(sumwy2Bin) ); 
            hist.SetBinEntries(i+1,h.binEntries(i) );


        hist.SetEntries(h.allEntries() )
        hist.Draw()
        # keep root object in memory 
        self.savePlot(h,hist)


