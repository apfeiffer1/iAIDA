#include "DummyStore.h"
#include "AIDA_Dev/IDevManagedObject.h"
#include "DummyObject.h"

#ifdef OLDSTREAMS
# include <iostream>
#else
# include <ostream>
#endif

iAIDA::aida_tree::DummyStore::DummyStore( const std::string& name, std::ostream& os ):
  m_name( name ),
  m_os( os )
{}


iAIDA::aida_tree::DummyStore::~DummyStore()
{}


const std::string&
iAIDA::aida_tree::DummyStore::name() const
{
  return m_name;
}


bool
iAIDA::aida_tree::DummyStore::writeObject( const AIDA::IManagedObject& dataObject,
				 const std::string& path )
{
  m_os << "  (Store \"" << m_name
       << "\" writes object \"" << dataObject.name()
       << "\" with full path \"" << path << "\")" << std::endl;
  m_objects.insert( path );
  return true;
}


/// Here we need the AIDA types
AIDA::IManagedObject*
iAIDA::aida_tree::DummyStore::copyAndWrite( const AIDA::IManagedObject& dataObject,
				  const std::string& path )
{
  m_os << "  (Store \"" << m_name
       << "\" copies object \"" << dataObject.name()
       << "\" and writes it in path \"" << path << "\")" << std::endl;

  // Identify the name
  unsigned int pos = path.size();
  for ( int iChar = path.size() - 1; iChar >= 0; --iChar ) {
    if ( path[iChar] == '/' ) break;
    --pos;
  }
  const std::string name = path.substr( pos );

  const iAIDA::aida_tree::DummyObject& object = dynamic_cast< const iAIDA::aida_tree::DummyObject& >( dataObject );

  AIDA::Dev::IDevManagedObject* newObject =  new iAIDA::aida_tree::DummyObject( object );

  // Register the new object
  if ( newObject ) {
    newObject->setName( name );
    writeObject( *newObject, path );
  }
  return newObject;
}


AIDA::IManagedObject*
iAIDA::aida_tree::DummyStore::retrieveObject( const std::string & path )
{
  m_os << "  (Store \"" << m_name
       << "\" retrieves object from path \"" << path << "\")" << std::endl;
  return 0;
}


bool
iAIDA::aida_tree::DummyStore::removeObject( const std::string& path )
{
  m_os << "  (Store \"" << m_name
       << "\" erases object in path \"" << path << "\")" << std::endl;
  m_objects.erase( path );
  return true;
}


bool
iAIDA::aida_tree::DummyStore::moveObject(const std::string& from, const std::string& to )
{
  m_os << "  (Store \"" << m_name
       << "\" moves object from path \"" << from << "\" to path \"" << to << "\")" << std::endl;
  if ( m_objects.find( from ) == m_objects.end() ) return false;
  m_objects.erase( from );
  m_objects.insert( to );
  return true;
}


std::vector< std::string >
iAIDA::aida_tree::DummyStore::listObjectPaths( const std::string directory,
				     bool recursive ) const
{
  std::string dir = directory;
  if ( dir[ dir.size() - 1 ] != '/' ) dir += "/";

  std::vector< std::string > result;

  std::set< std::string > directorySet;
  for ( std::set< std::string >::const_iterator iObj = m_objects.begin();
	iObj != m_objects.end(); ++iObj ) {
    const std::string& object = *iObj;
    std::string::size_type p = object.find( dir );
    if ( p == 0 ) {
      std::string complement = object.substr( dir.size() );
      // Remove extra "/" characters in the begining
      while ( complement.size() > 0 && complement[0] == '/' ) complement = complement.substr( 1 );
      if ( complement.size() == 0 ) continue;

      // Check if it is a leaf object
      std::string::size_type n = complement.find( "/" );
      if ( n == std::string::npos ) {
	result.push_back( object );
      }
      else { // update the daughter directory list
	directorySet.insert( dir + complement.substr( 0, n ) + "/" );
      }
    }
  }

  for ( std::set< std::string >::const_iterator iDir = directorySet.begin();
	iDir != directorySet.end(); ++iDir ) {
    result.push_back( *iDir );
    if ( recursive ) {
      std::vector< std::string > subDirResults = listObjectPaths( *iDir, recursive );
      for ( unsigned int i = 0; i < subDirResults.size(); ++i ) result.push_back( subDirResults[ i ] );
    }
  }

  return result;
}


std::vector< std::string > iAIDA::aida_tree::DummyStore::listObjectTypes( const std::string directory,
								 bool recursive ) const
{
  std::string dir = directory;
  if ( dir[ dir.size() - 1 ] != '/' ) dir += "/";

  std::vector< std::string > result;

  std::set< std::string > directorySet;
  for ( std::set< std::string >::const_iterator iObj = m_objects.begin();
	iObj != m_objects.end(); ++iObj ) {
    const std::string& object = *iObj;
    std::string::size_type p = object.find( dir );
    if ( p == 0 ) {
      std::string complement = object.substr( dir.size() );
      // Remove extra "/" characters in the begining
      while ( complement.size() > 0 && complement[0] == '/' ) complement = complement.substr( 1 );
      if ( complement.size() == 0 ) continue;

      // Check if it is a leaf object
      std::string::size_type n = complement.find( "/" );
      if ( n == std::string::npos ) {
	result.push_back( "DummyObject" );
      }
      else { // update the daughter directory list
	directorySet.insert( dir + complement.substr( 0, n ) + "/" );
      }
    }
  }

  for ( std::set< std::string >::const_iterator iDir = directorySet.begin();
	iDir != directorySet.end(); ++iDir ) {
    result.push_back( "dir" );
    if ( recursive ) {
      std::vector< std::string > subDirResults = listObjectTypes( *iDir, recursive );
      for ( unsigned int i = 0; i < subDirResults.size(); ++i ) result.push_back( subDirResults[ i ] );
    }
  }

  return result;
}


bool
iAIDA::aida_tree::DummyStore::commit()
{
  m_os << "  (Store \"" << m_name
       << "\" commits changes)" << std::endl;
  return true;
}


bool
iAIDA::aida_tree::DummyStore::close()
{
  m_os << "  (Store \"" << m_name
       << "\" is closed)" << std::endl;
  return true;
}

bool iAIDA::aida_tree::DummyStore::canMoveTuples() const {return false;}
bool iAIDA::aida_tree::DummyStore::canCopyTuples() const {return false;}

