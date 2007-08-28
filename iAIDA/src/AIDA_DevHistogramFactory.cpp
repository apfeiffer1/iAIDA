#include "AIDA_Histogram_native/AIDA_DevHistogramFactory.h"

#include "AIDA_Histogram1D.h"
#include "AIDA_Histogram2D.h"
#include "AIDA_Histogram3D.h"
#include "AIDA_Profile1D.h"
#include "AIDA_Profile2D.h"
#include "AIDA_BoundedProfile1D.h"
#include "AIDA_BoundedProfile2D.h"
#include "AIDA_Cloud1D.h"
#include "AIDA_Cloud2D.h"
#include "AIDA_Cloud3D.h"

#include "AIDA/IAxis.h"

#include <typeinfo>
#include <cmath>

static const int numberOfExtraBins = 2;
static const std::string emptyString = "";

bool
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::destroy( AIDA::IBaseHistogram * histo )
{
  delete histo;
  return true;
}


AIDA::Dev::IDevCloud1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createCloud1D( const std::string & title, int nMax, const std::string& )
{
  AIDA::Dev::IDevCloud1D* p = new iAIDA::AIDA_Histogram_native::AIDA_Cloud1D( title );
  if ( nMax == -1 ) p->setCacheSize( static_cast<unsigned int>( -1 ) );
  else p->setCacheSize( nMax );
  return p;
}


AIDA::Dev::IDevCloud1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createCloud1D()
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Cloud1D( emptyString );
}


AIDA::Dev::IDevCloud1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createCopy( const AIDA::ICloud1D & original )
{
  try {
    const iAIDA::AIDA_Histogram_native::AIDA_Cloud1D& h = dynamic_cast< const iAIDA::AIDA_Histogram_native::AIDA_Cloud1D& >( original );
    return new iAIDA::AIDA_Histogram_native::AIDA_Cloud1D( h );
  }
  catch( std::bad_cast ) {
    return new iAIDA::AIDA_Histogram_native::AIDA_Cloud1D( original );
  }
}


AIDA::Dev::IDevCloud2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createCloud2D( const std::string & title, int nMax, const std::string& )
{
  AIDA::Dev::IDevCloud2D* p = new iAIDA::AIDA_Histogram_native::AIDA_Cloud2D( title );
  if ( nMax == -1 ) p->setCacheSize( static_cast<unsigned int>( -1 ) );
  else p->setCacheSize( nMax );
  return p;
}


AIDA::Dev::IDevCloud2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createCloud2D()
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Cloud2D( emptyString );
}


AIDA::Dev::IDevCloud2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createCopy( const AIDA::ICloud2D & original )
{
  try {
    const iAIDA::AIDA_Histogram_native::AIDA_Cloud2D& h = dynamic_cast< const iAIDA::AIDA_Histogram_native::AIDA_Cloud2D& >( original );
    return new iAIDA::AIDA_Histogram_native::AIDA_Cloud2D( h );
  }
  catch( std::bad_cast ) {
    return new iAIDA::AIDA_Histogram_native::AIDA_Cloud2D( original );
  }
}


AIDA::Dev::IDevCloud3D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createCloud3D( const std::string & title, int nMax, const std::string& )
{
  AIDA::Dev::IDevCloud3D* p = new iAIDA::AIDA_Histogram_native::AIDA_Cloud3D( title );
  if ( nMax == -1 ) p->setCacheSize( static_cast<unsigned int>(-1) );
  else p->setCacheSize( nMax );
  return p;
}


AIDA::Dev::IDevCloud3D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createCloud3D()
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Cloud3D( emptyString );
}


AIDA::Dev::IDevCloud3D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createCopy( const AIDA::ICloud3D & original )
{
  try {
    const iAIDA::AIDA_Histogram_native::AIDA_Cloud3D& h = dynamic_cast< const iAIDA::AIDA_Histogram_native::AIDA_Cloud3D& >( original );
    return new iAIDA::AIDA_Histogram_native::AIDA_Cloud3D( h );
  }
  catch( std::bad_cast ) {
    return new iAIDA::AIDA_Histogram_native::AIDA_Cloud3D( original );
  }
}


AIDA::Dev::IDevHistogram1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createHistogram1D( const std::string & title, int nBins, double lowerEdge, double upperEdge, const std::string& )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( title, nBins, lowerEdge, upperEdge );
}


AIDA::Dev::IDevHistogram1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createHistogram1D( const std::string & title, const std::vector< double > & binEdge, const std::string& )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( title, binEdge );
}


AIDA::Dev::IDevHistogram1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createHistogram1D( int nBins, double lowerEdge, double upperEdge )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( emptyString, nBins, lowerEdge, upperEdge );
}


AIDA::Dev::IDevHistogram1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createCopy( const AIDA::IHistogram1D & original )
{
  try {
    const iAIDA::AIDA_Histogram_native::AIDA_Histogram1D& h = dynamic_cast< const iAIDA::AIDA_Histogram_native::AIDA_Histogram1D& >( original );
    return new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( h );
  }
  catch( std::bad_cast ) {
    return new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( original );
  }
}


AIDA::Dev::IDevHistogram2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createHistogram2D( const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, const std::string& )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( title, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY );
}


AIDA::Dev::IDevHistogram2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createHistogram2D( const std::string & title, const std::vector< double > & binEdgeX, const std::vector< double > & binEdgeY, const std::string& )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( title, binEdgeX, binEdgeY );
}


AIDA::Dev::IDevHistogram2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createHistogram2D( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( emptyString, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY );
}


AIDA::Dev::IDevHistogram2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createCopy( const AIDA::IHistogram2D & original )
{
  try {
    const iAIDA::AIDA_Histogram_native::AIDA_Histogram2D& h = dynamic_cast< const iAIDA::AIDA_Histogram_native::AIDA_Histogram2D& >( original );
    return new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( h );
  }
  catch( std::bad_cast ) {
    return new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( original );
  }
}


AIDA::Dev::IDevHistogram3D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createHistogram3D( const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY,
						int nBinsZ, double lowerEdgeZ, double upperEdgeZ, const std::string& )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Histogram3D( title, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, nBinsZ, lowerEdgeZ, upperEdgeZ );
}


AIDA::Dev::IDevHistogram3D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createHistogram3D( const std::string & title, const std::vector< double > & binEdgeX, const std::vector< double > & binEdgeY, const std::vector< double > & binEdgeZ, const std::string& )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Histogram3D( title, binEdgeX, binEdgeY, binEdgeZ );
}


AIDA::Dev::IDevHistogram3D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createHistogram3D( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nBinsZ, double lowerEdgeZ, double upperEdgeZ )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Histogram3D( emptyString, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, nBinsZ, lowerEdgeZ, upperEdgeZ );
}


AIDA::Dev::IDevHistogram3D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createCopy( const AIDA::IHistogram3D & original )
{
  try {
    const iAIDA::AIDA_Histogram_native::AIDA_Histogram3D& h = dynamic_cast< const iAIDA::AIDA_Histogram_native::AIDA_Histogram3D& >( original );
    return new iAIDA::AIDA_Histogram_native::AIDA_Histogram3D( h );
  }
  catch( std::bad_cast ) {
    return new iAIDA::AIDA_Histogram_native::AIDA_Histogram3D( original );
  }
}


AIDA::Dev::IDevProfile1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createProfile1D( const std::string & title, int nBins, double lowerEdge, double upperEdge, const std::string& )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Profile1D( title, nBins, lowerEdge, upperEdge );
}


AIDA::Dev::IDevProfile1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createProfile1D( const std::string & title, int nBins, double lowerEdge, double upperEdge, double lowerValue, double upperValue, const std::string& )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile1D( title, nBins, lowerEdge, upperEdge, lowerValue, upperValue );
}

AIDA::Dev::IDevProfile1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createProfile1D( const std::string & title, const std::vector< double > & binEdge, const std::string& )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Profile1D( title, binEdge );
}


AIDA::Dev::IDevProfile1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createProfile1D( const std::string & title, const std::vector< double > & binEdge, double lowerValue, double upperValue, const std::string& )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile1D( title, binEdge, lowerValue, upperValue );
}


AIDA::Dev::IDevProfile1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createProfile1D( int nBins, double lowerEdge, double upperEdge )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Profile1D( emptyString, nBins, lowerEdge, upperEdge );
}


AIDA::Dev::IDevProfile1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createProfile1D( int nBins, double lowerEdge, double upperEdge, double lowerValue, double upperValue )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile1D( emptyString, nBins, lowerEdge, upperEdge, lowerValue, upperValue );
}


AIDA::Dev::IDevProfile1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createCopy( const AIDA::IProfile1D & original )
{
  try {
    const iAIDA::AIDA_Histogram_native::AIDA_Profile1D& h = dynamic_cast< const iAIDA::AIDA_Histogram_native::AIDA_Profile1D& >( original );
    try {
      const iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile1D& b = dynamic_cast< const iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile1D& >( h );
      return new iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile1D( b );
    }
    catch( std::bad_cast ) {
      return new iAIDA::AIDA_Histogram_native::AIDA_Profile1D( h );
    }
  }
  catch( std::bad_cast ) {
    return new iAIDA::AIDA_Histogram_native::AIDA_Profile1D( original );
  }
}


AIDA::Dev::IDevProfile2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createProfile2D( const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, const std::string& )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Profile2D( title, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY );
}


AIDA::Dev::IDevProfile2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createProfile2D( const std::string & title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, double lowerValue, double upperValue, const std::string& )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile2D( title, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, lowerValue, upperValue );
}


AIDA::Dev::IDevProfile2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createProfile2D( const std::string & title, const std::vector< double > & binEdgeX, const std::vector< double > & binEdgeY, const std::string& )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Profile2D( title, binEdgeX, binEdgeY );
}


AIDA::Dev::IDevProfile2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createProfile2D( const std::string & title, const std::vector< double > & binEdgeX, const std::vector< double > & binEdgeY, double lowerValue, double upperValue, const std::string& )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile2D( title, binEdgeX, binEdgeY, lowerValue, upperValue );
}


AIDA::Dev::IDevProfile2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createProfile2D( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_Profile2D( emptyString, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY );
}


AIDA::Dev::IDevProfile2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createProfile2D( int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, double lowerValue, double upperValue )
{
  return new iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile2D( emptyString, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, lowerValue, upperValue );
}


AIDA::Dev::IDevProfile2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::createCopy( const AIDA::IProfile2D & original )
{
  try {
    const iAIDA::AIDA_Histogram_native::AIDA_Profile2D& h = dynamic_cast< const iAIDA::AIDA_Histogram_native::AIDA_Profile2D& >( original );
    try {
      const iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile2D& b = dynamic_cast< const iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile2D& >( h );
      return new iAIDA::AIDA_Histogram_native::AIDA_BoundedProfile2D( b );
    }
    catch( std::bad_cast ) {
      return new iAIDA::AIDA_Histogram_native::AIDA_Profile2D( h );
    }
  }
  catch( std::bad_cast ) {
    return new iAIDA::AIDA_Histogram_native::AIDA_Profile2D( original );
  }
}


AIDA::Dev::IDevHistogram1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::add( const AIDA::IHistogram1D & a, const AIDA::IHistogram1D & b )
{
  AIDA::Dev::IDevHistogram1D* p = createCopy( a );
  if ( ! ( p->add( b ) ) ) {
    delete p;
    p = 0;
  }
  return p;
}


AIDA::Dev::IDevHistogram2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::add( const AIDA::IHistogram2D & a, const AIDA::IHistogram2D & b )
{
  AIDA::Dev::IDevHistogram2D* p = createCopy( a );
  if ( ! ( p->add( b ) ) ) {
    delete p;
    p = 0;
  }
  return p;
}


AIDA::Dev::IDevHistogram3D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::add( const AIDA::IHistogram3D & a, const AIDA::IHistogram3D & b )
{
  AIDA::Dev::IDevHistogram3D* p = createCopy( a );
  if ( ! ( p->add( b ) ) ) {
    delete p;
    p = 0;
  }
  return p;
}


AIDA::Dev::IDevHistogram1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::subtract( const AIDA::IHistogram1D & a, const AIDA::IHistogram1D & b )
{
  AIDA::Dev::IDevHistogram1D* p = createCopy( b );
  p->scale( -1.0 );
  if ( ! ( p->add( a ) ) ) {
    delete p;
    p = 0;
  }
  return p;
}


AIDA::Dev::IDevHistogram2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::subtract( const AIDA::IHistogram2D & a, const AIDA::IHistogram2D & b )
{
  AIDA::Dev::IDevHistogram2D* p = createCopy( b );
  p->scale( -1.0 );
  if ( ! ( p->add( a ) ) ) {
    delete p;
    p = 0;
  }
  return p;
}


AIDA::Dev::IDevHistogram3D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::subtract( const AIDA::IHistogram3D & a, const AIDA::IHistogram3D & b )
{
  AIDA::Dev::IDevHistogram3D* p = createCopy( b );
  p->scale( -1.0 );
  if ( ! ( p->add( a ) ) ) {
    delete p;
    p = 0;
  }
  return p;
}


AIDA::Dev::IDevHistogram1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::multiply( const AIDA::IHistogram1D & a, const AIDA::IHistogram1D & b )
{
  const AIDA::IAxis& otherAxis = b.axis();
  const AIDA::IAxis& myAxis = a.axis();
  if ( otherAxis.bins() != myAxis.bins() ) return 0;
  for ( int iBin = 0; iBin < myAxis.bins(); ++iBin ) {
    const double otherLow = otherAxis.binLowerEdge( iBin );
    const double myLow = myAxis.binLowerEdge( iBin );
    if ( otherLow != myLow ) return 0;
  }
  if ( otherAxis.upperEdge() != myAxis.upperEdge() ) return 0;

  AIDA::Dev::IDevHistogram1D* p = createCopy( a );
  p->reset();

  for ( int i = -numberOfExtraBins; i < myAxis.bins(); ++i ) {
    const double heightA = a.binHeight(i);
    const double heightB = b.binHeight(i);
    const double height = heightA * heightB;
    const double errorA = a.binError(i);
    const double errorB = b.binError(i);
    const double error = std::sqrt( errorA*errorA + errorB*errorB);
    const double centreA = a.binMean(i);
    const double centreB = b.binMean(i);
    const double centre = ( ( height != 0 ) ? ( centreA*heightA + centreB*heightB ) / (heightA + heightB) : 0.5 * (centreA + centreB) );
    p->setBinContents( i, a.binEntries(i) + b.binEntries(i), height, error, centre );
  }

  return p;
}


AIDA::Dev::IDevHistogram2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::multiply( const AIDA::IHistogram2D & a, const AIDA::IHistogram2D & b )
{
  const AIDA::IAxis& xAxisA = a.xAxis();
  const AIDA::IAxis& xAxisB = b.xAxis();
  if ( xAxisA.bins() != xAxisB.bins() ) return 0;
  for ( int iBin = 0; iBin < xAxisA.bins(); ++iBin ) {
    const double otherLow = xAxisA.binLowerEdge( iBin );
    const double myLow = xAxisB.binLowerEdge( iBin );
    if ( otherLow != myLow ) return 0;
  }
  if ( xAxisA.upperEdge() != xAxisB.upperEdge() ) return 0;

  const AIDA::IAxis& yAxisA = a.yAxis();
  const AIDA::IAxis& yAxisB = b.yAxis();
  if ( yAxisA.bins() != yAxisB.bins() ) return 0;
  for ( int iBin = 0; iBin < yAxisA.bins(); ++iBin ) {
    const double otherLow = yAxisA.binLowerEdge( iBin );
    const double myLow = yAxisB.binLowerEdge( iBin );
    if ( otherLow != myLow ) return 0;
  }
  if ( yAxisA.upperEdge() != yAxisB.upperEdge() ) return 0;

  AIDA::Dev::IDevHistogram2D* p = createCopy( a );
  p->reset();

  for ( int i = -numberOfExtraBins; i < xAxisA.bins(); ++i ) {
    for ( int j = -numberOfExtraBins; j < yAxisA.bins(); ++j ) {
      const double heightA = a.binHeight(i,j);
      const double heightB = b.binHeight(i,j);
      const double height = heightA * heightB;
      const double errorA = a.binError(i,j);
      const double errorB = b.binError(i,j);
      const double error = std::sqrt( errorA*errorA + errorB*errorB);
      const double centreXA = a.binMeanX(i,j);
      const double centreXB = b.binMeanX(i,j);
      const double centreX = ( ( height != 0 ) ? ( centreXA*heightA + centreXB*heightB ) / (heightA + heightB) : 0.5 * (centreXA + centreXB) );
      const double centreYA = a.binMeanY(i,j);
      const double centreYB = b.binMeanY(i,j);
      const double centreY = ( ( height != 0 ) ? ( centreYA*heightA + centreYB*heightB ) / (heightA + heightB) : 0.5 * (centreYA + centreYB) );
      p->setBinContents( i, j, a.binEntries(i,j) + b.binEntries(i,j), height, error, centreX, centreY );
    }
  }

  return p;
}


AIDA::Dev::IDevHistogram3D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::multiply( const AIDA::IHistogram3D & a, const AIDA::IHistogram3D & b )
{
  const AIDA::IAxis& xAxisA = a.xAxis();
  const AIDA::IAxis& xAxisB = b.xAxis();
  if ( xAxisA.bins() != xAxisB.bins() ) return 0;
  for ( int iBin = 0; iBin < xAxisA.bins(); ++iBin ) {
    const double otherLow = xAxisA.binLowerEdge( iBin );
    const double myLow = xAxisB.binLowerEdge( iBin );
    if ( otherLow != myLow ) return 0;
  }
  if ( xAxisA.upperEdge() != xAxisB.upperEdge() ) return 0;

  const AIDA::IAxis& yAxisA = a.yAxis();
  const AIDA::IAxis& yAxisB = b.yAxis();
  if ( yAxisA.bins() != yAxisB.bins() ) return 0;
  for ( int iBin = 0; iBin < yAxisA.bins(); ++iBin ) {
    const double otherLow = yAxisA.binLowerEdge( iBin );
    const double myLow = yAxisB.binLowerEdge( iBin );
    if ( otherLow != myLow ) return 0;
  }
  if ( yAxisA.upperEdge() != yAxisB.upperEdge() ) return 0;

  const AIDA::IAxis& zAxisA = a.zAxis();
  const AIDA::IAxis& zAxisB = b.zAxis();
  if ( zAxisA.bins() != zAxisB.bins() ) return 0;
  for ( int iBin = 0; iBin < zAxisA.bins(); ++iBin ) {
    const double otherLow = zAxisA.binLowerEdge( iBin );
    const double myLow = zAxisB.binLowerEdge( iBin );
    if ( otherLow != myLow ) return 0;
  }
  if ( zAxisA.upperEdge() != zAxisB.upperEdge() ) return 0;

  AIDA::Dev::IDevHistogram3D* p = createCopy( a );
  p->reset();

  for ( int i = -numberOfExtraBins; i < xAxisA.bins(); ++i ) {
    for ( int j = -numberOfExtraBins; j < yAxisA.bins(); ++j ) {
      for ( int k = -numberOfExtraBins; k < zAxisA.bins(); ++k ) {
	const double heightA = a.binHeight(i,j,k);
	const double heightB = b.binHeight(i,j,k);
	const double height = heightA * heightB;
	const double errorA = a.binError(i,j,k);
	const double errorB = b.binError(i,j,k);
	const double error = std::sqrt( errorA*errorA + errorB*errorB);
	const double centreXA = a.binMeanX(i,j,k);
	const double centreXB = b.binMeanX(i,j,k);
	const double centreX = ( ( height != 0 ) ? ( centreXA*heightA + centreXB*heightB ) / (heightA + heightB) : 0.5 * (centreXA + centreXB) );
	const double centreYA = a.binMeanY(i,j,k);
	const double centreYB = b.binMeanY(i,j,k);
	const double centreY = ( ( height != 0 ) ? ( centreYA*heightA + centreYB*heightB ) / (heightA + heightB) : 0.5 * (centreYA + centreYB) );
	const double centreZA = a.binMeanZ(i,j,k);
	const double centreZB = b.binMeanZ(i,j,k);
	const double centreZ = ( ( height != 0 ) ? ( centreZA*heightA + centreZB*heightB ) / (heightA + heightB) : 0.5 * (centreZA + centreZB) );
	p->setBinContents( i, j, k, a.binEntries(i,j,k) + b.binEntries(i,j,k), height, error, centreX, centreY, centreZ );
      }
    }
  }

  return p;
}


AIDA::Dev::IDevHistogram1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::divide( const AIDA::IHistogram1D & a, const AIDA::IHistogram1D & b )
{
  const AIDA::IAxis& xAxisA = a.axis();
  const AIDA::IAxis& xAxisB = b.axis();
  if ( xAxisA.bins() != xAxisB.bins() ) return 0;
  for ( int iBin = 0; iBin < xAxisA.bins(); ++iBin ) {
    const double otherLow = xAxisA.binLowerEdge( iBin );
    const double myLow = xAxisB.binLowerEdge( iBin );
    if ( otherLow != myLow ) return 0;
  }
  if ( xAxisA.upperEdge() != xAxisB.upperEdge() ) return 0;

  AIDA::Dev::IDevHistogram1D* p = createCopy( a );
  p->reset();

  for ( int i = -numberOfExtraBins; i < xAxisA.bins(); ++i ) {
    const double heightA = a.binHeight(i);
    const double heightB = b.binHeight(i);
    const double height = ( ( heightB != 0 ) ? heightA / heightB : 0 ) ;
    double error = 0;
    if ( heightB != 0 ) {
      const double errorA = a.binError(i);
      const double errorB = b.binError(i);
      const double hb2 = heightB * heightB;
      error = ( std::sqrt( errorA * errorA * hb2 + errorB * errorB * heightA * heightA ) ) / hb2;
    }
    const double centreXA = a.binMean(i);
    const double centreXB = b.binMean(i);
    const double centreX = ( ( height != 0 ) ? ( centreXA*heightA + centreXB*heightB ) / (heightA+heightB) : 0.5 * (centreXA + centreXB) );
    p->setBinContents( i, a.binEntries(i) + b.binEntries(i), height, error, centreX );
  }

  return p;
}


AIDA::Dev::IDevHistogram2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::divide( const AIDA::IHistogram2D & a, const AIDA::IHistogram2D & b )
{
  const AIDA::IAxis& xAxisA = a.xAxis();
  const AIDA::IAxis& xAxisB = b.xAxis();
  if ( xAxisA.bins() != xAxisB.bins() ) return 0;
  for ( int iBin = 0; iBin < xAxisA.bins(); ++iBin ) {
    const double otherLow = xAxisA.binLowerEdge( iBin );
    const double myLow = xAxisB.binLowerEdge( iBin );
    if ( otherLow != myLow ) return 0;
  }
  if ( xAxisA.upperEdge() != xAxisB.upperEdge() ) return 0;

  const AIDA::IAxis& yAxisA = a.yAxis();
  const AIDA::IAxis& yAxisB = b.yAxis();
  if ( yAxisA.bins() != yAxisB.bins() ) return 0;
  for ( int iBin = 0; iBin < yAxisA.bins(); ++iBin ) {
    const double otherLow = yAxisA.binLowerEdge( iBin );
    const double myLow = yAxisB.binLowerEdge( iBin );
    if ( otherLow != myLow ) return 0;
  }
  if ( yAxisA.upperEdge() != yAxisB.upperEdge() ) return 0;

  AIDA::Dev::IDevHistogram2D* p = createCopy( a );
  p->reset();

  for ( int i = -numberOfExtraBins; i < xAxisA.bins(); ++i ) {
    for ( int j = -numberOfExtraBins; j < yAxisA.bins(); ++j ) {
      const double heightA = a.binHeight(i,j);
      const double heightB = b.binHeight(i,j);
      const double height = ( ( heightB != 0 ) ? heightA / heightB : 0 ) ;
      double error = 0;
      if ( heightB != 0 ) {
	const double errorA = a.binError(i,j);
	const double errorB = b.binError(i,j);
	const double hb2 = heightB * heightB;
	error = ( std::sqrt( errorA * errorA * hb2 + errorB * errorB * heightA * heightA ) ) / hb2;
      }
      const double centreXA = a.binMeanX(i,j);
      const double centreXB = b.binMeanX(i,j);
      const double centreX = ( ( height != 0 ) ? ( centreXA*heightA + centreXB*heightB ) / (heightA+heightB) : 0.5 * (centreXA + centreXB) );
      const double centreYA = a.binMeanY(i,j);
      const double centreYB = b.binMeanY(i,j);
      const double centreY = ( ( height != 0 ) ? ( centreYA*heightA + centreYB*heightB ) / (heightA+heightB) : 0.5 * (centreYA + centreYB) );
      p->setBinContents( i, j, a.binEntries(i,j) + b.binEntries(i,j), height, error, centreX, centreY );
    }
  }

  return p;
}


AIDA::Dev::IDevHistogram3D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::divide( const AIDA::IHistogram3D & a, const AIDA::IHistogram3D & b )
{
  const AIDA::IAxis& xAxisA = a.xAxis();
  const AIDA::IAxis& xAxisB = b.xAxis();
  if ( xAxisA.bins() != xAxisB.bins() ) return 0;
  for ( int iBin = 0; iBin < xAxisA.bins(); ++iBin ) {
    const double otherLow = xAxisA.binLowerEdge( iBin );
    const double myLow = xAxisB.binLowerEdge( iBin );
    if ( otherLow != myLow ) return 0;
  }
  if ( xAxisA.upperEdge() != xAxisB.upperEdge() ) return 0;

  const AIDA::IAxis& yAxisA = a.yAxis();
  const AIDA::IAxis& yAxisB = b.yAxis();
  if ( yAxisA.bins() != yAxisB.bins() ) return 0;
  for ( int iBin = 0; iBin < yAxisA.bins(); ++iBin ) {
    const double otherLow = yAxisA.binLowerEdge( iBin );
    const double myLow = yAxisB.binLowerEdge( iBin );
    if ( otherLow != myLow ) return 0;
  }
  if ( yAxisA.upperEdge() != yAxisB.upperEdge() ) return 0;

  const AIDA::IAxis& zAxisA = a.zAxis();
  const AIDA::IAxis& zAxisB = b.zAxis();
  if ( zAxisA.bins() != zAxisB.bins() ) return 0;
  for ( int iBin = 0; iBin < zAxisA.bins(); ++iBin ) {
    const double otherLow = zAxisA.binLowerEdge( iBin );
    const double myLow = zAxisB.binLowerEdge( iBin );
    if ( otherLow != myLow ) return 0;
  }
  if ( zAxisA.upperEdge() != zAxisB.upperEdge() ) return 0;

  AIDA::Dev::IDevHistogram3D* p = createCopy( a );
  p->reset();

  for ( int i = -numberOfExtraBins; i < xAxisA.bins(); ++i ) {
    for ( int j = -numberOfExtraBins; j < yAxisA.bins(); ++j ) {
      for ( int k = -numberOfExtraBins; k < zAxisA.bins(); ++k ) {
	const double heightA = a.binHeight(i,j,k);
	const double heightB = b.binHeight(i,j,k);
	const double height = ( ( heightB != 0 ) ? heightA / heightB : 0 ) ;
	double error = 0;
        if ( heightB != 0 ) {
	  const double errorA = a.binError(i,j,k);
	  const double errorB = b.binError(i,j,k);
	  const double hb2 = heightB * heightB;
	  error = ( std::sqrt( errorA * errorA * hb2 + errorB * errorB * heightA * heightA ) ) / hb2;
	}
	const double centreXA = a.binMeanX(i,j,k);
	const double centreXB = b.binMeanX(i,j,k);
	const double centreX = ( ( height != 0 ) ? ( centreXA*heightA + centreXB*heightB ) / (heightA+heightB) : 0.5 * (centreXA + centreXB) );
	const double centreYA = a.binMeanY(i,j,k);
	const double centreYB = b.binMeanY(i,j,k);
	const double centreY = ( ( height != 0 ) ? ( centreYA*heightA + centreYB*heightB ) / (heightA+heightB) : 0.5 * (centreYA + centreYB) );
	const double centreZA = a.binMeanZ(i,j,k);
	const double centreZB = b.binMeanZ(i,j,k);
	const double centreZ = ( ( height != 0 ) ? ( centreZA*heightA + centreZB*heightB ) / (heightA+heightB) : 0.5 * (centreZA + centreZB) );
	p->setBinContents( i, j, k, a.binEntries(i,j,k) + b.binEntries(i,j,k), height, error, centreX, centreY, centreZ );
      }
    }
  }

  return p;
}


AIDA::Dev::IDevHistogram1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::projectionX( const AIDA::IHistogram2D & h )
{
  AIDA::Dev::IDevHistogram1D* p = 0;
  const AIDA::IAxis& axis = h.xAxis();
  if ( axis.isFixedBinning() ) {
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( h.title(), axis.bins(), axis.lowerEdge(), axis.upperEdge() );
  }
  else {
    std::vector< double > edges( axis.bins() + 1 );
    for ( int i = 0; i < axis.bins(); ++i ) edges[i] = axis.binLowerEdge( i );
    edges[ axis.bins() ] = axis.upperEdge();
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( h.title(), edges );
  }

  for ( int i = -numberOfExtraBins; i < axis.bins(); ++i ) {
    double errorSquared = 0;
    double binMeanX = 0;
    double binHeight = 0;
    const AIDA::IAxis& axisY = h.yAxis();
    for ( int j = -numberOfExtraBins; j < axisY.bins(); ++j ) {
      const double be = h.binError( i, j );
      errorSquared += be * be;
      const double height = h.binHeight( i, j );
      binHeight += height;
      binMeanX += h.binMeanX( i, j ) * height;
    }
    if ( binHeight != 0 ) binMeanX /= binHeight;
    p->setBinContents( i,
		       h.binEntriesX(i),
                       binHeight,
		       std::sqrt( errorSquared ),
                       binMeanX );

  }

  // FIXME: need to set rms 
  // problem is that h.rmsX() does not include under/overflow entries in Y      
  // 
  //p->setRms( h.rmsX() );

  return p;
}


AIDA::Dev::IDevHistogram1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::projectionY( const AIDA::IHistogram2D & h )
{
  AIDA::Dev::IDevHistogram1D* p = 0;
  const AIDA::IAxis& axis = h.yAxis();
  if ( axis.isFixedBinning() ) {
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( h.title(), axis.bins(), axis.lowerEdge(), axis.upperEdge() );
  }
  else {
    std::vector< double > edges( axis.bins() + 1 );
    for ( int i = 0; i < axis.bins(); ++i ) edges[i] = axis.binLowerEdge( i );
    edges[ axis.bins() ] = axis.upperEdge();
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( h.title(), edges );
  }

  for ( int i = -numberOfExtraBins; i < axis.bins(); ++i ) {
    double errorSquared = 0;
    double binMeanY = 0;
    double binHeight = 0;
    const AIDA::IAxis& axisX = h.xAxis();
    for ( int j = -numberOfExtraBins; j < axisX.bins(); ++j ) {
      const double be = h.binError( j, i );
      errorSquared += be * be;
      const double height = h.binHeight( j, i );
      binHeight += height;
      binMeanY += h.binMeanY( j, i ) * height;
    }
    if ( binHeight != 0 ) binMeanY /= binHeight;
    p->setBinContents( i,
		       h.binEntriesY(i),
                       binHeight,
		       std::sqrt( errorSquared ),
                       binMeanY );

  }


  // FIXME: set rms 

  return p;
}

AIDA::Dev::IDevHistogram1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::sliceX( const AIDA::IHistogram2D & h, int indexY )
{
  AIDA::Dev::IDevHistogram1D* p = 0;
  const AIDA::IAxis& axis = h.xAxis();
  if ( axis.isFixedBinning() ) {
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( h.title(), axis.bins(), axis.lowerEdge(), axis.upperEdge() );
  }
  else {
    std::vector< double > edges( axis.bins() + 1 );
    for ( int i = 0; i < axis.bins(); ++i ) edges[i] = axis.binLowerEdge( i );
    edges[ axis.bins() ] = axis.upperEdge();
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( h.title(), edges );
  }

  for ( int i = -numberOfExtraBins; i < axis.bins(); ++i ) {
    p->setBinContents( i,
		       h.binEntries(i, indexY),
                       h.binHeight(i, indexY),
		       h.binError(i, indexY ),
                       h.binMeanX(i, indexY) );
  }

  // FIXME: set rms 

  return p;
}


AIDA::Dev::IDevHistogram1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::sliceY( const AIDA::IHistogram2D & h, int indexX )
{
  AIDA::Dev::IDevHistogram1D* p = 0;
  const AIDA::IAxis& axis = h.yAxis();
  if ( axis.isFixedBinning() ) {
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( h.title(), axis.bins(), axis.lowerEdge(), axis.upperEdge() );
  }
  else {
    std::vector< double > edges( axis.bins() + 1 );
    for ( int i = 0; i < axis.bins(); ++i ) edges[i] = axis.binLowerEdge( i );
    edges[ axis.bins() ] = axis.upperEdge();
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( h.title(), edges );
  }

  for ( int i = -numberOfExtraBins; i < axis.bins(); ++i ) {
    p->setBinContents( i,
		       h.binEntries(indexX, i),
                       h.binHeight(indexX, i),
		       h.binError(indexX, i ),
                       h.binMeanY(indexX, i) );
  }

  // FIXME: set rms 

  return p;
}


AIDA::Dev::IDevHistogram1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::sliceX( const AIDA::IHistogram2D & h, int indexY1, int indexY2 )
{
  AIDA::Dev::IDevHistogram1D* p = 0;
  const AIDA::IAxis& axis = h.xAxis();
  if ( axis.isFixedBinning() ) {
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( h.title(), axis.bins(), axis.lowerEdge(), axis.upperEdge() );
  }
  else {
    std::vector< double > edges( axis.bins() + 1 );
    for ( int i = 0; i < axis.bins(); ++i ) edges[i] = axis.binLowerEdge( i );
    edges[ axis.bins() ] = axis.upperEdge();
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( h.title(), edges );
  }

    // LM : fix problem when index == underflow/overflow
  int ifirst = indexY1; 
  if (indexY1 <= AIDA::IAxis::UNDERFLOW_BIN) ifirst = -1; 
  int ilast = indexY2;
  if (indexY2 == AIDA::IAxis::OVERFLOW_BIN || indexY2 > h.yAxis().bins() ) 
    ilast = h.yAxis().bins();  



  for ( int i = -numberOfExtraBins; i < axis.bins(); ++i ) {
    int entries = 0;
    double height = 0;
    double errorSquared = 0;
    double binMean = 0;

    for ( int k = ifirst; k <= ilast; ++k ) {
      int j = k; 
      if (k == -1) j = AIDA::IAxis::UNDERFLOW_BIN;
      if (k == h.yAxis().bins() ) j =  AIDA::IAxis::OVERFLOW_BIN;
      entries += h.binEntries(i, j );
      const double tempHeight = h.binHeight(i, j );
      height += tempHeight;
      binMean += h.binMeanX(i,j) * tempHeight;
      const double error = h.binError( i, j );
      errorSquared += error * error;
    };
    if ( height != 0 ) binMean /= height;
    p->setBinContents( i,
		       entries,
                       height,
		       std::sqrt( errorSquared ),
                       binMean );
  }

  // FIXME: set rms 

  return p;
}


AIDA::Dev::IDevHistogram1D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::sliceY( const AIDA::IHistogram2D & h, int indexX1, int indexX2 )
{
  AIDA::Dev::IDevHistogram1D* p = 0;
  const AIDA::IAxis& axis = h.yAxis();
  if ( axis.isFixedBinning() ) {
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( h.title(), axis.bins(), axis.lowerEdge(), axis.upperEdge() );
  }
  else {
    std::vector< double > edges( axis.bins() + 1 );
    for ( int i = 0; i < axis.bins(); ++i ) edges[i] = axis.binLowerEdge( i );
    edges[ axis.bins() ] = axis.upperEdge();
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram1D( h.title(), edges );
  }

    // LM : fix problem when index == underflow/overflow
  int ifirst = indexX1; 
  if (indexX1 <= AIDA::IAxis::UNDERFLOW_BIN) ifirst = -1; 
  int ilast = indexX2;
  if (indexX2 == AIDA::IAxis::OVERFLOW_BIN || indexX2 > h.xAxis().bins() ) 
    ilast = h.xAxis().bins();  

  for ( int i = -numberOfExtraBins; i < axis.bins(); ++i ) {
    int entries = 0;
    double height = 0;
    double errorSquared = 0;
    double binMean = 0;
    for ( int k = ifirst; k <= ilast; ++k ) {
      int j = k; 
      if (k == -1) j = AIDA::IAxis::UNDERFLOW_BIN;
      if (k == h.xAxis().bins() ) j =  AIDA::IAxis::OVERFLOW_BIN;
      entries += h.binEntries(j, i );
      const double tempHeight = h.binHeight(j, i );
      height += tempHeight;
      binMean += h.binMeanY(j, i) * tempHeight;
      const double error = h.binError( j, i );
      errorSquared += error * error;
    };
    if ( height != 0 ) binMean /= height;
    p->setBinContents( i,
		       entries,
                       height,
		       std::sqrt( errorSquared ),
                       binMean );
  }

  // FIXME: set rms 

  return p;
}


AIDA::Dev::IDevHistogram2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::projectionXY( const AIDA::IHistogram3D & h )
{
  AIDA::Dev::IDevHistogram2D* p = 0;
  const AIDA::IAxis& xAxis = h.xAxis();
  const AIDA::IAxis& yAxis = h.yAxis();
  const AIDA::IAxis& zAxis = h.zAxis();
  if ( xAxis.isFixedBinning() ) {
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( h.title(), xAxis.bins(), xAxis.lowerEdge(), xAxis.upperEdge(), yAxis.bins(), yAxis.lowerEdge(), yAxis.upperEdge() );
  }
  else {
    std::vector< double > edgesX( xAxis.bins() + 1 );
    for ( int i = 0; i < xAxis.bins(); ++i ) edgesX[i] = xAxis.binLowerEdge( i );
    edgesX[ xAxis.bins() ] = xAxis.upperEdge();
    std::vector< double > edgesY( yAxis.bins() + 1 );
    for ( int i = 0; i < yAxis.bins(); ++i ) edgesY[i] = yAxis.binLowerEdge( i );
    edgesY[ yAxis.bins() ] = yAxis.upperEdge();
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( h.title(), edgesX, edgesY );
  }

  for ( int i = -numberOfExtraBins; i < xAxis.bins(); ++i ) {
    for ( int j = -numberOfExtraBins; j < yAxis.bins(); ++j ) {
      int entries = 0;
      double height = 0;
      double errorSquared = 0;
      double binMeanX = 0;
      double binMeanY = 0;
      for ( int k = -numberOfExtraBins; k < zAxis.bins(); ++k ) {
	entries += h.binEntries( i, j, k );
	const double tempHeight = h.binHeight( i, j, k );
	height += tempHeight;
	binMeanX += h.binMeanX( i, j, k ) * tempHeight;
	binMeanY += h.binMeanY( i, j, k ) * tempHeight;
        const double error = h.binError( i, j, k );
	errorSquared += error*error;
      }
      if ( height != 0 ) {
	binMeanX /= height;
	binMeanY /= height;
      }
      p->setBinContents( i, j, entries, height, std::sqrt( errorSquared ), binMeanX, binMeanY );
    }
  }

  // FIXME: set rms 

  return p;
}


AIDA::Dev::IDevHistogram2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::projectionXZ( const AIDA::IHistogram3D & h )
{
  AIDA::Dev::IDevHistogram2D* p = 0;
  const AIDA::IAxis& xAxis = h.xAxis();
  const AIDA::IAxis& yAxis = h.yAxis();
  const AIDA::IAxis& zAxis = h.zAxis();
  if ( xAxis.isFixedBinning() ) {
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( h.title(), xAxis.bins(), xAxis.lowerEdge(), xAxis.upperEdge(), zAxis.bins(), zAxis.lowerEdge(), zAxis.upperEdge() );
  }
  else {
    std::vector< double > edgesX( xAxis.bins() + 1 );
    for ( int i = 0; i < xAxis.bins(); ++i ) edgesX[i] = xAxis.binLowerEdge( i );
    edgesX[ xAxis.bins() ] = xAxis.upperEdge();
    std::vector< double > edgesZ( zAxis.bins() + 1 );
    for ( int i = 0; i < zAxis.bins(); ++i ) edgesZ[i] = zAxis.binLowerEdge( i );
    edgesZ[ zAxis.bins() ] = zAxis.upperEdge();
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( h.title(), edgesX, edgesZ );
  }

  for ( int i = -numberOfExtraBins; i < xAxis.bins(); ++i ) {
    for ( int j = -numberOfExtraBins; j < zAxis.bins(); ++j ) {
      int entries = 0;
      double height = 0;
      double errorSquared = 0;
      double binMeanX = 0;
      double binMeanZ = 0;
      for ( int k = -numberOfExtraBins; k < yAxis.bins(); ++k ) {
	entries += h.binEntries( i, k, j );
	const double tempHeight = h.binHeight( i, k, j );
	height += tempHeight;
	binMeanX += h.binMeanX( i, k, j ) * tempHeight;
	binMeanZ += h.binMeanZ( i, k, j ) * tempHeight;
        const double error = h.binError( i, k, j );
	errorSquared += error*error;
      }
      if ( height != 0 ) {
	binMeanX /= height;
	binMeanZ /= height;
      }
      p->setBinContents( i, j, entries, height, std::sqrt( errorSquared ), binMeanX, binMeanZ );
    }
  }

  // FIXME: set rms 

  return p;
}


AIDA::Dev::IDevHistogram2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::projectionYZ( const AIDA::IHistogram3D & h )
{
  AIDA::Dev::IDevHistogram2D* p = 0;
  const AIDA::IAxis& xAxis = h.xAxis();
  const AIDA::IAxis& yAxis = h.yAxis();
  const AIDA::IAxis& zAxis = h.zAxis();
  if ( yAxis.isFixedBinning() ) {
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( h.title(), yAxis.bins(), yAxis.lowerEdge(), yAxis.upperEdge(), zAxis.bins(), zAxis.lowerEdge(), zAxis.upperEdge() );
  }
  else {
    std::vector< double > edgesY( yAxis.bins() + 1 );
    for ( int i = 0; i < yAxis.bins(); ++i ) edgesY[i] = yAxis.binLowerEdge( i );
    edgesY[ yAxis.bins() ] = yAxis.upperEdge();
    std::vector< double > edgesZ( zAxis.bins() + 1 );
    for ( int i = 0; i < zAxis.bins(); ++i ) edgesZ[i] = zAxis.binLowerEdge( i );
    edgesZ[ zAxis.bins() ] = zAxis.upperEdge();
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( h.title(), edgesY, edgesZ );
  }

  for ( int i = -numberOfExtraBins; i < yAxis.bins(); ++i ) {
    for ( int j = -numberOfExtraBins; j < zAxis.bins(); ++j ) {
      int entries = 0;
      double height = 0;
      double errorSquared = 0;
      double binMeanY = 0;
      double binMeanZ = 0;
      for ( int k = -numberOfExtraBins; k < xAxis.bins(); ++k ) {
	entries += h.binEntries( k, i, j );
	const double tempHeight = h.binHeight( k, i, j );
	height += tempHeight;
	binMeanY += h.binMeanY( k, i, j ) * tempHeight;
	binMeanZ += h.binMeanZ( k, i, j ) * tempHeight;
        const double error = h.binError( k, i, j );
	errorSquared += error*error;
      }
      if ( height != 0 ) {
	binMeanY /= height;
	binMeanZ /= height;
      }
      p->setBinContents( i, j, entries, height, std::sqrt( errorSquared ), binMeanY, binMeanZ );
    }
  }

  // FIXME: set rms 

  return p;
}


AIDA::Dev::IDevHistogram2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::sliceXY( const AIDA::IHistogram3D & h, int indexZ1, int indexZ2 )
{
  AIDA::Dev::IDevHistogram2D* p = 0;
  const AIDA::IAxis& xAxis = h.xAxis();
  const AIDA::IAxis& yAxis = h.yAxis();
  if ( xAxis.isFixedBinning() ) {
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( h.title(), xAxis.bins(), xAxis.lowerEdge(), xAxis.upperEdge(), yAxis.bins(), yAxis.lowerEdge(), yAxis.upperEdge() );
  }
  else {
    std::vector< double > edgesX( xAxis.bins() + 1 );
    for ( int i = 0; i < xAxis.bins(); ++i ) edgesX[i] = xAxis.binLowerEdge( i );
    edgesX[ xAxis.bins() ] = xAxis.upperEdge();
    std::vector< double > edgesY( yAxis.bins() + 1 );
    for ( int i = 0; i < yAxis.bins(); ++i ) edgesY[i] = yAxis.binLowerEdge( i );
    edgesY[ yAxis.bins() ] = yAxis.upperEdge();
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( h.title(), edgesX, edgesY );
  }

    // LM : fix problem when index == underflow/overflow
  int ifirst = indexZ1; 
  if (indexZ1 <= AIDA::IAxis::UNDERFLOW_BIN) ifirst = -1; 
  int ilast = indexZ2;
  if (indexZ2 == AIDA::IAxis::OVERFLOW_BIN || indexZ2 > h.zAxis().bins() ) 
    ilast = h.zAxis().bins();  

  for ( int i = -numberOfExtraBins; i < xAxis.bins(); ++i ) {
    for ( int j = -numberOfExtraBins; j < yAxis.bins(); ++j ) {
      int entries = 0;
      double height = 0;
      double errorSquared = 0;
      double binMeanX = 0;
      double binMeanY = 0;

      for ( int l = ifirst; l <= ilast; ++l ) {
	int k = l; 
	if (l == -1) k = AIDA::IAxis::UNDERFLOW_BIN;
	if (l == h.zAxis().bins() ) k =  AIDA::IAxis::OVERFLOW_BIN;

	entries += h.binEntries( i, j, k );
	const double tempHeight = h.binHeight( i, j, k );
	height += tempHeight;
        binMeanX += h.binMeanX( i, j, k ) * tempHeight;
        binMeanY += h.binMeanY( i, j, k ) * tempHeight;
        const double error = h.binError( i, j, k );
	errorSquared += error*error;
      }
      if ( height != 0 ) {
	binMeanX /= height;
	binMeanY /= height;
      }
      p->setBinContents( i, j, entries, height, std::sqrt( errorSquared ), binMeanX, binMeanY );
    }
  }

  // FIXME: set rms 

  return p;
}


AIDA::Dev::IDevHistogram2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::sliceXZ( const AIDA::IHistogram3D & h, int indexY1, int indexY2 )
{
  AIDA::Dev::IDevHistogram2D* p = 0;
  const AIDA::IAxis& xAxis = h.xAxis();
  const AIDA::IAxis& zAxis = h.zAxis();
  if ( xAxis.isFixedBinning() ) {
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( h.title(), xAxis.bins(), xAxis.lowerEdge(), xAxis.upperEdge(), zAxis.bins(), zAxis.lowerEdge(), zAxis.upperEdge() );
  }
  else {
    std::vector< double > edgesX( xAxis.bins() + 1 );
    for ( int i = 0; i < xAxis.bins(); ++i ) edgesX[i] = xAxis.binLowerEdge( i );
    edgesX[ xAxis.bins() ] = xAxis.upperEdge();
    std::vector< double > edgesZ( zAxis.bins() + 1 );
    for ( int i = 0; i < zAxis.bins(); ++i ) edgesZ[i] = zAxis.binLowerEdge( i );
    edgesZ[ zAxis.bins() ] = zAxis.upperEdge();
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( h.title(), edgesX, edgesZ );
  }

    // LM : fix problem when index == underflow/overflow
  int ifirst = indexY1; 
  if (indexY1 <= AIDA::IAxis::UNDERFLOW_BIN) ifirst = -1; 
  int ilast = indexY2;
  if (indexY2 == AIDA::IAxis::OVERFLOW_BIN || indexY2 > h.yAxis().bins() ) 
    ilast = h.yAxis().bins();  

  for ( int i = -numberOfExtraBins; i < xAxis.bins(); ++i ) {
    for ( int j = -numberOfExtraBins; j < zAxis.bins(); ++j ) {
      int entries = 0;
      double height = 0;
      double errorSquared = 0;
      double binMeanX = 0;
      double binMeanZ = 0;

      for ( int l = ifirst; l <= ilast; ++l ) {
	int k = l; 
	if (l == -1) k = AIDA::IAxis::UNDERFLOW_BIN;
	if (l == h.yAxis().bins() ) k =  AIDA::IAxis::OVERFLOW_BIN;

	entries += h.binEntries( i, k, j );
	const double tempHeight = h.binHeight( i, k, j );
	height += tempHeight;
	binMeanX += h.binMeanX( i, k, j ) * tempHeight;
	binMeanZ += h.binMeanZ( i, k, j ) * tempHeight;
        const double error = h.binError( i, k, j );
	errorSquared += error*error;
      }
      if ( height != 0 ) {
	binMeanX /= height;
	binMeanZ /= height;
      }
      p->setBinContents( i, j, entries, height, std::sqrt( errorSquared ), binMeanX, binMeanZ );
    }
  }

  // FIXME: set rms 

  return p;
}


AIDA::Dev::IDevHistogram2D*
iAIDA::AIDA_Histogram_native::AIDA_DevHistogramFactory::sliceYZ( const AIDA::IHistogram3D & h, int indexX1, int indexX2 )
{
  AIDA::Dev::IDevHistogram2D* p = 0;
  const AIDA::IAxis& yAxis = h.yAxis();
  const AIDA::IAxis& zAxis = h.zAxis();
  if ( yAxis.isFixedBinning() ) {
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( h.title(), yAxis.bins(), yAxis.lowerEdge(), yAxis.upperEdge(), zAxis.bins(), zAxis.lowerEdge(), zAxis.upperEdge() );
  }
  else {
    std::vector< double > edgesY( yAxis.bins() + 1 );
    for ( int i = 0; i < yAxis.bins(); ++i ) edgesY[i] = yAxis.binLowerEdge( i );
    edgesY[ yAxis.bins() ] = yAxis.upperEdge();
    std::vector< double > edgesZ( zAxis.bins() + 1 );
    for ( int i = 0; i < zAxis.bins(); ++i ) edgesZ[i] = zAxis.binLowerEdge( i );
    edgesZ[ zAxis.bins() ] = zAxis.upperEdge();
    p = new iAIDA::AIDA_Histogram_native::AIDA_Histogram2D( h.title(), edgesY, edgesZ );
  }

    // LM : fix problem when index == underflow/overflow
  int ifirst = indexX1; 
  if (indexX1 <= AIDA::IAxis::UNDERFLOW_BIN) ifirst = -1; 
  int ilast = indexX2;
  if (indexX2 == AIDA::IAxis::OVERFLOW_BIN || indexX2 > h.xAxis().bins() ) 
    ilast = h.xAxis().bins();  

  for ( int i = -numberOfExtraBins; i < yAxis.bins(); ++i ) {
    for ( int j = -numberOfExtraBins; j < zAxis.bins(); ++j ) {
      int entries = 0;
      double height = 0;
      double errorSquared = 0;
      double binMeanY = 0;
      double binMeanZ = 0;

      for ( int l = ifirst; l <= ilast; ++l ) {
	int k = l; 
	if (l == -1) k = AIDA::IAxis::UNDERFLOW_BIN;
	if (l == h.xAxis().bins() ) k =  AIDA::IAxis::OVERFLOW_BIN;

	entries += h.binEntries( k, i, j );
	const double tempHeight = h.binHeight( k, i, j );
	height += tempHeight;
	binMeanY += h.binMeanY( k, i, j ) * tempHeight;
	binMeanZ += h.binMeanZ( k, i, j ) * tempHeight;
        const double error = h.binError( k, i, j );
	errorSquared += error*error;
      }
      if ( height != 0 ) {
	binMeanY /= height;
	binMeanZ /= height;
      }
      p->setBinContents( i, j, entries, height, std::sqrt( errorSquared ), binMeanY, binMeanZ );
    }
  }

  // FIXME: set rms 

  return p;
}
