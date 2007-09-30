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

#include "AIDA_ROOT/Profile1D.h"

static std::string SpreadOpt = "s";
using AIDA_ROOT::Profile1D;

Profile1D::Profile1D(){
}


Profile1D::Profile1D(const std::string & title, int bins, double imin, double imax, const std::string & options) {
  initialize(title, bins, imin, imax, options);
}

Profile1D::Profile1D(const std::string & title, const std::vector<double>  & binEdges, const std::string & options) {
  initialize(title, binEdges, options);
}


void Profile1D::initialize(const std::string & title, int nBins, double lowerEdge, double upperEdge, const std::string & /*options*/) {
  newRep(REP(title.c_str(),title.c_str(),nBins,lowerEdge,upperEdge));
  axis().initialize(representation().GetXaxis(),true);
  // set error option to spread 
  representation().SetErrorOption(SpreadOpt.c_str()); 
  m_sumEntries = 0;
}

void Profile1D::initialize(const std::string & title, const std::vector<double>  & binEdges, const std::string & /*options*/) {
  newRep(REP(title.c_str(),title.c_str(),binEdges.size()-1,&binEdges.front()));
  axis().initialize(representation().GetXaxis(),false);
  representation().SetErrorOption(SpreadOpt.c_str()); 
  m_sumEntries = 0;
}

void Profile1D::initialize(const std::string & title, int nBins, double lowerEdge, double upperEdge,
			   double lowerValue, double upperValue,
			   const std::string & /*options*/) {
  newRep(REP(title.c_str(),title.c_str(),nBins,lowerEdge,upperEdge,lowerValue, upperValue));
  axis().initialize(representation().GetXaxis(),true);
  representation().SetErrorOption(SpreadOpt.c_str()); 
  m_sumEntries = 0;
}

void Profile1D::initialize(const std::string & title, const std::vector<double>  & binEdges, 
			   double lowerValue, double upperValue,
			   const std::string & /*options*/) {
  newRep(REP(title.c_str(),title.c_str(),binEdges.size()-1,&binEdges.front(),lowerValue, upperValue));
  axis().initialize(representation().GetXaxis(),false);
  representation().SetErrorOption(SpreadOpt.c_str()); 
  m_sumEntries = 0;
}



Profile1D::Profile1D(const Profile1D & rh) :
  super(rh) {
  axis().initialize(representation().GetXaxis(),rh.axis().isFixedBinning());
}

// constructor from AIDA_CPP used by GenFactory

Profile1D::Profile1D(const AIDA_CPP::IProfile1D & h) { 
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

Profile1D::Profile1D(const AIDA::IProfile1D & h){

  copyFromAida(h); 
}

Profile1D::~Profile1D(){}

Profile1D & Profile1D::operator  = ( const Profile1D & rh ) {
  newRep(rh.representation());
  axis().initialize(representation().GetXaxis(),rh.axis().isFixedBinning());
  return *this;

}

Profile1D & Profile1D::operator  = ( const AIDA::IProfile1D & h ) { 
  copyFromAida(h); 
  return *this;
}


void Profile1D::copyFromAida( const AIDA::IProfile1D & h ) { 

 // implement here the copy
  if (h.axis().isFixedBinning() )
    initialize(h.title(), h.axis().bins(), h.axis().lowerEdge(), h.axis().upperEdge() ); 
  else { 
    std::vector<double> binEdges; (h.axis().bins()); 
    for (int i =0; i < h.axis().bins(); ++i) 
      binEdges.push_back(h.axis().binLowerEdge(i)); 
    // add also upperedges at the end
    binEdges.push_back(h.axis().upperEdge() ); 
    initialize(h.title(), binEdges); 
  }

  // statistics
  double sumw = h.sumBinHeights();    
  double sumw2 = sumw; // no sumw2 is in AIDA (what is used for ? )
  double sumwx = h.mean()*h.sumBinHeights(); 
  double sumwx2 = (h.mean()*h.mean() + h.rms()*h.rms() )*h.sumBinHeights();
  double sumwy = 0;  // for profile is total of the bin sum of weights*y
  double sumwy2 = 0;     // this is the total of the bin sum of weight*y*y     

  // copy the contents in 
  for (int iA=-2; iA < axis().bins(); ++iA) { 
    int i = rIndex(iA); 
     // content for profile is height*entries and error is std::sqrt(sumw2) 
    double sumwyBin = h.binHeight(iA)*h.binEntries(iA); 
    double sumwy2Bin = ( h.binRms(iA)*h.binRms(iA) + h.binHeight(iA)*h.binHeight(iA) )*h.binEntries(iA); 
    representation().SetBinEntries(i,h.binEntries(iA) ); 
    representation().SetBinContent(i, sumwyBin ); 
    representation().SetBinError(i, std::sqrt(sumwy2Bin) ); 
    // calculate statistics 
    if ( i >= 0 ) { 
      sumwy  += sumwyBin; 
      sumwy2 += sumwy2Bin; 
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

}


// set bin content (centre and error are not used ) 
bool Profile1D::setBinContents( int i, int entries,double height,double /*error*/, double spread, double /* centre */ ) { 
    representation().SetBinEntries(rIndex(i), entries ); 
    // set content takes in root height * entries 
    representation().SetBinContent(rIndex(i), height*entries );
    // set error takes std::sqrt of bin sum(w*y**2) 
    double sumwy2Bin = ( spread*spread + height*height )*entries; 
    representation().SetBinError(rIndex(i), std::sqrt(sumwy2Bin) ); 
    
    m_sumEntries += entries;
    // not very efficient (but do evey bin since root cannot figure out by himself)
    representation().SetEntries(m_sumEntries);

    return true;
}

// need to overwrite reset to reset the sums 

bool  Profile1D::reset (  )   { 
  m_sumEntries = 0; 
  representation().Reset ( ); 
  return true; 
}

