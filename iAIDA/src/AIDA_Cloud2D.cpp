#include "AIDA_Cloud2D.h"
#include "AIDA_Histogram2D.h"
#include "AIDA/IAnnotation.h"
#include "AIDA/IAxis.h"
#include <cmath>
#include <set>
#include "AnnotationNumberFormater.h"
#include "EdgeCalculator.h"

static const std::string meanXKey = "MeanX";
static const std::string rmsXKey = "RmsX";
static const std::string meanYKey = "MeanY";
static const std::string rmsYKey = "RmsY";
static const std::string emptyString = "";


iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::AIDA_Cloud2D( const std::string& title ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( title, "ICloud2D", 2 ),
  m_cacheSize( iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::defaultCacheSize ),
  m_elements(),
  m_sumOfWeights( 0 ),
  m_sumOfWeightTimesValueX( 0 ),
  m_sumOfWeightTimesValueY( 0 ),
  m_sumOfWeightTimesSquaredValueX( 0 ),
  m_sumOfWeightTimesSquaredValueY( 0 ),
  m_lowEdgeX( 0 ),
  m_lowEdgeY( 0 ),
  m_highEdgeX( 0 ),
  m_highEdgeY( 0 ),
  m_histo( 0 )
{
  setCacheSize( m_cacheSize );

  /// Define the items in the annotation.
  AIDA::IAnnotation& annotation = annotationNoUpdate();
  annotation.addItem( meanXKey, emptyString, false );
  annotation.addItem( rmsXKey, emptyString, false );
  annotation.addItem( meanYKey, emptyString, false );
  annotation.addItem( rmsYKey, emptyString, false );
}


iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::AIDA_Cloud2D( const AIDA_Cloud2D& h ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( h.title(), "ICloud2D", 2 ),
  m_cacheSize( h.m_cacheSize ),
  m_elements( h.m_elements ),
  m_sumOfWeights( h.m_sumOfWeights ),
  m_sumOfWeightTimesValueX( h.m_sumOfWeightTimesValueX ),
  m_sumOfWeightTimesValueY( h.m_sumOfWeightTimesValueY ),
  m_sumOfWeightTimesSquaredValueX( h.m_sumOfWeightTimesSquaredValueX ),
  m_sumOfWeightTimesSquaredValueY( h.m_sumOfWeightTimesSquaredValueY ),
  m_lowEdgeX( h.m_lowEdgeX ),
  m_lowEdgeY( h.m_lowEdgeY ),
  m_highEdgeX( h.m_highEdgeX ),
  m_highEdgeY( h.m_highEdgeY ),
  m_histo( 0 )
{
  if ( h.m_histo ) {
    m_histo = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( * ( dynamic_cast<const iAIDA::AIDA_Histogram_native::AIDA_Histogram2D*>( h.m_histo ) ) );
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


iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::AIDA_Cloud2D( const AIDA::ICloud2D& h ):
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram( h.title(), "ICloud2D", 2 ),
  m_cacheSize( iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::defaultCacheSize ),
  m_elements(),
  m_sumOfWeights( 0 ),
  m_sumOfWeightTimesValueX( 0 ),
  m_sumOfWeightTimesValueY( 0 ),
  m_sumOfWeightTimesSquaredValueX( 0 ),
  m_sumOfWeightTimesSquaredValueY( 0 ),
  m_lowEdgeX( 0 ),
  m_lowEdgeY( 0 ),
  m_highEdgeX( 0 ),
  m_highEdgeY( 0 ),
  m_histo( 0 )
{
  if ( h.isConverted() ) {
    AIDA::ICloud2D& c = const_cast<AIDA::ICloud2D&>( h );
    const AIDA::IHistogram2D& histo = c.histogram();
    m_histo = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( histo );
  }
  else {
    setCacheSize( m_cacheSize );    
    for ( int i = 0; i < h.entries(); ++i ) this->fill( h.valueX( i ), h.valueY( i ), h.weight( i ) );
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


iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::~AIDA_Cloud2D()
{
  if ( m_histo ) delete m_histo;
}


void
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::updateAnnotation() const
{
  iAIDA::AIDA_Histogram_native::AIDA_BaseHistogram::updateAnnotation();
  const AIDA::IAnnotation& anno = annotationNoUpdate();
  AIDA::IAnnotation& annotation = const_cast< AIDA::IAnnotation& >( anno );

  annotation.setValue( meanXKey, iAIDA_annotationNumberFormater.formatDouble( meanX() ) );
  annotation.setValue( rmsXKey, iAIDA_annotationNumberFormater.formatDouble( rmsX() ) );
  annotation.setValue( meanYKey, iAIDA_annotationNumberFormater.formatDouble( meanY() ) );
  annotation.setValue( rmsYKey, iAIDA_annotationNumberFormater.formatDouble( rmsY() ) );
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::reset()
{
  if ( m_histo ) {
    delete m_histo;
    m_histo = 0;
  }
  m_elements.clear();
  setCacheSize( m_cacheSize );
  m_sumOfWeights = 0;
  m_sumOfWeightTimesValueX = m_sumOfWeightTimesValueY = 0;
  m_sumOfWeightTimesSquaredValueX = m_sumOfWeightTimesSquaredValueY = 0;
  m_lowEdgeX = m_lowEdgeY = 0;
  m_highEdgeX = m_highEdgeY = 0;
  return true;
}


int
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::entries() const
{
  if ( m_histo ) {
    return m_histo->entries();
  }
  else {
    return m_elements.size();
  }
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::sumOfWeights() const
{
  if ( m_histo ) {
    return m_histo->sumBinHeights();
  }
  else {
    return m_sumOfWeights;
  }
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::convertToHistogram()
{
  if ( m_histo || m_elements.size() < 2 ) return false;
  double lowEdgeX, highEdgeX;
  edgeCalculator.calculateEdges( m_lowEdgeX, m_highEdgeX, iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::defaultNumberOfBins, lowEdgeX, highEdgeX );
  double lowEdgeY, highEdgeY;
  edgeCalculator.calculateEdges( m_lowEdgeY, m_highEdgeY, iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::defaultNumberOfBins, lowEdgeY, highEdgeY );
  iAIDA::AIDA_Histogram_native::AIDA_Histogram2D* p =
    new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( title() + " (supporting histogram)",
							 iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::defaultNumberOfBins,
							 lowEdgeX, highEdgeX,
							 iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::defaultNumberOfBins,
							 lowEdgeY, highEdgeY );
  p->setName( name() + "_supportingHistogram" );
  m_histo = p;
  for ( unsigned int i = 0; i < m_elements.size(); ++i ) m_histo->fill( m_elements[i].x, m_elements[i].y, m_elements[i].w );
  m_elements.clear();
  return true;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::isConverted() const
{
  return ( m_histo != 0 );
}

int 
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::maxEntries() const 
{
  return m_cacheSize;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::scale( double scaleFactor )
{
  if ( m_histo ) return m_histo->scale( scaleFactor );
  else {
    for ( unsigned int i = 0; i < m_elements.size(); ++i ) {
      m_elements[i].w *= scaleFactor;
    }
    m_sumOfWeights *= scaleFactor;
    m_sumOfWeightTimesValueX *= scaleFactor;
    m_sumOfWeightTimesValueY *= scaleFactor;
    m_sumOfWeightTimesSquaredValueX *= scaleFactor;
    m_sumOfWeightTimesSquaredValueY *= scaleFactor;
    return true;
  }
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::fill( double x, double y, double weight )
{
  if ( m_histo ) return m_histo->fill( x, y, weight );
  else {
    if ( m_elements.empty() ) {
      m_lowEdgeX = m_highEdgeX = x;
      m_lowEdgeY = m_highEdgeY = y;
    }
    else {
      if ( m_lowEdgeX > x ) m_lowEdgeX = x;
      if ( m_highEdgeX < x ) m_highEdgeX = x;
      if ( m_lowEdgeY > y ) m_lowEdgeY = y;
      if ( m_highEdgeY < y ) m_highEdgeY = y;
    }
    m_elements.push_back( iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::Cloud2DElement( x, y, weight ) );
    m_sumOfWeights += weight;
    m_sumOfWeightTimesValueX += weight * x;
    m_sumOfWeightTimesValueY += weight * y;
    m_sumOfWeightTimesSquaredValueX += weight * x * x;
    m_sumOfWeightTimesSquaredValueY += weight * y * y;
    if ( m_elements.size() >= m_cacheSize ) convertToHistogram();
    return true;
  }
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::lowerEdgeX() const
{
  if ( m_histo ) return m_histo->xAxis().lowerEdge();
  else return m_lowEdgeX;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::lowerEdgeY() const
{
  if ( m_histo ) return m_histo->yAxis().lowerEdge();
  else return m_lowEdgeY;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::upperEdgeX() const
{
  if ( m_histo ) return m_histo->xAxis().upperEdge();
  else return m_highEdgeX;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::upperEdgeY() const
{
  if ( m_histo ) return m_histo->yAxis().upperEdge();
  else return m_highEdgeY;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::valueX( int index ) const
{
  if ( m_histo || index < 0 ) return 0;
  if ( static_cast< unsigned int >( index ) >= m_elements.size() ) return 0;
  return m_elements[index].x;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::valueY( int index ) const
{
  if ( m_histo || index < 0 ) return 0;
  if ( static_cast< unsigned int >( index ) >= m_elements.size() ) return 0;
  return m_elements[index].y;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::weight( int index ) const
{
  if ( m_histo || index < 0 ) return 0;
  if ( static_cast< unsigned int >( index ) >= m_elements.size() ) return 0;
  return m_elements[index].w;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::meanX() const
{
  if ( m_histo ) return m_histo->meanX();
  else if ( m_sumOfWeights ) {
    return ( m_sumOfWeightTimesValueX / m_sumOfWeights );
  }
  else return 0;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::meanY() const
{
  if ( m_histo ) return m_histo->meanY();
  else if ( m_sumOfWeights ) {
    return ( m_sumOfWeightTimesValueY / m_sumOfWeights );
  }
  else return 0;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::rmsX() const
{
  if ( m_histo ) return m_histo->rmsX();
  else if ( m_sumOfWeights ) {
    return std::sqrt(std::abs( m_sumOfWeightTimesSquaredValueX * m_sumOfWeights - m_sumOfWeightTimesValueX*m_sumOfWeightTimesValueX))/std::abs(m_sumOfWeights);
  }
  else return 0;
}


double
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::rmsY() const
{
  if ( m_histo ) return m_histo->rmsY();
  else if ( m_sumOfWeights ) {
    return std::sqrt(std::abs( m_sumOfWeightTimesSquaredValueY * m_sumOfWeights - m_sumOfWeightTimesValueY*m_sumOfWeightTimesValueY))/std::abs(m_sumOfWeights);
  }
  else return 0;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::convert( int nBinsX, double lowerEdgeX, double upperEdgeX,
                              int nBinsY, double lowerEdgeY, double upperEdgeY )
{
  if ( m_histo ) return false;
  iAIDA::AIDA_Histogram_native::AIDA_Histogram2D* p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( title() + " (supporting histogram)",
													    nBinsX, lowerEdgeX, upperEdgeX,
													    nBinsY, lowerEdgeY, upperEdgeY );
  p->setName( name() + "_supportingHistogram" );
  m_histo = p;
  for ( unsigned int i = 0; i < m_elements.size(); ++i ) m_histo->fill( m_elements[i].x, m_elements[i].y, m_elements[i].w );
  m_elements.clear();
  return true;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::convert( const std::vector< double >& binEdgeX, const std::vector< double >& binEdgeY )
{
  if ( m_histo ) return false;
  iAIDA::AIDA_Histogram_native::AIDA_Histogram2D* p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( title() + " (supporting histogram)", binEdgeX, binEdgeY );
  p->setName( name() + "_supportingHistogram" );
  m_histo = p;
  for ( unsigned int i = 0; i < m_elements.size(); ++i ) m_histo->fill( m_elements[i].x, m_elements[i].y, m_elements[i].w );
  m_elements.clear();
  return true;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::fillHistogram( AIDA::IHistogram2D& hist ) const
{
  if ( m_histo ) {
    AIDA::IHistogram2D& h = *m_histo;
    for ( int ib = -2; ib < h.yAxis().bins(); ++ib ) {
      int i = ib;
      if ( ib == -2 ) i = static_cast<int>( AIDA::IAxis::OVERFLOW_BIN );
      else if ( ib == -1 ) i = static_cast<int>( AIDA::IAxis::UNDERFLOW_BIN );

      for ( int jb = -2; jb < h.yAxis().bins(); ++jb ) {
	int j = ib;
	if ( jb == -2 ) j = static_cast<int>( AIDA::IAxis::OVERFLOW_BIN );
	else if ( jb == -1 ) j = static_cast<int>( AIDA::IAxis::UNDERFLOW_BIN );

	const int entries = h.binEntries( i, j );
	if ( entries == 0 ) continue;
	const double newCentreX = h.binMeanX( i, j );
	const double newCentreY = h.binMeanY( i, j );
	const double newValue = h.binHeight( i, j );
	if ( entries == 1 ) hist.fill( newCentreX, newCentreY, newValue );
	else {
	  double newError = h.binError( i, j );
	  const double w1 =
	    ( 2 * newValue - std::sqrt( ( 4 * entries * newError * newError - newValue * newValue ) / ( entries - 1 ) ) ) / ( 2 * entries );
	  const double w2 = newValue - ( entries - 1 ) * w1;
	  for ( int ie = 0; ie < entries - 1; ++ie ) {
	    hist.fill( newCentreX, newCentreY, w1 );
	  }
	  hist.fill( newCentreX, newCentreY, w2 );
	}
      }
    }
  }
  else {
    for ( std::vector< iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::Cloud2DElement >::const_iterator iElement = m_elements.begin();
	  iElement != m_elements.end(); ++iElement ) {
      hist.fill( iElement->x, iElement->y, iElement->w );
    }
  }
  return true;
}


const AIDA::IHistogram2D&
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::histogram() const
{
  if ( ! m_histo ) const_cast<iAIDA::AIDA_Histogram_native::AIDA_Cloud2D*>(this)->convertToHistogram();
  return *m_histo;
}


bool
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::setCacheSize( unsigned int newCacheSize )
{
  m_cacheSize = newCacheSize;
  if ( m_cacheSize != static_cast<unsigned int>( -1 ) ) m_elements.reserve( newCacheSize );
  return true;
}


unsigned int
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::cacheSize() const
{
  return m_cacheSize;
}

// new from AIDA 3.2.1 
void *
iAIDA::AIDA_Histogram_native::AIDA_Cloud2D::cast(const std::string & className) const
{ 
  if (className.find(userLevelClassType()) != std::string::npos ) 
    return dynamic_cast<AIDA::ICloud2D *>( const_cast<iAIDA::AIDA_Histogram_native::AIDA_Cloud2D *> (this) );
  else 
    return 0;  
}
