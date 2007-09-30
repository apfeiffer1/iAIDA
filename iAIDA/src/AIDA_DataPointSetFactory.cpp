#include "AIDA_DataPointSet_native/AIDA_DataPointSetFactory.h"
#include "AIDA_Dev/IDevTree.h"
#include "AIDA_Dev/IDevDataPointSetFactory.h"
#include "AIDA_Dev/IDevDataPointSet.h"

#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
#include <exception>
#include <iostream>
#define DATAPOINTSET_REPORT_ERROR( XXX ) std::cerr << XXX << std::endl
#endif

static const std::string emptyString = "";

iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::AIDA_DataPointSetFactory( AIDA::Dev::IDevTree& tree,
										      AIDA::Dev::IDevDataPointSetFactory& factory ):
  m_tree( tree ),
  m_factory( factory )
{}


bool
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::destroy( AIDA::IDataPointSet* dps )
{
  AIDA::IManagedObject* object = dynamic_cast< AIDA::IManagedObject* >( dps );
  if ( ! object ) return false;
  return m_tree.rm( m_tree.findPath( *object ) );
}


AIDA::IDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::create( const std::string& name,
								    const std::string& title,
								    int dimensionOfPoint )
{
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevDataPointSet* p = m_factory.create( title, dimensionOfPoint );
  return registerObject( p, name );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::IDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::create( const std::string& name,
								    int dimensionOfPoint )
{
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevDataPointSet* p = m_factory.create( dimensionOfPoint );
  return registerObject( p, name );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::IDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createCopy( const std::string& name,
									const AIDA::IDataPointSet & original )
{
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevDataPointSet* p = m_factory.createCopy( original );
  return registerObject( p, name );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::IDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::create( const std::string& name,
								    const AIDA::IHistogram1D & h, const std::string & options )
{
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevDataPointSet* p = m_factory.create( h, options );
  return registerObject( p, name );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::IDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::create( const std::string& name,
								    const AIDA::IHistogram2D & h, const std::string & options )
{
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevDataPointSet* p = m_factory.create( h, options );
  return registerObject( p, name );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::IDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::create( const std::string& name,
								    const AIDA::IHistogram3D & h, const std::string & options )
{
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevDataPointSet* p = m_factory.create( h, options );
  return registerObject( p, name );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::IDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::create( const std::string& name,
								    const AIDA::ICloud1D & c, const std::string & options )
{
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevDataPointSet* p = m_factory.create( c, options );
  return registerObject( p, name );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::IDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::create( const std::string& name,
								    const AIDA::ICloud2D & c, const std::string & options )
{
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevDataPointSet* p = m_factory.create( c, options );
  return registerObject( p, name );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::IDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::create( const std::string& name,
								    const AIDA::ICloud3D & c, const std::string & options )
{
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevDataPointSet* p = m_factory.create( c, options );
  return registerObject( p, name );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::IDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::create( const std::string& name,
								    const AIDA::IProfile1D & ph, const std::string & options )
{
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevDataPointSet* p = m_factory.create( ph, options );
  return registerObject( p, name );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::IDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::create( const std::string& name,
								    const AIDA::IProfile2D & ph, const std::string & options )
{
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevDataPointSet* p = m_factory.create( ph, options );
  return registerObject( p, name );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::IDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::add( const std::string & name,
								 const AIDA::IDataPointSet& a,
								 const AIDA::IDataPointSet& b )
{
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevDataPointSet* p = m_factory.add( a, b, emptyString );
  return registerObject( p, name );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::IDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::subtract( const std::string & name,
								      const AIDA::IDataPointSet& a,
								      const AIDA::IDataPointSet& b )
{
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevDataPointSet* p = m_factory.subtract( a, b, emptyString );
  return registerObject( p, name );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::IDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::multiply( const std::string & name,
								      const AIDA::IDataPointSet& a,
								      const AIDA::IDataPointSet& b )
{
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevDataPointSet* p = m_factory.multiply( a, b, emptyString );
  return registerObject( p, name );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::IDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::divide( const std::string & name,
								    const AIDA::IDataPointSet& a,
								    const AIDA::IDataPointSet& b )
{
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevDataPointSet* p = m_factory.divide( a, b, emptyString );
  return registerObject( p, name );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


AIDA::IDataPointSet*
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::weightedMean( const std::string & name,
									  const AIDA::IDataPointSet& a,
									  const AIDA::IDataPointSet& b )
{
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
  AIDA::Dev::IDevDataPointSet* p = m_factory.weightedMean( a, b, emptyString );
  return registerObject( p, name );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}


// new methods from AIDA 3.2

  // new methods from Aida 3.2.1

//===========================================================================
// createY

AIDA::IDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createY(const std::string & path, const std::string & title, const std::vector<double>  & y, const std::vector<double>  & eyp, const std::vector<double>  & eym) {
 
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
    AIDA::Dev::IDevDataPointSet* p = m_factory.createY(title, y, eyp, eym );
    return registerObject( p, path );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createY(const std::string & path, const std::string & title, const std::vector<double>  & y, const std::vector<double>  & ey) { 
  return createY(path,title,y,ey,ey); 
}

// use title = path or an empty string ?? 
// do as in other packages (e.g. histogram: empty strings) 
AIDA::IDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createY(const std::string & path, const std::vector<double>  & y, const std::vector<double>  & ey)
{
  return createY( path, emptyString, y, ey, ey); 
}

AIDA::IDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createY(const std::string & path, const std::vector<double>  & y, const std::vector<double>  & eyp, const std::vector<double>  & eym) { 
  return createY( path, emptyString, y, eyp, eym); 
}

//===========================================================================
// createX 

AIDA::IDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createX(const std::string & path, const std::string & title, const std::vector<double>  & x, const std::vector<double>  & exp, const std::vector<double>  & exm) { 
 
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
    AIDA::Dev::IDevDataPointSet* p = m_factory.createX(title, x, exp, exm );
    return registerObject( p, path );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createX(const std::string & path, const std::string & title, const std::vector<double>  & x, const std::vector<double>  & ex) {
  return createX( path, title, x, ex, ex); 
}


AIDA::IDataPointSet * iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createX(const std::string & path, const std::vector<double>  & x, const std::vector<double>  & ex) { 
  return createX( path, emptyString, x, ex, ex) ; 
}
AIDA::IDataPointSet * iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createX(const std::string & path, const std::vector<double>  & x, const std::vector<double>  & exp, const std::vector<double>  & exm) { 
  return createX( path, emptyString, x, exp, exm) ; 
}

//===========================================================================
// createXY

AIDA::IDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createXY(const std::string & path, const std::string & title, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & exp, const std::vector<double>  & eyp, const std::vector<double>  & exm, const std::vector<double>  & eym) { 
  
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
    AIDA::Dev::IDevDataPointSet* p = m_factory.createXY(title, x, y, exp, eyp, exm, eym );
    return registerObject( p, path );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif
}

AIDA::IDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createXY(const std::string & path, const std::string & title, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & ex, const std::vector<double>  & ey) { 
  return createXY(path, title,  x, y, ex, ey, ex, ey );
}
AIDA::IDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createXY(const std::string & path, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & exp, const std::vector<double>  & eyp, const std::vector<double>  & exm, const std::vector<double>  & eym) { 
  return createXY(path, emptyString, x, y, exp, eyp, exm, eym );
}
AIDA::IDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createXY(const std::string & path, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & ex, const std::vector<double>  & ey) {
  return createXY(path, emptyString,  x, y, ex, ey, ex, ey );
} 

//===========================================================================
// createXYZ

AIDA::IDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createXYZ(const std::string & path, const std::string & title, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & z, const std::vector<double>  & exp, const std::vector<double>  & eyp, const std::vector<double>  & ezp, const std::vector<double>  & exm, const std::vector<double>  & eym, const std::vector<double>  & ezm) { 
  
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  try{
#endif
    AIDA::Dev::IDevDataPointSet* p = m_factory.createXYZ(title, x, y, z, exp, eyp, ezp, exm, eym, ezm );
    return registerObject( p, path );
#ifdef IAIDA_AIDA_DATAPOINTSET_DATAPOINTSET_NO_EXCEPTIONS_TO_USER
  }
  catch( std::exception& e ) {
    DATAPOINTSET_REPORT_ERROR( e.what() );
    return 0;
  }
#endif

}
AIDA::IDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createXYZ(const std::string & path, const std::string & title, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & z, const std::vector<double>  & ex, const std::vector<double>  & ey, const std::vector<double>  & ez) { 
  return createXYZ(path, title, x, y, z, ex, ey, ez, ex, ey, ez );
}
AIDA::IDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createXYZ(const std::string & path, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & z, const std::vector<double>  & exp, const std::vector<double>  & eyp, const std::vector<double>  & ezp, const std::vector<double>  & exm, const std::vector<double>  & eym, const std::vector<double>  & ezm) { 
  return createXYZ(path, emptyString, x, y, z, exp, eyp, ezp, exm, eym, ezm );
}
AIDA::IDataPointSet * 
iAIDA::AIDA_DataPointSet_native::AIDA_DataPointSetFactory::createXYZ(const std::string & path, const std::vector<double>  & x, const std::vector<double>  & y, const std::vector<double>  & z, const std::vector<double>  & ex, const std::vector<double>  & ey, const std::vector<double>  & ez) {   
  return createXYZ(path, emptyString, x, y, z, ex, ey, ez, ex, ey, ez );
}
