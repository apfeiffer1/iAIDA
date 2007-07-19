# plotter module to interface AIDA (Lizard) to HippoDraw
#

from hippo import *

# plotter class 

import  pylcgdict 
pylcgdict.loadDictionary("SealSTLDict")
g=pylcgdict.Namespace('')
std=g.std


class HippoPlotter :
    def __init__(self):
        # create canvas 
        app = HDApp()
        self.canvas = app.canvas()
        # dictionary with instances of display objects
        self.d = {}
        # disctionary with tuples
        self.tuples = {}
        # list of colors
        self.colors = ("black", "red", "green", "blue", "yellow", "cyan", "magenta")
        self.colorIndex = 0


    # get histogram name 
    def getName(self, obj) : 
        # check if python object has name
        if (dir(obj).__contains__("name") ) :
          name = obj.name()
        # in case of Swig bindings I have this
        else :
          if (dir(obj).__contains__("this") ) :
            name = obj.this
          else :
            name = obj.title() # not unique

        return name

    def savePlot(self, c , plot, xmin = 0, xmax = 1, nbin = 0) :
        name = self.getName(c)
        if (self.d.has_key(name) ) :
            self.d[name].append(plot)
        else : 
            self.d[name] = [plot]

        self.lastDisplay = plot
        self.xmin = xmin
        self.xmax = xmax
        self.nbinPlot = nbin

        # plotting  
    def plot( self, aidaObj, opt1 = None, opt2 = None, opt3 = None) :
        type = aidaObj.__class__.__name__
#        print "plot " + type + " with options ",opt1,opt2

        # choose function according to object type 
        if (type.find("Histogram1D") >= 0 ) :
            self.plotH1D(aidaObj,opt1,opt2)
            return
        if (type.find("Histogram2D") >= 0 ) :
            self.plotH2D(aidaObj,opt1,opt2)
            return
        if (type.find("Cloud1D") >= 0 ) :
            self.plotC1D(aidaObj,opt1,opt2)
            return
        if (type.find("Cloud2D") >= 0 ) :
            self.plotC2D(aidaObj,opt1,opt2)
            return
        if (type.find("Cloud3D") >= 0 ) :
            self.plotC3D(aidaObj,opt1,opt2)
            return
        if (type.find("DataPointSet") >= 0 ) :
            self.plotDPS(aidaObj,opt1,opt2)
            return
        if (type.find("Function") >= 0 ) :
            self.plotFunction(aidaObj,opt1, opt2, opt3)
            return
        
        print "Plotting for " + type + " is not yet implemented " 


    def plotH1D(self, h1, opt1, opt2) : 
        # create Hippodraw st histograms
        #
        plotType="Static Histogram"
        xlabel = "x"
        ylabel = "Entries"
        if ( opt2.__class__.__name__ == "list" or opt2.__class__.__name__ == "tuple" ) :
            if (len(opt2) == 2 ) :
                xlabel = opt2[0]
                ylabel = opt2[1]

         
        sthist = Display ( plotType )

        sthist.setTitle ( h1.title() )
        sthist.setLabel ( "x", xlabel )
        if (not  h1.axis().isFixedBinning() ) :
            print "Cannot plot variable bin histograms"
            return

#        xrange = (h1.axis().lowerEdge(),h1.axis().upperEdge() )
#        sthist = StaticHist(h1.title(),xlabel,xrange,h1.axis().binWidth(0))
        

        sthist.setRange ( xlabel, h1.axis().lowerEdge(), h1.axis().upperEdge(), 1 )
        sthist.setBinWidth ( xlabel, h1.axis().binWidth(0) )
        # fill histogram copying the contents from aida
        for i in range(0,h1.axis().bins() ) :
            for j in range(0,h1.binEntries(i) ) :
                # add values takes a tuple
                val=(h1.binMean(i),)
                sthist.addValues( val )
        
        self.canvas.addDisplay(sthist)
        #print "histogram displayed "
        # save for further use
        self.savePlot(h1,sthist,h1.axis().lowerEdge(), h1.axis().upperEdge() , h1.axis().bins())


    def plotC1D(self, c1, opt1 , opt2) :
        # check if converted
        if c1.isConverted() :
            h1=c1.histogram()
            self.plotH1D(h1,opt1,opt2)
            return
                       

        # create Hippodraw ntuple
        #
        plotType = "Histogram"
        labels = ["x","weight"] 
        if (opt1.__class__.__name__ == "str") :
            plotType = opt1
            
        if ( opt2.__class__.__name__ == "list" or opt2.__class__.__name__ == "tuple" ) :
            for i in range(0,len(opt2)) :
                if (opt2[i].__class__.__name__ == "str" ):
                    print opt2[i]
                    labels[i] = opt2[i]



        v = [0,0]


        nt = 0
        name = self.getName(c1)
        if (self.tuples.has_key( name ) ) :
            nt = self.tuples[name]
        else : 
            nt = NTuple(labels)
        
            # fill hippodraw tuple
            for i in range(0,c1.entries() ) :
                v[0] = c1.value(i)
                v[1] = c1.weight(i)
                nt.addRow(v)

            self.tuples[name] = nt
#            print "created HD ntuple with ", nt.rows, "rows "     


        # now create Histogram


        cdisp = Display(plotType,nt,(labels))
        cdisp.setTitle ( c1.title() )
        self.canvas.addDisplay(cdisp)
        self.savePlot(c1,cdisp)

# plot histogram in twoD as an XYZ plot

    def plotH2D(self, h, opt1, opt2):
### fix me: does not work : use x-y p

        self.plotH2D_as_XYZPlot(h,opt1,opt2)
        return
        
        
        # create Hippodraw ntuple
        #
        plotType = "Static 2D Histogram"
        xlabel = "x"
        ylabel = "y"
        if ( opt2.__class__.__name__ == "list" or opt2.__class__.__name__ == "tuple" ) :
            if (len(opt2) == 2 ) :
                xlabel = opt2[0]
                ylabel = opt2[1]


        # now create

        sthist = Display ( plotType )
        sthist.setTitle ( h.title() )
        sthist.setLabel ( "x", xlabel )
        sthist.setLabel ( "y", ylabel )
        if (not  h.xAxis().isFixedBinning() ) :
            print "Cannot plot variable bin histograms"
            return

        if (not  h.yAxis().isFixedBinning() ) :
            print "Cannot plot variable bin histograms"
            return


        sthist.setRange ( xlabel, h.xAxis().lowerEdge(), h.xAxis().upperEdge(), 1 )
        sthist.setBinWidth ( xlabel, h.xAxis().binWidth(0) )
        sthist.setRange ( ylabel, h.yAxis().lowerEdge(), h.yAxis().upperEdge(), 1 )
        sthist.setBinWidth ( ylabel, h.yAxis().binWidth(0) )

        # fill the data
        for i in range(0,h.xAxis().bins() ) :
            for j in range(0,h.yAxis().bins() ) :
                for k in range(0,h.binEntries(i,j) ) :
                    # add values takes a tuple
                    val=(h.binMeanX(i,j),h.binMeanY(i,j))
                    sthist.addValues( val )
        


        self.canvas.addDisplay(sthist)
        #print "histogram displayed "
        # save for further use
        self.savePlot(h,sthist)

    def plotH2D_as_XYZPlot(self, h, opt1, opt2): 
        # create Hippodraw ntuple
        #
        plotType = "XYZ Plot"
        labels = ["x","y","z"]
        v = [0,0,0]
        # use options if given
        if (opt1.__class__.__name__ == "str") :
            plotType = opt1
            
        if ( opt2.__class__.__name__ == "list" or opt2.__class__.__name__ == "tuple" ) :
            for i in range(0,len(opt2)) :
                if (opt2[i].__class__.__name__ == "str" ) : 
                    labels[i] = opt2[i]
            
        nt = 0
        name = self.getName(h)
        if (self.tuples.has_key(name) ) :
            nt = self.tuples[name]
        else : 
            nt = NTuple(labels)
        
            # fill hippodraw tuple
            for i in range(0,h.xAxis().bins() ) :
                for j in range(0,h.yAxis().bins() ) :
                    v[0] = h.binMeanX(i,j)
                    v[1] = h.binMeanY(i,j)
                    v[2] = h.binHeight(i,j)
                    nt.addRow(v)

            self.tuples[name] = nt


        # now create a Scatter plot
        hist = Display(plotType,nt,(labels[0],labels[1],labels[2]))
        hist.setTitle(h.title() )
        self.canvas.addDisplay(hist)
        # save instances
        self.savePlot(h,hist)

    def plotC2D(self, c, opt1, opt2): 
        # check if converted
        if c.isConverted() :
            h=c.histogram()
            self.plotH2D(h,opt1,opt2)
            return
        
        # create Hippodraw ntuple
        #
        plotType = "Scatter Plot"
        labels = ["x","y","weight"]
        v = [0,0,0]
        # use options if given
        if (opt1.__class__.__name__ == "str") :
            plotType = opt1
            
        if ( opt2.__class__.__name__ == "list" or opt2.__class__.__name__ == "tuple" ) :
            for i in range(0,len(opt2)) :
                if (opt2[i].__class__.__name__ == "str" ) : 
                    labels[i] = opt2[i]
            
        nt = 0
        name = self.getName(c)
        if (self.tuples.has_key(name) ) :
            nt = self.tuples[name]
        else : 
            nt = NTuple(labels)
        
            # fill hippodraw tuple
            for i in range(0,c.entries() ) :
                v[0] = c.valueX(i)
                v[1] = c.valueY(i)
                v[2] = c.weight(i)
                nt.addRow(v)

            self.tuples[name] = nt


        # now create a Scatter plot
        hist = Display(plotType,nt,labels[0:2])
        hist.setTitle( c.title() )
        self.canvas.addDisplay(hist)
        # save instances
        self.savePlot(c,hist)


    def plotC3D(self, c, opt1, opt2): 
        pass
#
# data point sets 
#

    def plotDPS(self,dps,opt1,opt2) :
        dim = dps.dimension()
#
#  create hippodraw tuple
#
        plotType = "XY Plot"
        labels = ["x","y","ex","ey"]
        v = [0,0,0,0]

        
        if (dim == 3) :  
            labels.insert(2,"z")
            labels.append("ez")
            v.append(0)
            v.append(0)
        if (dim > 3) :
            for i in range(4,dim+1) :
                labels.insert(i-1,"x"+str(i))
                labels.append("e"+str(i))
                v.append(0)
                v.append(0)
            
        
        # use options if given
        if (opt1.__class__.__name__ == "str") :
            plotType = opt1
            
        if ( opt2.__class__.__name__ == "list" or opt2.__class__.__name__ == "tuple" ) :
            for i in range(0,len(opt2)) :
                if (opt2[i].__class__.__name__ == "str" ) : 
                    labels[i] = opt2[i]
        
        nt = 0
        name = self.getName(dps)
        if (self.tuples.has_key(name) ) :
            nt = self.tuples[name]
        else : 
            nt = NTuple(labels)


            # fill hippodraw tuple
            for i in range(0,dps.size() ) :
                point = dps.point(i)
                for j in range(0,dim) :
                    value =  point.coordinate(j).value()
                    error = (point.coordinate(j).errorPlus() + point.coordinate(j).errorMinus() )/2
                    if (dim == 1) : 
                        v[0] = i
                        v[1] = value
                        v[2] = 0
                        v[3] = error
                    else :
                        v[j] = value
                        v[dim+j] = error
                            
                nt.addRow(v)
                
            self.tuples[name] = nt

            
        # now create Display
        disp = 0
        if (plotType == "Strip Chart") : 
            disp = Display(plotType,nt,labels[0:2])
        else :
            disp = Display(plotType,nt,labels[0:4])

        disp.setTitle( dps.title() )
        self.canvas.addDisplay(disp)
        self.savePlot(dps,disp)

    def plotFunction(self,func,opt1,opt2,opt3) :
        #  need to create first an hippodraw tuple with function values 

        # use options if given
        if (opt1.__class__.__name__ == "str") :

            # default number of bins 
            nbin = 100
            if len(opt1) > 0 and opt1[0] == 'S' :
                overlay = 1
            else :
                overlay = 0

        if ( opt2.__class__.__name__ == "list" or opt2.__class__.__name__ == "tuple" ) :
            xmin = opt2[0]
            xmax = opt2[1]
            # see if also nbin is specified 
            if ( len(opt2) > 2 ) :
                nbin = opt[2]

        elif (opt2.__class__.__name__ == "float" or  opt2.__class__.__name__ == "int" ) and (opt3.__class__.__name__ == "float" or  opt3.__class__.__name__ == "int" ) :
            xmin = opt2
            xmax = opt3            
        else :
            if overlay :
                xmin = self.xmin
                xmax = self.xmax
                nbin = max(100, self.nbinPlot*5)

        # create hippodraw tuple                        
        nt = 0
        name = self.getName(func)
        if (self.tuples.has_key(name) ) :
            nt = self.tuples[name]
        else : 
            labels = ["x","f(x)"]
            nt = NTuple(labels)
            v = [0,0]

            # fill hippodraw tuple
            dx = float(xmax-xmin)/nbin
            xv = std.vector('double')(1)
            for i in range(0, nbin + 1 ) :
                x = xmin + i*dx
                v[0] = x
                xv.assign(1,x)
                v[1] = func.value(xv)
                # add hipoodraw raw            
                nt.addRow(v)
                
            self.tuples[name] = nt

        #now make the display
        if ( overlay == 0) :
            disp = Display('Strip Chart',nt,labels[0:2]) 
            disp.setTitle( func.title() )
            self.canvas.addDisplay(disp)
            self.savePlot(func,disp)
            self.colorIndex = 0
        else :
            #update color index 
            self.colorIndex += 1
            if (self.colorIndex == len(self.colors) ) :
                self.colorIndex = 0
                
            color = self.colors[self.colorIndex]
                
            # need to get last item displayed
            disp = self.lastDisplay
            dataRep = DataRep('Strip Chart',nt,labels[0:2])
            disp.addDataRep(dataRep)
            dataRep.setColor(color)


        return
