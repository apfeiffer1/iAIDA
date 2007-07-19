# analysis using tuples based on root

from AidaProxy import *
# need also ROOT Dict
pylcgdict.loadDictionary('SealROOTDict')
g=pylcgdict.Namespace("")
TVector3 = g.TVector3

# for plotting
from hippoPlotter import *
# book histos
cx=Cloud1D("px")
cy=Cloud1D("py")
cz=Cloud1D("pz")
cm=Cloud1D("mass")


def writeTuple() : 

    # create Tuple

    s=Proxy_Store("treeDemo.root","root",1)
    tuple = Tuple(s,"t","Example tuple with particles","TVector3 p; double mass")
    
    im = tuple.findColumn("mass")
    
    # create p vector
    v=TVector3()
    v_pointer = v._C_instance
    
    tuple.setAddress("p",v_pointer.address())
    im=tuple.findColumn("mass")
    
    import random
    for i in range(0,1000) : 
        v.SetXYZ( random.gauss(0,1), random.gauss(1,2), random.gauss(2,5) )
        tuple.fill(im, random.gauss(10,1) )
        tuple.addRow()
        
        
    print "Tuple filled with ",tuple.rows()," rows "
    s.close()

    return


def readTuple() :

    # read Tuple
    
    s=Proxy_Store("treeDemo.root","root",3)
    tuple=s.retrieveTuple("t")
    print "Read tuple with ",tuple.rows()," rows "
    for i in range(0,tuple.columns() ) :
        print "Column ",i,tuple.columnName(i)," type ",tuple.columnType(i)

    # create p vector
    v=TVector3()
    v_pointer = v._C_instance
    
    tuple.setAddress("p",v_pointer.address())
    im=tuple.findColumn("mass")

    
    tuple.start()
    while ( tuple.next() ) :
        cx.fill(v.Px())
        cy.fill(v.Py())
        cz.fill(v.Pz())
        cm.fill(tuple.getDouble(im) )

    
    return


def main() : 
    writeTuple()
    readTuple()
    

if ( __name__ == "__main__" ) :
    main()



    #plot the histograms
    pl=HippoPlotter()
    pl.plot(cx)
    pl.plot(cy)
    pl.plot(cz)
    pl.plot(cm)
