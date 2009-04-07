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

 /**********************************************************************
  *                                                                    *
  * Copyright (c) 2002 Lorenzo Moneta, CERN/IT                       *
  *                   <Lorenzo.Moneta.cern.ch>                       *
  *                                                                    *
  * This library is free software; you can redistribute it and/or      *
  * modify it under the terms of the GNU Lesser General Public         *
  * License as published by the Free Software Foundation; either       *
  * version 2.1 of the License, or (at your option) any later version. *
  *                                                                    *
  * This library is distributed in the hope that it will be useful,    *
  * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU   *
  * Lesser General Public License for more details.                    *
  *                                                                    *
  * You should have received a copy of the GNU Lesser General Public   *
  * License along with this library (see file COPYING); if not, write  *
  * to the Free Software Foundation, Inc., 59 Temple Place, Suite      *
  * 330, Boston, MA 02111-1307 USA, or contact the author.             *
  *                                                                    *
  **********************************************************************/

// Implementation file for class Histo1DTranslator
// 
// Created by: moneta  17/06/2003
// 
// 
#include <iostream> 
#include <cmath>
#include <assert.h>

#include "AIDA_ROOT/HistoConverter.h"

#include "AIDA/IHistogram1D.h"
#include "AIDA/IHistogram2D.h"
#include "AIDA/IHistogram3D.h"
#include "AIDA_Dev/IDevHistogram1D.h"
#include "AIDA_Dev/IDevHistogram2D.h"
#include "AIDA_Dev/IDevHistogram3D.h"
#include "AIDA_Dev/IDevProfile1D.h"
#include "AIDA_Dev/IDevProfile2D.h"
#include "AIDA_Dev/IDevHistogramFactory.h"
#include "AIDA/IAxis.h"
#include "AIDA_RootConverterHelper.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TProfile.h"
#include "TProfile2D.h"


static const std::string emptyString = "";


iAIDA::AIDA_RootConverter::HistoConverter::HistoConverter(const iAIDA::AIDA_RootConverter::HistoConverter &)  
{
}

iAIDA::AIDA_RootConverter::HistoConverter & iAIDA::AIDA_RootConverter::HistoConverter::operator = (const iAIDA::AIDA_RootConverter::HistoConverter &rhs) 
{
   if (this == &rhs) return *this;  // time saving self-test

   return *this;
}



// implementation of copy to Root 

// 1D histo (T.B.D. : template on the TH1 type ? ) 


TH1D *  iAIDA::AIDA_RootConverter::HistoConverter::copyToRoot(const AIDA::IHistogram1D & h, const std::string & name) 
{ 
  TH1D * th = 0;
  if (h.axis().isFixedBinning() ) { 
    th = new TH1D(name.c_str(),h.title().c_str(), h.axis().bins(), h.axis().lowerEdge(), h.axis().upperEdge() ); 
    
  }
  else { 
    std::vector<double> binEdges; 
    getBinEdges(h.axis(), binEdges); 
    th = new TH1D(name.c_str(),h.title().c_str(), binEdges.size()-1,&binEdges.front() );
  }

  // copy the contents in 
  for (int i=0; i < h.axis().bins() + 2; ++i) { 
    int iAida = AIDA_RootConverter_Helper::indexAida(i,h.axis().bins()); 
    // root binning starts from one !
    th->SetBinContent(i,h.binHeight(iAida) ); 
    th->SetBinError(i,h.binError(iAida) ); 
  }

  // copy statistics 
  th->SetEntries(h.allEntries());

  double sumw = h.sumBinHeights();    
  double sumw2 = 0; 
  if (h.equivalentBinEntries() != 0) 
    sumw2 = ( sumw * sumw ) /h.equivalentBinEntries();
  double sumwx = h.mean()*h.sumBinHeights(); 
  double sumwx2 = (h.mean()*h.mean() + h.rms()*h.rms() )*h.sumBinHeights();
  std::vector<double> stat(11); 
  stat[0] = sumw; 
  stat[1] = sumw2; 
  stat[2] = sumwx; 
  stat[3] = sumwx2; 
  th->PutStats(&stat.front()); 

  return th; 

}

// 2D histo

TH2D *  iAIDA::AIDA_RootConverter::HistoConverter::copyToRoot(const AIDA::IHistogram2D & h, const std::string & name) 
{ 
  TH2D * th = 0;
  if (h.xAxis().isFixedBinning() &&  h.yAxis().isFixedBinning() ) { 
    th = new TH2D(name.c_str(),h.title().c_str(), 
		h.xAxis().bins(), h.xAxis().lowerEdge(), h.xAxis().upperEdge(),
		h.yAxis().bins(), h.yAxis().lowerEdge(), h.yAxis().upperEdge() ); 
    
  }
  else { 
    std::vector<double> binEdgesX; 
    getBinEdges(h.xAxis(), binEdgesX); 
    std::vector<double> binEdgesY;  
    getBinEdges(h.yAxis(), binEdgesY); 
    th = new TH2D(name.c_str(),h.title().c_str(), binEdgesX.size()-1,&binEdgesX.front(), binEdgesY.size()-1,&binEdgesY.front() );
  }

  double sumwxy = 0;
  // copy the contents in 
  for (int i=0; i < h.xAxis().bins() + 2; ++i) { 
    // root binning starts from one !
    int iAida = AIDA_RootConverter_Helper::indexAida(i,h.xAxis().bins()); 
    for (int j=0; j < h.yAxis().bins() + 2; ++j) { 
      int jAida = AIDA_RootConverter_Helper::indexAida(j,h.yAxis().bins()); 
      th->SetBinContent(i,j,h.binHeight(iAida,jAida) ); 
      th->SetBinError(i,j,h.binError(iAida,jAida) ); 
      // calculate also sum of weight * x * y ( approximation using bin mean)
      // (underflow and overflow are excluded)
      if ( iAida >= 0 && jAida >= 0)  
	sumwxy += h.binHeight(iAida,jAida)*h.binMeanX(iAida,jAida)*h.binMeanY(iAida,jAida); 
      

    }
  }

  th->SetEntries(h.allEntries());
  // copy statistics 
    double sumw = h.sumBinHeights();    
  double sumw2 = 0; 
  if (h.equivalentBinEntries() != 0) 
    sumw2 = ( sumw * sumw ) /h.equivalentBinEntries();
  double sumwx = h.meanX()*h.sumBinHeights(); 
  double sumwx2 = (h.meanX()*h.meanX() + h.rmsX()*h.rmsX() )*h.sumBinHeights();
  double sumwy = h.meanY()*h.sumBinHeights(); 
  double sumwy2 = (h.meanY()*h.meanY() + h.rmsY()*h.rmsY() )*h.sumBinHeights();


  // fill stat vector  
  std::vector<double> stat(11); 
  stat[0] = sumw; 
  stat[1] = sumw2; 
  stat[2] = sumwx; 
  stat[3] = sumwx2; 
  stat[4] = sumwy; 
  stat[5] = sumwy2; 
  stat[6] = sumwxy; 
  th->PutStats(&stat.front()); 

  return th; 
}


// 3D histo

TH3D *  iAIDA::AIDA_RootConverter::HistoConverter::copyToRoot(const AIDA::IHistogram3D & h, const std::string & name) 
{ 
  TH3D * th = 0;
  if (h.xAxis().isFixedBinning() &&  h.yAxis().isFixedBinning() ) { 
    th = new TH3D(name.c_str(),h.title().c_str(), 
		h.xAxis().bins(), h.xAxis().lowerEdge(), h.xAxis().upperEdge(),
		h.yAxis().bins(), h.yAxis().lowerEdge(), h.yAxis().upperEdge(),
		h.zAxis().bins(), h.zAxis().lowerEdge(), h.zAxis().upperEdge() ); 
    
  }
  else { 
    std::vector<double> binEdgesX; 
    getBinEdges(h.xAxis(), binEdgesX); 
    std::vector<double> binEdgesY; 
    getBinEdges(h.yAxis(), binEdgesY); 
    std::vector<double> binEdgesZ;  
    getBinEdges(h.zAxis(), binEdgesZ); 
    th = new TH3D(name.c_str(),h.title().c_str(), 
		  binEdgesX.size()-1,&binEdgesX.front(), 
		  binEdgesY.size()-1,&binEdgesY.front(),  
		  binEdgesZ.size()-1,&binEdgesZ.front() );
  }

  // calculate also mixed statistics for correlations
  double sumwxy = 0;
  double sumwxz = 0;
  double sumwyz = 0;
  // copy the contents in 
  for (int i=0; i < h.xAxis().bins() + 2; ++i) { 
    // root binning starts from one !
    int iAida = AIDA_RootConverter_Helper::indexAida(i,h.xAxis().bins()); 
    for (int j=0; j < h.yAxis().bins() + 2; ++j) { 
      int jAida = AIDA_RootConverter_Helper::indexAida(j,h.yAxis().bins()); 
      for (int k=0; k < h.zAxis().bins() + 2; ++k) { 
	int kAida = AIDA_RootConverter_Helper::indexAida(k,h.zAxis().bins()); 
	th->SetBinContent(i,j,k, h.binHeight(iAida,jAida,kAida) ); 
	th->SetBinError(i,j,k, h.binError(iAida,jAida, kAida) ); 
	// calculate statistics 
	if ( iAida >= 0 && jAida >= 0 && kAida >= 0) { 
	  sumwxy += h.binHeight(iAida,jAida,kAida)*h.binMeanX(iAida,jAida,kAida)*h.binMeanY(iAida,jAida,kAida); 
	  sumwxz += h.binHeight(iAida,jAida,kAida)*h.binMeanX(iAida,jAida,kAida)*h.binMeanZ(iAida,jAida,kAida); 
	  sumwyz += h.binHeight(iAida,jAida,kAida)*h.binMeanY(iAida,jAida,kAida)*h.binMeanZ(iAida,jAida,kAida); 
	}
 
      }
    }
  }
  th->SetEntries(h.allEntries()); 

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
  stat[10]= sumwyz; 
  th->PutStats(&stat.front()); 

  return th; 

}


// 1D Profile

TProfile *  iAIDA::AIDA_RootConverter::HistoConverter::copyToRoot(const AIDA::IProfile1D & h, const std::string & name) 
{ 
  TProfile * th = 0;
  if (h.axis().isFixedBinning() ) { 
    th = new TProfile(name.c_str(),h.title().c_str(), h.axis().bins(), h.axis().lowerEdge(), h.axis().upperEdge() ); 
    
  }
  else { 
    std::vector<double> binEdges; 
    getBinEdges(h.axis(), binEdges); 
    th = new TProfile(name.c_str(),h.title().c_str(), binEdges.size()-1,&binEdges.front() );
  }

  // copy the contents in 
  // for profile setters in Root are bizarre: 
  // SetContent : set height*entries
  // SetError : set std::sqrt(sumw2) 
  double sumwy = 0;  // for profile is total of the bin sum of weights*y
  double sumwy2 = 0;     // this is the total of the bin sum of weight*y*y     

  for (int i=0; i < h.axis().bins() + 2; ++i) { 
    int iAida = AIDA_RootConverter_Helper::indexAida(i,h.axis().bins()); 
    
    double sumwyBin = h.binHeight(iAida)*h.binEntries(iAida);
    double sumwy2Bin = ( h.binRms(iAida)*h.binRms(iAida) + h.binHeight(iAida)*h.binHeight(iAida) )*h.binEntries(iAida);
    th->SetBinContent(i, sumwyBin ); 
    th->SetBinError(i,  std::sqrt(sumwy2Bin) ); 
    th->SetBinEntries(i, h.binEntries(iAida) ); 
    // accumulate the sums for statistics 
    sumwy  += sumwyBin; 
    sumwy2 += sumwy2Bin; 
  }

  th->SetEntries(h.allEntries()); 

  // set statistics 


  double sumw = h.sumBinHeights();    
  double sumw2 = sumw; // no sumw2 is in AIDA (what is used for ? )
  double sumwx = h.mean()*h.sumBinHeights(); 
  double sumwx2 = (h.mean()*h.mean() + h.rms()*h.rms() )*h.sumBinHeights();
  //double sumwy = 0;  // for profile is total of the bin sum of weights*y
  //double sumwy2 = 0;     // this is the total of the bin sum of weight*y*y     

  std::vector<double> stat(11);   
  stat[0] = sumw; 
  stat[1] = sumw2; 
  stat[2] = sumwx; 
  stat[3] = sumwx2; 
  stat[4] = sumwy; 
  stat[5] = sumwy2; 

  th->PutStats(&stat.front()); 

  return th; 
}

// 2D Profile

TProfile2D *  iAIDA::AIDA_RootConverter::HistoConverter::copyToRoot(const AIDA::IProfile2D & h, const std::string & name) 
{ 
  TProfile2D * th = 0;
  if (h.xAxis().isFixedBinning() &&  h.yAxis().isFixedBinning() ) { 
    th = new TProfile2D(name.c_str(),h.title().c_str(), 
		h.xAxis().bins(), h.xAxis().lowerEdge(), h.xAxis().upperEdge(),
		h.yAxis().bins(), h.yAxis().lowerEdge(), h.yAxis().upperEdge() ); 
    
  }
  else { 
    std::cerr << "Warning: variable bin profile2d not supported in ROOT - convert to a fix profile" << std::endl; 
    std::vector<double> binEdgesX; 
    getBinEdges(h.xAxis(), binEdgesX); 
    std::vector<double> binEdgesY;  
    getBinEdges(h.yAxis(), binEdgesY); 
    th = new TProfile2D(name.c_str(),h.title().c_str(), 
			binEdgesX.size()-1,binEdgesX.front(), binEdgesX.back(), 
			binEdgesY.size()-1,binEdgesY.front(), binEdgesX.back() );
  }


  double sumwz = 0; 
  double sumwz2 = 0; 
  double sumwxy = 0;
  // copy the contents in 
  for (int i=0; i < h.xAxis().bins() + 2; ++i) { 
    // root binning starts from one !
    int iAida = AIDA_RootConverter_Helper::indexAida(i,h.xAxis().bins()); 
    for (int j=0; j < h.yAxis().bins() + 2; ++j) { 
      int jAida = AIDA_RootConverter_Helper::indexAida(j,h.yAxis().bins()); 
      double totBinContent = h.binHeight(iAida,jAida)*h.binEntries(iAida,jAida);
      double sumw2Bin = ( h.binRms(iAida,jAida)*h.binRms(iAida,jAida) + h.binHeight(iAida,jAida)*h.binHeight(iAida,jAida) )*h.binEntries(iAida,jAida); 

      th->SetBinContent(i ,j,totBinContent ); 
      th->SetBinError(i, j, std::sqrt(sumw2Bin) ); 
      int iBin = th->GetBin(i,j); 
      th->SetBinEntries(iBin,h.binEntries(iAida,jAida) ); 
      if (iAida >=0 && jAida >= 0) { 
	sumwz  += totBinContent; 
	sumwz2 += sumw2Bin; 
      // calculate also sum of weight * x * y ( approximation using bin mean)
      // (underflow and overflow are excluded)
	sumwxy += h.binHeight(iAida,jAida)*h.binMeanX(iAida,jAida)*h.binMeanY(iAida,jAida); 
      }

    }
  }

  th->SetEntries(h.allEntries());
  // copy statistics 
  double sumw = h.sumBinHeights();    
  double sumw2 = sumw; // no sumw2 is in AIDA (what is used for ? )
  double sumwx = h.meanX()*h.sumBinHeights(); 
  double sumwx2 = (h.meanX()*h.meanX() + h.rmsX()*h.rmsX() )*h.sumBinHeights();
  double sumwy = h.meanY()*h.sumBinHeights(); 
  double sumwy2 = (h.meanY()*h.meanY() + h.rmsY()*h.rmsY() )*h.sumBinHeights();

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
  th->PutStats(&stat[0]); 

  return th; 
}


AIDA::Dev::IDevHistogram1D * iAIDA::AIDA_RootConverter::HistoConverter::copyToAida(AIDA::Dev::IDevHistogramFactory& factory, const TH1 &  th )
{ 

  // read header 
  std::string title = th.GetTitle(); 
  

  // read axis
  int numberOfBinsX = th.GetNbinsX();

  
  TAxis * axis = th.GetXaxis(); 
  bool eqBinSize = isEqBinSize(axis); 

  // Now create the histogram 

  AIDA::Dev::IDevHistogram1D * h = 0; 
  if (eqBinSize) {  
    // equal bins histograms
    double lowerEdge = (th.GetXaxis() )->GetXmin(); 
    double upperEdge = (th.GetXaxis() )->GetXmax(); 
    h = factory.createHistogram1D( title, numberOfBinsX, lowerEdge, upperEdge );
  }
  else { 
    // construct edges vector
    std::vector<double> edgesX;
    getBinEdges(*axis, edgesX);
    // variable bins histograms 
    h = factory.createHistogram1D( title, edgesX, emptyString );
  }
  // set the data in the AIDA histogram 
 
   for (int iRBin = 0; iRBin < numberOfBinsX + 2; ++iRBin) { 
     int iABin = AIDA_RootConverter_Helper::indexAida(iRBin, numberOfBinsX); 

     double wMean = th.GetBinCenter(iRBin); 
     double height = th.GetBinContent(iRBin); 
     double error = th.GetBinError(iRBin); 
     int entries = AIDA_RootConverter_Helper::getEntries(height,error); 
     h->setBinContents(iABin, entries, height, error, wMean);
   }

   // set also rms
   h->setRms(th.GetRMS() ); 
   // I would like to set the mean also if coming from different impl. 

   return h; 
}

// 2D histogram 



AIDA::Dev::IDevHistogram2D * iAIDA::AIDA_RootConverter::HistoConverter::copyToAida(AIDA::Dev::IDevHistogramFactory& factory, const TH2 &  th ) 
{

  std::string title = th.GetTitle(); 
  // read axis
  int numberOfBinsX = th.GetNbinsX();
  int numberOfBinsY = th.GetNbinsY();
  
  TAxis * xAxis = th.GetXaxis(); 
  TAxis * yAxis = th.GetYaxis(); 

  bool eqBinSizeX = isEqBinSize(xAxis); 
  bool eqBinSizeY = isEqBinSize(yAxis);   

  // Now create the histogram 

  AIDA::Dev::IDevHistogram2D * h = 0; 
  if (eqBinSizeX && eqBinSizeY) {  
    // equal bins histograms

    h = factory.createHistogram2D( title, numberOfBinsX, xAxis->GetXmin(), xAxis->GetXmax(),
				   numberOfBinsY,  yAxis->GetXmin(), yAxis->GetXmax()  );
  }
  else { 
    // variable bins histograms 
  // construct edges vector
    std::vector<double> edgesX;
    std::vector<double> edgesY;
    getBinEdges( *xAxis, edgesX);
    getBinEdges( *yAxis, edgesY);
    h = factory.createHistogram2D( title, edgesX, edgesY, emptyString );
  }

  // set the data in the AIDA histogram 
 
   for (int iRBin = 0; iRBin < numberOfBinsX + 2; ++iRBin) { 
     int iABin = AIDA_RootConverter_Helper::indexAida(iRBin, numberOfBinsX); 

     for (int jRBin = 0; jRBin < numberOfBinsY + 2; ++jRBin) { 
       int jABin = AIDA_RootConverter_Helper::indexAida(jRBin, numberOfBinsY); 

       double wMeanX = xAxis->GetBinCenter(iRBin); 
       double wMeanY = yAxis->GetBinCenter(jRBin); 
       double height = th.GetBinContent(iRBin,jRBin); 
       double error = th.GetBinError(iRBin,jRBin); 
       int entries = AIDA_RootConverter_Helper::getEntries(height,error); 
       h->setBinContents(iABin, jABin, entries, height, error, wMeanX, wMeanY);
     }
   }

   // set also rms
   h->setRms(th.GetRMS(1), th.GetRMS(2) ); 
   // I would like to set the mean also if coming from different impl. 

   return h; 
}


// 3D histogram 

AIDA::Dev::IDevHistogram3D * iAIDA::AIDA_RootConverter::HistoConverter::copyToAida(AIDA::Dev::IDevHistogramFactory& factory, const TH3 &  th ) 
{
  std::string title = th.GetTitle(); 
  // read axis
  int numberOfBinsX = th.GetNbinsX();
  int numberOfBinsY = th.GetNbinsY();
  int numberOfBinsZ = th.GetNbinsZ();
  
  TAxis * xAxis = th.GetXaxis(); 
  TAxis * yAxis = th.GetYaxis(); 
  TAxis * zAxis = th.GetZaxis(); 

  bool eqBinSizeX = isEqBinSize(xAxis); 
  bool eqBinSizeY = isEqBinSize(yAxis);   
  bool eqBinSizeZ = isEqBinSize(zAxis);   

  // Now create the histogram 

  AIDA::Dev::IDevHistogram3D * h = 0; 
  if (eqBinSizeX && eqBinSizeY && eqBinSizeZ ) {  
    // equal bins histograms
    h = factory.createHistogram3D( title, numberOfBinsX, xAxis->GetXmin(), xAxis->GetXmax(),   
				   numberOfBinsY,  yAxis->GetXmin(), yAxis->GetXmax(), 				   
				   numberOfBinsZ,  zAxis->GetXmin(), zAxis->GetXmax() );
  }
  else {
    // variable bins histograms 
    // construct edges vector
    std::vector<double> edgesX;
    std::vector<double> edgesY;
    std::vector<double> edgesZ;
    getBinEdges( *xAxis, edgesX);
    getBinEdges( *yAxis, edgesY);
    getBinEdges( *zAxis, edgesZ);
    
    h = factory.createHistogram3D( title, edgesX, edgesY, edgesZ, emptyString );
  }
  // set the data in the AIDA histogram 
 
   for (int iRBin = 0; iRBin < numberOfBinsX + 2; ++iRBin) { 
     int iABin = AIDA_RootConverter_Helper::indexAida(iRBin, numberOfBinsX); 
     double wMeanX = xAxis->GetBinCenter(iRBin); 
	 
     for (int jRBin = 0; jRBin < numberOfBinsY + 2; ++jRBin) { 
       int jABin = AIDA_RootConverter_Helper::indexAida(jRBin, numberOfBinsY); 
       double wMeanY = yAxis->GetBinCenter(jRBin); 
	 
       for (int kRBin = 0; kRBin < numberOfBinsZ + 2; ++kRBin) { 
	 int kABin = AIDA_RootConverter_Helper::indexAida(kRBin, numberOfBinsZ); 

	 double wMeanZ = zAxis->GetBinCenter(kRBin); 
	 double height = th.GetBinContent(iRBin,jRBin,kRBin); 
	 double error = th.GetBinError(iRBin,jRBin, kRBin); 
	 int entries = AIDA_RootConverter_Helper::getEntries(height,error); 
	 h->setBinContents(iABin, jABin, kABin, entries, height, error, wMeanX, wMeanY, wMeanZ);
       }
     }
   }

   // set also rms
   h->setRms(th.GetRMS(1), th.GetRMS(2), th.GetRMS(3) ); 
   // I would like to set the mean also if coming from different impl. 

   return h; 
}

// profile 1D 

AIDA::Dev::IDevProfile1D * iAIDA::AIDA_RootConverter::HistoConverter::copyToAida(AIDA::Dev::IDevHistogramFactory& factory, const TProfile &  tprof ) 
{

  // read header 
  std::string title = tprof.GetTitle(); 
  

  // read axis
  int numberOfBinsX = tprof.GetNbinsX();

  
  TAxis * axis = tprof.GetXaxis(); 
  bool eqBinSize = isEqBinSize(axis); 

  // Now create the histogram 

  AIDA::Dev::IDevProfile1D * p = 0; 
  if (eqBinSize) {  
    // equal bins histograms
    double lowerEdge = (tprof.GetXaxis() )->GetXmin(); 
    double upperEdge = (tprof.GetXaxis() )->GetXmax(); 
    p = factory.createProfile1D( title, numberOfBinsX, lowerEdge, upperEdge );
  }
  else { 
    // variable bins histograms 
    // construct edges vector
    std::vector<double> edgesX;
    getBinEdges( *axis, edgesX);
    p = factory.createProfile1D( title, edgesX, emptyString );
  }
  // set the data in the AIDA histogram 
 
   for (int iRBin = 0; iRBin < numberOfBinsX + 2; ++iRBin) { 
     int iABin = AIDA_RootConverter_Helper::indexAida(iRBin, numberOfBinsX); 

     double wMean = tprof.GetBinCenter(iRBin); 
     double height = tprof.GetBinContent(iRBin); 
     double error = tprof.GetBinError(iRBin); 
     // suppose GetBinEntries(iRBin) is sum of weights 
     double spread = error*std::sqrt(tprof.GetBinEntries(iRBin) );
     //double spread = error;   // no possibility to have both in root 
     // to fix : check with error option
     int entries =  static_cast<int> ( tprof.GetBinEntries(iRBin) );
     //std::cout << "P1D conv : " << entries << "  " 
     //	       << height << "  " << error << "  " 
     //	       << wMean << std::endl; 
     p->setBinContents(iABin, entries, height, error, spread, wMean);
   }


   return p; 
}

// 2D profiles



AIDA::Dev::IDevProfile2D * iAIDA::AIDA_RootConverter::HistoConverter::copyToAida(AIDA::Dev::IDevHistogramFactory& factory, const TProfile2D &  tprof )
{
 

  std::string title = tprof.GetTitle(); 
  // read axis
  int numberOfBinsX = tprof.GetNbinsX();
  int numberOfBinsY = tprof.GetNbinsY();
  
  TAxis * xAxis = tprof.GetXaxis(); 
  TAxis * yAxis = tprof.GetYaxis(); 

  bool eqBinSizeX = isEqBinSize(xAxis); 
  bool eqBinSizeY = isEqBinSize(yAxis);   

  // Now create the histogram 

  AIDA::Dev::IDevProfile2D * p = 0; 
  if (eqBinSizeX && eqBinSizeY) {  
    // equal bins histograms
    p = factory.createProfile2D( title, numberOfBinsX,  xAxis->GetXmin(), xAxis->GetXmax(),
				 numberOfBinsY,  yAxis->GetXmin(), yAxis->GetXmax()  ); 

  }
  else { 
    // variable bins histograms 
    // construct edges vector
    std::vector<double> edgesX;
    std::vector<double> edgesY;
    getBinEdges( *xAxis, edgesX);
    getBinEdges( *yAxis, edgesY);
    p = factory.createProfile2D( title, edgesX, edgesY, emptyString );
  }
  // set the data in the AIDA histogram 
 
   for (int iRBin = 0; iRBin < numberOfBinsX + 2; ++iRBin) { 
     int iABin = AIDA_RootConverter_Helper::indexAida(iRBin, numberOfBinsX); 

     for (int jRBin = 0; jRBin < numberOfBinsY + 2; ++jRBin) { 
       int jABin = AIDA_RootConverter_Helper::indexAida(jRBin, numberOfBinsY); 

       double wMeanX = xAxis->GetBinCenter(iRBin); 
       double wMeanY = yAxis->GetBinCenter(jRBin); 
       double height = tprof.GetBinContent(iRBin,jRBin); 
       double error = tprof.GetBinError(iRBin,jRBin);
       //double spread = error; 
       // suppose GetBinEntries(iRBin) is sum of weights 
       double sumOfBinWeights =  tprof.GetBinEntries  (tprof.GetBin(iRBin,jRBin) );
       double spread = error*std::sqrt( sumOfBinWeights );
       int entries = static_cast<int> ( sumOfBinWeights );
       p->setBinContents(iABin, jABin, entries, height, error, spread, wMeanX, wMeanY);
     }
   }


   return p; 
}




// helper functions

// get bin edges vectors from AIDA histogram (axis)

void iAIDA::AIDA_RootConverter::HistoConverter::getBinEdges(const AIDA::IAxis & axis, std::vector<double> & binEdges) { 

  binEdges.resize( axis.bins() + 1 );
  for (int i =0; i < axis.bins(); ++i) 
    binEdges[i] = axis.binLowerEdge(i); 

  // add also upperedges at the end
  binEdges[axis.bins()] = axis.upperEdge();
  return;
}

void iAIDA::AIDA_RootConverter::HistoConverter::getBinEdges(const TAxis & axis, std::vector<double> & binEdges) { 

  int nbin =  axis.GetNbins(); 
  binEdges.resize( nbin + 1 );
  for (int i =0; i < nbin; ++i) { 
    int iRoot = i + 1;
    binEdges[i] = axis.GetBinLowEdge(iRoot); 
  }
  // add also upperedges at the end
  binEdges[ nbin ] = axis.GetBinUpEdge( nbin );
  return;
}

bool iAIDA::AIDA_RootConverter::HistoConverter::isEqBinSize(const  TAxis * axis) 
{ 
  // do as Root does to check if variable bins 
  assert(axis);
  const TArrayD * rootEdges = axis->GetXbins(); 
  if (rootEdges == 0) return true; 
  return  ( rootEdges->fN == 0 );
  /* 
  bool eqBinSize = true;    
  int numberOfBins =  axis->GetNbins();
  // loop on bins (root starts from 1) 
  for (int jBin = 1; jBin < numberOfBins+1; ++jBin) { 
    if (jBin > 1) 
      if (eqBinSize && axis->GetBinWidth(jBin) != axis->GetBinWidth(jBin-1) ) 
	{  
	  // I whould put a tolerance here 
	  eqBinSize = false; 
	  //	  std::cout << axis->GetBinWidth(jBin) << "  -  " << axis->GetBinWidth(jBin-1) << std::endl; 
	}

    edges.push_back(axis->GetBinLowEdge(jBin) ); 
  }
  // add also upper edge at the end
  edges.push_back( axis->GetBinUpEdge(numberOfBins) ); 

  return eqBinSize; 
  */
}
