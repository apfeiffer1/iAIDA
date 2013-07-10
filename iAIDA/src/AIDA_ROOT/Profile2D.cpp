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

#include "AIDA_ROOT/Profile2D.h"

static std::string SpreadOpt = "s"; 

using AIDA_ROOT::Profile2D;

Profile2D::Profile2D(){
}

/*
Profile2D::Profile2D(const std::string & title, int binsX, double iminX, double imaxX, int binsY, double iminY, double imaxY, const std::string & options) { 
 initialize(title, binsX, iminX, imaxX, binsY, iminY, imaxY, options);
}

Profile2D::Profile2D(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, const std::string & options) {
  initialize(title, binEdgesX, binEdgesY, options);
}
*/


void Profile2D::initialize(const std::string & title,  int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, const std::string & /*options*/) {
  newRep(REP(title.c_str(),title.c_str(),nBinsX,lowerEdgeX,upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY));
  xAxis().initialize(representation().GetXaxis(),true);
  yAxis().initialize(representation().GetYaxis(),true);
  representation().SetErrorOption(SpreadOpt.c_str()); 
  m_sumEntries = 0;
}

void Profile2D::initialize(const std::string & title, const std::vector<double>  & binEdgesX,const std::vector<double>  & binEdgesY, const std::string & /*options*/) {
  // no corresponding constructor in root - use fixed bins 
  newRep(REP(title.c_str(),title.c_str(),binEdgesX.size()-1,binEdgesX.front(), binEdgesX.back(), binEdgesY.size()-1, binEdgesY.front(), binEdgesY.back() ) );
  xAxis().initialize(representation().GetXaxis(),false);
  yAxis().initialize(representation().GetYaxis(),false);
  representation().SetErrorOption(SpreadOpt.c_str()); 
  m_sumEntries = 0;
}


void Profile2D::initialize(const std::string & title,  int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, double lowerValue, double upperValue, const std::string & /*options*/) {
  newRep(REP(title.c_str(),title.c_str(),nBinsX,lowerEdgeX,upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, lowerValue, upperValue));
  xAxis().initialize(representation().GetXaxis(),true);
  yAxis().initialize(representation().GetYaxis(),true);
  representation().SetErrorOption(SpreadOpt.c_str()); 
  m_sumEntries = 0;
}


void Profile2D::initialize(const std::string & title, const std::vector<double>  & binEdgesX,const std::vector<double>  & binEdgesY, double lowerValue, double upperValue, const std::string & /*options*/) {
  newRep(REP(title.c_str(),title.c_str(),binEdgesX.size()-1,binEdgesX.front(), binEdgesX.back(), binEdgesY.size()-1, binEdgesY.front(), binEdgesY.back(), lowerValue, upperValue ) );
  xAxis().initialize(representation().GetXaxis(),false);
  yAxis().initialize(representation().GetYaxis(),false);
  representation().SetErrorOption(SpreadOpt.c_str()); 
  m_sumEntries = 0;
}



Profile2D::Profile2D(const Profile2D & rh) :
  super(rh) {
  xAxis().initialize(representation().GetXaxis(),rh.xAxis().isFixedBinning());
  yAxis().initialize(representation().GetYaxis(),rh.yAxis().isFixedBinning());
}

// constructor from AIDA_CPP used by GenFactory

Profile2D::Profile2D(const AIDA_CPP::IProfile2D & h) { 
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


Profile2D::Profile2D(const AIDA::IProfile2D & h){
  copyFromAida(h); 
}

Profile2D::~Profile2D(){}

Profile2D & Profile2D::operator  = ( const Profile2D & rh ) {
  newRep(rh.representation());
  xAxis().initialize(representation().GetXaxis(),rh.xAxis().isFixedBinning());
  yAxis().initialize(representation().GetYaxis(),rh.yAxis().isFixedBinning());
  return *this;

}

Profile2D & Profile2D::operator  = ( const AIDA::IProfile2D & h ) { 
  copyFromAida(h); 
  return *this;
}


void Profile2D::copyFromAida( const AIDA::IProfile2D & h ) { 

 // implement here the copy
  if (h.xAxis().isFixedBinning() && h.yAxis().isFixedBinning())
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
  double sumw2 = sumw; // no sumw2 is in AIDA (what is used for ? )
  double sumwx = h.meanX()*h.sumBinHeights(); 
  double sumwx2 = (h.meanX()*h.meanX() + h.rmsX()*h.rmsX() )*h.sumBinHeights();
  double sumwy = h.meanY()*h.sumBinHeights(); 
  double sumwy2 = (h.meanY()*h.meanY() + h.rmsY()*h.rmsY() )*h.sumBinHeights();
  double sumwxy = 0;
  double sumwz = 0; // for profile is total of the bin sum of weights*z
  double sumwz2 = 0;    // this is the total of the bin sum of weight*z*z 

  // copy the contents in  (AIDA underflow/overflow are -2,-1)

  for (int i=-2; i < xAxis().bins(); ++i) { 
    for (int j=-2; j < yAxis().bins(); ++j) { 
    // root binning starts from one !
      int rBin = representation().GetBin(rIndexX(i), rIndexY(j) ); 
      double totBinContent = h.binHeight(i,j)*h.binEntries(i,j);
      double sumw2Bin = ( h.binRms(i,j)*h.binRms(i,j) + h.binHeight(i,j)*h.binHeight(i,j) )*h.binEntries(i,j); 

      representation().SetBinContent(rBin, h.binHeight(i,j) ); 
      representation().SetBinError(rBin, totBinContent ); 
      representation().SetBinEntries(rBin,  std::sqrt(sumw2Bin) ); 
      // calculate statistics 
      if ( i >= 0 && j >= 0) { 
	sumwz  += totBinContent; 
	sumwz2 += sumw2Bin; 
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
  representation().PutStats(&stat[0]); 

}


// set bin content (bin centres are not used ) 
bool Profile2D::setBinContents( int i, int j, int entries,double height,double /*error */, double spread, double /*centreX*/, double /*centreY*/  ) { 
    int rBin = representation().GetBin(rIndexX(i), rIndexY(j) ); 
    representation().SetBinEntries(rBin, entries ); 
    // set content takes in root height * entries 
    representation().SetBinContent(rBin, height*entries );
    // set error takes std::sqrt of bin sum(w*y**2) 
    double sumwy2Bin = ( spread*spread + height*height )*entries; 
    representation().SetBinError(rBin, std::sqrt(sumwy2Bin) ); 

    m_sumEntries += entries;
    // not very efficient (but do evey bin since root cannot figure out by himself)
    representation().SetEntries(m_sumEntries);

    return true;
}

// need to overwrite reset to reset the sums 

bool  Profile2D::reset (  )   { 
  m_sumEntries = 0; 
  representation().Reset ( ); 
  return true; 
}
