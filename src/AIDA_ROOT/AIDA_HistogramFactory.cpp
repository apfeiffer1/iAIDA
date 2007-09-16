#include "AIDA_ROOT/AIDA_HistogramFactory.h"
#include "AIDA_Dev/IDevTree.h"
#include "AIDA_Dev/IDevHistogramFactory.h"

#include "AIDA_Dev/IDevHistogram1D.h"
#include "AIDA_Dev/IDevHistogram2D.h"
#include "AIDA_Dev/IDevHistogram3D.h"
#include "AIDA_Dev/IDevCloud1D.h"
#include "AIDA_Dev/IDevCloud2D.h"
#include "AIDA_Dev/IDevCloud3D.h"
#include "AIDA_Dev/IDevProfile1D.h"
#include "AIDA_Dev/IDevProfile2D.h"

#include <typeinfo>

#define AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER 1

#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
#include <exception>
#include <iostream>
#define HISTOGRAM_REPORT_ERROR( XXX ) std::cerr << XXX << std::endl
#endif


AIDA_ROOT::AIDA_HistogramFactory::AIDA_HistogramFactory( AIDA::Dev::IDevTree& tree,
						      AIDA::Dev::IDevHistogramFactory& factory ):
  m_tree( tree ),
  m_factory( factory )
{}

bool
AIDA_ROOT::AIDA_HistogramFactory::destroy( AIDA::IBaseHistogram* histo )
{
  AIDA::IManagedObject* object = dynamic_cast< AIDA::IManagedObject* >( histo );
  if ( ! object ) return false;
  return m_tree.rm( m_tree.findPath( *object ) );
}

AIDA::ICloud1D*
AIDA_ROOT::AIDA_HistogramFactory::createCloud1D( const std::string& name, const std::string& title, int nMax, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevCloud1D* p = m_factory.createCloud1D( title, nMax, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::ICloud1D*
AIDA_ROOT::AIDA_HistogramFactory::createCloud1D( const std::string& name )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevCloud1D* p = m_factory.createCloud1D();
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::ICloud1D*
AIDA_ROOT::AIDA_HistogramFactory::createCopy( const std::string& name, const AIDA::ICloud1D& original )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevCloud1D* p = m_factory.createCopy( original );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::ICloud2D*
AIDA_ROOT::AIDA_HistogramFactory::createCloud2D( const std::string& name, const std::string& title, int nMax, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevCloud2D* p = m_factory.createCloud2D( title, nMax, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::ICloud2D*
AIDA_ROOT::AIDA_HistogramFactory::createCloud2D( const std::string& name )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevCloud2D* p = m_factory.createCloud2D();
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::ICloud2D*
AIDA_ROOT::AIDA_HistogramFactory::createCopy( const std::string& name, const AIDA::ICloud2D& original )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevCloud2D* p = m_factory.createCopy( original );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::ICloud3D*
AIDA_ROOT::AIDA_HistogramFactory::createCloud3D( const std::string& name, const std::string& title, int nMax, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevCloud3D* p = m_factory.createCloud3D( title, nMax, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::ICloud3D*
AIDA_ROOT::AIDA_HistogramFactory::createCloud3D( const std::string& name )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevCloud3D* p = m_factory.createCloud3D();
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::ICloud3D*
AIDA_ROOT::AIDA_HistogramFactory::createCopy( const std::string& name, const AIDA::ICloud3D& original )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevCloud3D* p = m_factory.createCopy( original );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram1D*
AIDA_ROOT::AIDA_HistogramFactory::createHistogram1D( const std::string& name, const std::string& title, int nBins, double lowerEdge, double upperEdge, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram1D* p = m_factory.createHistogram1D( title, nBins, lowerEdge, upperEdge, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram1D*
AIDA_ROOT::AIDA_HistogramFactory::createHistogram1D( const std::string& name, const std::string& title, const std::vector< double >& binEdge, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram1D* p = m_factory.createHistogram1D( title, binEdge, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram1D*
AIDA_ROOT::AIDA_HistogramFactory::createHistogram1D( const std::string& name, int nBins, double lowerEdge, double upperEdge )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram1D* p = m_factory.createHistogram1D( nBins, lowerEdge, upperEdge );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram1D*
AIDA_ROOT::AIDA_HistogramFactory::createCopy( const std::string& name, const AIDA::IHistogram1D& original )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram1D* p = m_factory.createCopy( original );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram2D*
AIDA_ROOT::AIDA_HistogramFactory::createHistogram2D( const std::string& name, const std::string& title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram2D* p = m_factory.createHistogram2D( title, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram2D*
AIDA_ROOT::AIDA_HistogramFactory::createHistogram2D( const std::string& name, const std::string& title, const std::vector< double >& binEdgeX, const std::vector< double >& binEdgeY, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram2D* p = m_factory.createHistogram2D( title, binEdgeX, binEdgeY, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram2D*
AIDA_ROOT::AIDA_HistogramFactory::createHistogram2D( const std::string& name, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram2D* p = m_factory.createHistogram2D( nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram2D*
AIDA_ROOT::AIDA_HistogramFactory::createCopy( const std::string& name, const AIDA::IHistogram2D& original )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram2D* p = m_factory.createCopy( original );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram3D*
AIDA_ROOT::AIDA_HistogramFactory::createHistogram3D( const std::string& name, const std::string& title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nBinsZ, double lowerEdgeZ, double upperEdgeZ, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram3D* p = m_factory.createHistogram3D( title, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, nBinsZ, lowerEdgeZ, upperEdgeZ, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram3D*
AIDA_ROOT::AIDA_HistogramFactory::createHistogram3D( const std::string& name, const std::string& title, const std::vector< double >& binEdgeX, const std::vector< double >& binEdgeY, const std::vector< double >& binEdgeZ, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram3D* p = m_factory.createHistogram3D( title, binEdgeX, binEdgeY, binEdgeZ, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram3D*
AIDA_ROOT::AIDA_HistogramFactory::createHistogram3D( const std::string& name, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, int nBinsZ, double lowerEdgeZ, double upperEdgeZ )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram3D* p = m_factory.createHistogram3D( nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, nBinsZ, lowerEdgeZ, upperEdgeZ );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram3D*
AIDA_ROOT::AIDA_HistogramFactory::createCopy( const std::string& name, const AIDA::IHistogram3D& original )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram3D* p = m_factory.createCopy( original );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IProfile1D*
AIDA_ROOT::AIDA_HistogramFactory::createProfile1D( const std::string& name, const std::string& title, int nBins, double lowerEdge, double upperEdge, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevProfile1D* p = m_factory.createProfile1D( title, nBins, lowerEdge, upperEdge, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception&  ) {
    return 0;
  }
#endif
}

AIDA::IProfile1D*
AIDA_ROOT::AIDA_HistogramFactory::createProfile1D( const std::string& name, const std::string& title, int nBins, double lowerEdge, double upperEdge, double lowerValue, double upperValue, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevProfile1D* p = m_factory.createProfile1D( title, nBins, lowerEdge, upperEdge, lowerValue, upperValue, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IProfile1D*
AIDA_ROOT::AIDA_HistogramFactory::createProfile1D( const std::string& name, const std::string& title, const std::vector< double >& binEdge, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevProfile1D* p = m_factory.createProfile1D( title, binEdge, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IProfile1D*
AIDA_ROOT::AIDA_HistogramFactory::createProfile1D( const std::string& name, const std::string& title, const std::vector< double >& binEdge, double lowerValue, double upperValue, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevProfile1D* p = m_factory.createProfile1D( title, binEdge, lowerValue, upperValue, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IProfile1D*
AIDA_ROOT::AIDA_HistogramFactory::createProfile1D( const std::string& name, int nBins, double lowerEdge, double upperEdge )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevProfile1D* p = m_factory.createProfile1D( nBins, lowerEdge, upperEdge );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IProfile1D*
AIDA_ROOT::AIDA_HistogramFactory::createProfile1D( const std::string& name, int nBins, double lowerEdge, double upperEdge, double lowerValue, double upperValue )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevProfile1D* p = m_factory.createProfile1D( nBins, lowerEdge, upperEdge, lowerValue, upperValue );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IProfile1D*
AIDA_ROOT::AIDA_HistogramFactory::createCopy( const std::string& name, const AIDA::IProfile1D& original )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevProfile1D* p = m_factory.createCopy( original );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IProfile2D*
AIDA_ROOT::AIDA_HistogramFactory::createProfile2D( const std::string& name, const std::string& title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevProfile2D* p = m_factory.createProfile2D( title, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IProfile2D*
AIDA_ROOT::AIDA_HistogramFactory::createProfile2D( const std::string& name, const std::string& title, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, double lowerValue, double upperValue, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevProfile2D* p = m_factory.createProfile2D( title, nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, lowerValue, upperValue, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IProfile2D*
AIDA_ROOT::AIDA_HistogramFactory::createProfile2D( const std::string& name, const std::string& title, const std::vector< double >& binEdgeX, const std::vector< double >& binEdgeY, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevProfile2D* p = m_factory.createProfile2D( title, binEdgeX, binEdgeY, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IProfile2D*
AIDA_ROOT::AIDA_HistogramFactory::createProfile2D( const std::string& name, const std::string& title, const std::vector< double >& binEdgeX, const std::vector< double >& binEdgeY, double lowerValue, double upperValue, const std::string& options )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevProfile2D* p = m_factory.createProfile2D( title, binEdgeX, binEdgeY, lowerValue, upperValue, options );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IProfile2D*
AIDA_ROOT::AIDA_HistogramFactory::createProfile2D( const std::string& name, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevProfile2D* p = m_factory.createProfile2D( nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IProfile2D*
AIDA_ROOT::AIDA_HistogramFactory::createProfile2D( const std::string& name, int nBinsX, double lowerEdgeX, double upperEdgeX, int nBinsY, double lowerEdgeY, double upperEdgeY, double lowerValue, double upperValue )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevProfile2D* p = m_factory.createProfile2D( nBinsX, lowerEdgeX, upperEdgeX, nBinsY, lowerEdgeY, upperEdgeY, lowerValue, upperValue );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IProfile2D*
AIDA_ROOT::AIDA_HistogramFactory::createCopy( const std::string& name, const AIDA::IProfile2D& original )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevProfile2D* p = m_factory.createCopy( original );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram1D*
AIDA_ROOT::AIDA_HistogramFactory::add( const std::string& name, const AIDA::IHistogram1D& a, const AIDA::IHistogram1D& b )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram1D* p = m_factory.add( a, b );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram2D*
AIDA_ROOT::AIDA_HistogramFactory::add( const std::string& name, const AIDA::IHistogram2D& a, const AIDA::IHistogram2D& b )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram2D* p = m_factory.add( a, b );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram3D*
AIDA_ROOT::AIDA_HistogramFactory::add( const std::string& name, const AIDA::IHistogram3D& a, const AIDA::IHistogram3D& b )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram3D* p = m_factory.add( a, b );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram1D*
AIDA_ROOT::AIDA_HistogramFactory::subtract( const std::string& name, const AIDA::IHistogram1D& a, const AIDA::IHistogram1D& b )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram1D* p = m_factory.subtract( a, b );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram2D*
AIDA_ROOT::AIDA_HistogramFactory::subtract( const std::string& name, const AIDA::IHistogram2D& a, const AIDA::IHistogram2D& b )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram2D* p = m_factory.subtract( a, b );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram3D*
AIDA_ROOT::AIDA_HistogramFactory::subtract( const std::string& name, const AIDA::IHistogram3D& a, const AIDA::IHistogram3D& b )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram3D* p = m_factory.subtract( a, b );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram1D*
AIDA_ROOT::AIDA_HistogramFactory::multiply( const std::string& name, const AIDA::IHistogram1D& a, const AIDA::IHistogram1D& b )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram1D* p = m_factory.multiply( a, b );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram2D*
AIDA_ROOT::AIDA_HistogramFactory::multiply( const std::string& name, const AIDA::IHistogram2D& a, const AIDA::IHistogram2D& b )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram2D* p = m_factory.multiply( a, b );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram3D*
AIDA_ROOT::AIDA_HistogramFactory::multiply( const std::string& name, const AIDA::IHistogram3D& a, const AIDA::IHistogram3D& b )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram3D* p = m_factory.multiply( a, b );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram1D*
AIDA_ROOT::AIDA_HistogramFactory::divide( const std::string& name, const AIDA::IHistogram1D& a, const AIDA::IHistogram1D& b )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram1D* p = m_factory.divide( a, b );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram2D*
AIDA_ROOT::AIDA_HistogramFactory::divide( const std::string& name, const AIDA::IHistogram2D& a, const AIDA::IHistogram2D& b )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram2D* p = m_factory.divide( a, b );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram3D*
AIDA_ROOT::AIDA_HistogramFactory::divide( const std::string& name, const AIDA::IHistogram3D& a, const AIDA::IHistogram3D& b )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram3D* p = m_factory.divide( a, b );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram1D*
AIDA_ROOT::AIDA_HistogramFactory::projectionX( const std::string& name, const AIDA::IHistogram2D& h )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram1D* p = m_factory.projectionX( h );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram1D*
AIDA_ROOT::AIDA_HistogramFactory::projectionY( const std::string& name, const AIDA::IHistogram2D& h )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram1D* p = m_factory.projectionY( h );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram1D*
AIDA_ROOT::AIDA_HistogramFactory::sliceX( const std::string& name, const AIDA::IHistogram2D& h, int indexY )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram1D* p = m_factory.sliceX( h, indexY );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram1D*
AIDA_ROOT::AIDA_HistogramFactory::sliceY( const std::string& name, const AIDA::IHistogram2D& h, int indexX )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram1D* p = m_factory.sliceY( h, indexX );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram1D*
AIDA_ROOT::AIDA_HistogramFactory::sliceX( const std::string& name, const AIDA::IHistogram2D& h, int indexY1, int indexY2 )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram1D* p = m_factory.sliceX( h, indexY1, indexY2 );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram1D*
AIDA_ROOT::AIDA_HistogramFactory::sliceY( const std::string& name, const AIDA::IHistogram2D& h, int indexX1, int indexX2 )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram1D* p = m_factory.sliceY( h, indexX1, indexX2 );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram2D*
AIDA_ROOT::AIDA_HistogramFactory::projectionXY( const std::string& name, const AIDA::IHistogram3D& h )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram2D* p = m_factory.projectionXY( h );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram2D*
AIDA_ROOT::AIDA_HistogramFactory::projectionXZ( const std::string& name, const AIDA::IHistogram3D& h )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram2D* p = m_factory.projectionXZ( h );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram2D*
AIDA_ROOT::AIDA_HistogramFactory::projectionYZ( const std::string& name, const AIDA::IHistogram3D& h )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram2D* p = m_factory.projectionYZ( h );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram2D*
AIDA_ROOT::AIDA_HistogramFactory::sliceXY( const std::string& name, const AIDA::IHistogram3D& h, int indexZ1, int indexZ2 )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram2D* p = m_factory.sliceXY( h, indexZ1, indexZ2 );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram2D*
AIDA_ROOT::AIDA_HistogramFactory::sliceXZ( const std::string& name, const AIDA::IHistogram3D& h, int indexY1, int indexY2 )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram2D* p = m_factory.sliceXZ( h, indexY1, indexY2 );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IHistogram2D*
AIDA_ROOT::AIDA_HistogramFactory::sliceYZ( const std::string& name, const AIDA::IHistogram3D& h, int indexX1, int indexX2 )
{
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevHistogram2D* p = m_factory.sliceYZ( h, indexX1, indexX2 );
  return registerObject( p, name );
#ifdef AIDA_ROOT_HISTOGRAM_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    HISTOGRAM_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}
