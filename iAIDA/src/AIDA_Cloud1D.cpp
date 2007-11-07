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

#include "AIDA_Cloud1D.h"
#include "AIDA_Histogram1D.h"
#include "AIDA/IAnnotation.h"
#include "AIDA/IAxis.h"
#include <cmath>
#include <set>
#include "AnnotationNumberFormater.h"
#include "EdgeCalculator.h"

static const std::string meanKey = "Mean";
static const std::string rmsKey = "Rms";
static const std::string emptyString = "";



iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::AIDA_Cloud1D( const std::string& title ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( title, "ICloud1D", 1 ),
  m_cacheSize( iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::defaultCacheSize ),
  m_elements(),
  m_sumOfWeights( 0 ),
  m_sumOfWeightTimesValue( 0 ),
  m_sumOfWeightTimesSquaredValue( 0 ),
  m_lowEdge( 0 ),
  m_highEdge( 0 ),
  m_histo( 0 )
{
  setCacheSize( m_cacheSize );

  /// Define the items in the annotation.
  AIDA::IAnnotation& annotation = annotationNoUpdate();
  annotation.addItem( meanKey, emptyString, false );
  annotation.addItem( rmsKey, emptyString, false );
}


iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::AIDA_Cloud1D( const AIDA_Cloud1D& h ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( h.title(), "ICloud1D", 1 ),
  m_cacheSize( h.m_cacheSize ),
  m_elements( h.m_elements ),
  m_sumOfWeights( h.m_sumOfWeights ),
  m_sumOfWeightTimesValue( h.m_sumOfWeightTimesValue ),
  m_sumOfWeightTimesSquaredValue( h.m_sumOfWeightTimesSquaredValue ),
  m_lowEdge( h.m_lowEdge ),
  m_highEdge( h.m_highEdge ),
  m_histo( 0 )
{
  if ( h.m_histo ) {
    m_histo = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( * ( dynamic_cast<const iAIDA::AIDA_Histogram_native::AIDA_Histogram1D*>( h.m_histo ) ) );
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


iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::AIDA_Cloud1D( const AIDA::ICloud1D& h ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( h.title(), "ICloud1D", 1 ),
  m_cacheSize( iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::defaultCacheSize ),
  m_elements(),
  m_sumOfWeights( 0 ),
  m_sumOfWeightTimesValue( 0 ),
  m_sumOfWeightTimesSquaredValue( 0 ),
  m_lowEdge( 0 ),
  m_highEdge( 0 ),
  m_histo( 0 )
{
  if ( h.isConverted() ) {
    AIDA::ICloud1D& c = const_cast<AIDA::ICloud1D&>( h );
    const AIDA::IHistogram1D& histo = c.histogram();
    m_histo = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( histo );
  }
  else {
    setCacheSize( m_cacheSize );    
    for ( int i = 0; i < h.entries(); ++i ) this->fill( h.value( i ), h.weight( i ) );
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


iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::~AIDA_Cloud1D()
{
  if ( m_histo ) delete m_histo;
}


void
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::updateAnnotation() const
{
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::updateAnnotation();
  const AIDA::IAnnotation& anno = annotationNoUpdate();
  AIDA::IAnnotation& annotation = const_cast< AIDA::IAnnotation& >( anno );

  annotation.setValue( meanKey, iAIDA_annotationNumberFormater.formatDouble( mean() ) );
  annotation.setValue( rmsKey, iAIDA_annotationNumberFormater.formatDouble( rms() ) );
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::reset()
{
  if ( m_histo ) {
    delete m_histo;
    m_histo = 0;
  }
  m_elements.clear();
  setCacheSize( m_cacheSize );
  m_sumOfWeights = m_sumOfWeightTimesValue = m_sumOfWeightTimesSquaredValue = m_lowEdge = m_highEdge = 0;
  return true;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::entries() const
{
  if ( m_histo ) {
    return m_histo->entries();
  }
  else {
    return m_elements.size();
  }
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::sumOfWeights() const
{
  if ( m_histo ) {
    return m_histo->sumBinHeights();
  }
  else {
    return m_sumOfWeights;
  }
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::convertToHistogram()
{
  if ( m_histo || m_elements.size() < 2 ) return false;
  double lowEdge, highEdge;
  edgeCalculator.calculateEdges( m_lowEdge, m_highEdge, iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::defaultNumberOfBins, lowEdge, highEdge );
  iAIDA::AIDA_Histogram_native::AIDA_Histogram1D* p =
    new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( title() + " (supporting histogram)",
							 iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::defaultNumberOfBins,
							 lowEdge, highEdge );
  p->setName( name() + "_supportingHistogram" );
  m_histo = p;
  for ( unsigned int i = 0; i < m_elements.size(); ++i ) m_histo->fill( m_elements[i].x, m_elements[i].w );
  m_elements.clear();
  return true;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::isConverted() const
{
  return ( m_histo != 0 );
}

int 
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::maxEntries() const 
{
  return m_cacheSize;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::scale( double scaleFactor )
{
  if ( m_histo ) return m_histo->scale( scaleFactor );
  else {
    for ( unsigned int i = 0; i < m_elements.size(); ++i ) {
      m_elements[i].w *= scaleFactor;
    }
    m_sumOfWeights *= scaleFactor;
    m_sumOfWeightTimesValue *= scaleFactor;
    m_sumOfWeightTimesSquaredValue *= scaleFactor;
    return true;
  }
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::fill( double x, double weight )
{
  if ( m_histo ) return m_histo->fill( x, weight );
  else {
    if ( m_elements.empty() ) m_lowEdge = m_highEdge = x;
    else {
      if ( m_lowEdge > x ) m_lowEdge = x;
      if ( m_highEdge < x ) m_highEdge = x;
    }
    m_elements.push_back( iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::Cloud1DElement( x, weight ) );
    m_sumOfWeights += weight;
    m_sumOfWeightTimesValue += weight * x;
    m_sumOfWeightTimesSquaredValue += weight * x * x;
    if ( m_elements.size() >= m_cacheSize ) convertToHistogram();
    return true;
  }
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::lowerEdge() const
{
  if ( m_histo ) return m_histo->axis().lowerEdge();
  else return m_lowEdge;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::upperEdge() const
{
  if ( m_histo ) return m_histo->axis().upperEdge();
  else return m_highEdge;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::value( int index ) const
{
  if ( m_histo || index < 0 ) return 0;
  if ( static_cast< unsigned int >( index ) >= m_elements.size() ) return 0;
  return m_elements[index].x;
}

double
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::weight( int index ) const
{
  if ( m_histo || index < 0 ) return 0;
  if ( static_cast< unsigned int >( index ) >= m_elements.size() ) return 0;
  return m_elements[index].w;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::mean() const
{
  if ( m_histo ) return m_histo->mean();
  else if ( m_sumOfWeights ) {
    return ( m_sumOfWeightTimesValue / m_sumOfWeights );
  }
  else return 0;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::rms() const
{
  if ( m_histo ) return m_histo->rms();
  else if ( m_sumOfWeights ) {
    return std::sqrt(std::abs( m_sumOfWeightTimesSquaredValue * m_sumOfWeights - m_sumOfWeightTimesValue*m_sumOfWeightTimesValue))/std::abs(m_sumOfWeights);
  }
  else return 0;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::convert( int nBins, double lowerEdge, double upperEdge )
{
  if ( m_histo ) return false;
  iAIDA::AIDA_Histogram_native::AIDA_Histogram1D* p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( title() + " (supporting histogram)", nBins, lowerEdge, upperEdge );
  p->setName( name() + "_supportingHistogram" );
  m_histo = p;
  for ( unsigned int i = 0; i < m_elements.size(); ++i ) m_histo->fill( m_elements[i].x, m_elements[i].w );
  m_elements.clear();
  return true;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::convert( const std::vector< double >& binEdge )
{
  if ( m_histo ) return false;
  iAIDA::AIDA_Histogram_native::AIDA_Histogram1D* p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( title() + " (supporting histogram)", binEdge );
  p->setName( name() + "_supportingHistogram" );
  m_histo = p;
  for ( unsigned int i = 0; i < m_elements.size(); ++i ) m_histo->fill( m_elements[i].x, m_elements[i].w );
  m_elements.clear();
  return true;
}


const AIDA::IHistogram1D&
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::histogram() const
{
  if ( ! m_histo ) const_cast<iAIDA::AIDA_Histogram_native::AIDA_Cloud1D*>(this)->convertToHistogram();
  return *m_histo;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::fillHistogram( AIDA::IHistogram1D& hist ) const
{
  if ( m_histo ) {
    AIDA::IHistogram1D& h = *m_histo;
    for ( int ib = -2; ib < h.axis().bins(); ++ib ) {
      int i = ib;
      if ( ib == -2 ) i = static_cast<int>( AIDA::IAxis::OVERFLOW_BIN );
      else if ( ib == -1 ) i = static_cast<int>( AIDA::IAxis::UNDERFLOW_BIN );
      const int entries = h.binEntries( i );
      if ( entries == 0 ) continue;
      const double newCentre = h.binMean( i );
      const double newValue = h.binHeight( i );
      if ( entries == 1 ) hist.fill( newCentre, newValue );
      else {
	double newError = h.binError( i );
	const double w1 =
	  ( 2 * newValue - std::sqrt( ( 4 * entries * newError * newError - newValue * newValue ) / ( entries - 1 ) ) ) / ( 2 * entries );
	const double w2 = newValue - ( entries - 1 ) * w1;
	for ( int ie = 0; ie < entries - 1; ++ie ) {
	  hist.fill( newCentre, w1 );
	}
	hist.fill( newCentre, w2 );
      }
    }
  }
  else {
    for ( std::vector< iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::Cloud1DElement >::const_iterator iElement = m_elements.begin();
	  iElement != m_elements.end(); ++iElement ) {
      hist.fill( iElement->x, iElement->w );
    }
  }
  return true;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::setCacheSize( unsigned int newCacheSize )
{
  m_cacheSize = newCacheSize;
  if ( m_cacheSize != static_cast<unsigned int>( -1 ) ) m_elements.reserve( newCacheSize );
  return true;
}


unsigned int
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::cacheSize() const
{
  return m_cacheSize;
}

// new from AIDA 3.2.1 
void *
iAIDA::AIDA_Histogram_native::AIDA_Cloud1D::cast(const std::string & className) const
{ 
  if (className.find(userLevelClassType()) != std::string::npos ) 
    return  dynamic_cast<AIDA::ICloud1D *>( const_cast<iAIDA::AIDA_Histogram_native::AIDA_Cloud1D *> (this) );
  else 
    return 0;  
}
