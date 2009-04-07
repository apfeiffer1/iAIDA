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

#include "HistogramConverter.h"
#include "HBook.h"

#include "AIDA_Dev/IDevHistogramFactory.h"
#include "AIDA_Dev/IDevHistogram1D.h"
#include "AIDA_Dev/IDevHistogram2D.h"
#include "AIDA_Dev/IDevProfile1D.h"

#include "AIDA/IAxis.h"

#include <cmath>

#ifdef NONUMERICLIMITS
static const float positive_infinity = 3.40282347e+38;
static const float negative_infinity = -3.40282347e+38;
#else
#include <limits>
static const float positive_infinity = std::numeric_limits<float>::max();
static const float negative_infinity = -positive_infinity;
#endif

static const std::string emptyString = "";

const iAIDA::AIDA_HBookStore::HistogramConverter&
iAIDA::AIDA_HBookStore::HistogramConverter::theConverter()
{
  static iAIDA::AIDA_HBookStore::HistogramConverter hc;
  return hc;
}


bool
iAIDA::AIDA_HBookStore::HistogramConverter::convertToHBook( const AIDA::IHistogram1D& h,
							     int id, bool requestStoreErrors ) const
{
  bool storeErrors = false;  
  if ( h.axis().isFixedBinning() ) {
    iAIDA::AIDA_HBookStore::HBook::bookFixedBin1DHisto( id, h.title(),
							 h.axis().bins(),
							 static_cast< float >( h.axis().lowerEdge() ),
							 static_cast< float >( h.axis().upperEdge() ) );
  }
  else {
    const int nBins = h.axis().bins();
    std::vector< float > edges( nBins + 1 );
    for ( int i = 0; i < h.axis().bins(); ++i ) edges[i] = h.axis().binLowerEdge( i );
    edges[ nBins ] = h.axis().upperEdge();
    iAIDA::AIDA_HBookStore::HBook::bookVariableBin1DHisto( id, h.title(), edges );
  }

      
  std::vector<float> heights; 
  std::vector<float> errors; 
  // fill vector of bins WITHOUT overflow/underflow 
  for ( int i = 0; i < h.axis().bins(); ++i ) {
    heights.push_back(static_cast<float> ( h.binHeight(i) ) ); 
    errors.push_back(static_cast<float> ( h.binError(i) ));
    // check if errors are needed ( when error  NOT EQ sqrt(height) )
    
    if (requestStoreErrors && std::abs(h.binHeight(i) - h.binError(i)*h.binError(i)) > h.binHeight(i)*1.0E-15 )
      storeErrors = true; 
  }


  if ( storeErrors ) iAIDA::AIDA_HBookStore::HBook::useErrorsForHisto1D( id );


  // set contents bin by bin using Olivier fortran routines
  
  // set underflow and overflow (before entries) 
  const float x1 = static_cast< float >( h.binMean(AIDA::IAxis::UNDERFLOW_BIN) );
  const float w1 = static_cast< float >( h.binHeight(AIDA::IAxis::UNDERFLOW_BIN) );
  const float y = 0;
  iAIDA::AIDA_HBookStore::HBook::fillHisto( id, x1, y, w1 );
  const float x2 = static_cast< float >( h.binMean(AIDA::IAxis::OVERFLOW_BIN) );
  const float w2 = static_cast< float >( h.binHeight(AIDA::IAxis::OVERFLOW_BIN) );
  iAIDA::AIDA_HBookStore::HBook::fillHisto( id, x2, y, w2 );
  
  const int entries = h.allEntries();
  const float eqBinEntries = static_cast<float> ( h.equivalentBinEntries() );
  const float mean = static_cast<float>( h.mean() );
  const float rms = static_cast<float> ( h.rms() );
  iAIDA::AIDA_HBookStore::HBook::setHisto1DContents(id, entries, eqBinEntries, mean, rms,heights, errors, storeErrors);   
  
      
  return true;
}


bool
iAIDA::AIDA_HBookStore::HistogramConverter::convertToHBook( const AIDA::IHistogram2D& h,
							     int id, bool requestStoreErrors ) const
{
  bool storeErrors = false; 
  iAIDA::AIDA_HBookStore::HBook::bookFixedBin2DHisto( id, h.title(),
						       h.xAxis().bins(),
						       static_cast< float >( h.xAxis().lowerEdge() ),
						       static_cast< float >( h.xAxis().upperEdge() ),
						       h.yAxis().bins(),
						       static_cast< float >( h.yAxis().lowerEdge() ),
						       static_cast< float >( h.yAxis().upperEdge() ) );



  std::vector<float> heights; 
  std::vector<float> errors; 
  // fill vector of bins WITHOUT overflow/underflow 
  // fortran order is inverted so start from y 
  for ( int j = 0; j < h.yAxis().bins(); ++j ) {
    for ( int i = 0; i < h.xAxis().bins(); ++i ) {
      heights.push_back(static_cast<float> ( h.binHeight(i,j) ) ); 
      errors.push_back(static_cast<float> ( h.binError(i,j)   ) );
      // check if errors are needed ( when error  NOT EQ sqrt(height) )
      if (requestStoreErrors && std::abs(h.binHeight(i,j) - h.binError(i,j)*h.binError(i,j)   ) > h.binHeight(i,j)*1.E-16  )
	storeErrors = true; 
    }
  }

  if ( storeErrors ) iAIDA::AIDA_HBookStore::HBook::useErrorsForHisto2D( id );


  // set contents bin by bin using Olivier fortran routines

  
  // for 2d need to do under/overflow afterwards ( reason not know) so subtract 
  // following 8 fill call from entries 
  const int entries = h.allEntries() - 8;

  iAIDA::AIDA_HBookStore::HBook::setHisto2DContents(id, entries,heights, errors, storeErrors);   

  
  // set underflow and overflow (before entries) 
  // all combinations of underflow/overflow in a 2d histo
  int under = AIDA::IAxis::UNDERFLOW_BIN; 
  int over =  AIDA::IAxis::OVERFLOW_BIN; 
  // underflow x and y values
  const float  xlow = static_cast<float>( h.xAxis().lowerEdge() ) - 1; 
  const float  xup  = static_cast<float>( h.xAxis().upperEdge() ) + 1; 
  const float  ylow = static_cast<float>( h.yAxis().lowerEdge() ) - 1; 
  const float  yup  = static_cast<float>( h.yAxis().upperEdge() ) + 1; 
  // x and y value in hiostograms
  const float  xin = static_cast<float>( ( h.xAxis().upperEdge() +  h.xAxis().lowerEdge() )/2 ); 
  const float  yin = static_cast<float>( ( h.yAxis().upperEdge() +  h.yAxis().lowerEdge() )/2 ); 
  
  /*
   * fill under/overflow with following order  (clockwise)      
   *
   *                   234 
   *                   1 5
   *                   076
   */                 
  
  const float w0 = static_cast<float>( h.binHeight(under,under) ); 
  iAIDA::AIDA_HBookStore::HBook::fillHisto( id, xlow, ylow, w0 );
  
  const float w1 =  static_cast<float>( h.binHeightX(under ) - h.binHeight( under, under) - h.binHeight(under,over) );
  iAIDA::AIDA_HBookStore::HBook::fillHisto( id, xlow, yin, w1 );

  const float w2 = static_cast<float>( h.binHeight(under,over) ); 
  iAIDA::AIDA_HBookStore::HBook::fillHisto( id, xlow, yup, w2 );

  const float w3 =  static_cast<float>( h.binHeightY(over ) - h.binHeight( under, over) - h.binHeight(over,over) );
  iAIDA::AIDA_HBookStore::HBook::fillHisto( id, xin, yup, w3 );

  const float w4 = static_cast<float>( h.binHeight(over,over) ); 
  iAIDA::AIDA_HBookStore::HBook::fillHisto( id, xup, yup, w4 );

  const float w5 =  static_cast<float>( h.binHeightX(over ) - h.binHeight( over, under) - h.binHeight(over,over) );
  iAIDA::AIDA_HBookStore::HBook::fillHisto( id, xup, yin, w5 );

  const float w6 = static_cast<float>( h.binHeight(over,under) ); 
  iAIDA::AIDA_HBookStore::HBook::fillHisto( id, xup, ylow, w6 );

  const float w7 =  static_cast<float>( h.binHeightY(under) -  h.binHeight( under, under) -  h.binHeight(over,under) ); 
  iAIDA::AIDA_HBookStore::HBook::fillHisto( id, xin, ylow, w7 );


  return true;
}


bool
iAIDA::AIDA_HBookStore::HistogramConverter::convertToHBook( const AIDA::IProfile1D& h,
							     int id ) const
{
  const float lowY = negative_infinity;
  const float highY = positive_infinity;
  iAIDA::AIDA_HBookStore::HBook::bookFixedBinProfileHisto( id, h.title(),
							    h.axis().bins(),
							    static_cast< float >( h.axis().lowerEdge() ),
							    static_cast< float >( h.axis().upperEdge() ),
							    lowY,
							    highY );
  for ( int ib = -2; ib < h.axis().bins(); ++ib ) {
    int i = ib;
    if ( ib == -2 ) i = static_cast<int>( AIDA::IAxis::OVERFLOW_BIN );
    else if ( ib == -1 ) i = static_cast<int>( AIDA::IAxis::UNDERFLOW_BIN );
    int entries = h.binEntries( i );
    if ( entries == 0 ) continue;
    double newValue = h.binHeight( i );
    double newError = h.binError( i );
    double newCentre = h.binMean( i );
    const float x = static_cast<float>( newCentre );
    if ( entries == 1 ) {
      const float y = static_cast<float>( newValue );
      const float w = static_cast<float>( std::abs( y / (newError * newError) ) );
      iAIDA::AIDA_HBookStore::HBook::fillHisto( id, x, y, w );
    }
    else {
      const double newSpread = h.binRms( i );
      const float yo = static_cast<float>( newValue - newSpread / ( entries - 1 ) );
      const float y = static_cast<float>( newSpread * std::sqrt( static_cast< float >( entries - 1 ) ) + newValue );
      float w = 1;
      if ( newError != 0 ) {
	if ( newSpread != 0 ) w = static_cast< float >( newSpread * newSpread / ( entries * newError * newError ) );
	else w = static_cast< float >( std::abs( newValue ) / ( newError * newError * entries ) );
      }
      for ( int n = 0; n < entries - 1; ++n ) iAIDA::AIDA_HBookStore::HBook::fillHisto( id, x, yo, w );
      iAIDA::AIDA_HBookStore::HBook::fillHisto( id, x, y, w );
    }
  }
  return true;
}


AIDA::Dev::IDevHistogram1D*
iAIDA::AIDA_HBookStore::HistogramConverter::createHistogram1DFromHBook( AIDA::Dev::IDevHistogramFactory& factory,
									 int id ) const
{
  std::string title;
  int numberOfXBins, numberOfYBins = 0;
  float lowerEdgeX, lowerEdgeY = 0;
  float upperEdgeX, upperEdgeY = 0;
  iAIDA::AIDA_HBookStore::HBook::getHistoParameters(id, title, numberOfXBins, lowerEdgeX, upperEdgeX,
						     numberOfYBins, lowerEdgeY, upperEdgeY);
  AIDA::Dev::IDevHistogram1D* h = 0;
  if ( iAIDA::AIDA_HBookStore::HBook::hasVariableBinSizes( id ) ) {
    std::vector< double > edges( numberOfXBins + 1 );
    for ( int i = 0; i < numberOfXBins; ++i ) {
      edges[i] = iAIDA::AIDA_HBookStore::HBook::binLowerEdge( id, i + 1 );
    }
    edges[numberOfXBins] = upperEdgeX;
    h = factory.createHistogram1D( title, edges, emptyString );
  }
  else {
    h = factory.createHistogram1D( title, numberOfXBins, lowerEdgeX, upperEdgeX );
  }

  const int totalEntries = iAIDA::AIDA_HBookStore::HBook::numberOfEntries( id );
  if ( totalEntries == 0 ) return h;

  double sumOfWeightsOfAllEntries = 0;
  for ( int i = 0; i <= numberOfXBins + 1; ++i ) {
    sumOfWeightsOfAllEntries += iAIDA::AIDA_HBookStore::HBook::binContent( id, i );
  }
  const double entriesOverWeight = totalEntries / sumOfWeightsOfAllEntries;
  int entriesInBins = 0;
  for ( int i = 0; i <= numberOfXBins + 1; ++i ) {
    const float height = iAIDA::AIDA_HBookStore::HBook::binContent( id, i );
    int entries = static_cast<int>( std::floor( height * entriesOverWeight + 0.5 ) );
    int ib = i - 1;
    float lowBinEdgeX, highBinEdgeX;
    if ( i == 0 ) {
      ib = static_cast<int>( AIDA::IAxis::UNDERFLOW_BIN );
      lowBinEdgeX = lowerEdgeX - 1;
      highBinEdgeX = lowerEdgeX;
    }
    else if ( i == numberOfXBins + 1 ) {
      ib = static_cast<int>( AIDA::IAxis::OVERFLOW_BIN );
      lowBinEdgeX = upperEdgeX;
      highBinEdgeX = upperEdgeX + 1;
      entries = totalEntries - entriesInBins;
    }
    else {
      lowBinEdgeX = iAIDA::AIDA_HBookStore::HBook::binLowerEdge( id, i );
      if ( i < numberOfXBins ) {
	highBinEdgeX = iAIDA::AIDA_HBookStore::HBook::binLowerEdge( id, i + 1 );
      }
      else {
	highBinEdgeX = upperEdgeX;
      }
    }
    float error;
    if ( iAIDA::AIDA_HBookStore::HBook::areErrorsStored( id ) ) {
      error = iAIDA::AIDA_HBookStore::HBook::binContent( id, i );
    }
    else {
      error = std::sqrt( std::abs( static_cast< float >( entries ) ) ) / entriesOverWeight;
    }
    h->setBinContents( ib, entries, height, error, 0.5 * ( lowBinEdgeX + highBinEdgeX ) );
    entriesInBins += entries;
  }
  h->setRms( iAIDA::AIDA_HBookStore::HBook::histoRms( id ) );
  return h;
}


AIDA::Dev::IDevHistogram2D*
iAIDA::AIDA_HBookStore::HistogramConverter::createHistogram2DFromHBook( AIDA::Dev::IDevHistogramFactory& factory,
									 int id ) const
{
  std::string title;
  int numberOfXBins, numberOfYBins = 0;
  float lowerEdgeX, lowerEdgeY = 0;
  float upperEdgeX, upperEdgeY = 0;
  iAIDA::AIDA_HBookStore::HBook::getHistoParameters(id, title, numberOfXBins, lowerEdgeX, upperEdgeX,
						     numberOfYBins, lowerEdgeY, upperEdgeY);
  AIDA::Dev::IDevHistogram2D* h = factory.createHistogram2D( title,
							     numberOfXBins,
							     lowerEdgeX,
							     upperEdgeX,
							     numberOfYBins,
							     lowerEdgeY,
							     upperEdgeY );
  const int totalEntries = iAIDA::AIDA_HBookStore::HBook::numberOfEntries( id );
  if ( totalEntries == 0 ) return h;
  // For a 2D histo we cannot have the information whether the errors are stored or NOT :-(
  double sumOfWeightsOfAllEntries = 0;
  double sw = 0;
  for ( int i = 0; i <= numberOfXBins + 1; ++i ) {
    for ( int j = 0; j <= numberOfYBins + 1; ++j ) {
      float binHeight = iAIDA::AIDA_HBookStore::HBook::binContent( id, i, j );
      sumOfWeightsOfAllEntries += binHeight;
      if ( i > 0 && i <= numberOfXBins && j > 0 && j <= numberOfYBins ) {
	sw += binHeight;
      }
    }
  }
  const double entriesOverWeight = totalEntries / sumOfWeightsOfAllEntries;
  int entriesInBins = 0;
  double swx = 0;
  double swxx = 0;
  double swy = 0;
  double swyy = 0;

  for ( int i = 0; i <= numberOfXBins + 1; ++i ) {
    for ( int j = 0; j <= numberOfYBins + 1; ++j ) {
      const float height = iAIDA::AIDA_HBookStore::HBook::binContent( id, i, j );
      int entries = static_cast<int>( std::floor( height * entriesOverWeight + 0.5 ) );

      bool inRangeInX = false;
      int ib = i - 1;
      float lowBinEdgeX, highBinEdgeX;
      if ( i == 0 ) {
	ib = static_cast<int>( AIDA::IAxis::UNDERFLOW_BIN );
	lowBinEdgeX = lowerEdgeX - 1;
	highBinEdgeX = lowerEdgeX;
      }
      else if ( i == numberOfXBins + 1 ) {
	ib = static_cast<int>( AIDA::IAxis::OVERFLOW_BIN );
	lowBinEdgeX = upperEdgeX;
	highBinEdgeX = upperEdgeX + 1;
      }
      else {
	lowBinEdgeX = iAIDA::AIDA_HBookStore::HBook::binLowerEdges( id, i, 1 ).first;
	if ( i < numberOfXBins ) {
	  highBinEdgeX = iAIDA::AIDA_HBookStore::HBook::binLowerEdges( id, i + 1, 1 ).first;
	}
	else {
	  highBinEdgeX = upperEdgeX;
	}
	inRangeInX = true;
      }

      bool inRangeInY = false;
      int jb = j - 1;
      float lowBinEdgeY, highBinEdgeY;
      if ( j == 0 ) {
	jb = static_cast<int>( AIDA::IAxis::UNDERFLOW_BIN );
	lowBinEdgeY = lowerEdgeY - 1;
	highBinEdgeY = lowerEdgeY;
      }
      else if ( j == numberOfYBins + 1 ) {
	jb = static_cast<int>( AIDA::IAxis::OVERFLOW_BIN );
	lowBinEdgeY = upperEdgeY;
	highBinEdgeY = upperEdgeY + 1;
      }
      else {
	lowBinEdgeY = iAIDA::AIDA_HBookStore::HBook::binLowerEdges( id, 1, j ).second;
	if ( j < numberOfYBins ) {
	  highBinEdgeY = iAIDA::AIDA_HBookStore::HBook::binLowerEdges( id, 1, j + 1 ).second;
	}
	else {
	  highBinEdgeY = upperEdgeY;
	}
	inRangeInY = true;
      }

      if ( i == numberOfXBins + 1 && j == numberOfYBins + 1 ) {
	entries = totalEntries - entriesInBins;
      }

      double meanBinX =  0.5 * ( lowBinEdgeX + highBinEdgeX );
      double meanBinY = 0.5 * ( lowBinEdgeY + highBinEdgeY );

      if ( inRangeInX && inRangeInY ) {
	swx += height * meanBinX;
	swy += height * meanBinY;
	swxx += height * meanBinX * meanBinX;
	swyy += height * meanBinY * meanBinY;
      }

      const float error = std::sqrt( std::abs( static_cast< float >( entries ) ) ) / entriesOverWeight;
      h->setBinContents( ib, jb, entries, height, error, meanBinX, meanBinY );
      entriesInBins += entries;
    }
  }
  double meanX = 0;
  double rmsX = 0;
  double meanY = 0;
  double rmsY = 0;
  if ( sw != 0 ) {
    meanX = swx / sw;
    rmsX = std::sqrt( std::abs( swxx/sw - meanX*meanX ) );
    meanY = swy / sw;
    rmsY = std::sqrt( std::abs( swyy/sw - meanY*meanY ) );
  }
  h->setRms( rmsX, rmsY );
  return h;
}


AIDA::Dev::IDevProfile1D*
iAIDA::AIDA_HBookStore::HistogramConverter::createProfile1DFromHBook( AIDA::Dev::IDevHistogramFactory& factory,
								       int id ) const
{
  std::string title;
  int numberOfXBins, numberOfYBins = 0;
  float lowerEdgeX, lowerEdgeY = 0;
  float upperEdgeX, upperEdgeY = 0;
  iAIDA::AIDA_HBookStore::HBook::getHistoParameters(id, title, numberOfXBins, lowerEdgeX, upperEdgeX,
						     numberOfYBins, lowerEdgeY, upperEdgeY);
  AIDA::Dev::IDevProfile1D* h = factory.createProfile1D( title, numberOfXBins, lowerEdgeX, upperEdgeX );
  const int totalEntries = iAIDA::AIDA_HBookStore::HBook::numberOfEntries( id );
  if ( totalEntries == 0 ) return h;

  double totalWeight = 0;
  for ( int i = 1; i <= numberOfXBins; ++i ) {
    const float error = iAIDA::AIDA_HBookStore::HBook::binError( id, i );
    if ( error == 0 ) continue;
    const double weight = 1.0 / ( error * error );
    totalWeight += weight;
  }
  if ( totalWeight == 0 ) return h;
  const float ebe =  iAIDA::AIDA_HBookStore::HBook::histoEbe( id );
  const double entriesPerWeight = ebe / totalWeight;

  int entriesInBins = 0;
  for ( int i = 0; i <= numberOfXBins; ++i ) {
    const float binSpread = iAIDA::AIDA_HBookStore::HBook::binError( id, i );
    if ( binSpread == 0 ) continue;
    const double weight = 1.0 / ( binSpread * binSpread );
    int binEntries = static_cast<int>( std::floor( weight * entriesPerWeight + 0.5 ) );
    const double binHeight = iAIDA::AIDA_HBookStore::HBook::binContent( id, i );
    const double binError = binSpread / std::sqrt( ( weight * entriesPerWeight ) );
    int ib = i - 1;
    float lowBinEdgeX, highBinEdgeX;
    if ( i == 0 ) {
      ib = static_cast<int>( AIDA::IAxis::UNDERFLOW_BIN );
      lowBinEdgeX = lowerEdgeX - 1;
      highBinEdgeX = lowerEdgeX;
    }
    else {
      lowBinEdgeX = iAIDA::AIDA_HBookStore::HBook::binLowerEdge( id, i );
      if ( i < numberOfXBins ) {
	highBinEdgeX = iAIDA::AIDA_HBookStore::HBook::binLowerEdge( id, i + 1 );
      }
      else {
	highBinEdgeX = upperEdgeX;
      }
    }
    h->setBinContents( ib, binEntries, binHeight, binError, binSpread, 0.5 * ( lowBinEdgeX + highBinEdgeX ) );
    entriesInBins += binEntries;
  }

  return h;
}
