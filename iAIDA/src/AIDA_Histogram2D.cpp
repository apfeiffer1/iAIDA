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

#include "AIDA_Histogram2D.h"
#include "EvenBinAxis.h"
#include "VariableBinAxis.h"
#include "GravityBin2D.h"
#include "AIDA/IAnnotation.h"
#include <typeinfo>
#include <cmath>
#include <cstdlib> // to get std::abs(int) on Fedora9 wiht gcc 4.3

#include <set>
#include "AnnotationNumberFormater.h"

static const unsigned int numberOfExtraBins = 2;
static const std::string meanXKey = "MeanX";
static const std::string rmsXKey = "RmsX";
static const std::string meanYKey = "MeanY";
static const std::string rmsYKey = "RmsY";
static const std::string extra_entriesKey = "Extra Entries";
static const std::string emptyString = "";


iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::AIDA_Histogram2D( const std::string& title,
					  int numberOfBinsX,
					  double lowEdgeX,
					  double highEdgeX,
					  int numberOfBinsY,
					  double lowEdgeY,
					  double highEdgeY ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( title, "IHistogram2D", 2 ),
  m_axisX( new EvenBinAxis( std::abs( numberOfBinsX ), lowEdgeX, highEdgeX ) ),
  m_axisY( new EvenBinAxis( std::abs( numberOfBinsY ), lowEdgeY, highEdgeY ) ),
  m_bins( std::abs( numberOfBinsX ) + numberOfExtraBins, std::vector< GravityBin2D* >( std::abs( numberOfBinsY ) + numberOfExtraBins, 0 ) ),
  m_validStatistics( false )
{
  m_sumWeightTimesSquaredX = 0;
  m_sumWeightTimesSquaredY = 0;

  for ( unsigned int i = 0; i < m_bins.size(); ++i )
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j )
      m_bins[i][j] = new iAIDA::AIDA_Histogram_native::GravityBin2D;

  /// Define the items in the annotation.
  AIDA::IAnnotation& annotation = annotationNoUpdate();
  annotation.addItem( meanXKey, emptyString, false );
  annotation.addItem( rmsXKey, emptyString, false );
  annotation.addItem( meanYKey, emptyString, false );
  annotation.addItem( rmsYKey, emptyString, false );
  annotation.addItem( extra_entriesKey, emptyString, false );
}

iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::AIDA_Histogram2D( const std::string& title,
					  const std::vector< double >& edgesX,
					  const std::vector< double >& edgesY ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( title, "IHistogram2D", 2 ),
  m_axisX( new VariableBinAxis( edgesX ) ),
  m_axisY( new VariableBinAxis( edgesY ) ),
  m_bins( edgesX.size() - 1 + numberOfExtraBins, std::vector< GravityBin2D* >( edgesY.size() - 1 + numberOfExtraBins, 0 ) ),
  m_validStatistics( false )
{
  m_sumWeightTimesSquaredX = 0;
  m_sumWeightTimesSquaredY = 0;

  for ( unsigned int i = 0; i < m_bins.size(); ++i )
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j )
      m_bins[i][j] = new iAIDA::AIDA_Histogram_native::GravityBin2D;

  /// Define the items in the annotation.
  AIDA::IAnnotation& annotation = annotationNoUpdate();
  annotation.addItem( meanXKey, emptyString, false );
  annotation.addItem( rmsXKey, emptyString, false );
  annotation.addItem( meanYKey, emptyString, false );
  annotation.addItem( rmsYKey, emptyString, false );
  annotation.addItem( extra_entriesKey, emptyString, false );
}


iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::AIDA_Histogram2D( const iAIDA::AIDA_Histogram_native::AIDA_Histogram2D& h ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( h.title(), "IHistogram2D", 2 ),
  m_axisX( 0 ),
  m_axisY( 0 ),
  m_bins( h.m_bins.size(), std::vector< GravityBin2D* >( h.m_bins[0].size(), 0 ) ),
  m_validStatistics( false )
{
  m_sumWeightTimesSquaredX = 0;
  m_sumWeightTimesSquaredY = 0;

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
      m_bins[i][j] = new GravityBin2D( *( h.m_bins[i][j] ) );
  setRms( h.rmsX(), h.rmsY() );

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


iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::AIDA_Histogram2D( const AIDA::IHistogram2D& h ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( h.title(), "IHistogram2D", 2 ),
  m_axisX( 0 ),
  m_axisY( 0 ),
  m_bins( h.xAxis().bins() + numberOfExtraBins, std::vector< GravityBin2D* >( h.yAxis().bins() + numberOfExtraBins, 0 ) ),
  m_validStatistics( false )
{
  m_sumWeightTimesSquaredX = 0;
  m_sumWeightTimesSquaredY = 0;

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
      m_bins[iBin][jBin] = new GravityBin2D; 
      m_bins[iBin][jBin]->set( h.binEntries( iBin - numberOfExtraBins, jBin - numberOfExtraBins ),
			       h.binHeight( iBin - numberOfExtraBins, jBin - numberOfExtraBins ),
			       h.binError( iBin - numberOfExtraBins, jBin - numberOfExtraBins ),
			       h.binMeanX( iBin - numberOfExtraBins, jBin - numberOfExtraBins ),
			       h.binMeanY( iBin - numberOfExtraBins, jBin - numberOfExtraBins ) );
    }
  }
  setRms( h.rmsX(), h.rmsY() );

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


iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::~AIDA_Histogram2D()
{
  if ( m_axisX ) delete m_axisX;
  if ( m_axisY ) delete m_axisY;
  for ( unsigned int i = 0; i < m_bins.size(); ++i ) {
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j )
      if ( m_bins[i][j] ) delete m_bins[i][j];
  }
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::reset()
{
  for ( unsigned int i = 0; i < m_bins.size(); ++i )
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j )
       m_bins[i][j]->reset();
  m_sumWeightTimesSquaredX = m_sumWeightTimesSquaredY = 0;
  setUpToDate( false );  
  m_validStatistics = false;
  return true;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::entries() const
{
  calculateStatistics();
  return m_entries;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::allEntries() const
{
  return ( entries() + extraEntries() );
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::extraEntries() const
{
  calculateStatistics();
  return m_extraEntries;
}


void
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::updateAnnotation() const
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
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::equivalentBinEntries() const
{
  calculateStatistics();
  return m_ebe;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::sumBinHeights() const
{
  calculateStatistics();
  return m_sumBinHeights;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::sumExtraBinHeights() const
{
  calculateStatistics();
  return m_sumExtraBinHeights;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::sumAllBinHeights() const
{
  return ( sumBinHeights() + sumExtraBinHeights() );
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::minBinHeight() const
{
  calculateStatistics();
  return m_minHeight;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::maxBinHeight() const
{
  calculateStatistics();
  return m_maxHeight;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::scale( double scaleFactor )
{
  for ( unsigned int i = 0; i < m_bins.size(); ++i )
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j )
      m_bins[i][j]->scale( scaleFactor );
  m_sumWeightTimesSquaredX *= scaleFactor;
  m_sumWeightTimesSquaredY *= scaleFactor;
  setUpToDate( false );
  m_validStatistics = false;
  return true;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::fill( double x, double y, double weight )
{
  // ignore Nan's filled in histograms 
  if (isNaN(x) || isNaN(y) || isNaN(weight) ) return false;  
  const int binIndexX = m_axisX->coordToIndex( x );
  const int binIndexY = m_axisY->coordToIndex( y );
  m_bins[binIndexX + numberOfExtraBins][binIndexY + numberOfExtraBins]->fill( weight, x, y );
  if ( binIndexX >= 0 && binIndexY >= 0 ) {
    m_sumWeightTimesSquaredX += weight * x * x;
    m_sumWeightTimesSquaredY += weight * y * y;
  }
  setUpToDate( false );  
  m_validStatistics = false;
  return true;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::binMeanX( int indexX,  int indexY  ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;

  const GravityBin2D& bin = *( m_bins[ realIndexX ][ realIndexY ] );
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
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::binMeanY( int indexX,  int indexY  ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;

  const GravityBin2D& bin = *( m_bins[ realIndexX ][ realIndexY ] );
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
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::binEntries( int indexX, int indexY ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;
  return m_bins[realIndexX][realIndexY]->entries();
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::binEntriesX( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if (  realIndex>= m_bins.size() ) return 0;
  int e = 0;
  for ( unsigned int j = 0; j < m_bins[0].size(); ++j ) {
    const GravityBin2D& bin = *( m_bins[ realIndex ][j] );
    e += bin.entries();
  }
  return e;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::binEntriesY( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if (  realIndex>= m_bins[0].size() ) return 0;
  int e = 0;
  for ( unsigned int j = 0; j < m_bins.size(); ++j ) {
    const GravityBin2D& bin = *( m_bins[j][ realIndex ] );
    e += bin.entries();
  }
  return e;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::binHeight( int indexX, int indexY ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;
  return m_bins[realIndexX][realIndexY]->height();
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::binHeightX( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if (  realIndex>= m_bins.size() ) return 0;
  double e = 0;
  for ( unsigned int j = 0; j < m_bins[0].size(); ++j ) {
    const GravityBin2D& bin = *( m_bins[ realIndex ][j] );
    e += bin.height();
  }
  return e;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::binHeightY( int index ) const
{
  unsigned int realIndex = index + numberOfExtraBins;
  if (  realIndex>= m_bins[0].size() ) return 0;
  double e = 0;
  for ( unsigned int j = 0; j < m_bins.size(); ++j ) {
    const GravityBin2D& bin = *( m_bins[j][ realIndex ] );
    e += bin.height();
  }
  return e;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::binError( int indexX, int indexY ) const
{
  unsigned int realIndexX = indexX + numberOfExtraBins;
  if (  realIndexX>= m_bins.size() ) return 0;
  unsigned int realIndexY = indexY + numberOfExtraBins;
  if (  realIndexY>= m_bins[0].size() ) return 0;
  return m_bins[realIndexX][realIndexY]->error();
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::meanX() const
{
  calculateStatistics();
  return m_meanX;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::rmsX() const
{
  calculateStatistics();
  return m_rmsX;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::meanY() const
{
  calculateStatistics();
  return m_meanY;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::rmsY() const
{
  calculateStatistics();
  return m_rmsY;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::calculateStatistics() const
{
  if ( m_validStatistics ) return true;

  // For the in-range bins:
  m_sumBinHeights = 0;
  double sww = 0;
  double swx = 0;
  double swy = 0;
  m_entries = 0;
  m_sumExtraBinHeights = 0;
  m_extraEntries = 0;
  m_maxHeight = m_minHeight = m_bins[numberOfExtraBins][numberOfExtraBins]->height();
  for ( unsigned int i = 0; i < m_bins.size(); ++i ) {
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j ) {
      const GravityBin2D& bin = *( m_bins[i][j] );
      if ( i < numberOfExtraBins || j < numberOfExtraBins ) {
	m_extraEntries += bin.entries();
	m_sumExtraBinHeights += bin.height();
      }
      else {
	const double binMeanX = bin.centreOfGravityX();
	const double binMeanY = bin.centreOfGravityY();
	const double binHeight = bin.height();
	const double binErrorSquared = bin.errorSquared();
	m_entries += bin.entries();
	m_sumBinHeights += binHeight;
	sww += binErrorSquared;
	swx += binMeanX * binHeight;
	swy += binMeanY * binHeight;
	if ( m_maxHeight < binHeight ) m_maxHeight = binHeight;
	if ( m_minHeight > binHeight ) m_minHeight = binHeight;
      }
    }
  }

  if ( m_sumBinHeights == 0 ) {
    m_meanX = m_rmsX = m_meanY = m_rmsY = 0;
  }
  else {
    m_meanX = swx / m_sumBinHeights;
    m_rmsX = std::sqrt( std::abs( m_sumWeightTimesSquaredX / m_sumBinHeights - m_meanX * m_meanX ) );
    m_meanY = swy / m_sumBinHeights;
    m_rmsY = std::sqrt( std::abs( m_sumWeightTimesSquaredY / m_sumBinHeights - m_meanY * m_meanY ) );
  }

  if ( sww ) {
    m_ebe = m_sumBinHeights * m_sumBinHeights / sww;
  }
  else {
    m_ebe = 0;
  }

  // Done !
  m_validStatistics = true;
  return m_validStatistics;
}



const AIDA::IAxis&
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::xAxis() const
{
  return *m_axisX;
}

const AIDA::IAxis&
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::yAxis() const
{
  return *m_axisY;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::coordToIndexX( double coord ) const
{
  return m_axisX->coordToIndex( coord );
}


int
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::coordToIndexY( double coord ) const
{
  return m_axisY->coordToIndex( coord );
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::add( const AIDA::IHistogram2D & h )
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
    const iAIDA::AIDA_Histogram_native::AIDA_Histogram2D& other = dynamic_cast<const iAIDA::AIDA_Histogram_native::AIDA_Histogram2D&>( h );
    return increment( other );
  }
  catch( std::bad_cast ) {
    iAIDA::AIDA_Histogram_native::AIDA_Histogram2D * transformed = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( h );
    bool ret = increment( *transformed );
    delete transformed;
    return ret;
  }
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::increment( const AIDA_Histogram2D& h )
{
  for ( unsigned int i = 0; i < m_bins.size(); ++i ) {
    for ( unsigned int j = 0; j < m_bins[0].size(); ++j ) {
      m_bins[i][j]->increment( *( h.m_bins[i][j] ) );
    }
  }
  m_sumWeightTimesSquaredX += h.m_sumWeightTimesSquaredX;
  m_sumWeightTimesSquaredY += h.m_sumWeightTimesSquaredY;
  setUpToDate( false );  
  m_validStatistics = false;
  return true;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::setBinContents( int binIndexX,
								 int binIndexY,
								 int entries,
								 double height,
								 double error,
								 double centreX,
								 double centreY )
{
  unsigned int realIndexX = binIndexX + numberOfExtraBins;
  if (  realIndexX >= m_bins.size() ) return false;
  unsigned int realIndexY = binIndexY + numberOfExtraBins;
  if (  realIndexY >= m_bins[0].size() ) return false;
  // set also sumwx2 in case setRms will not be called
  if (binIndexX >= 0) m_sumWeightTimesSquaredX += height*centreX*centreX;
  if (binIndexY >= 0) m_sumWeightTimesSquaredY += height*centreY*centreY;
  setUpToDate( false );  
  m_validStatistics = false;
  return m_bins[ realIndexX ][realIndexY]->set( entries, height, error, centreX, centreY );
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::setRms( double rmsX, double rmsY )
{
  const double sw = sumBinHeights();
  const double mnX = meanX();
  const double mnY = meanY();
  m_sumWeightTimesSquaredX = ( mnX*mnX + rmsX*rmsX) * sw;
  m_sumWeightTimesSquaredY = ( mnY*mnY + rmsY*rmsY) * sw;
  m_validStatistics = false;
  return true;
}

// new from AIDA 3.2.1 
void *
iAIDA::AIDA_Histogram_native::AIDA_Histogram2D::cast(const std::string & className) const
{ 
  if (className.find(userLevelClassType()) != std::string::npos ) 
    return  dynamic_cast<AIDA::IHistogram2D *> (const_cast<iAIDA::AIDA_Histogram_native::AIDA_Histogram2D *> (this) );
  else 
    return 0;  
}
