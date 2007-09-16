#include "AIDA_ROOT/Histogram2D.h"

#include <iostream> 


using AIDA_ROOT::Histogram2D;

Histogram2D::Histogram2D(){
}


Histogram2D::Histogram2D(const std::string & title, int binsX, double iminX, double imaxX, int binsY, double iminY, double imaxY, const std::string & options) {
  initialize(title, binsX, iminX, imaxX, binsY, iminY, imaxY, options);
}

Histogram2D::Histogram2D(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, const std::string & options) {
  initialize(title, binEdgesX, binEdgesY, options);
}


void Histogram2D::initialize(const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, const std::string & /*options*/) {
  newRep(REP(title.c_str(),title.c_str(),nBinsX,lowerEdgeX,upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY));
  xAxis().initialize(representation().GetXaxis(),true);
  yAxis().initialize(representation().GetYaxis(),true);
  representation().Sumw2();
  m_sumEntries = 0;
  m_sumwx = 0;
  m_sumwy = 0;
}

void Histogram2D::initialize(const std::string & title, const std::vector<double>  & binEdgesX,const std::vector<double>  & binEdgesY, const std::string & /*options*/) {
  newRep(REP(title.c_str(),title.c_str(),binEdgesX.size()-1,&binEdgesX.front(), binEdgesY.size()-1,&binEdgesY.front() ) );
  xAxis().initialize(representation().GetXaxis(),false);
  yAxis().initialize(representation().GetYaxis(),false);
  representation().Sumw2();
  m_sumEntries = 0;
  m_sumwx = 0;
  m_sumwy = 0;
}

Histogram2D::Histogram2D(const Histogram2D & rh) :
  super(rh) {
  xAxis().initialize(representation().GetXaxis(),rh.xAxis().isFixedBinning());
  yAxis().initialize(representation().GetYaxis(),rh.yAxis().isFixedBinning());
}

// constructor from AIDA_CPP used by GenFactory

Histogram2D::Histogram2D(const AIDA_CPP::IHistogram2D & h) { 
  try { 
    const super & rh = dynamic_cast<const super&>(h); 
    newRep(rh.representation());
    xAxis().initialize(representation().GetXaxis(),rh.xAxis().isFixedBinning());
    yAxis().initialize(representation().GetYaxis(),rh.yAxis().isFixedBinning());
  } 
  catch (std::bad_cast & )
    {
      copyFromAida(h);
    }
}



Histogram2D::Histogram2D(const AIDA::IHistogram2D & h){    

  copyFromAida(h); 
    
}

Histogram2D::~Histogram2D(){  }

Histogram2D & Histogram2D::operator  = ( const Histogram2D & rh ) {
  newRep(rh.representation());
  xAxis().initialize(representation().GetXaxis(),rh.xAxis().isFixedBinning());
  yAxis().initialize(representation().GetYaxis(),rh.yAxis().isFixedBinning());
  return *this;
}

Histogram2D & Histogram2D::operator  = ( const AIDA::IHistogram2D & h ) { 

  copyFromAida(h); 
    
  return *this;
}


void Histogram2D::copyFromAida( const AIDA::IHistogram2D & h ) { 

 // implement here the copy
  if (h.xAxis().isFixedBinning() && h.yAxis().isFixedBinning()  )
    initialize(h.title(), h.xAxis().bins(), h.xAxis().lowerEdge(), h.xAxis().upperEdge(), 
	                  h.yAxis().bins(), h.yAxis().lowerEdge(), h.yAxis().upperEdge() ); 
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
    initialize(h.title(), binEdgesX, binEdgesY); 
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
  double sumwxy = 0; 

  // copy the contents in  (AIDA underflow/overflow are -2,-1)
  for (int i=-2; i < xAxis().bins(); ++i) { 
    for (int j=-2; j < yAxis().bins(); ++j) { 
      // root binning starts from one !
      representation().SetBinContent(rIndexX(i), rIndexY(j), h.binHeight(i,j) ); 
      representation().SetBinError(rIndexX(i), rIndexY(j), h.binError(i,j) ); 
      // calculate statistics 
      if ( i >= 0 && j >= 0) { 
	sumwxy += h.binHeight(i,j)*h.binMeanX(i,j)*h.binMeanY(i,j); 
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
  representation().PutStats(&stat.front()); 

}



// set bin content (entries and centre are not used ) 
bool Histogram2D::setBinContents( int i, int j, int entries, double height,double error,double centreX, double centreY ) { 
  representation().SetBinContent(rIndexX(i), rIndexY(j), height); 
  representation().SetBinError(rIndexX(i), rIndexY(j), error); 
  // accumulate sumwx for in range bins 
  if (i >=0 && j >= 0) { 
    m_sumwx += centreX*height; 
    m_sumwy += centreY*height; 
  }
  m_sumEntries += entries;
  return true;
}
    
// set rms (statistics information)
//
//  set all statistics info in the AIDA_ROOT histogram

bool Histogram2D::setRms(double rmsX, double rmsY  ) { 

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
  representation().PutStats(&stat.front()); 
  return true; 
}
    
// needd to overwrite reset to reset the sums 

bool  Histogram2D::reset (  )   { 
  m_sumEntries = 0;
  m_sumwx = 0; 
  m_sumwy = 0; 
  representation().Reset ( ); 
  return true; 
}
