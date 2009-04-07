//
//    Copyright 2007 Andreas Pfeiffer
//
/*
 *  This file is part of iAIDA.
 *
 *  iAIDA is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as 
 *  published by the Free Software Foundation, either version 3 of 
 *  the License.
 *
 *  iAIDA is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public 
 *  License along with iAIDA.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "AIDA_ROOT/Histogram1D.h"

#include <iostream> 


using AIDA_ROOT::Histogram1D;

Histogram1D::Histogram1D(){
}


Histogram1D::Histogram1D(const std::string & title, int bins, double imin, double imax, const std::string & options) {
  initialize(title, bins, imin, imax, options);
}

Histogram1D::Histogram1D(const std::string & title, const std::vector<double>  & binEdges, const std::string & options) {
  initialize(title, binEdges, options);
}


void Histogram1D::initialize(const std::string & title, int nBins, double lowerEdge, double upperEdge, const std::string & /*options*/) {
  newRep(REP(title.c_str(),title.c_str(),nBins,lowerEdge,upperEdge));
  axis().initialize(representation().GetXaxis(),true);
  representation().Sumw2();
  m_sumEntries = 0;
  m_sumwx = 0;
}

void Histogram1D::initialize(const std::string & title, const std::vector<double>  & binEdges, const std::string & /*options*/) {
  newRep(REP(title.c_str(),title.c_str(),binEdges.size()-1,&binEdges.front()));
  axis().initialize(representation().GetXaxis(),false);
  representation().Sumw2();
  m_sumEntries = 0;
  m_sumwx = 0;
}

Histogram1D::Histogram1D(const Histogram1D & rh) : super() { 
  newRep(rh.representation()); 
  axis().initialize(representation().GetXaxis(),rh.axis().isFixedBinning());
}

// constructor from AIDA_CPP used by GenFactory

Histogram1D::Histogram1D(const AIDA_CPP::IHistogram1D & h) { 
  try { 
    const super & rh = dynamic_cast<const super&>(h); 
    newRep(rh.representation());
    axis().initialize(representation().GetXaxis(),rh.axis().isFixedBinning());
  } 
  catch (std::bad_cast & )
    {
      copyFromAida(h);
    }
}


Histogram1D::Histogram1D(const AIDA::IHistogram1D & h){    
  copyFromAida(h);     
}

Histogram1D::~Histogram1D(){ }

Histogram1D & Histogram1D::operator  = ( const Histogram1D & rh ) {  
  newRep(rh.representation() );
  axis().initialize(representation().GetXaxis(),rh.axis().isFixedBinning());
  return *this;
}

Histogram1D & Histogram1D::operator  = ( const AIDA::IHistogram1D & h ) { 
  copyFromAida(h);     
  return *this;
}


void Histogram1D::copyFromAida( const AIDA::IHistogram1D & h ) { 

 // implement here the copy
  if (h.axis().isFixedBinning() )
    initialize(h.title()+"Copy", h.axis().bins(), h.axis().lowerEdge(), h.axis().upperEdge() ); 
  else { 
    std::vector<double> binEdges; 
    for (int i =0; i < h.axis().bins(); ++i) 
      binEdges.push_back(h.axis().binLowerEdge(i)); 
    // add also upperedges at the end
    binEdges.push_back(h.axis().upperEdge() ); 
    initialize(h.title(), binEdges); 
  }
  // sumw 
  double sumw = h.sumBinHeights();    
  // sumw2 
  double sumw2 = 0; 
  if (h.equivalentBinEntries() != 0) 
    sumw2 = ( sumw * sumw ) /h.equivalentBinEntries();

  double sumwx = h.mean()*h.sumBinHeights(); 
  double sumwx2 = (h.mean()*h.mean() + h.rms()*h.rms() )*h.sumBinHeights();

  // copy the contents in 
  for (int i=-2; i < axis().bins(); ++i) { 
    // root binning starts from one !
    representation().SetBinContent(rIndex(i),h.binHeight(i) ); 
    representation().SetBinError(rIndex(i),h.binError(i) ); 
  }
  // need to do set entries after setting contents otherwise root will recalulate them 
  // taking into account how many time  SetBinContents() has been called 
  representation().SetEntries(h.allEntries()); 
    // stat vector  
  std::vector<double> stat(11); 
  stat[0] = sumw; 
  stat[1] = sumw2; 
  stat[2] = sumwx; 
  stat[3] = sumwx2; 
  representation().PutStats(&stat.front()); 

}


// set bin content (entries and centre are not used ) 
bool Histogram1D::setBinContents( int i, int entries ,double height,double error,double centre ) { 
  representation().SetBinContent(rIndex(i), height); 
  representation().SetBinError(rIndex(i),error); 
  // accumulate sumwx for in range bins
  if (i != AIDA::IAxis::UNDERFLOW_BIN && i !=  AIDA::IAxis::OVERFLOW_BIN ) 
    m_sumwx  += centre*height; 
  m_sumEntries += entries;
  return true;
}
    
// set rms (statistics information)
//
//  set all statistics info in the AIDA_ROOT histogram

bool Histogram1D::setRms(double rms  ) { 

  representation().SetEntries(m_sumEntries); 

  std::vector<double> stat(11); 
  // sum weights 
  stat[0] =  sumBinHeights();  
  stat[1] = 0; 
  if (equivalentBinEntries() != 0) 
    stat[1] = (  sumBinHeights() * sumBinHeights() ) / equivalentBinEntries();
  stat[2] = m_sumwx;
  double mean = 0; 
  if ( sumBinHeights() != 0 ) mean =  m_sumwx/ sumBinHeights(); 

  stat[3] = ( mean*mean + rms*rms )* sumBinHeights(); 
  representation().PutStats(&stat.front()); 
  return true; 
}
    
// need to overwrite reset to reset the sums 

bool  Histogram1D::reset (  )   { 
  m_sumwx = 0; 
  m_sumEntries = 0; 
  representation().Reset ( ); 
  return true; 
}

// set histogram statistics 

bool Histogram1D::setStatistics(int allEntries, double eqBinEntries, double mean, double rms  ) { 

  representation().SetEntries(allEntries); 

    // fill statistcal vector for Root
  std::vector<double> stat(11); 
  // sum weights 
  stat[0] =  sumBinHeights();  
  // sum weights **2
  stat[1] = 0; 
  if (eqBinEntries != 0) 
    stat[1] = (  sumBinHeights() * sumBinHeights() ) / eqBinEntries;
  // sum weights * x
  stat[2] = mean*sumBinHeights();
  // sum weight * x **2 
  stat[3] = ( mean*mean + rms*rms )* sumBinHeights(); 

  representation().PutStats(&stat.front()); 

  return true; 
}


