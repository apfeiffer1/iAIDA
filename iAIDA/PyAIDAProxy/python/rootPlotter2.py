# plotter module to interface AIDA  to ROOT
#
# use PyLCGDict bindings to ROOT 

import os,sys

import pylcgdict
#use SealRootDict from pylcgdict
pylcgdict.loadDictionary('SealROOTDict')
g=pylcgdict.Namespace("")
TCanvas  = g.TCanvas
TH1D     = g.TH1D
TH2D     = g.TH2D
TProfile     = g.TProfile
TF1     = g.TF1

# create root application (needs to be global) 
gApplication = g.ROOTApplication('ROOTApp')
# root globals (not sure if needed) 
TGlobals = g.TGlobals
gSystem = TGlobals.System()
gROOT = TGlobals.ROOT();

### root thread --------
import thread, time
def _processRootEvents():
  while True:
    gSystem.ProcessEvents()
    time.sleep(0.1)
#print 'Starting ROOT thread.....'

# need to start root thread to have an active GUI window
thread.start_new_thread( _processRootEvents, () )

gROOT.Reset()


# function converter
try : 
  pylcgdict.loadDictionary("AIDA_RootConverterDict")
  FunctionConverter = g.pi.AIDA_RootConverter.FunctionConverter
  theFunctionConverter = FunctionConverter()
except :
  print "Warning : cannot load AIDA_RootConverter dictionary " 
  pass



import math

# plotter class 

class RootPlotter :
    def __init__(self,title="Root Canvas",x1=200,y1=10,x2=700,y2=510):
        

        # dictionary with instances of display objects
        self.d = {}
        self.myFunc = {}
        self.namecount = 0
        self.nameMap = {}
        
        # create canvas 
        self.canvas = TCanvas('canvas',title,x1,y1,x2,y2)
        self.canvas.GetFrame().SetBorderSize( 6 )
        self.canvas.GetFrame().SetBorderMode( -1 )

        self.lineColor = 1
        return


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


        # plotting of histograms   
    def plot( self, aidaObj, option = "", xmin= 0, xmax = 0 ) :
        type = aidaObj.__class__.__name__
        # can be IHistogram1D or Histogram1D
        # select function according to type

        overlay = 0
        error = 0 
        if (option.find("S") >=0 ) :
          overlay = 1
        if (option.find("E") >=0  ) :
          error = 1
        
        if (type.find("Histogram1D") >= 0 ) :
            self.plotH1D(aidaObj, error, overlay )
        elif (type.find("Histogram2D") >=0 ) :
            self.plotH2D(aidaObj, error, overlay)
        elif (type.find("Profile1D") >=0 ) :
            self.plotP1D(aidaObj, error, overlay)
        # functions: case of AIDA functions
        elif (type.find("Function") >=0 ) :
            if (overlay) :
              xmin = self.minX
              xmax = self.maxX
              
            self.plotFunc(aidaObj, xmin, xmax, overlay)

        # case of a Python function (needs to be a simple function of x (1 dim and no params) 
        # create an AIDA functions and plot it
        elif (type == "function" ) :
          try :
            pylcgdict.loadDictionary("UserFunctionsDict")
          except :
            print "Error plotting a Python function : cannot load UserFunctionDict"
            return

          UserFunction =  g.pi_aida.UserFunction
          #convert to Aida function
          aidaFunc = UserFunction(aidaObj,1,0)
          # need to save the aida functions
          self.myFunc[aidaObj] = aidaFunc
          if (overlay) :
            xmin = self.minX
            xmax = self.maxX
              
          self.plotFunc(aidaFunc, xmin, xmax, overlay)

        # case of unknown object
        else : 
            print "Plotting for " + type + " is not yet implemented "
            return

        self.canvas.Modified()
        self.canvas.Update()


#
#   Histo 1D plotting
#
    def plotH1D(self, h1, error, overlay) : 
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
        #print "drawing the object now... " 
        # plot and set color according to layer 
        if (overlay) :
          self.lineColor += 1
          hist.SetLineColor(self.lineColor)
          hist.Draw("same")
        else : 
          self.lineColor = 1
          hist.SetLineColor(self.lineColor)
          hist.Draw()
          
        # keep root object in memory 
        self.savePlot(h1,hist)
        self.minX = h1.axis().lowerEdge()
        self.maxX = h1.axis().upperEdge()
        return ;
#
#   Histo 2D plotting
#
    def plotH2D(self, h, error, overlay) : 
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
        # plot and set color according to layer 
        if (overlay) :
          self.lineColor += 1
          hist.SetLineColor(self.lineColor)
          hist.Draw("same")
        else : 
          self.lineColor = 1
          hist.SetLineColor(self.lineColor)
          hist.Draw()

        self.minX = h.xAxis().lowerEdge()
        self.maxX = h.xAxis().upperEdge()

        # keep root object in memory 
        self.savePlot(h,hist)
        return ; 

#
#   Profile 1D plotting
#
    def plotP1D(self, h, error, overlay) : 
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


        if (overlay) :
          self.lineColor += 1
          hist.SetLineColor(self.lineColor)
          hist.Draw("same")
        else : 
          self.lineColor = 1
          hist.SetLineColor(self.lineColor)
          hist.Draw()

        hist.SetEntries(h.allEntries() )
        self.minX = h.axis().lowerEdge()
        self.maxX = h.axis().upperEdge()
        # keep root object in memory 
        self.savePlot(h,hist)
        return 
#
#  Function plotting
#


    def plotFunc(self,func,xmin,xmax, overlay) :
       name = self.getName(func)
       # make unique
       name = self.getUniqueName(name)
       try : 
         rootFunc = theFunctionConverter.toRoot(func,name,xmin,xmax)
       except:
         print "Cannot convert function ",func.__class__.__name__," to ROOT"
         return

       if (overlay) :
          self.lineColor += 1
          rootFunc.SetLineColor(self.lineColor)
          rootFunc.Draw("same")
       else : 
          self.lineColor = 1
          rootFunc.SetLineColor(self.lineColor)
          rootFunc.Draw()
         
       self.savePlot(func,rootFunc)
       self.minX = xmin
       self.maxX = xmax
       return
     
       
