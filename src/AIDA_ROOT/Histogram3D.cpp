#include "AIDA_ROOT/Histogram3D.h"

#include <iostream> 


using AIDA_ROOT::Histogram3D;

Histogram3D::Histogram3D(){
}


Histogram3D::Histogram3D(const std::string & title, int binsX, double iminX, double imaxX, int binsY, double iminY, double imaxY, int binsZ, double iminZ, double imaxZ, const std::string & options) {
  initialize(title, binsX, iminX, imaxX, binsY, iminY, imaxY,  binsZ, iminZ, imaxZ, options);
}

Histogram3D::Histogram3D(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, const std::vector<double>  & binEdgesZ, const std::string & options) {
  initialize(title, binEdgesX, binEdgesY, binEdgesZ, options);
}


void Histogram3D::initialize(const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nBinsZ, double lowerEdgeZ, double upperEdgeZ, const std::string & /*options*/) {
  newRep(REP(title.c_str(),title.c_str(),nBinsX,lowerEdgeX,upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, nBinsZ, lowerEdgeZ, upperEdgeZ ));
  xAxis().initialize(representation().GetXaxis(),true);
  yAxis().initialize(representation().GetYaxis(),true);
  zAxis().initialize(representation().GetZaxis(),true);
  representation().Sumw2();
  m_sumEntries = 0;
  m_sumwx = 0;
  m_sumwy = 0;
  m_sumwz = 0;
}

void Histogram3D::initialize(const std::string & title, const std::vector<double>  & binEdgesX,const std::vector<double>  & binEdgesY, const std::vector<double>  & binEdgesZ, const std::string & /*options*/) {
  newRep(REP(title.c_str(),title.c_str(),binEdgesX.size()-1,&binEdgesX.front(), binEdgesY.size()-1,&binEdgesY.front(), binEdgesZ.size()-1,&binEdgesZ.front() ) );
  xAxis().initialize(representation().GetXaxis(),false);
  yAxis().initialize(representation().GetYaxis(),false);
  zAxis().initialize(representation().GetZaxis(),false);
  representation().Sumw2();
  m_sumEntries = 0;
  m_sumwx = 0;
  m_sumwy = 0;
  m_sumwz = 0;
}

Histogram3D::Histogram3D(const Histogram3D & rh) :
  super(rh) {
  xAxis().initialize(representation().GetXaxis(),rh.xAxis().isFixedBinning());
  yAxis().initialize(representation().GetYaxis(),rh.yAxis().isFixedBinning());
  zAxis().initialize(representation().GetZaxis(),rh.zAxis().isFixedBinning());
}

// constructor from AIDA_CPP used by GenFactory

Histogram3D::Histogram3D(const AIDA_CPP::IHistogram3D & h) { 
  try { 
    const super & rh = dynamic_cast<const super&>(h); 
    newRep(rh.representation());
    xAxis().initialize(representation().GetXaxis(),rh.xAxis().isFixedBinning());
    yAxis().initialize(representation().GetYaxis(),rh.yAxis().isFixedBinning());
    zAxis().initialize(representation().GetZaxis(),rh.zAxis().isFixedBinning());
  } 
  catch (std::bad_cast & )
    {
      copyFromAida(h);
    }
}

Histogram3D::Histogram3D(const AIDA::IHistogram3D & h){  
  copyFromAida(h);     
}

Histogram3D::~Histogram3D(){  }

Histogram3D & Histogram3D::operator  = ( const Histogram3D & rh ) {  
  newRep(rh.representation());
  xAxis().initialize(representation().GetXaxis(),rh.xAxis().isFixedBinning());
  yAxis().initialize(representation().GetYaxis(),rh.yAxis().isFixedBinning());
  zAxis().initialize(representation().GetZaxis(),rh.zAxis().isFixedBinning());
  return *this;

}

Histogram3D & Histogram3D::operator  = ( const AIDA::IHistogram3D & h ) { 
  copyFromAida(h);     
  return *this;
}


void Histogram3D::copyFromAida( const AIDA::IHistogram3D & h ) { 

 // implement here the copy
  if (h.xAxis().isFixedBinning() && h.yAxis().isFixedBinning() &&  h.zAxis().isFixedBinning()  )
    initialize(h.title(), h.xAxis().bins(), h.xAxis().lowerEdge(), h.xAxis().upperEdge(), 
	                  h.yAxis().bins(), h.yAxis().lowerEdge(), h.yAxis().upperEdge(), 
	                  h.zAxis().bins(), h.zAxis().lowerEdge(), h.zAxis().upperEdge() ); 
  else { 
    std::vector<double> binEdgesX;
    for (int i =0; i < h.xAxis().bins(); ++i) 
      binEdgesX.push_back(h.xAxis().binLowerEdge(i)); 
    // add also upperedges at the end
    binEdgesX.push_back(h.xAxis().upperEdge() ); 

    std::vector<double> binEdgesY;
    for (int i =0; i < h.yAxis().bins(); ++i) 
      binEdgesY.push_back(h.yAxis().binLowerEdge(i)); 
    // add also upperedges at the end
    binEdgesY.push_back(h.yAxis().upperEdge() ); 

    std::vector<double> binEdgesZ;
    for (int i =0; i < h.zAxis().bins(); ++i) 
      binEdgesZ.push_back(h.zAxis().binLowerEdge(i)); 
    // add also upperedges at the end
    binEdgesZ.push_back(h.zAxis().upperEdge() ); 

    initialize(h.title(), binEdgesX, binEdgesY, binEdgesZ); 
  }

  // statistics
  double sumw = h.sumBinHeights();    
  double sumw2 = 0; 
  if (h.equivalentBinEntries() != 0) 
    sumw2 = ( sumw * sumw ) /h.equivalentBinEntries();
  double sumwx = h.meanX()*h.sumBinHeights(); 
  double sumwx2 = (h.meanX()*h.meanX() + h.rmsX()*h.rmsX() )*h.sumBinHeights();
  double sumwy = h.meanY()*h.sumBinHeights(); 
  double sumwy2 = (h.meanY()*h.meanY() + h.rmsY()*h.rmsY() )*h.sumBinHeights();
  double sumwz = h.meanZ()*h.sumBinHeights(); 
  double sumwz2 = (h.meanZ()*h.meanZ() + h.rmsZ()*h.rmsZ() )*h.sumBinHeights();
  double sumwxy = 0; 
  double sumwxz = 0; 
  double sumwyz = 0; 

  // copy the contents in  (AIDA underflow/overflow are -2,-1)
  for (int i=-2; i < xAxis().bins(); ++i) { 
    for (int j=-2; j < yAxis().bins(); ++j) { 
      for (int k=-2; k < zAxis().bins(); ++k) { 
	representation().SetBinContent(rIndexX(i), rIndexY(j), rIndexZ(k), h.binHeight(i,j,k) ); 
	representation().SetBinError(rIndexX(i), rIndexY(j), rIndexZ(k), h.binError(i,j,k) );     
	// calculate statistics 
	if ( i >= 0 && j >= 0 && k >= 0) { 
	  sumwxy += h.binHeight(i,j,k)*h.binMeanX(i,j,k)*h.binMeanY(i,j,k); 
	  sumwxz += h.binHeight(i,j,k)*h.binMeanX(i,j,k)*h.binMeanZ(i,j,k); 
	  sumwyz += h.binHeight(i,j,k)*h.binMeanY(i,j,k)*h.binMeanZ(i,j,k); 
	}
 
      }
    }
  }
  // need to do set entries after setting contents otherwise root will recalulate them 
  // taking into account how many time  SetBinContents() has been called 
  representation().SetEntries(h.allEntries()); 

  // fill stat vector  
  std::vector<double> stat(11); 
  stat[0] = sumw; 
  stat[1] = sumw2; 
  stat[2] = sumwx; 
  stat[3] = sumwx2; 
  stat[4] = sumwy; 
  stat[5] = sumwy2; 
  stat[6] = sumwxy; 
  stat[7] = sumwz; 
  stat[8] = sumwz2; 
  stat[9] = sumwxz; 
  stat[10] = sumwyz; 
  representation().PutStats(&stat.front()); 

}


// set bin content (entries and centre are not used ) 
bool Histogram3D::setBinContents( int i, int j, int k, int entries,double height,double error,double centreX, double centreY, double centreZ ) { 
  representation().SetBinContent(rIndexX(i), rIndexY(j), rIndexZ(k), height); 
  representation().SetBinError(rIndexX(i), rIndexY(j), rIndexZ(k), error); 
  // accumulate sum bin centers
  if (i >=0 && j >= 0 && k >= 0) { 
    m_sumwx += centreX*height; 
    m_sumwy += centreY*height; 
    m_sumwz += centreZ*height; 
  }
  m_sumEntries += entries;
  return true;
}
    
// set rms (statistics information)
//
//  set all statistics info in the AIDA_ROOT histogram

bool Histogram3D::setRms(double rmsX, double rmsY, double rmsZ   ) { 

  representation().SetEntries(m_sumEntries); 

  std::vector<double> stat(11); 
  // sum weights 
  stat[0] =  sumBinHeights();  
  stat[1] = 0; 
  if (equivalentBinEntries() != 0) 
    stat[1] = (  sumBinHeights() * sumBinHeights() ) / equivalentBinEntries();
  stat[2] = m_sumwx;
  double meanX = 0; 
  if ( sumBinHeights() != 0 ) meanX =  m_sumwx/ sumBinHeights(); 
  stat[3] = ( meanX*meanX + rmsX*rmsX )* sumBinHeights(); 
  stat[4] = m_sumwy;
  double meanY = 0; 
  if ( sumBinHeights() != 0 ) meanY =  m_sumwy/ sumBinHeights(); 
  stat[5] = ( meanY*meanY + rmsY*rmsY )* sumBinHeights(); 
  stat[6] = 0; 
  stat[7] = m_sumwz;
  double meanZ = 0; 
  if ( sumBinHeights() != 0 ) meanZ =  m_sumwz/ sumBinHeights(); 
  stat[8] = ( meanZ*meanZ + rmsZ*rmsZ )* sumBinHeights(); 
  // do not need to use sumwxy sumwxz and sumwyz

  representation().PutStats(&stat.front()); 
  return true; 
}
    
// needd to overwrite reset to reset the sums 

bool  Histogram3D::reset (  )   { 
  m_sumEntries = 0;
  m_sumwx = 0;
  m_sumwy = 0;
  m_sumwz = 0;
  representation().Reset ( ); 
  return true; 
}
