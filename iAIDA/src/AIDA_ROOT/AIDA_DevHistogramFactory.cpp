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

//  This file is part of the AIDA library
//  Copyright (C) 2002 by the AIDA team.  All rights reserved.
//  This library is free software and under the terms of the
//  GNU Library General Public License described in the LGPL.txt 

#include <string>
#include <vector>
#include "AIDA_ROOT/HistogramFactory.h"
#include "AIDA_ROOT/AIDA_DevHistogramFactory.h"


static const std::string emptyTitle = ""; 

namespace AIDA_ROOT {


/**
 * User level interface for factory classes of Histograms (binned, unbinned, and profile)
 * The created objects are assumed to be managed by the tree which is associated to the factory.
 *
 * @author The AIDA team (http://aida.freehep.org/)
 *
 */
 

  AIDA_DevHistogramFactory::AIDA_DevHistogramFactory( )    {}


    /// Destructor.
  AIDA_DevHistogramFactory::~AIDA_DevHistogramFactory() { /* nop */; }

  bool AIDA_DevHistogramFactory::destroy(AIDA::IBaseHistogram * hist) {delete hist;  return true;}

  AIDA::Dev::IDevCloud1D * AIDA_DevHistogramFactory::createCloud1D( const std::string & , int , const std::string &  ) { return 0;}

  AIDA::Dev::IDevCloud1D * AIDA_DevHistogramFactory::createCloud1D( ) {return 0;}
  
  AIDA::Dev::IDevCloud1D * AIDA_DevHistogramFactory::createCopy( const AIDA::ICloud1D & ) {return 0;}
  
  AIDA::Dev::IDevCloud2D * AIDA_DevHistogramFactory::createCloud2D( const std::string & , int , const std::string & ) {return 0;}
  
  AIDA::Dev::IDevCloud2D * AIDA_DevHistogramFactory::createCloud2D( ) { return 0;}
  
  AIDA::Dev::IDevCloud2D * AIDA_DevHistogramFactory::createCopy( const AIDA::ICloud2D & ) { return 0;}
  
  AIDA::Dev::IDevCloud3D * AIDA_DevHistogramFactory::createCloud3D( const std::string & , int , const std::string &  ) { return 0;}
  
  AIDA::Dev::IDevCloud3D * AIDA_DevHistogramFactory::createCloud3D( ) { return 0;}
  
  AIDA::Dev::IDevCloud3D * AIDA_DevHistogramFactory::createCopy( const AIDA::ICloud3D & ) { return 0;}
  

  AIDA::Dev::IDevHistogram1D * AIDA_DevHistogramFactory::createHistogram1D(const std::string & title, int nBins, double lowerEdge, double upperEdge, const std::string & options ) {
    AIDA_CPP::IHistogram1D * h = factory.create<AIDA_CPP::IHistogram1D>();
    //manage< AIDA_ROOT::Histogram1D>(h); 
    h->initialize(title, nBins, lowerEdge, upperEdge, options); 
    return h;
  }
  
  AIDA::Dev::IDevHistogram1D * AIDA_DevHistogramFactory::createHistogram1D(int nBins, double lowerEdge, double upperEdge) { return createHistogram1D(emptyTitle, nBins, lowerEdge, upperEdge);}
  
  AIDA::Dev::IDevHistogram1D * AIDA_DevHistogramFactory::createHistogram1D(const std::string & title, const std::vector<double>  & binEdges, const std::string & options ) {
    AIDA_CPP::IHistogram1D * h = factory.create<AIDA_CPP::IHistogram1D>();
    //manage< AIDA_ROOT::Histogram1D>(h); 
    h->initialize(title, binEdges, options);
    return h;
  }
  
  AIDA::Dev::IDevHistogram1D * AIDA_DevHistogramFactory::createCopy(const AIDA::IHistogram1D & hist) {
    AIDA::Dev::IDevHistogram1D * newHist = 0; 
    // try if it is a AIDA_ROOT implementation based on AIDA_CPP
    try 
      { 
	const AIDA_CPP::IHistogram1D& hcpp = dynamic_cast<const AIDA_CPP::IHistogram1D&>(hist); 
	newHist = factory.create<AIDA_CPP::IHistogram1D>(hcpp);
      } 
    catch (std::bad_cast & )
      { 
	// case of foreign implementation
	newHist = dynamic_cast<AIDA::Dev::IDevHistogram1D *> (factory.create<AIDA::IHistogram1D>(hist) ); 
      }

    //manage< AIDA_ROOT::Histogram1D>(newHist); 
    return newHist;	
  }
  
  
    AIDA::Dev::IDevHistogram2D * AIDA_DevHistogramFactory::createHistogram2D(const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, const std::string & options ) 
  { 
    AIDA_CPP::IHistogram2D * h = factory.create<AIDA_CPP::IHistogram2D>();
    //manage< AIDA_ROOT::Histogram2D>(h); 
    h->initialize(title, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, options); 
    return h;
  }

    AIDA::Dev::IDevHistogram2D * AIDA_DevHistogramFactory::createHistogram2D( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY) 
  { 
    return createHistogram2D( emptyTitle, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY);  
  }

    AIDA::Dev::IDevHistogram2D * AIDA_DevHistogramFactory::createHistogram2D(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, const std::string & options )
  {
    AIDA_CPP::IHistogram2D * h = factory.create<AIDA_CPP::IHistogram2D>();
    //manage< AIDA_ROOT::Histogram2D>(h); 
    h->initialize(title, binEdgesX, binEdgesY, options); 
    return h;
  }

    
    AIDA::Dev::IDevHistogram2D * AIDA_DevHistogramFactory::createCopy(const AIDA::IHistogram2D & hist) 
  {     
    AIDA::Dev::IDevHistogram2D * newHist = 0; 
    // try if it is a AIDA_ROOT implementation based on AIDA_CPP
    try 
      { 
	const AIDA_CPP::IHistogram2D& hcpp = dynamic_cast<const AIDA_CPP::IHistogram2D&>(hist); 
	newHist = factory.create<AIDA_CPP::IHistogram2D>(hcpp);
      } 
    catch (std::bad_cast & )
      { 
	// case of foreign implementation
	newHist = dynamic_cast<AIDA::Dev::IDevHistogram2D *> ( factory.create<AIDA::IHistogram2D>(hist) ); 
      }

    //manage< AIDA_ROOT::Histogram2D>(newHist); 
    return newHist;     
  }

    
    AIDA::Dev::IDevHistogram3D * AIDA_DevHistogramFactory::createHistogram3D(const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nBinsZ, double lowerEdgeZ, double upperEdgeZ, const std::string & options ) 
    { 
      AIDA_CPP::IHistogram3D * h = factory.create<AIDA_CPP::IHistogram3D>();
      //manage< AIDA_ROOT::Histogram3D>(h); 
      h->initialize(title, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, nBinsZ, lowerEdgeZ, upperEdgeZ, options); 
      return h;
    }

     AIDA::Dev::IDevHistogram3D * AIDA_DevHistogramFactory::createHistogram3D( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nBinsZ, double lowerEdgeZ, double upperEdgeZ) 
     { 
       return createHistogram3D( emptyTitle, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, nBinsZ, lowerEdgeZ, upperEdgeZ);         
     }

    
    AIDA::Dev::IDevHistogram3D * AIDA_DevHistogramFactory::createHistogram3D(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, const std::vector<double>  & binEdgesZ, const std::string & options ) 
    { 
      AIDA_CPP::IHistogram3D * h = factory.create<AIDA_CPP::IHistogram3D>();
      //manage< AIDA_ROOT::Histogram3D>(h); 
      h->initialize(title, binEdgesX, binEdgesY, binEdgesZ, options);
      return h;
    }

    AIDA::Dev::IDevHistogram3D * AIDA_DevHistogramFactory::createCopy(const AIDA::IHistogram3D & hist) 
    { 
      AIDA::Dev::IDevHistogram3D * newHist = 0; 
      // try if it is a AIDA_ROOT implementation based on AIDA_CPP
      try 
	{ 
	  const AIDA_CPP::IHistogram3D& hcpp = dynamic_cast<const AIDA_CPP::IHistogram3D&>(hist); 
	  newHist = factory.create<AIDA_CPP::IHistogram3D>(hcpp);
	} 
      catch (std::bad_cast & )
	{ 
	  // case of foreign implementation
	  newHist = dynamic_cast<AIDA::Dev::IDevHistogram3D *> (factory.create<AIDA::IHistogram3D>(hist) ); 
	}

      //manage< AIDA_ROOT::Histogram3D>(newHist); 
      return newHist;
    }

  AIDA::Dev::IDevProfile1D * AIDA_DevHistogramFactory::createProfile1D(const std::string & title, int nBins, double lowerEdge, double upperEdge, const std::string & /* options */ ) {       
       AIDA_CPP::IProfile1D * h = factory.create<AIDA_CPP::IProfile1D>();
       //manage< AIDA_ROOT::Profile1D>(h); 
       h->initialize(title, nBins, lowerEdge, upperEdge);
       return h;
}

  AIDA::Dev::IDevProfile1D * AIDA_DevHistogramFactory::createProfile1D(const std::string & title, int nBins, double lowerEdge, double upperEdge, double lowerValue, double upperValue, const std::string & /* options */ ) { 
       AIDA_CPP::IProfile1D * h = factory.create<AIDA_CPP::IProfile1D>();
       //manage< AIDA_ROOT::Profile1D>(h); 
       h->initialize(title, nBins, lowerEdge, upperEdge, lowerValue, upperValue);
       return h;
   }

  AIDA::Dev::IDevProfile1D * AIDA_DevHistogramFactory::createProfile1D(const std::string & title, const std::vector<double>  & binEdges, const std::string & /* options */ ) { 
      AIDA_CPP::IProfile1D * h = factory.create<AIDA_CPP::IProfile1D>();
      //manage< AIDA_ROOT::Profile1D>(h); 
      h->initialize(title, binEdges);
      return h;
    }

    
  AIDA::Dev::IDevProfile1D * AIDA_DevHistogramFactory::createProfile1D(const std::string & title, const std::vector<double>  & binEdges, double lowerValue, double upperValue, const std::string & /* options */ ) { 
      AIDA_CPP::IProfile1D * h = factory.create<AIDA_CPP::IProfile1D>();
      //manage< AIDA_ROOT::Profile1D>(h); 
      h->initialize(title, binEdges, lowerValue, upperValue);
      return h;
    }

    AIDA::Dev::IDevProfile1D * AIDA_DevHistogramFactory::createProfile1D( int nBins, double lowerEdge, double upperEdge) { return createProfile1D(emptyTitle, nBins, lowerEdge, upperEdge);}

    
  AIDA::Dev::IDevProfile1D * AIDA_DevHistogramFactory::createProfile1D( int nBins, double lowerEdge, double upperEdge, double lowerValue, double upperValue) { 
    return createProfile1D(emptyTitle, nBins, lowerEdge, upperEdge, lowerValue, upperValue);
  }
  
  AIDA::Dev::IDevProfile1D * AIDA_DevHistogramFactory::createCopy(const AIDA::IProfile1D & profile) { 
    AIDA::Dev::IDevProfile1D * newProf = 0; 
    // try if it is a AIDA_ROOT implementation based on AIDA_CPP
    try 
      { 
	const AIDA_CPP::IProfile1D& hcpp = dynamic_cast<const AIDA_CPP::IProfile1D&>(profile); 
	newProf = factory.create<AIDA_CPP::IProfile1D>(hcpp);
      } 
    catch (std::bad_cast & )
      { 
	// case of foreign implementation
	newProf = dynamic_cast<AIDA::Dev::IDevProfile1D *> (factory.create<AIDA::IProfile1D>(profile) ); 
      }

    //manage< AIDA_ROOT::Profile1D>(newProf); 
    return newProf;	
  }
  
  AIDA::Dev::IDevProfile2D * AIDA_DevHistogramFactory::createProfile2D(const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, const std::string & options ) 
  {       
    AIDA_CPP::IProfile2D * h = factory.create<AIDA_CPP::IProfile2D>();
    //manage< AIDA_ROOT::Profile2D>(h); 
    h->initialize(title, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, options); 
    return h;
  }

  AIDA::Dev::IDevProfile2D * AIDA_DevHistogramFactory::createProfile2D(const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY,  double lowerValue, double upperValue, const std::string & options ) 
  { 
    AIDA_CPP::IProfile2D * h = factory.create<AIDA_CPP::IProfile2D>();
    //manage< AIDA_ROOT::Profile2D>(h); 
    h->initialize(title, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, lowerValue, upperValue, options); 
    return h;
  }
  
  AIDA::Dev::IDevProfile2D * AIDA_DevHistogramFactory::createProfile2D(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, double lowerValue, double upperValue, const std::string & options ) 
  { 
    AIDA_CPP::IProfile2D * h = factory.create<AIDA_CPP::IProfile2D>();
    //manage< AIDA_ROOT::Profile2D>(h); 
    h->initialize(title, binEdgesX, binEdgesY, lowerValue, upperValue, options); 
    return h;
  }

  AIDA::Dev::IDevProfile2D * AIDA_DevHistogramFactory::createProfile2D(const std::string & title, const std::vector<double>  & binEdgesX, const std::vector<double>  & binEdgesY, const std::string & options ) 
  { 
    AIDA_CPP::IProfile2D * h = factory.create<AIDA_CPP::IProfile2D>();
    //manage< AIDA_ROOT::Profile2D>(h); 
    h->initialize(title, binEdgesX, binEdgesY, options); 
    return h;
  }
  
  
  AIDA::Dev::IDevProfile2D * AIDA_DevHistogramFactory::createProfile2D( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY) 
  { 
    return createProfile2D( emptyTitle, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY);  
  }
  
  
  AIDA::Dev::IDevProfile2D * AIDA_DevHistogramFactory::createProfile2D( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, double lowerValue, double upperValue) 
  { 
    return createProfile2D( emptyTitle, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, lowerValue, upperValue);  
  }
  
  
  AIDA::Dev::IDevProfile2D * AIDA_DevHistogramFactory::createCopy(const AIDA::IProfile2D & profile) 
  { 
    AIDA::Dev::IDevProfile2D * newHist = 0; 
    // try if it is a AIDA_ROOT implementation based on AIDA_CPP
    try 
      { 
	const AIDA_CPP::IProfile2D& hcpp = dynamic_cast<const AIDA_CPP::IProfile2D&>(profile); 
	newHist = factory.create<AIDA_CPP::IProfile2D>(hcpp);
      } 
    catch (std::bad_cast & )
      { 
	// case of foreign implementation
	newHist = dynamic_cast<AIDA::Dev::IDevProfile2D *> (factory.create<AIDA::IProfile2D>(profile) ); 
      }

    //manage< AIDA_ROOT::Profile2D>(newHist); 
    return newHist;     
  }
  
  /******************************************************************************************
   *
   * histograms  operations
   *****************************************************************************************/

  // all dummy in AIDA_ROOT

  
  AIDA::Dev::IDevHistogram1D * AIDA_DevHistogramFactory::add(  const AIDA::IHistogram1D &  , const AIDA::IHistogram1D &  ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram1D * AIDA_DevHistogramFactory::subtract(  const AIDA::IHistogram1D &  , const AIDA::IHistogram1D &  ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram1D * AIDA_DevHistogramFactory::multiply(  const AIDA::IHistogram1D &  , const AIDA::IHistogram1D &  ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram1D * AIDA_DevHistogramFactory::divide(  const AIDA::IHistogram1D &  , const AIDA::IHistogram1D &  ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram2D * AIDA_DevHistogramFactory::add(  const AIDA::IHistogram2D &  , const AIDA::IHistogram2D &  ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram2D * AIDA_DevHistogramFactory::subtract(  const AIDA::IHistogram2D &  , const AIDA::IHistogram2D &  ) { return 0;}
  
  
  AIDA::Dev::IDevHistogram2D * AIDA_DevHistogramFactory::multiply(  const AIDA::IHistogram2D &  , const AIDA::IHistogram2D &  ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram2D * AIDA_DevHistogramFactory::divide(  const AIDA::IHistogram2D &  , const AIDA::IHistogram2D &  ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram3D * AIDA_DevHistogramFactory::add(  const AIDA::IHistogram3D &  , const AIDA::IHistogram3D &  ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram3D * AIDA_DevHistogramFactory::subtract(  const AIDA::IHistogram3D &  , const AIDA::IHistogram3D &  ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram3D * AIDA_DevHistogramFactory::multiply(  const AIDA::IHistogram3D &  , const AIDA::IHistogram3D &  ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram3D * AIDA_DevHistogramFactory::divide(  const AIDA::IHistogram3D &  , const AIDA::IHistogram3D &  ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram1D * AIDA_DevHistogramFactory::projectionX(  const AIDA::IHistogram2D & ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram1D * AIDA_DevHistogramFactory::projectionY(  const AIDA::IHistogram2D & ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram1D * AIDA_DevHistogramFactory::sliceX(  const AIDA::IHistogram2D & , int  ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram1D * AIDA_DevHistogramFactory::sliceY(  const AIDA::IHistogram2D & , int  ) { return 0;}
  
  
  AIDA::Dev::IDevHistogram1D * AIDA_DevHistogramFactory::sliceX(  const AIDA::IHistogram2D & , int  , int  ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram1D * AIDA_DevHistogramFactory::sliceY(  const AIDA::IHistogram2D & , int  , int  ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram2D * AIDA_DevHistogramFactory::projectionXY(  const AIDA::IHistogram3D & ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram2D * AIDA_DevHistogramFactory::projectionXZ(  const AIDA::IHistogram3D & ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram2D * AIDA_DevHistogramFactory::projectionYZ(  const AIDA::IHistogram3D & ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram2D * AIDA_DevHistogramFactory::sliceXY(  const AIDA::IHistogram3D & , int , int ) { return 0;}
  
  
  
  AIDA::Dev::IDevHistogram2D * AIDA_DevHistogramFactory::sliceXZ(  const AIDA::IHistogram3D & , int, int ) { return 0;}
  
  
  AIDA::Dev::IDevHistogram2D * AIDA_DevHistogramFactory::sliceYZ(  const AIDA::IHistogram3D & , int , int ) { return 0;}




}  // namespace AIDA_ROOT
