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

#include "AIDA_Profile2D.h"
#include "EvenBinAxis.h"
#include "VariableBinAxis.h"
#include "ProfileBin2D.h"
#include "AIDA/IAnnotation.h"
#include <typeinfo>
#include <cmath>
#include <cstdlib> // to get std::abs(int) on Fedora9 wiht gcc 4.3

#include <set>
#include "AnnotationNumberFormater.h"

#include <iostream>

static const unsigned int numberOfExtraBins = 2;
static const std::string meanXKey = "MeanX";
static const std::string rmsXKey = "RmsX";
static const std::string meanYKey = "MeanY";
static const std::string rmsYKey = "RmsY";
static const std::string extra_entriesKey = "Extra Entries";
static const std::string emptyString = "";


iAIDA::AIDA_Histogram_native::AIDA_Profile2D::AIDA_Profile2D( const std::string& title,
							       int numberOfBinsX,
							       double lowEdgeX,
							       double highEdgeX,
							       int numberOfBinsY,
							       double lowEdgeY,
							       double highEdgeY ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( title, "IProfile2D", 2 ),
  m_axisX( new EvenBinAxis( std::abs( numberOfBinsX ), lowEdgeX, highEdgeX ) ),
  m_axisY( new EvenBinAxis( std::abs( numberOfBinsY ), lowEdgeY, highEdgeY ) ),
  m_bins( std::abs( numberOfBinsX ) + numberOfExtraBins, std::vector< ProfileBin2D* >( std::abs( numberOfBinsY ) + numberOfExtraBins, 0 ) ),
  m_validStatistics( false )
{
  for ( unsigned int i = 0; i < m_bins.size(); ++i )
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j )
      m_bins[i][j] = new iAIDA::AIDA_Histogram_native::ProfileBin2D;

  /// Define the items in the annotation.
  AIDA::IAnnotation& annotation = annotationNoUpdate();
  annotation.addItem( meanXKey, emptyString, false );
  annotation.addItem( rmsXKey, emptyString, false );
  annotation.addItem( meanYKey, emptyString, false );
  annotation.addItem( rmsYKey, emptyString, false );
  annotation.addItem( extra_entriesKey, emptyString, false );
}

iAIDA::AIDA_Histogram_native::AIDA_Profile2D::AIDA_Profile2D( const std::string& title,
							       const std::vector< double >& edgesX,
							       const std::vector< double >& edgesY ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( title, "IProfile2D", 2 ),
  m_axisX( new VariableBinAxis( edgesX ) ),
  m_axisY( new VariableBinAxis( edgesY ) ),
  m_bins( edgesX.size() - 1 + numberOfExtraBins, std::vector< ProfileBin2D* >( edgesY.size() - 1 + numberOfExtraBins, 0 ) ),
  m_validStatistics( false )
{
  for ( unsigned int i = 0; i < m_bins.size(); ++i )
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j )
      m_bins[i][j] = new iAIDA::AIDA_Histogram_native::ProfileBin2D;

  /// Define the items in the annotation.
  AIDA::IAnnotation& annotation = annotationNoUpdate();
  annotation.addItem( meanXKey, emptyString, false );
  annotation.addItem( rmsXKey, emptyString, false );
  annotation.addItem( meanYKey, emptyString, false );
  annotation.addItem( rmsYKey, emptyString, false );
  annotation.addItem( extra_entriesKey, emptyString, false );
}


iAIDA::AIDA_Histogram_native::AIDA_Profile2D::AIDA_Profile2D( const iAIDA::AIDA_Histogram_native::AIDA_Profile2D& h ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( h.title(), "IProfile2D", 2 ),
  m_axisX( 0 ),
  m_axisY( 0 ),
  m_bins( h.m_bins.size(), std::vector< ProfileBin2D* >( h.m_bins[0].size(), 0 ) ),
  m_validStatistics( false )
{
  // Construct the axes first
  const AIDA::IAxis& otherAxisX = h.xAxis();
  if ( otherAxisX.isFixedBinning() ) {
    m_axisX = new EvenBinAxis( otherAxisX.bins(),
			       otherAxisX.lowerEdge(),
			       otherAxisX.upperEdge() );
  }
  else {
    std::vector< double > edges( otherAxisX.bins() + 1 );
    for ( int i = 0; i < otherAxisX.bins(); ++i ) edges[i] = otherAxisX.binLowerEdge( i );
    edges.back() = otherAxisX.upperEdge();
    m_axisX = new VariableBinAxis( edges );
  };
  const AIDA::IAxis& otherAxisY = h.yAxis();
  if ( otherAxisY.isFixedBinning() ) {
    m_axisY = new EvenBinAxis( otherAxisY.bins(),
			       otherAxisY.lowerEdge(),
			       otherAxisY.upperEdge() );
  }
  else {
    std::vector< double > edges( otherAxisY.bins() + 1 );
    for ( int i = 0; i < otherAxisY.bins(); ++i ) edges[i] = otherAxisY.binLowerEdge( i );
    edges.back() = otherAxisY.upperEdge();
    m_axisY = new VariableBinAxis( edges );
  }

  // Copy the bin contents
  for ( unsigned int i = 0; i < m_bins.size(); ++i )
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j )
      m_bins[i][j] = new ProfileBin2D( *( h.m_bins[i][j] ) );

  /// Copy the annotation items
  AIDA::IAnnotation& annotation = annotationNoUpdate();
  std::set< std::string > existingAnnotationItems;
  for ( int item = 0; item < annotation.size(); ++item ) {
      existingAnnotationItems.insert( annotation.key( item ) );
  }
  const AIDA::IAnnotation& otherAnnotation = h.annotation();
  for ( int itemNew = 0; itemNew < otherAnnotation.size(); ++itemNew ) {
      const std::string& key = otherAnnotation.key( itemNew );
      if ( existingAnnotationItems.find( key ) == existingAnnotationItems.end() ) {
	  annotation.addItem( key, otherAnnotation.value( itemNew ), false );
      }
  }

}


iAIDA::AIDA_Histogram_native::AIDA_Profile2D::AIDA_Profile2D( const AIDA::IProfile2D& h ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( h.title(), "IProfile2D", 2 ),
  m_axisX( 0 ),
  m_axisY( 0 ),
  m_bins( h.xAxis().bins() + numberOfExtraBins, std::vector< ProfileBin2D* >( h.yAxis().bins() + numberOfExtraBins, 0 ) ),
  m_validStatistics( false )
{
  // Construct the axes first
  const AIDA::IAxis& otherAxisX = h.xAxis();
  if ( otherAxisX.isFixedBinning() ) {
    m_axisX = new EvenBinAxis( otherAxisX.bins(),
			       otherAxisX.lowerEdge(),
			       otherAxisX.upperEdge() );
  }
  else {
    std::vector< double > edges( otherAxisX.bins() + 1 );
    for ( int i = 0; i < otherAxisX.bins(); ++i ) edges[i] = otherAxisX.binLowerEdge( i );
    edges.back() = otherAxisX.upperEdge();
    m_axisX = new VariableBinAxis( edges );
  };
  const AIDA::IAxis& otherAxisY = h.yAxis();
  if ( otherAxisY.isFixedBinning() ) {
    m_axisY = new EvenBinAxis( otherAxisY.bins(),
			       otherAxisY.lowerEdge(),
			       otherAxisY.upperEdge() );
  }
  else {
    std::vector< double > edges( otherAxisY.bins() + 1 );
    for ( int i = 0; i < otherAxisY.bins(); ++i ) edges[i] = otherAxisY.binLowerEdge( i );
    edges.back() = otherAxisY.upperEdge();
    m_axisY = new VariableBinAxis( edges );
  }

  // Copy the bin contents
  for ( unsigned int iBin = 0; iBin < m_bins.size(); ++iBin ) {
    for ( unsigned int jBin = 0; jBin < m_bins[0].size(); ++jBin ) {
      m_bins[iBin][jBin] = new ProfileBin2D;
      m_bins[iBin][jBin]->set( h.binEntries( iBin - numberOfExtraBins, jBin - numberOfExtraBins ),
			       h.binHeight( iBin - numberOfExtraBins, jBin - numberOfExtraBins ),
			       h.binError( iBin - numberOfExtraBins, jBin - numberOfExtraBins ),
			       h.binRms( iBin - numberOfExtraBins, jBin - numberOfExtraBins ),
			       h.binMeanX( iBin - numberOfExtraBins, jBin - numberOfExtraBins ),
			       h.binMeanY( iBin - numberOfExtraBins, jBin - numberOfExtraBins ) );
    }
  }

  /// Copy the annotation items
  AIDA::IAnnotation& annotation = annotationNoUpdate();
  std::set< std::string > existingAnnotationItems;
  for ( int item = 0; item < annotation.size(); ++item ) {
      existingAnnotationItems.insert( annotation.key( item ) );
  }
  const AIDA::IAnnotation& otherAnnotation = h.annotation();
  for ( int itemNew = 0; itemNew < otherAnnotation.size(); ++itemNew ) {
      const std::string& key = otherAnnotation.key( itemNew );
      if ( existingAnnotationItems.find( key ) == existingAnnotationItems.end() ) {
	  annotation.addItem( key, otherAnnotation.value( itemNew ), false );
      }
  }

}


iAIDA::AIDA_Histogram_native::AIDA_Profile2D::~AIDA_Profile2D()
{
  if ( m_axisX ) delete m_axisX;
  if ( m_axisY ) delete m_axisY;
  for ( unsigned int i = 0; i < m_bins.size(); ++i ) {
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j )
      if ( m_bins[i][j] ) delete m_bins[i][j];
  }
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::reset()
{
  for ( unsigned int i = 0; i < m_bins.size(); ++i )
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j )
       m_bins[i][j]->reset();
  setUpToDate( false );  
  m_validStatistics = false;
  return true;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::entries() const
{
  calculateStatistics();
  return m_entries;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::allEntries() const
{
  return ( entries() + extraEntries() );
}


int
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::extraEntries() const
{
  calculateStatistics();
  return m_extraEntries;
}


void
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::updateAnnotation() const
{
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::updateAnnotation();
  const AIDA::IAnnotation& anno = annotationNoUpdate();
  AIDA::IAnnotation& annotation = const_cast< AIDA::IAnnotation& >( anno );

  annotation.setValue( meanXKey, iAIDA_annotationNumberFormater.formatDouble( meanX() ) );
  annotation.setValue( rmsXKey, iAIDA_annotationNumberFormater.formatDouble( rmsX() ) );
  annotation.setValue( meanYKey, iAIDA_annotationNumberFormater.formatDouble( meanY() ) );
  annotation.setValue( rmsYKey, iAIDA_annotationNumberFormater.formatDouble( rmsY() ) );
  annotation.setValue( extra_entriesKey, iAIDA_annotationNumberFormater.formatInteger( extraEntries() ) );
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::sumBinHeights() const
{
  calculateStatistics();
  return m_sumBinHeights;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::sumExtraBinHeights() const
{
  calculateStatistics();
  return m_sumExtraBinHeights;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::sumAllBinHeights() const
{
  return ( sumBinHeights() + sumExtraBinHeights() );
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::minBinHeight() const
{
  calculateStatistics();
  return m_minHeight;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::maxBinHeight() const
{
  calculateStatistics();
  return m_maxHeight;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::fill( double x, double y, double z, double weight )
{
  // ignore Nan's filled in histograms 
  if (isNaN(x) || isNaN(y) || isNaN(z) || isNaN(weight) ) return false;  
  const int binIndexX = m_axisX->coordToIndex( x );
  const int binIndexY = m_axisY->coordToIndex( y );
  m_bins[binIndexX + numberOfExtraBins][binIndexY + numberOfExtraBins]->fill( weight, x, y, z );
  setUpToDate( false );  
  m_validStatistics = false;
  return true;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::binMeanX( int indexX, int indexY ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;

  

  const ProfileBin2D& bin = *( m_bins[ realIndexX ][ realIndexY ] );
  if ( bin.entries() == 0 ) {
    if (indexX == AIDA::IAxis::UNDERFLOW_BIN)
      return m_axisX->lowerEdge(  ) - 0.5 * m_axisX->binWidth( 0 );
    else if (indexX == AIDA::IAxis::OVERFLOW_BIN)
      return m_axisX->upperEdge(  ) + 0.5 *  m_axisX->binWidth( m_axisX->bins() - 1 );
    else return m_axisX->binLowerEdge( indexX ) + 0.5 * m_axisX->binWidth( indexX );
  }
  return bin.centreOfGravityX();
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::binMeanY( int indexX, int indexY ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;

  const ProfileBin2D& bin = *( m_bins[ realIndexX ][ realIndexY ] );
  if ( bin.entries() == 0 ) {
    if (indexY == AIDA::IAxis::UNDERFLOW_BIN)
      return m_axisY->lowerEdge(  ) - 0.5 * m_axisY->binWidth( 0 );
    else if (indexY == AIDA::IAxis::OVERFLOW_BIN)
      return m_axisY->upperEdge(  ) + 0.5 *  m_axisY->binWidth( m_axisY->bins() - 1 );
    else return m_axisY->binLowerEdge( indexY ) + 0.5 * m_axisY->binWidth( indexY );
  }
  return bin.centreOfGravityY();
}


int
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::binEntries( int indexX, int indexY ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;
  return m_bins[realIndexX][realIndexY]->entries();
}


int
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::binEntriesX( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if (  realIndex>= m_bins.size() ) return 0;
  int e = 0;
  for ( unsigned int j = 0; j < m_bins[0].size(); ++j ) {
    const ProfileBin2D& bin = *( m_bins[ realIndex ][j] );
    e += bin.entries();
  }
  return e;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::binEntriesY( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if (  realIndex>= m_bins[0].size() ) return 0;
  int e = 0;
  for ( unsigned int j = 0; j < m_bins.size(); ++j ) {
    const ProfileBin2D& bin = *( m_bins[j][ realIndex ] );
    e += bin.entries();
  }
  return e;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::binHeight( int indexX, int indexY ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;
  return m_bins[realIndexX][realIndexY]->value();
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::binHeightX( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if (  realIndex>= m_bins.size() ) return 0;
  double e = 0;
  for ( unsigned int j = 0; j < m_bins[0].size(); ++j ) {
    const ProfileBin2D& bin = *( m_bins[ realIndex ][j] );
    e += bin.value();
  }
  return e;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::binHeightY( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if (  realIndex>= m_bins[0].size() ) return 0;
  double e = 0;
  for ( unsigned int j = 0; j < m_bins.size(); ++j ) {
    const ProfileBin2D& bin = *( m_bins[j][ realIndex ] );
    e += bin.value();
  }
  return e;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::binError( int indexX, int indexY ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;
  return m_bins[realIndexX][realIndexY]->error();
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::binRms( int indexX, int indexY ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;
  return m_bins[realIndexX][realIndexY]->spread();
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::meanX() const
{
  calculateStatistics();
  return m_meanX;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::rmsX() const
{
  calculateStatistics();
  return m_rmsX;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::meanY() const
{
  calculateStatistics();
  return m_meanY;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::rmsY() const
{
  calculateStatistics();
  return m_rmsY;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::calculateStatistics() const
{
  if ( m_validStatistics ) return true;

  // For the in-range bins:
  m_sumBinHeights = 0;
  double swx = 0;
  double swxx = 0;
  double swy = 0;
  double swyy = 0;
  m_entries = 0;
  m_sumExtraBinHeights = 0;
  m_extraEntries = 0;
  m_maxHeight = m_minHeight = m_bins[numberOfExtraBins][numberOfExtraBins]->value();
  // LM: need to use sum of bin weights for calculating statistics 
  m_sumBinWeights = 0;
  for ( unsigned int i = 0; i < m_bins.size(); ++i ) {
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j ) {
      const ProfileBin2D& bin = *( m_bins[i][j] );
      if ( i < numberOfExtraBins || j < numberOfExtraBins ) {
	m_extraEntries += bin.entries();
	m_sumExtraBinHeights += bin.value();
      }
      else {
	const double binMeanX = bin.centreOfGravityX();
	const double binMeanY = bin.centreOfGravityY();
	const double binHeight = bin.value();
	const double binWeight = bin.sumOfWeights();
	m_entries += bin.entries();
	m_sumBinHeights += binHeight;
	m_sumBinWeights += binWeight;
	swx += binMeanX * binWeight;
	swxx += binMeanX * binMeanX * binWeight;
	swy += binMeanY * binWeight;
	swyy += binMeanY * binMeanY * binWeight;
	if ( m_maxHeight < binHeight ) m_maxHeight = binHeight;
	if ( m_minHeight > binHeight ) m_minHeight = binHeight;
      }
    }
  }

  if ( m_sumBinWeights == 0 ) {
    m_meanX = m_rmsX = m_meanY = m_rmsY = 0;
  }
  else {
    m_meanX = swx / m_sumBinWeights;
    m_rmsX = std::sqrt( std::abs( swxx / m_sumBinWeights - m_meanX * m_meanX ) );
    m_meanY = swy / m_sumBinWeights;
    m_rmsY = std::sqrt( std::abs( swyy / m_sumBinWeights - m_meanY * m_meanY ) );
  }

  // Done !
  m_validStatistics = true;
  return m_validStatistics;
}



const AIDA::IAxis&
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::xAxis() const
{
  return *m_axisX;
}

const AIDA::IAxis&
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::yAxis() const
{
  return *m_axisY;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::coordToIndexX( double coord ) const
{
  return m_axisX->coordToIndex( coord );
}


int
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::coordToIndexY( double coord ) const
{
  return m_axisY->coordToIndex( coord );
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::add( const AIDA::IProfile2D & h )
{
  // First check that the histograms are compatible
  const AIDA::IAxis& otherAxisX = h.xAxis();
  const AIDA::IAxis& myAxisX = *m_axisX;
  if ( otherAxisX.bins() != myAxisX.bins() ) return false;
  for ( int iBin = 0; iBin < myAxisX.bins(); ++iBin ) {
    const double otherLowEdge = otherAxisX.binLowerEdge( iBin );
    const double myLowEdge = myAxisX.binLowerEdge( iBin );
    if ( std::abs(otherLowEdge - myLowEdge)/myLowEdge > 1.e-14 ) { return false; }
  }
  if ( otherAxisX.upperEdge() != myAxisX.upperEdge() ) return false;

  const AIDA::IAxis& otherAxisY = h.yAxis();
  const AIDA::IAxis& myAxisY = *m_axisY;
  if ( otherAxisY.bins() != myAxisY.bins() ) return false;
  for ( int iBin = 0; iBin < myAxisY.bins(); ++iBin ) {
    const double otherLowEdge = otherAxisY.binLowerEdge( iBin );
    const double myLowEdge = myAxisY.binLowerEdge( iBin );
    if ( std::abs(otherLowEdge - myLowEdge)/myLowEdge > 1.e-14 ) { return false; }
  }
  if ( otherAxisY.upperEdge() != myAxisY.upperEdge() ) return false;



  // OK, the histograms are compatible.
  // Let's check if the histograms are the PI ones, to make our life a bit easier.
  try {
    const iAIDA::AIDA_Histogram_native::AIDA_Profile2D& other = dynamic_cast<const iAIDA::AIDA_Histogram_native::AIDA_Profile2D&>( h );
    return increment( other );
  }
  catch( std::bad_cast ) {
    iAIDA::AIDA_Histogram_native::AIDA_Profile2D * transformed = new iAIDA::AIDA_Histogram_native::AIDA_Profile2D( h );
    bool ret = increment( *transformed );
    delete transformed;
    return ret;
  }
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::increment( const AIDA_Profile2D& h )
{
  for ( unsigned int i = 0; i < m_bins.size(); ++i ) {
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j ) {
      m_bins[i][j]->increment( *( h.m_bins[i][j] ) );
    }
  }
  setUpToDate( false );  
  m_validStatistics = false;
  return true;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::setBinContents( int binIndexX,
							       int binIndexY,
							       int entries,
							       double height,
							       double error,
							       double spread,
							       double centreX,
							       double centreY )
{
  unsigned int realIndexX = binIndexX + numberOfExtraBins;
  if (  realIndexX >= m_bins.size() ) return false;
  unsigned int realIndexY = binIndexY + numberOfExtraBins;
  if (  realIndexY >= m_bins[0].size() ) return false;
  setUpToDate( false );  
  m_validStatistics = false;
  return m_bins[ realIndexX ][realIndexY]->set( entries, height, error, spread, centreX, centreY );
}

// new from AIDA 3.2.1 
void *
iAIDA::AIDA_Histogram_native::AIDA_Profile2D::cast(const std::string & className) const
{ 
  if (className.find(userLevelClassType()) != std::string::npos ) 
    return  dynamic_cast<AIDA::IProfile2D *>( const_cast<iAIDA::AIDA_Histogram_native::AIDA_Profile2D *> (this) );
  else 
    return 0;  
}
